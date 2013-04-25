#include "VBOBoxTransferInfo.h"

VBOBoxTransferInfo& VBOBoxTransferInfo::operator=(const VBOBoxTransferInfo& v){
	if(this != &v){
		pointsOfDirtToDraw=			v.pointsOfDirtToDraw;
		pointsOfGrassTopToDraw=		v.pointsOfGrassTopToDraw;
		pointsOfGrassSideToDraw=	v.pointsOfGrassSideToDraw;
		pointsOfStoneToDraw=		v.pointsOfStoneToDraw;
		pointsOfSandToDraw=			v.pointsOfSandToDraw;
		pointsOfWaterToDraw=		v.pointsOfWaterToDraw;
		pointsOfLeafsToDraw=		v.pointsOfLeafsToDraw;
		pointsOfWoodToDraw=			v.pointsOfWoodToDraw;
		pointsOfSnowToDraw=		v.pointsOfSnowToDraw;

		offsetOfDirt=				v.offsetOfDirt;
		offsetOfGrassTop=			v.offsetOfGrassTop;
		offsetOfGrassSide=			v.offsetOfGrassSide;
		offsetOfStone=				v.offsetOfStone;
		offsetOfSand=				v.offsetOfSand;
		offsetOfWater=				v.offsetOfWater;
		offsetOfLeafs=				v.offsetOfLeafs;
		offsetOfWood=				v.offsetOfWood;
		offsetOfSnow=				v.offsetOfSnow;
	}
	
	return *this;
}

VBOBoxTransferInfo updateGraphicsArraysForChunk(SimpleArray<GLint,SIZE_OF_VERTICES_FINAL_ARRAYS> &verticesFinal
	,SimpleArray<GLint,SIZE_OF_VERTICES_FINAL_ARRAYS> &normalsFinal
	,SimpleArray<GLfloat,SIZE_OF_TEXTURES_FINAL_ARRAYS> &texturesFinal,
	GameMain* gameMain,Chunk* _chunkPtr,int chNumX,int chNumZ,bool isPreloading,
	Chunk* chBackPreloaded /*= nullptr*/,Chunk* chFrontPreloaded /*= nullptr*/,
	Chunk* chLeftPreloaded /*= nullptr*/,Chunk* chRightPreloaded /*= nullptr*/){
		//проверим исходные данные согласно логике
		if(isPreloading && (chBackPreloaded == nullptr ||
			chRightPreloaded == nullptr ||
			chFrontPreloaded == nullptr ||
			chLeftPreloaded == nullptr)){
				//если поступили неверные данные
				qDebug()<<"Wrong input args were found in function updateGraphicsArraysForChunk of VBOBox.cpp"
					<<"The next work will be not right";
				while(true)
					qDebug()<<"Critical error!";
		}

		Chunk* chunkPtr=_chunkPtr;

		int pointsOfDirtToDraw=0;
		int pointsOfGrassTopToDraw=0;
		int pointsOfGrassSideToDraw=0;
		int pointsOfStoneToDraw=0;
		int pointsOfSandToDraw=0;
		int pointsOfWaterToDraw=0;
		int pointsOfLeafsToDraw=0;
		int pointsOfWoodToDraw=0;
		int pointsOfSnowToDraw=0;


		int offsetOfDirt=0;
		int offsetOfGrassTop=0;
		int offsetOfGrassSide=0;
		int offsetOfStone=0;
		int offsetOfSand=0;
		int offsetOfWater=0;
		int offsetOfLeafs=0;
		int offsetOfWood=0;
		int offsetOfSnow=0;

		SimpleArray<GLint,SIZE_OF_VERTICES_DIRT_ARRAYS> verticesDirt;
		SimpleArray<GLint,SIZE_OF_VERTICES_DIRT_ARRAYS> normalsDirt;
		SimpleArray<GLfloat,SIZE_OF_TEXTURES_DIRT_ARRAYS> texturesDirt;

		SimpleArray<GLint,SIZE_OF_VERTICES_GRASS_TOP_ARRAYS> verticesGrassTop;
		SimpleArray<GLint,SIZE_OF_VERTICES_GRASS_TOP_ARRAYS> normalsGrassTop;
		SimpleArray<GLfloat,SIZE_OF_TEXTURES_GRASS_TOP_ARRAYS> texturesGrassTop;

		SimpleArray<GLint,SIZE_OF_VERTICES_GRASS_SIDE_ARRAYS> verticesGrassSide;
		SimpleArray<GLint,SIZE_OF_VERTICES_GRASS_SIDE_ARRAYS> normalsGrassSide;
		SimpleArray<GLfloat,SIZE_OF_TEXTURES_GRASS_SIDE_ARRAYS> texturesGrassSide;

		SimpleArray<GLint,SIZE_OF_VERTICES_STONE_ARRAYS> verticesStone;
		SimpleArray<GLint,SIZE_OF_VERTICES_STONE_ARRAYS> normalsStone;
		SimpleArray<GLfloat,SIZE_OF_TEXTURES_STONE_ARRAYS> texturesStone;

		SimpleArray<GLint,SIZE_OF_VERTICES_SAND_ARRAYS> verticesSand;
		SimpleArray<GLint,SIZE_OF_VERTICES_SAND_ARRAYS> normalsSand;
		SimpleArray<GLfloat,SIZE_OF_TEXTURES_SAND_ARRAYS> texturesSand;

		SimpleArray<GLint,SIZE_OF_VERTICES_WATER_ARRAYS> verticesWater;
		SimpleArray<GLint,SIZE_OF_VERTICES_WATER_ARRAYS> normalsWater;
		SimpleArray<GLfloat,SIZE_OF_TEXTURES_WATER_ARRAYS> texturesWater;

		SimpleArray<GLint,SIZE_OF_VERTICES_LEAFS_ARRAYS> verticesLeafs;
		SimpleArray<GLint,SIZE_OF_VERTICES_LEAFS_ARRAYS> normalsLeafs;
		SimpleArray<GLfloat,SIZE_OF_TEXTURES_LEAFS_ARRAYS> texturesLeafs;

		SimpleArray<GLint,SIZE_OF_VERTICES_WOOD_ARRAYS> verticesWood;
		SimpleArray<GLint,SIZE_OF_VERTICES_WOOD_ARRAYS> normalsWood;
		SimpleArray<GLfloat,SIZE_OF_TEXTURES_WOOD_ARRAYS> texturesWood;

		SimpleArray<GLint,SIZE_OF_VERTICES_SNOW_ARRAYS> verticesSnow;
		SimpleArray<GLint,SIZE_OF_VERTICES_SNOW_ARRAYS> normalsSnow;
		SimpleArray<GLfloat,SIZE_OF_TEXTURES_SNOW_ARRAYS> texturesSnow;

		//get chunk`s coordinates
		int chCoordX=chunkPtr->getCoordX();
		int chCoordZ=chunkPtr->getCoordZ();

		for(int blX=0; blX<BLOCK_COUNT; ++blX){
			for(int blZ=0; blZ<BLOCK_COUNT; ++blZ){
				for(int blY=0; blY<BLOCK_HEIGHT_COUNT; ++blY){
					//for every block in the chunk
					if(chunkPtr->blocks[blX][blY][blZ].getType()==AIR)
						continue;
					//get coordinates of the block
					int blCoordX=chCoordX+blX*CUBE_SIZE;
					int blCoordY=blY*CUBE_SIZE;
					int blCoordZ=chCoordZ+blZ*CUBE_SIZE;

					//get the transparency around the block
					BlockTransparencyAround blocksTransAround(true,true,true,true,true,true);//предварительное создание объекта
					if(!isPreloading){
						if(gameMain == nullptr){
							qDebug()<<"VBOBoxTransferInfo updateGraphicsArraysForChunk got nullptr as GameMain* when it was not preloading, check for mistakes! Program will be crashed";
							while(true)
								qDebug()<<"Critical error!";
						}
						blocksTransAround=gameMain->world->getBlockTransparencyAround(chNumX,chNumZ,blX,blY,blZ);
					}
					else{//isPreloading == true
						//создадим объект вручную
						blocksTransAround=BlockTransparencyAround(
							blY<BLOCK_HEIGHT_COUNT-1?chunkPtr->isTransparent(blX,blY+1,blZ):true,
							blY>0?chunkPtr->isTransparent(blX,blY-1,blZ):false,
							blX>0?chunkPtr->isTransparent(blX-1,blY,blZ):
							chLeftPreloaded->isTransparent(BLOCK_COUNT-1,blY,blZ),
							blX<BLOCK_COUNT-1?chunkPtr->isTransparent(blX+1,blY,blZ):
							chRightPreloaded->isTransparent(0,blY,blZ),
							blZ<BLOCK_COUNT-1?chunkPtr->isTransparent(blX,blY,blZ+1):
							chFrontPreloaded->isTransparent(blX,blY,0),
							blZ>0?chunkPtr->isTransparent(blX,blY,blZ-1):
							chBackPreloaded->isTransparent(blX,blY,BLOCK_COUNT-1)
							);
					}//инициализация проведена успешно

					switch (chunkPtr->blocks[blX][blY][blZ].getType())
					{
					case DIRT:
						//top
						if(blocksTransAround.top==true){
							verticesDirt.push_back(blCoordX);
							verticesDirt.push_back(blCoordY+CUBE_SIZE);
							verticesDirt.push_back(blCoordZ);
							texturesDirt.push_back(1.0);
							texturesDirt.push_back(0.0);
							normalsDirt.pushUpNormal();

							verticesDirt.push_back(blCoordX);
							verticesDirt.push_back(blCoordY+CUBE_SIZE);
							verticesDirt.push_back(blCoordZ+CUBE_SIZE);
							texturesDirt.push_back(1.0);
							texturesDirt.push_back(1.0);
							normalsDirt.pushUpNormal();

							verticesDirt.push_back(blCoordX+CUBE_SIZE);
							verticesDirt.push_back(blCoordY+CUBE_SIZE);
							verticesDirt.push_back(blCoordZ+CUBE_SIZE);
							texturesDirt.push_back(0.0);
							texturesDirt.push_back(1.0);
							normalsDirt.pushUpNormal();

							verticesDirt.push_back(blCoordX+CUBE_SIZE);
							verticesDirt.push_back(blCoordY+CUBE_SIZE);
							verticesDirt.push_back(blCoordZ);
							texturesDirt.push_back(0.0);
							texturesDirt.push_back(0.0);
							normalsDirt.pushUpNormal();
						}

						//down
						if(blocksTransAround.down==true){
							verticesDirt.push_back(blCoordX+CUBE_SIZE);
							verticesDirt.push_back(blCoordY);
							verticesDirt.push_back(blCoordZ);
							texturesDirt.push_back(1.0);
							texturesDirt.push_back(0.0);
							normalsDirt.pushDownNormal();

							verticesDirt.push_back(blCoordX+CUBE_SIZE);
							verticesDirt.push_back(blCoordY);
							verticesDirt.push_back(blCoordZ+CUBE_SIZE);
							texturesDirt.push_back(1.0);
							texturesDirt.push_back(1.0);
							normalsDirt.pushDownNormal();

							verticesDirt.push_back(blCoordX);
							verticesDirt.push_back(blCoordY);
							verticesDirt.push_back(blCoordZ+CUBE_SIZE);
							texturesDirt.push_back(0.0);
							texturesDirt.push_back(1.0);
							normalsDirt.pushDownNormal();

							verticesDirt.push_back(blCoordX);
							verticesDirt.push_back(blCoordY);
							verticesDirt.push_back(blCoordZ);
							texturesDirt.push_back(0.0);
							texturesDirt.push_back(0.0);
							normalsDirt.pushDownNormal();
						}

						//front
						if(blocksTransAround.front==true){
							verticesDirt.push_back(blCoordX+CUBE_SIZE);
							verticesDirt.push_back(blCoordY);
							verticesDirt.push_back(blCoordZ+CUBE_SIZE);
							texturesDirt.push_back(1.0);
							texturesDirt.push_back(0.0);
							normalsDirt.pushFrontNormal();

							verticesDirt.push_back(blCoordX+CUBE_SIZE);
							verticesDirt.push_back(blCoordY+CUBE_SIZE);
							verticesDirt.push_back(blCoordZ+CUBE_SIZE);
							texturesDirt.push_back(1.0);
							texturesDirt.push_back(1.0);
							normalsDirt.pushFrontNormal();

							verticesDirt.push_back(blCoordX);
							verticesDirt.push_back(blCoordY+CUBE_SIZE);
							verticesDirt.push_back(blCoordZ+CUBE_SIZE);
							texturesDirt.push_back(0.0);
							texturesDirt.push_back(1.0);
							normalsDirt.pushFrontNormal();

							verticesDirt.push_back(blCoordX);
							verticesDirt.push_back(blCoordY);
							verticesDirt.push_back(blCoordZ+CUBE_SIZE);
							texturesDirt.push_back(0.0);
							texturesDirt.push_back(0.0);
							normalsDirt.pushFrontNormal();
						}

						//back
						if(blocksTransAround.back==true){
							verticesDirt.push_back(blCoordX);
							verticesDirt.push_back(blCoordY);
							verticesDirt.push_back(blCoordZ);
							texturesDirt.push_back(1.0);
							texturesDirt.push_back(0.0);
							normalsDirt.pushBackNormal();

							verticesDirt.push_back(blCoordX);
							verticesDirt.push_back(blCoordY+CUBE_SIZE);
							verticesDirt.push_back(blCoordZ);
							texturesDirt.push_back(1.0);
							texturesDirt.push_back(1.0);
							normalsDirt.pushBackNormal();

							verticesDirt.push_back(blCoordX+CUBE_SIZE);
							verticesDirt.push_back(blCoordY+CUBE_SIZE);
							verticesDirt.push_back(blCoordZ);
							texturesDirt.push_back(0.0);
							texturesDirt.push_back(1.0);
							normalsDirt.pushBackNormal();

							verticesDirt.push_back(blCoordX+CUBE_SIZE);
							verticesDirt.push_back(blCoordY);
							verticesDirt.push_back(blCoordZ);
							texturesDirt.push_back(0.0);
							texturesDirt.push_back(0.0);
							normalsDirt.pushBackNormal();
						}

						//left
						if(blocksTransAround.left==true){
							verticesDirt.push_back(blCoordX);
							verticesDirt.push_back(blCoordY);
							verticesDirt.push_back(blCoordZ+CUBE_SIZE);
							texturesDirt.push_back(1.0);
							texturesDirt.push_back(0.0);
							normalsDirt.pushLeftNormal();

							verticesDirt.push_back(blCoordX);
							verticesDirt.push_back(blCoordY+CUBE_SIZE);
							verticesDirt.push_back(blCoordZ+CUBE_SIZE);
							texturesDirt.push_back(1.0);
							texturesDirt.push_back(1.0);
							normalsDirt.pushLeftNormal();

							verticesDirt.push_back(blCoordX);
							verticesDirt.push_back(blCoordY+CUBE_SIZE);
							verticesDirt.push_back(blCoordZ);
							texturesDirt.push_back(0.0);
							texturesDirt.push_back(1.0);
							normalsDirt.pushLeftNormal();

							verticesDirt.push_back(blCoordX);
							verticesDirt.push_back(blCoordY);
							verticesDirt.push_back(blCoordZ);
							texturesDirt.push_back(0.0);
							texturesDirt.push_back(0.0);
							normalsDirt.pushLeftNormal();
						}

						//right
						if(blocksTransAround.right==true){
							verticesDirt.push_back(blCoordX+CUBE_SIZE);
							verticesDirt.push_back(blCoordY);
							verticesDirt.push_back(blCoordZ);
							texturesDirt.push_back(1.0);
							texturesDirt.push_back(0.0);
							normalsDirt.pushRightNormal();

							verticesDirt.push_back(blCoordX+CUBE_SIZE);
							verticesDirt.push_back(blCoordY+CUBE_SIZE);
							verticesDirt.push_back(blCoordZ);
							texturesDirt.push_back(1.0);
							texturesDirt.push_back(1.0);
							normalsDirt.pushRightNormal();

							verticesDirt.push_back(blCoordX+CUBE_SIZE);
							verticesDirt.push_back(blCoordY+CUBE_SIZE);
							verticesDirt.push_back(blCoordZ+CUBE_SIZE);
							texturesDirt.push_back(0.0);
							texturesDirt.push_back(1.0);
							normalsDirt.pushRightNormal();

							verticesDirt.push_back(blCoordX+CUBE_SIZE);
							verticesDirt.push_back(blCoordY);
							verticesDirt.push_back(blCoordZ+CUBE_SIZE);
							texturesDirt.push_back(0.0);
							texturesDirt.push_back(0.0);
							normalsDirt.pushRightNormal();
						}
						break;
					case GRASS:
						//top
						if(blocksTransAround.top==true){
							verticesGrassTop.push_back(blCoordX);
							verticesGrassTop.push_back(blCoordY+CUBE_SIZE);
							verticesGrassTop.push_back(blCoordZ);
							texturesGrassTop.push_back(1.0);
							texturesGrassTop.push_back(0.0);
							normalsGrassTop.pushUpNormal();

							verticesGrassTop.push_back(blCoordX);
							verticesGrassTop.push_back(blCoordY+CUBE_SIZE);
							verticesGrassTop.push_back(blCoordZ+CUBE_SIZE);
							texturesGrassTop.push_back(1.0);
							texturesGrassTop.push_back(1.0);
							normalsGrassTop.pushUpNormal();

							verticesGrassTop.push_back(blCoordX+CUBE_SIZE);
							verticesGrassTop.push_back(blCoordY+CUBE_SIZE);
							verticesGrassTop.push_back(blCoordZ+CUBE_SIZE);
							texturesGrassTop.push_back(0.0);
							texturesGrassTop.push_back(1.0);
							normalsGrassTop.pushUpNormal();

							verticesGrassTop.push_back(blCoordX+CUBE_SIZE);
							verticesGrassTop.push_back(blCoordY+CUBE_SIZE);
							verticesGrassTop.push_back(blCoordZ);
							texturesGrassTop.push_back(0.0);
							texturesGrassTop.push_back(0.0);
							normalsGrassTop.pushUpNormal();
						}

						//down
						if(blocksTransAround.down==true){
							verticesDirt.push_back(blCoordX+CUBE_SIZE);
							verticesDirt.push_back(blCoordY);
							verticesDirt.push_back(blCoordZ);
							texturesDirt.push_back(1.0);
							texturesDirt.push_back(0.0);
							normalsDirt.pushDownNormal();

							verticesDirt.push_back(blCoordX+CUBE_SIZE);
							verticesDirt.push_back(blCoordY);
							verticesDirt.push_back(blCoordZ+CUBE_SIZE);
							texturesDirt.push_back(1.0);
							texturesDirt.push_back(1.0);
							normalsDirt.pushDownNormal();

							verticesDirt.push_back(blCoordX);
							verticesDirt.push_back(blCoordY);
							verticesDirt.push_back(blCoordZ+CUBE_SIZE);
							texturesDirt.push_back(0.0);
							texturesDirt.push_back(1.0);
							normalsDirt.pushDownNormal();

							verticesDirt.push_back(blCoordX);
							verticesDirt.push_back(blCoordY);
							verticesDirt.push_back(blCoordZ);
							texturesDirt.push_back(0.0);
							texturesDirt.push_back(0.0);
							normalsDirt.pushDownNormal();
						}

						//front		
						if(blocksTransAround.front==true){
							verticesGrassSide.push_back(blCoordX+CUBE_SIZE);
							verticesGrassSide.push_back(blCoordY);
							verticesGrassSide.push_back(blCoordZ+CUBE_SIZE);
							texturesGrassSide.push_back(1.0);
							texturesGrassSide.push_back(0.0);
							normalsGrassSide.pushFrontNormal();

							verticesGrassSide.push_back(blCoordX+CUBE_SIZE);
							verticesGrassSide.push_back(blCoordY+CUBE_SIZE);
							verticesGrassSide.push_back(blCoordZ+CUBE_SIZE);
							texturesGrassSide.push_back(1.0);
							texturesGrassSide.push_back(1.0);
							normalsGrassSide.pushFrontNormal();

							verticesGrassSide.push_back(blCoordX);
							verticesGrassSide.push_back(blCoordY+CUBE_SIZE);
							verticesGrassSide.push_back(blCoordZ+CUBE_SIZE);
							texturesGrassSide.push_back(0.0);
							texturesGrassSide.push_back(1.0);
							normalsGrassSide.pushFrontNormal();

							verticesGrassSide.push_back(blCoordX);
							verticesGrassSide.push_back(blCoordY);
							verticesGrassSide.push_back(blCoordZ+CUBE_SIZE);
							texturesGrassSide.push_back(0.0);
							texturesGrassSide.push_back(0.0);
							normalsGrassSide.pushFrontNormal();
						}			

						//back		
						if(blocksTransAround.back==true){
							verticesGrassSide.push_back(blCoordX);
							verticesGrassSide.push_back(blCoordY);
							verticesGrassSide.push_back(blCoordZ);
							texturesGrassSide.push_back(1.0);
							texturesGrassSide.push_back(0.0);
							normalsGrassSide.pushBackNormal();

							verticesGrassSide.push_back(blCoordX);
							verticesGrassSide.push_back(blCoordY+CUBE_SIZE);
							verticesGrassSide.push_back(blCoordZ);
							texturesGrassSide.push_back(1.0);
							texturesGrassSide.push_back(1.0);
							normalsGrassSide.pushBackNormal();

							verticesGrassSide.push_back(blCoordX+CUBE_SIZE);
							verticesGrassSide.push_back(blCoordY+CUBE_SIZE);
							verticesGrassSide.push_back(blCoordZ);
							texturesGrassSide.push_back(0.0);
							texturesGrassSide.push_back(1.0);
							normalsGrassSide.pushBackNormal();

							verticesGrassSide.push_back(blCoordX+CUBE_SIZE);
							verticesGrassSide.push_back(blCoordY);
							verticesGrassSide.push_back(blCoordZ);
							texturesGrassSide.push_back(0.0);
							texturesGrassSide.push_back(0.0);
							normalsGrassSide.pushBackNormal();
						}

						//left
						if(blocksTransAround.left==true){
							verticesGrassSide.push_back(blCoordX);
							verticesGrassSide.push_back(blCoordY);
							verticesGrassSide.push_back(blCoordZ+CUBE_SIZE);
							texturesGrassSide.push_back(1.0);
							texturesGrassSide.push_back(0.0);
							normalsGrassSide.pushLeftNormal();

							verticesGrassSide.push_back(blCoordX);
							verticesGrassSide.push_back(blCoordY+CUBE_SIZE);
							verticesGrassSide.push_back(blCoordZ+CUBE_SIZE);
							texturesGrassSide.push_back(1.0);
							texturesGrassSide.push_back(1.0);
							normalsGrassSide.pushLeftNormal();

							verticesGrassSide.push_back(blCoordX);
							verticesGrassSide.push_back(blCoordY+CUBE_SIZE);
							verticesGrassSide.push_back(blCoordZ);
							texturesGrassSide.push_back(0.0);
							texturesGrassSide.push_back(1.0);
							normalsGrassSide.pushLeftNormal();

							verticesGrassSide.push_back(blCoordX);
							verticesGrassSide.push_back(blCoordY);
							verticesGrassSide.push_back(blCoordZ);
							texturesGrassSide.push_back(0.0);
							texturesGrassSide.push_back(0.0);
							normalsGrassSide.pushLeftNormal();
						}			

						//right		
						if(blocksTransAround.right==true){
							verticesGrassSide.push_back(blCoordX+CUBE_SIZE);
							verticesGrassSide.push_back(blCoordY);
							verticesGrassSide.push_back(blCoordZ);
							texturesGrassSide.push_back(1.0);
							texturesGrassSide.push_back(0.0);
							normalsGrassSide.pushRightNormal();

							verticesGrassSide.push_back(blCoordX+CUBE_SIZE);
							verticesGrassSide.push_back(blCoordY+CUBE_SIZE);
							verticesGrassSide.push_back(blCoordZ);
							texturesGrassSide.push_back(1.0);
							texturesGrassSide.push_back(1.0);
							normalsGrassSide.pushRightNormal();

							verticesGrassSide.push_back(blCoordX+CUBE_SIZE);
							verticesGrassSide.push_back(blCoordY+CUBE_SIZE);
							verticesGrassSide.push_back(blCoordZ+CUBE_SIZE);
							texturesGrassSide.push_back(0.0);
							texturesGrassSide.push_back(1.0);
							normalsGrassSide.pushRightNormal();

							verticesGrassSide.push_back(blCoordX+CUBE_SIZE);
							verticesGrassSide.push_back(blCoordY);
							verticesGrassSide.push_back(blCoordZ+CUBE_SIZE);
							texturesGrassSide.push_back(0.0);
							texturesGrassSide.push_back(0.0);
							normalsGrassSide.pushRightNormal();
						}
						break;
					case STONE:
						//top
						if(blocksTransAround.top==true){
							verticesStone.push_back(blCoordX);
							verticesStone.push_back(blCoordY+CUBE_SIZE);
							verticesStone.push_back(blCoordZ);
							texturesStone.push_back(1.0);
							texturesStone.push_back(0.0);
							normalsStone.pushUpNormal();

							verticesStone.push_back(blCoordX);
							verticesStone.push_back(blCoordY+CUBE_SIZE);
							verticesStone.push_back(blCoordZ+CUBE_SIZE);
							texturesStone.push_back(1.0);
							texturesStone.push_back(1.0);
							normalsStone.pushUpNormal();

							verticesStone.push_back(blCoordX+CUBE_SIZE);
							verticesStone.push_back(blCoordY+CUBE_SIZE);
							verticesStone.push_back(blCoordZ+CUBE_SIZE);
							texturesStone.push_back(0.0);
							texturesStone.push_back(1.0);
							normalsStone.pushUpNormal();

							verticesStone.push_back(blCoordX+CUBE_SIZE);
							verticesStone.push_back(blCoordY+CUBE_SIZE);
							verticesStone.push_back(blCoordZ);
							texturesStone.push_back(0.0);
							texturesStone.push_back(0.0);
							normalsStone.pushUpNormal();
						}

						//down
						if(blocksTransAround.down==true){
							verticesStone.push_back(blCoordX+CUBE_SIZE);
							verticesStone.push_back(blCoordY);
							verticesStone.push_back(blCoordZ);
							texturesStone.push_back(1.0);
							texturesStone.push_back(0.0);
							normalsStone.pushDownNormal();

							verticesStone.push_back(blCoordX+CUBE_SIZE);
							verticesStone.push_back(blCoordY);
							verticesStone.push_back(blCoordZ+CUBE_SIZE);
							texturesStone.push_back(1.0);
							texturesStone.push_back(1.0);
							normalsStone.pushDownNormal();

							verticesStone.push_back(blCoordX);
							verticesStone.push_back(blCoordY);
							verticesStone.push_back(blCoordZ+CUBE_SIZE);
							texturesStone.push_back(0.0);
							texturesStone.push_back(1.0);
							normalsStone.pushDownNormal();

							verticesStone.push_back(blCoordX);
							verticesStone.push_back(blCoordY);
							verticesStone.push_back(blCoordZ);
							texturesStone.push_back(0.0);
							texturesStone.push_back(0.0);
							normalsStone.pushDownNormal();
						}

						//front
						if(blocksTransAround.front==true){
							verticesStone.push_back(blCoordX+CUBE_SIZE);
							verticesStone.push_back(blCoordY);
							verticesStone.push_back(blCoordZ+CUBE_SIZE);
							texturesStone.push_back(1.0);
							texturesStone.push_back(0.0);
							normalsStone.pushFrontNormal();

							verticesStone.push_back(blCoordX+CUBE_SIZE);
							verticesStone.push_back(blCoordY+CUBE_SIZE);
							verticesStone.push_back(blCoordZ+CUBE_SIZE);
							texturesStone.push_back(1.0);
							texturesStone.push_back(1.0);
							normalsStone.pushFrontNormal();

							verticesStone.push_back(blCoordX);
							verticesStone.push_back(blCoordY+CUBE_SIZE);
							verticesStone.push_back(blCoordZ+CUBE_SIZE);
							texturesStone.push_back(0.0);
							texturesStone.push_back(1.0);
							normalsStone.pushFrontNormal();

							verticesStone.push_back(blCoordX);
							verticesStone.push_back(blCoordY);
							verticesStone.push_back(blCoordZ+CUBE_SIZE);
							texturesStone.push_back(0.0);
							texturesStone.push_back(0.0);
							normalsStone.pushFrontNormal();
						}

						//back
						if(blocksTransAround.back==true){
							verticesStone.push_back(blCoordX);
							verticesStone.push_back(blCoordY);
							verticesStone.push_back(blCoordZ);
							texturesStone.push_back(1.0);
							texturesStone.push_back(0.0);
							normalsStone.pushBackNormal();

							verticesStone.push_back(blCoordX);
							verticesStone.push_back(blCoordY+CUBE_SIZE);
							verticesStone.push_back(blCoordZ);
							texturesStone.push_back(1.0);
							texturesStone.push_back(1.0);
							normalsStone.pushBackNormal();

							verticesStone.push_back(blCoordX+CUBE_SIZE);
							verticesStone.push_back(blCoordY+CUBE_SIZE);
							verticesStone.push_back(blCoordZ);
							texturesStone.push_back(0.0);
							texturesStone.push_back(1.0);
							normalsStone.pushBackNormal();

							verticesStone.push_back(blCoordX+CUBE_SIZE);
							verticesStone.push_back(blCoordY);
							verticesStone.push_back(blCoordZ);
							texturesStone.push_back(0.0);
							texturesStone.push_back(0.0);
							normalsStone.pushBackNormal();
						}

						//left
						if(blocksTransAround.left==true){
							verticesStone.push_back(blCoordX);
							verticesStone.push_back(blCoordY);
							verticesStone.push_back(blCoordZ+CUBE_SIZE);
							texturesStone.push_back(1.0);
							texturesStone.push_back(0.0);
							normalsStone.pushLeftNormal();

							verticesStone.push_back(blCoordX);
							verticesStone.push_back(blCoordY+CUBE_SIZE);
							verticesStone.push_back(blCoordZ+CUBE_SIZE);
							texturesStone.push_back(1.0);
							texturesStone.push_back(1.0);
							normalsStone.pushLeftNormal();

							verticesStone.push_back(blCoordX);
							verticesStone.push_back(blCoordY+CUBE_SIZE);
							verticesStone.push_back(blCoordZ);
							texturesStone.push_back(0.0);
							texturesStone.push_back(1.0);
							normalsStone.pushLeftNormal();

							verticesStone.push_back(blCoordX);
							verticesStone.push_back(blCoordY);
							verticesStone.push_back(blCoordZ);
							texturesStone.push_back(0.0);
							texturesStone.push_back(0.0);
							normalsStone.pushLeftNormal();
						}

						//right
						if(blocksTransAround.right==true){
							verticesStone.push_back(blCoordX+CUBE_SIZE);
							verticesStone.push_back(blCoordY);
							verticesStone.push_back(blCoordZ);
							texturesStone.push_back(1.0);
							texturesStone.push_back(0.0);
							normalsStone.pushRightNormal();

							verticesStone.push_back(blCoordX+CUBE_SIZE);
							verticesStone.push_back(blCoordY+CUBE_SIZE);
							verticesStone.push_back(blCoordZ);
							texturesStone.push_back(1.0);
							texturesStone.push_back(1.0);
							normalsStone.pushRightNormal();

							verticesStone.push_back(blCoordX+CUBE_SIZE);
							verticesStone.push_back(blCoordY+CUBE_SIZE);
							verticesStone.push_back(blCoordZ+CUBE_SIZE);
							texturesStone.push_back(0.0);
							texturesStone.push_back(1.0);
							normalsStone.pushRightNormal();

							verticesStone.push_back(blCoordX+CUBE_SIZE);
							verticesStone.push_back(blCoordY);
							verticesStone.push_back(blCoordZ+CUBE_SIZE);
							texturesStone.push_back(0.0);
							texturesStone.push_back(0.0);
							normalsStone.pushRightNormal();
						}
						break;
					case SAND:
						//top
						if(blocksTransAround.top==true){
							verticesSand.push_back(blCoordX);
							verticesSand.push_back(blCoordY+CUBE_SIZE);
							verticesSand.push_back(blCoordZ);
							texturesSand.push_back(1.0);
							texturesSand.push_back(0.0);
							normalsSand.pushUpNormal();

							verticesSand.push_back(blCoordX);
							verticesSand.push_back(blCoordY+CUBE_SIZE);
							verticesSand.push_back(blCoordZ+CUBE_SIZE);
							texturesSand.push_back(1.0);
							texturesSand.push_back(1.0);
							normalsSand.pushUpNormal();

							verticesSand.push_back(blCoordX+CUBE_SIZE);
							verticesSand.push_back(blCoordY+CUBE_SIZE);
							verticesSand.push_back(blCoordZ+CUBE_SIZE);
							texturesSand.push_back(0.0);
							texturesSand.push_back(1.0);
							normalsSand.pushUpNormal();

							verticesSand.push_back(blCoordX+CUBE_SIZE);
							verticesSand.push_back(blCoordY+CUBE_SIZE);
							verticesSand.push_back(blCoordZ);
							texturesSand.push_back(0.0);
							texturesSand.push_back(0.0);
							normalsSand.pushUpNormal();
						}

						//down
						if(blocksTransAround.down==true){
							verticesSand.push_back(blCoordX+CUBE_SIZE);
							verticesSand.push_back(blCoordY);
							verticesSand.push_back(blCoordZ);
							texturesSand.push_back(1.0);
							texturesSand.push_back(0.0);
							normalsSand.pushDownNormal();

							verticesSand.push_back(blCoordX+CUBE_SIZE);
							verticesSand.push_back(blCoordY);
							verticesSand.push_back(blCoordZ+CUBE_SIZE);
							texturesSand.push_back(1.0);
							texturesSand.push_back(1.0);
							normalsSand.pushDownNormal();

							verticesSand.push_back(blCoordX);
							verticesSand.push_back(blCoordY);
							verticesSand.push_back(blCoordZ+CUBE_SIZE);
							texturesSand.push_back(0.0);
							texturesSand.push_back(1.0);
							normalsSand.pushDownNormal();

							verticesSand.push_back(blCoordX);
							verticesSand.push_back(blCoordY);
							verticesSand.push_back(blCoordZ);
							texturesSand.push_back(0.0);
							texturesSand.push_back(0.0);
							normalsSand.pushDownNormal();
						}

						//front
						if(blocksTransAround.front==true){
							verticesSand.push_back(blCoordX+CUBE_SIZE);
							verticesSand.push_back(blCoordY);
							verticesSand.push_back(blCoordZ+CUBE_SIZE);
							texturesSand.push_back(1.0);
							texturesSand.push_back(0.0);
							normalsSand.pushFrontNormal();

							verticesSand.push_back(blCoordX+CUBE_SIZE);
							verticesSand.push_back(blCoordY+CUBE_SIZE);
							verticesSand.push_back(blCoordZ+CUBE_SIZE);
							texturesSand.push_back(1.0);
							texturesSand.push_back(1.0);
							normalsSand.pushFrontNormal();

							verticesSand.push_back(blCoordX);
							verticesSand.push_back(blCoordY+CUBE_SIZE);
							verticesSand.push_back(blCoordZ+CUBE_SIZE);
							texturesSand.push_back(0.0);
							texturesSand.push_back(1.0);
							normalsSand.pushFrontNormal();

							verticesSand.push_back(blCoordX);
							verticesSand.push_back(blCoordY);
							verticesSand.push_back(blCoordZ+CUBE_SIZE);
							texturesSand.push_back(0.0);
							texturesSand.push_back(0.0);
							normalsSand.pushFrontNormal();
						}

						//back
						if(blocksTransAround.back==true){
							verticesSand.push_back(blCoordX);
							verticesSand.push_back(blCoordY);
							verticesSand.push_back(blCoordZ);
							texturesSand.push_back(1.0);
							texturesSand.push_back(0.0);
							normalsSand.pushBackNormal();

							verticesSand.push_back(blCoordX);
							verticesSand.push_back(blCoordY+CUBE_SIZE);
							verticesSand.push_back(blCoordZ);
							texturesSand.push_back(1.0);
							texturesSand.push_back(1.0);
							normalsSand.pushBackNormal();

							verticesSand.push_back(blCoordX+CUBE_SIZE);
							verticesSand.push_back(blCoordY+CUBE_SIZE);
							verticesSand.push_back(blCoordZ);
							texturesSand.push_back(0.0);
							texturesSand.push_back(1.0);
							normalsSand.pushBackNormal();

							verticesSand.push_back(blCoordX+CUBE_SIZE);
							verticesSand.push_back(blCoordY);
							verticesSand.push_back(blCoordZ);
							texturesSand.push_back(0.0);
							texturesSand.push_back(0.0);
							normalsSand.pushBackNormal();
						}

						//left
						if(blocksTransAround.left==true){
							verticesSand.push_back(blCoordX);
							verticesSand.push_back(blCoordY);
							verticesSand.push_back(blCoordZ+CUBE_SIZE);
							texturesSand.push_back(1.0);
							texturesSand.push_back(0.0);
							normalsSand.pushLeftNormal();

							verticesSand.push_back(blCoordX);
							verticesSand.push_back(blCoordY+CUBE_SIZE);
							verticesSand.push_back(blCoordZ+CUBE_SIZE);
							texturesSand.push_back(1.0);
							texturesSand.push_back(1.0);
							normalsSand.pushBackNormal();

							verticesSand.push_back(blCoordX);
							verticesSand.push_back(blCoordY+CUBE_SIZE);
							verticesSand.push_back(blCoordZ);
							texturesSand.push_back(0.0);
							texturesSand.push_back(1.0);
							normalsSand.pushBackNormal();

							verticesSand.push_back(blCoordX);
							verticesSand.push_back(blCoordY);
							verticesSand.push_back(blCoordZ);
							texturesSand.push_back(0.0);
							texturesSand.push_back(0.0);
							normalsSand.pushBackNormal();
						}

						//right
						if(blocksTransAround.right==true){
							verticesSand.push_back(blCoordX+CUBE_SIZE);
							verticesSand.push_back(blCoordY);
							verticesSand.push_back(blCoordZ);
							texturesSand.push_back(1.0);
							texturesSand.push_back(0.0);
							normalsSand.pushRightNormal();

							verticesSand.push_back(blCoordX+CUBE_SIZE);
							verticesSand.push_back(blCoordY+CUBE_SIZE);
							verticesSand.push_back(blCoordZ);
							texturesSand.push_back(1.0);
							texturesSand.push_back(1.0);
							normalsSand.pushRightNormal();

							verticesSand.push_back(blCoordX+CUBE_SIZE);
							verticesSand.push_back(blCoordY+CUBE_SIZE);
							verticesSand.push_back(blCoordZ+CUBE_SIZE);
							texturesSand.push_back(0.0);
							texturesSand.push_back(1.0);
							normalsSand.pushRightNormal();

							verticesSand.push_back(blCoordX+CUBE_SIZE);
							verticesSand.push_back(blCoordY);
							verticesSand.push_back(blCoordZ+CUBE_SIZE);
							texturesSand.push_back(0.0);
							texturesSand.push_back(0.0);
							normalsSand.pushRightNormal();
						}
						break;
					case WATER:
						//top
						if(blocksTransAround.top==true){
							verticesWater.push_back(blCoordX);
							verticesWater.push_back(blCoordY+CUBE_SIZE);
							verticesWater.push_back(blCoordZ);
							texturesWater.push_back(1.0);
							texturesWater.push_back(0.0);
							normalsWater.pushUpNormal();
									
							verticesWater.push_back(blCoordX);
							verticesWater.push_back(blCoordY+CUBE_SIZE);
							verticesWater.push_back(blCoordZ+CUBE_SIZE);
							texturesWater.push_back(1.0);
							texturesWater.push_back(1.0);
							normalsWater.pushUpNormal();
									
							verticesWater.push_back(blCoordX+CUBE_SIZE);
							verticesWater.push_back(blCoordY+CUBE_SIZE);
							verticesWater.push_back(blCoordZ+CUBE_SIZE);
							texturesWater.push_back(0.0);
							texturesWater.push_back(1.0);
							normalsWater.pushUpNormal();
									
							verticesWater.push_back(blCoordX+CUBE_SIZE);
							verticesWater.push_back(blCoordY+CUBE_SIZE);
							verticesWater.push_back(blCoordZ);
							texturesWater.push_back(0.0);
							texturesWater.push_back(0.0);
							normalsWater.pushUpNormal();
						}

						//down
						if(blocksTransAround.down==true){
							verticesWater.push_back(blCoordX+CUBE_SIZE);
							verticesWater.push_back(blCoordY);
							verticesWater.push_back(blCoordZ);
							texturesWater.push_back(1.0);
							texturesWater.push_back(0.0);
							normalsWater.pushDownNormal();
									
							verticesWater.push_back(blCoordX+CUBE_SIZE);
							verticesWater.push_back(blCoordY);
							verticesWater.push_back(blCoordZ+CUBE_SIZE);
							texturesWater.push_back(1.0);
							texturesWater.push_back(1.0);
							normalsWater.pushDownNormal();
									
							verticesWater.push_back(blCoordX);
							verticesWater.push_back(blCoordY);
							verticesWater.push_back(blCoordZ+CUBE_SIZE);
							texturesWater.push_back(0.0);
							texturesWater.push_back(1.0);
							normalsWater.pushDownNormal();
									
							verticesWater.push_back(blCoordX);
							verticesWater.push_back(blCoordY);
							verticesWater.push_back(blCoordZ);
							texturesWater.push_back(0.0);
							texturesWater.push_back(0.0);
							normalsWater.pushDownNormal();
						}

						//front
						if(blocksTransAround.front==true){
							verticesWater.push_back(blCoordX+CUBE_SIZE);
							verticesWater.push_back(blCoordY);
							verticesWater.push_back(blCoordZ+CUBE_SIZE);
							texturesWater.push_back(1.0);
							texturesWater.push_back(0.0);
							normalsWater.pushFrontNormal();
									
							verticesWater.push_back(blCoordX+CUBE_SIZE);
							verticesWater.push_back(blCoordY+CUBE_SIZE);
							verticesWater.push_back(blCoordZ+CUBE_SIZE);
							texturesWater.push_back(1.0);
							texturesWater.push_back(1.0);
							normalsWater.pushFrontNormal();
									
							verticesWater.push_back(blCoordX);
							verticesWater.push_back(blCoordY+CUBE_SIZE);
							verticesWater.push_back(blCoordZ+CUBE_SIZE);
							texturesWater.push_back(0.0);
							texturesWater.push_back(1.0);
							normalsWater.pushFrontNormal();
									
							verticesWater.push_back(blCoordX);
							verticesWater.push_back(blCoordY);
							verticesWater.push_back(blCoordZ+CUBE_SIZE);
							texturesWater.push_back(0.0);
							texturesWater.push_back(0.0);
							normalsWater.pushFrontNormal();
						}

						//back
						if(blocksTransAround.back==true){
							verticesWater.push_back(blCoordX);
							verticesWater.push_back(blCoordY);
							verticesWater.push_back(blCoordZ);
							texturesWater.push_back(1.0);
							texturesWater.push_back(0.0);
							normalsWater.pushBackNormal();
	
							verticesWater.push_back(blCoordX);
							verticesWater.push_back(blCoordY+CUBE_SIZE);
							verticesWater.push_back(blCoordZ);
							texturesWater.push_back(1.0);
							texturesWater.push_back(1.0);
							normalsWater.pushBackNormal();
		
							verticesWater.push_back(blCoordX+CUBE_SIZE);
							verticesWater.push_back(blCoordY+CUBE_SIZE);
							verticesWater.push_back(blCoordZ);
							texturesWater.push_back(0.0);
							texturesWater.push_back(1.0);
							normalsWater.pushBackNormal();
			
							verticesWater.push_back(blCoordX+CUBE_SIZE);
							verticesWater.push_back(blCoordY);
							verticesWater.push_back(blCoordZ);
							texturesWater.push_back(0.0);
							texturesWater.push_back(0.0);
							normalsWater.pushBackNormal();
						}

						//left
						if(blocksTransAround.left==true){
							verticesWater.push_back(blCoordX);
							verticesWater.push_back(blCoordY);
							verticesWater.push_back(blCoordZ+CUBE_SIZE);
							texturesWater.push_back(1.0);
							texturesWater.push_back(0.0);
							normalsWater.pushLeftNormal();
									
							verticesWater.push_back(blCoordX);
							verticesWater.push_back(blCoordY+CUBE_SIZE);
							verticesWater.push_back(blCoordZ+CUBE_SIZE);
							texturesWater.push_back(1.0);
							texturesWater.push_back(1.0);
							normalsWater.pushLeftNormal();
									
							verticesWater.push_back(blCoordX);
							verticesWater.push_back(blCoordY+CUBE_SIZE);
							verticesWater.push_back(blCoordZ);
							texturesWater.push_back(0.0);
							texturesWater.push_back(1.0);
							normalsWater.pushLeftNormal();
									
							verticesWater.push_back(blCoordX);
							verticesWater.push_back(blCoordY);
							verticesWater.push_back(blCoordZ);
							texturesWater.push_back(0.0);
							texturesWater.push_back(0.0);
							normalsWater.pushLeftNormal();
						}

						//right
						if(blocksTransAround.right==true){
							verticesWater.push_back(blCoordX+CUBE_SIZE);
							verticesWater.push_back(blCoordY);
							verticesWater.push_back(blCoordZ);
							texturesWater.push_back(1.0);
							texturesWater.push_back(0.0);
							normalsWater.pushRightNormal();
									
							verticesWater.push_back(blCoordX+CUBE_SIZE);
							verticesWater.push_back(blCoordY+CUBE_SIZE);
							verticesWater.push_back(blCoordZ);
							texturesWater.push_back(1.0);
							texturesWater.push_back(1.0);
							normalsWater.pushRightNormal();
							
							verticesWater.push_back(blCoordX+CUBE_SIZE);
							verticesWater.push_back(blCoordY+CUBE_SIZE);
							verticesWater.push_back(blCoordZ+CUBE_SIZE);
							texturesWater.push_back(0.0);
							texturesWater.push_back(1.0);
							normalsWater.pushRightNormal();
									
							verticesWater.push_back(blCoordX+CUBE_SIZE);
							verticesWater.push_back(blCoordY);
							verticesWater.push_back(blCoordZ+CUBE_SIZE);
							texturesWater.push_back(0.0);
							texturesWater.push_back(0.0);
							normalsWater.pushRightNormal();
						}
						break;
					case LEAFS:
						//top
						if(blocksTransAround.top==true){
							verticesLeafs.push_back(blCoordX);
							verticesLeafs.push_back(blCoordY+CUBE_SIZE);
							verticesLeafs.push_back(blCoordZ);
							texturesLeafs.push_back(1.0);
							texturesLeafs.push_back(0.0);
							normalsLeafs.pushUpNormal();
							
							verticesLeafs.push_back(blCoordX);
							verticesLeafs.push_back(blCoordY+CUBE_SIZE);
							verticesLeafs.push_back(blCoordZ+CUBE_SIZE);
							texturesLeafs.push_back(1.0);
							texturesLeafs.push_back(1.0);
							normalsLeafs.pushUpNormal();
						
							verticesLeafs.push_back(blCoordX+CUBE_SIZE);
							verticesLeafs.push_back(blCoordY+CUBE_SIZE);
							verticesLeafs.push_back(blCoordZ+CUBE_SIZE);
							texturesLeafs.push_back(0.0);
							texturesLeafs.push_back(1.0);
							normalsLeafs.pushUpNormal();
							
							verticesLeafs.push_back(blCoordX+CUBE_SIZE);
							verticesLeafs.push_back(blCoordY+CUBE_SIZE);
							verticesLeafs.push_back(blCoordZ);
							texturesLeafs.push_back(0.0);
							texturesLeafs.push_back(0.0);
							normalsLeafs.pushUpNormal();
						}

						//down
						if(blocksTransAround.down==true){
							verticesLeafs.push_back(blCoordX+CUBE_SIZE);
							verticesLeafs.push_back(blCoordY);
							verticesLeafs.push_back(blCoordZ);
							texturesLeafs.push_back(1.0);
							texturesLeafs.push_back(0.0);
							normalsLeafs.pushDownNormal();

							verticesLeafs.push_back(blCoordX+CUBE_SIZE);
							verticesLeafs.push_back(blCoordY);
							verticesLeafs.push_back(blCoordZ+CUBE_SIZE);
							texturesLeafs.push_back(1.0);
							texturesLeafs.push_back(1.0);
							normalsLeafs.pushDownNormal();

							verticesLeafs.push_back(blCoordX);
							verticesLeafs.push_back(blCoordY);
							verticesLeafs.push_back(blCoordZ+CUBE_SIZE);
							texturesLeafs.push_back(0.0);
							texturesLeafs.push_back(1.0);
							normalsLeafs.pushDownNormal();

							verticesLeafs.push_back(blCoordX);
							verticesLeafs.push_back(blCoordY);
							verticesLeafs.push_back(blCoordZ);
							texturesLeafs.push_back(0.0);
							texturesLeafs.push_back(0.0);
							normalsLeafs.pushDownNormal();
						}

						//front
						if(blocksTransAround.front==true){
							verticesLeafs.push_back(blCoordX+CUBE_SIZE);
							verticesLeafs.push_back(blCoordY);
							verticesLeafs.push_back(blCoordZ+CUBE_SIZE);
							texturesLeafs.push_back(1.0);
							texturesLeafs.push_back(0.0);
							normalsLeafs.pushFrontNormal();

							verticesLeafs.push_back(blCoordX+CUBE_SIZE);
							verticesLeafs.push_back(blCoordY+CUBE_SIZE);
							verticesLeafs.push_back(blCoordZ+CUBE_SIZE);
							texturesLeafs.push_back(1.0);
							texturesLeafs.push_back(1.0);
							normalsLeafs.pushFrontNormal();

							verticesLeafs.push_back(blCoordX);
							verticesLeafs.push_back(blCoordY+CUBE_SIZE);
							verticesLeafs.push_back(blCoordZ+CUBE_SIZE);
							texturesLeafs.push_back(0.0);
							texturesLeafs.push_back(1.0);
							normalsLeafs.pushFrontNormal();

							verticesLeafs.push_back(blCoordX);
							verticesLeafs.push_back(blCoordY);
							verticesLeafs.push_back(blCoordZ+CUBE_SIZE);
							texturesLeafs.push_back(0.0);
							texturesLeafs.push_back(0.0);
							normalsLeafs.pushFrontNormal();
						}

						//back
						if(blocksTransAround.back==true){
							verticesLeafs.push_back(blCoordX);
							verticesLeafs.push_back(blCoordY);
							verticesLeafs.push_back(blCoordZ);
							texturesLeafs.push_back(1.0);
							texturesLeafs.push_back(0.0);
							normalsLeafs.pushBackNormal();

							verticesLeafs.push_back(blCoordX);
							verticesLeafs.push_back(blCoordY+CUBE_SIZE);
							verticesLeafs.push_back(blCoordZ);
							texturesLeafs.push_back(1.0);
							texturesLeafs.push_back(1.0);
							normalsLeafs.pushBackNormal();

							verticesLeafs.push_back(blCoordX+CUBE_SIZE);
							verticesLeafs.push_back(blCoordY+CUBE_SIZE);
							verticesLeafs.push_back(blCoordZ);
							texturesLeafs.push_back(0.0);
							texturesLeafs.push_back(1.0);
							normalsLeafs.pushBackNormal();

							verticesLeafs.push_back(blCoordX+CUBE_SIZE);
							verticesLeafs.push_back(blCoordY);
							verticesLeafs.push_back(blCoordZ);
							texturesLeafs.push_back(0.0);
							texturesLeafs.push_back(0.0);
							normalsLeafs.pushBackNormal();
						}

						//left
						if(blocksTransAround.left==true){
							verticesLeafs.push_back(blCoordX);
							verticesLeafs.push_back(blCoordY);
							verticesLeafs.push_back(blCoordZ+CUBE_SIZE);
							texturesLeafs.push_back(1.0);
							texturesLeafs.push_back(0.0);
							normalsLeafs.pushLeftNormal();

							verticesLeafs.push_back(blCoordX);
							verticesLeafs.push_back(blCoordY+CUBE_SIZE);
							verticesLeafs.push_back(blCoordZ+CUBE_SIZE);
							texturesLeafs.push_back(1.0);
							texturesLeafs.push_back(1.0);
							normalsLeafs.pushLeftNormal();

							verticesLeafs.push_back(blCoordX);
							verticesLeafs.push_back(blCoordY+CUBE_SIZE);
							verticesLeafs.push_back(blCoordZ);
							texturesLeafs.push_back(0.0);
							texturesLeafs.push_back(1.0);
							normalsLeafs.pushLeftNormal();

							verticesLeafs.push_back(blCoordX);
							verticesLeafs.push_back(blCoordY);
							verticesLeafs.push_back(blCoordZ);
							texturesLeafs.push_back(0.0);
							texturesLeafs.push_back(0.0);
							normalsLeafs.pushLeftNormal();
						}

						//right
						if(blocksTransAround.right==true){
							verticesLeafs.push_back(blCoordX+CUBE_SIZE);
							verticesLeafs.push_back(blCoordY);
							verticesLeafs.push_back(blCoordZ);
							texturesLeafs.push_back(1.0);
							texturesLeafs.push_back(0.0);
							normalsLeafs.pushRightNormal();

							verticesLeafs.push_back(blCoordX+CUBE_SIZE);
							verticesLeafs.push_back(blCoordY+CUBE_SIZE);
							verticesLeafs.push_back(blCoordZ);
							texturesLeafs.push_back(1.0);
							texturesLeafs.push_back(1.0);
							normalsLeafs.pushRightNormal();

							verticesLeafs.push_back(blCoordX+CUBE_SIZE);
							verticesLeafs.push_back(blCoordY+CUBE_SIZE);
							verticesLeafs.push_back(blCoordZ+CUBE_SIZE);
							texturesLeafs.push_back(0.0);
							texturesLeafs.push_back(1.0);
							normalsLeafs.pushRightNormal();

							verticesLeafs.push_back(blCoordX+CUBE_SIZE);
							verticesLeafs.push_back(blCoordY);
							verticesLeafs.push_back(blCoordZ+CUBE_SIZE);
							texturesLeafs.push_back(0.0);
							texturesLeafs.push_back(0.0);
							normalsLeafs.pushRightNormal();
						}
						break;
					case WOOD:
						//top
						if(blocksTransAround.top==true){
							verticesWood.push_back(blCoordX);
							verticesWood.push_back(blCoordY+CUBE_SIZE);
							verticesWood.push_back(blCoordZ);
							texturesWood.push_back(1.0);
							texturesWood.push_back(0.0);
							normalsWood.pushUpNormal();

							verticesWood.push_back(blCoordX);
							verticesWood.push_back(blCoordY+CUBE_SIZE);
							verticesWood.push_back(blCoordZ+CUBE_SIZE);
							texturesWood.push_back(1.0);
							texturesWood.push_back(1.0);
							normalsWood.pushUpNormal();

							verticesWood.push_back(blCoordX+CUBE_SIZE);
							verticesWood.push_back(blCoordY+CUBE_SIZE);
							verticesWood.push_back(blCoordZ+CUBE_SIZE);
							texturesWood.push_back(0.0);
							texturesWood.push_back(1.0);
							normalsWood.pushUpNormal();

							verticesWood.push_back(blCoordX+CUBE_SIZE);
							verticesWood.push_back(blCoordY+CUBE_SIZE);
							verticesWood.push_back(blCoordZ);
							texturesWood.push_back(0.0);
							texturesWood.push_back(0.0);
							normalsWood.pushUpNormal();
						}

						//down
						if(blocksTransAround.down==true){
							verticesWood.push_back(blCoordX+CUBE_SIZE);
							verticesWood.push_back(blCoordY);
							verticesWood.push_back(blCoordZ);
							texturesWood.push_back(1.0);
							texturesWood.push_back(0.0);
							normalsWood.pushDownNormal();

							verticesWood.push_back(blCoordX+CUBE_SIZE);
							verticesWood.push_back(blCoordY);
							verticesWood.push_back(blCoordZ+CUBE_SIZE);
							texturesWood.push_back(1.0);
							texturesWood.push_back(1.0);
							normalsWood.pushDownNormal();

							verticesWood.push_back(blCoordX);
							verticesWood.push_back(blCoordY);
							verticesWood.push_back(blCoordZ+CUBE_SIZE);
							texturesWood.push_back(0.0);
							texturesWood.push_back(1.0);
							normalsWood.pushDownNormal();

							verticesWood.push_back(blCoordX);
							verticesWood.push_back(blCoordY);
							verticesWood.push_back(blCoordZ);
							texturesWood.push_back(0.0);
							texturesWood.push_back(0.0);
							normalsWood.pushDownNormal();
						}

						//front
						if(blocksTransAround.front==true){
							verticesWood.push_back(blCoordX+CUBE_SIZE);
							verticesWood.push_back(blCoordY);
							verticesWood.push_back(blCoordZ+CUBE_SIZE);
							texturesWood.push_back(1.0);
							texturesWood.push_back(0.0);
							normalsWood.pushFrontNormal();

							verticesWood.push_back(blCoordX+CUBE_SIZE);
							verticesWood.push_back(blCoordY+CUBE_SIZE);
							verticesWood.push_back(blCoordZ+CUBE_SIZE);
							texturesWood.push_back(1.0);
							texturesWood.push_back(1.0);
							normalsWood.pushFrontNormal();

							verticesWood.push_back(blCoordX);
							verticesWood.push_back(blCoordY+CUBE_SIZE);
							verticesWood.push_back(blCoordZ+CUBE_SIZE);
							texturesWood.push_back(0.0);
							texturesWood.push_back(1.0);
							normalsWood.pushFrontNormal();

							verticesWood.push_back(blCoordX);
							verticesWood.push_back(blCoordY);
							verticesWood.push_back(blCoordZ+CUBE_SIZE);
							texturesWood.push_back(0.0);
							texturesWood.push_back(0.0);
							normalsWood.pushFrontNormal();
						}

						//back
						if(blocksTransAround.back==true){
							verticesWood.push_back(blCoordX);
							verticesWood.push_back(blCoordY);
							verticesWood.push_back(blCoordZ);
							texturesWood.push_back(1.0);
							texturesWood.push_back(0.0);
							normalsWood.pushBackNormal();

							verticesWood.push_back(blCoordX);
							verticesWood.push_back(blCoordY+CUBE_SIZE);
							verticesWood.push_back(blCoordZ);
							texturesWood.push_back(1.0);
							texturesWood.push_back(1.0);
							normalsWood.pushBackNormal();

							verticesWood.push_back(blCoordX+CUBE_SIZE);
							verticesWood.push_back(blCoordY+CUBE_SIZE);
							verticesWood.push_back(blCoordZ);
							texturesWood.push_back(0.0);
							texturesWood.push_back(1.0);
							normalsWood.pushBackNormal();

							verticesWood.push_back(blCoordX+CUBE_SIZE);
							verticesWood.push_back(blCoordY);
							verticesWood.push_back(blCoordZ);
							texturesWood.push_back(0.0);
							texturesWood.push_back(0.0);
							normalsWood.pushBackNormal();
						}

						//left
						if(blocksTransAround.left==true){
							verticesWood.push_back(blCoordX);
							verticesWood.push_back(blCoordY);
							verticesWood.push_back(blCoordZ+CUBE_SIZE);
							texturesWood.push_back(1.0);
							texturesWood.push_back(0.0);
							normalsWood.pushLeftNormal();

							verticesWood.push_back(blCoordX);
							verticesWood.push_back(blCoordY+CUBE_SIZE);
							verticesWood.push_back(blCoordZ+CUBE_SIZE);
							texturesWood.push_back(1.0);
							texturesWood.push_back(1.0);
							normalsWood.pushLeftNormal();

							verticesWood.push_back(blCoordX);
							verticesWood.push_back(blCoordY+CUBE_SIZE);
							verticesWood.push_back(blCoordZ);
							texturesWood.push_back(0.0);
							texturesWood.push_back(1.0);
							normalsWood.pushLeftNormal();

							verticesWood.push_back(blCoordX);
							verticesWood.push_back(blCoordY);
							verticesWood.push_back(blCoordZ);
							texturesWood.push_back(0.0);
							texturesWood.push_back(0.0);
							normalsWood.pushLeftNormal();
						}

						//right
						if(blocksTransAround.right==true){
							verticesWood.push_back(blCoordX+CUBE_SIZE);
							verticesWood.push_back(blCoordY);
							verticesWood.push_back(blCoordZ);
							texturesWood.push_back(1.0);
							texturesWood.push_back(0.0);
							normalsWood.pushRightNormal();

							verticesWood.push_back(blCoordX+CUBE_SIZE);
							verticesWood.push_back(blCoordY+CUBE_SIZE);
							verticesWood.push_back(blCoordZ);
							texturesWood.push_back(1.0);
							texturesWood.push_back(1.0);
							normalsWood.pushRightNormal();

							verticesWood.push_back(blCoordX+CUBE_SIZE);
							verticesWood.push_back(blCoordY+CUBE_SIZE);
							verticesWood.push_back(blCoordZ+CUBE_SIZE);
							texturesWood.push_back(0.0);
							texturesWood.push_back(1.0);
							normalsWood.pushRightNormal();

							verticesWood.push_back(blCoordX+CUBE_SIZE);
							verticesWood.push_back(blCoordY);
							verticesWood.push_back(blCoordZ+CUBE_SIZE);
							texturesWood.push_back(0.0);
							texturesWood.push_back(0.0);
							normalsWood.pushRightNormal();
						}
						break;
					case SNOW:
						//top
						if(blocksTransAround.top==true){
							verticesSnow.push_back(blCoordX);
							verticesSnow.push_back(blCoordY+CUBE_SIZE);
							verticesSnow.push_back(blCoordZ);
							texturesSnow.push_back(1.0);
							texturesSnow.push_back(0.0);
							normalsSnow.pushUpNormal();

							verticesSnow.push_back(blCoordX);
							verticesSnow.push_back(blCoordY+CUBE_SIZE);
							verticesSnow.push_back(blCoordZ+CUBE_SIZE);
							texturesSnow.push_back(1.0);
							texturesSnow.push_back(1.0);
							normalsSnow.pushUpNormal();

							verticesSnow.push_back(blCoordX+CUBE_SIZE);
							verticesSnow.push_back(blCoordY+CUBE_SIZE);
							verticesSnow.push_back(blCoordZ+CUBE_SIZE);
							texturesSnow.push_back(0.0);
							texturesSnow.push_back(1.0);
							normalsSnow.pushUpNormal();

							verticesSnow.push_back(blCoordX+CUBE_SIZE);
							verticesSnow.push_back(blCoordY+CUBE_SIZE);
							verticesSnow.push_back(blCoordZ);
							texturesSnow.push_back(0.0);
							texturesSnow.push_back(0.0);
							normalsSnow.pushUpNormal();
						}

						//down
						if(blocksTransAround.down==true){
							verticesSnow.push_back(blCoordX+CUBE_SIZE);
							verticesSnow.push_back(blCoordY);
							verticesSnow.push_back(blCoordZ);
							texturesSnow.push_back(1.0);
							texturesSnow.push_back(0.0);
							normalsSnow.pushDownNormal();

							verticesSnow.push_back(blCoordX+CUBE_SIZE);
							verticesSnow.push_back(blCoordY);
							verticesSnow.push_back(blCoordZ+CUBE_SIZE);
							texturesSnow.push_back(1.0);
							texturesSnow.push_back(1.0);
							normalsSnow.pushDownNormal();

							verticesSnow.push_back(blCoordX);
							verticesSnow.push_back(blCoordY);
							verticesSnow.push_back(blCoordZ+CUBE_SIZE);
							texturesSnow.push_back(0.0);
							texturesSnow.push_back(1.0);
							normalsSnow.pushDownNormal();

							verticesSnow.push_back(blCoordX);
							verticesSnow.push_back(blCoordY);
							verticesSnow.push_back(blCoordZ);
							texturesSnow.push_back(0.0);
							texturesSnow.push_back(0.0);
							normalsSnow.pushDownNormal();
						}

						//front
						if(blocksTransAround.front==true){
							verticesSnow.push_back(blCoordX+CUBE_SIZE);
							verticesSnow.push_back(blCoordY);
							verticesSnow.push_back(blCoordZ+CUBE_SIZE);
							texturesSnow.push_back(1.0);
							texturesSnow.push_back(0.0);
							normalsSnow.pushFrontNormal();

							verticesSnow.push_back(blCoordX+CUBE_SIZE);
							verticesSnow.push_back(blCoordY+CUBE_SIZE);
							verticesSnow.push_back(blCoordZ+CUBE_SIZE);
							texturesSnow.push_back(1.0);
							texturesSnow.push_back(1.0);
							normalsSnow.pushFrontNormal();

							verticesSnow.push_back(blCoordX);
							verticesSnow.push_back(blCoordY+CUBE_SIZE);
							verticesSnow.push_back(blCoordZ+CUBE_SIZE);
							texturesSnow.push_back(0.0);
							texturesSnow.push_back(1.0);
							normalsSnow.pushFrontNormal();

							verticesSnow.push_back(blCoordX);
							verticesSnow.push_back(blCoordY);
							verticesSnow.push_back(blCoordZ+CUBE_SIZE);
							texturesSnow.push_back(0.0);
							texturesSnow.push_back(0.0);
							normalsSnow.pushFrontNormal();
						}

						//back
						if(blocksTransAround.back==true){
							verticesSnow.push_back(blCoordX);
							verticesSnow.push_back(blCoordY);
							verticesSnow.push_back(blCoordZ);
							texturesSnow.push_back(1.0);
							texturesSnow.push_back(0.0);
							normalsSnow.pushBackNormal();

							verticesSnow.push_back(blCoordX);
							verticesSnow.push_back(blCoordY+CUBE_SIZE);
							verticesSnow.push_back(blCoordZ);
							texturesSnow.push_back(1.0);
							texturesSnow.push_back(1.0);
							normalsSnow.pushBackNormal();

							verticesSnow.push_back(blCoordX+CUBE_SIZE);
							verticesSnow.push_back(blCoordY+CUBE_SIZE);
							verticesSnow.push_back(blCoordZ);
							texturesSnow.push_back(0.0);
							texturesSnow.push_back(1.0);
							normalsSnow.pushBackNormal();

							verticesSnow.push_back(blCoordX+CUBE_SIZE);
							verticesSnow.push_back(blCoordY);
							verticesSnow.push_back(blCoordZ);
							texturesSnow.push_back(0.0);
							texturesSnow.push_back(0.0);
							normalsSnow.pushBackNormal();
						}

						//left
						if(blocksTransAround.left==true){
							verticesSnow.push_back(blCoordX);
							verticesSnow.push_back(blCoordY);
							verticesSnow.push_back(blCoordZ+CUBE_SIZE);
							texturesSnow.push_back(1.0);
							texturesSnow.push_back(0.0);
							normalsSnow.pushLeftNormal();

							verticesSnow.push_back(blCoordX);
							verticesSnow.push_back(blCoordY+CUBE_SIZE);
							verticesSnow.push_back(blCoordZ+CUBE_SIZE);
							texturesSnow.push_back(1.0);
							texturesSnow.push_back(1.0);
							normalsSnow.pushLeftNormal();

							verticesSnow.push_back(blCoordX);
							verticesSnow.push_back(blCoordY+CUBE_SIZE);
							verticesSnow.push_back(blCoordZ);
							texturesSnow.push_back(0.0);
							texturesSnow.push_back(1.0);
							normalsSnow.pushLeftNormal();

							verticesSnow.push_back(blCoordX);
							verticesSnow.push_back(blCoordY);
							verticesSnow.push_back(blCoordZ);
							texturesSnow.push_back(0.0);
							texturesSnow.push_back(0.0);
							normalsSnow.pushLeftNormal();
						}

						//right
						if(blocksTransAround.right==true){
							verticesSnow.push_back(blCoordX+CUBE_SIZE);
							verticesSnow.push_back(blCoordY);
							verticesSnow.push_back(blCoordZ);
							texturesSnow.push_back(1.0);
							texturesSnow.push_back(0.0);
							normalsSnow.pushRightNormal();

							verticesSnow.push_back(blCoordX+CUBE_SIZE);
							verticesSnow.push_back(blCoordY+CUBE_SIZE);
							verticesSnow.push_back(blCoordZ);
							texturesSnow.push_back(1.0);
							texturesSnow.push_back(1.0);
							normalsSnow.pushRightNormal();

							verticesSnow.push_back(blCoordX+CUBE_SIZE);
							verticesSnow.push_back(blCoordY+CUBE_SIZE);
							verticesSnow.push_back(blCoordZ+CUBE_SIZE);
							texturesSnow.push_back(0.0);
							texturesSnow.push_back(1.0);
							normalsSnow.pushRightNormal();

							verticesSnow.push_back(blCoordX+CUBE_SIZE);
							verticesSnow.push_back(blCoordY);
							verticesSnow.push_back(blCoordZ+CUBE_SIZE);
							texturesSnow.push_back(0.0);
							texturesSnow.push_back(0.0);
							normalsSnow.pushRightNormal();
						}
						break;
					}
				}
			}
		}//конец создания предварительных массивов
		//объединим массивы вершин и текстурных координат
		verticesFinal.goToStart();
		texturesFinal.goToStart();

		//заполним verticesFinal

		for(int i=0; i < verticesDirt.getElementsCount(); ++i)
			verticesFinal.push_back(verticesDirt[i]);

		for(int i=0; i < verticesGrassTop.getElementsCount(); ++i)
			verticesFinal.push_back(verticesGrassTop[i]);

		for(int i=0; i < verticesGrassSide.getElementsCount(); ++i)
			verticesFinal.push_back(verticesGrassSide[i]);

		for(int i=0; i < verticesStone.getElementsCount(); ++i)
			verticesFinal.push_back(verticesStone[i]);

		for(int i=0; i < verticesSand.getElementsCount(); ++i)
			verticesFinal.push_back(verticesSand[i]);

		for(int i=0; i < verticesWater.getElementsCount(); ++i)
			verticesFinal.push_back(verticesWater[i]);

		for(int i=0; i < verticesLeafs.getElementsCount(); ++i)
			verticesFinal.push_back(verticesLeafs[i]);

		for(int i=0; i < verticesWood.getElementsCount(); ++i)
			verticesFinal.push_back(verticesWood[i]);

		for(int i=0; i < verticesSnow.getElementsCount(); ++i)
			verticesFinal.push_back(verticesSnow[i]);

		//заполним normalsFinal

		for(int i=0; i < normalsDirt.getElementsCount(); ++i)
			normalsFinal.push_back(normalsDirt[i]);

		for(int i=0; i < normalsGrassTop.getElementsCount(); ++i)
			normalsFinal.push_back(normalsGrassTop[i]);

		for(int i=0; i < normalsGrassSide.getElementsCount(); ++i)
			normalsFinal.push_back(normalsGrassSide[i]);

		for(int i=0; i < normalsStone.getElementsCount(); ++i)
			normalsFinal.push_back(normalsStone[i]);

		for(int i=0; i < normalsSand.getElementsCount(); ++i)
			normalsFinal.push_back(normalsSand[i]);

		for(int i=0; i < normalsWater.getElementsCount(); ++i)
			normalsFinal.push_back(normalsWater[i]);

		for(int i=0; i < normalsLeafs.getElementsCount(); ++i)
			normalsFinal.push_back(normalsLeafs[i]);

		for(int i=0; i < normalsWood.getElementsCount(); ++i)
			normalsFinal.push_back(normalsWood[i]);

		for(int i=0; i < normalsSnow.getElementsCount(); ++i)
			normalsFinal.push_back(normalsSnow[i]);

		//заполним texturesFinal

		for(int i=0; i < texturesDirt.getElementsCount(); ++i)
			texturesFinal.push_back(texturesDirt[i]);

		for(int i=0; i < texturesGrassTop.getElementsCount(); ++i)
			texturesFinal.push_back(texturesGrassTop[i]);

		for(int i=0; i < texturesGrassSide.getElementsCount(); ++i)
			texturesFinal.push_back(texturesGrassSide[i]);

		for(int i=0; i < texturesStone.getElementsCount(); ++i)
			texturesFinal.push_back(texturesStone[i]);

		for(int i=0; i < texturesSand.getElementsCount(); ++i)
			texturesFinal.push_back(texturesSand[i]);

		for(int i=0; i < texturesWater.getElementsCount(); ++i)
			texturesFinal.push_back(texturesWater[i]);

		for(int i=0; i < texturesLeafs.getElementsCount(); ++i)
			texturesFinal.push_back(texturesLeafs[i]);

		for(int i=0; i < texturesWood.getElementsCount(); ++i)
			texturesFinal.push_back(texturesWood[i]);

		for(int i=0; i < texturesSnow.getElementsCount(); ++i)
			texturesFinal.push_back(texturesSnow[i]);

		//сколько каких видов точек надо рисовать

		pointsOfDirtToDraw=verticesDirt.getElementsCount()/3;
		pointsOfGrassTopToDraw=verticesGrassTop.getElementsCount()/3;
		pointsOfGrassSideToDraw=verticesGrassSide.getElementsCount()/3;
		pointsOfStoneToDraw=verticesStone.getElementsCount()/3;
		pointsOfSandToDraw=verticesSand.getElementsCount()/3;
		pointsOfWaterToDraw=verticesWater.getElementsCount()/3;
		pointsOfLeafsToDraw=verticesLeafs.getElementsCount()/3;
		pointsOfWoodToDraw=verticesWood.getElementsCount()/3;
		pointsOfSnowToDraw=verticesSnow.getElementsCount()/3;

		//зададим смещение
		offsetOfDirt=0;
		offsetOfGrassTop=offsetOfDirt+pointsOfDirtToDraw;
		offsetOfGrassSide=offsetOfGrassTop+pointsOfGrassTopToDraw;
		offsetOfStone=offsetOfGrassSide+pointsOfGrassSideToDraw;
		offsetOfSand=offsetOfStone+pointsOfStoneToDraw;
		offsetOfWater=offsetOfSand+pointsOfSandToDraw;
		offsetOfLeafs=offsetOfWater+pointsOfWaterToDraw;
		offsetOfWood=offsetOfLeafs+pointsOfLeafsToDraw;
		offsetOfSnow=offsetOfWood+pointsOfWoodToDraw;

		VBOBoxTransferInfo toReturn;

		toReturn.pointsOfDirtToDraw=pointsOfDirtToDraw;
		toReturn.pointsOfGrassTopToDraw=pointsOfGrassTopToDraw;
		toReturn.pointsOfGrassSideToDraw=pointsOfGrassSideToDraw;
		toReturn.pointsOfStoneToDraw=pointsOfStoneToDraw;
		toReturn.pointsOfSandToDraw=pointsOfSandToDraw;
		toReturn.pointsOfWaterToDraw=pointsOfWaterToDraw;
		toReturn.pointsOfLeafsToDraw=pointsOfLeafsToDraw;
		toReturn.pointsOfWoodToDraw=pointsOfWoodToDraw;
		toReturn.pointsOfSnowToDraw=pointsOfSnowToDraw;

		toReturn.offsetOfDirt=offsetOfDirt;
		toReturn.offsetOfGrassTop=offsetOfGrassTop;
		toReturn.offsetOfGrassSide=offsetOfGrassSide;
		toReturn.offsetOfStone=offsetOfStone;
		toReturn.offsetOfSand=offsetOfSand;
		toReturn.offsetOfWater=offsetOfWater;
		toReturn.offsetOfLeafs=offsetOfLeafs;
		toReturn.offsetOfWood=offsetOfWood;
		toReturn.offsetOfSnow=offsetOfSnow;

		return toReturn;
}