#include "VBOBoxTransferInfo.h"

VBOBoxTransferInfo& VBOBoxTransferInfo::operator=(const VBOBoxTransferInfo& v){
	if(this != &v){
		pointsOfDirtToDraw=			v.pointsOfDirtToDraw;
		pointsOfGrassTopToDraw=		v.pointsOfGrassTopToDraw;
		pointsOfGrassSideToDraw=	v.pointsOfGrassSideToDraw;
		pointsOfStoneToDraw=		v.pointsOfStoneToDraw;
		pointsOfSandToDraw=			v.pointsOfSandToDraw;

		offsetOfDirt=				v.offsetOfDirt;
		offsetOfGrassTop=			v.offsetOfGrassTop;
		offsetOfGrassSide=			v.offsetOfGrassSide;
		offsetOfStone=				v.offsetOfStone;
		offsetOfSand=				v.offsetOfSand;
	}
	
	return *this;
}

VBOBoxTransferInfo updateGraphicsArraysForChunk(SimpleArray<GLint,SIZE_OF_VERTICES_FINAL_ARRAYS> &verticesFinal
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

		int offsetOfDirt=0;
		int offsetOfGrassTop=0;
		int offsetOfGrassSide=0;
		int offsetOfStone=0;
		int offsetOfSand=0;

		SimpleArray<GLint,SIZE_OF_VERTICES_DIRT_ARRAYS> verticesDirt;
		SimpleArray<GLfloat,SIZE_OF_TEXTURES_DIRT_ARRAYS> texturesDirt;

		SimpleArray<GLint,SIZE_OF_VERTICES_GRASS_TOP_ARRAYS> verticesGrassTop;
		SimpleArray<GLfloat,SIZE_OF_TEXTURES_GRASS_TOP_ARRAYS> texturesGrassTop;

		SimpleArray<GLint,SIZE_OF_VERTICES_GRASS_SIDE_ARRAYS> verticesGrassSide;
		SimpleArray<GLfloat,SIZE_OF_TEXTURES_GRASS_SIDE_ARRAYS> texturesGrassSide;

		SimpleArray<GLint,SIZE_OF_VERTICES_STONE_ARRAYS> verticesStone;
		SimpleArray<GLfloat,SIZE_OF_TEXTURES_STONE_ARRAYS> texturesStone;

		SimpleArray<GLint,SIZE_OF_VERTICES_SAND_ARRAYS> verticesSand;
		SimpleArray<GLfloat,SIZE_OF_TEXTURES_SAND_ARRAYS> texturesSand;

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

							verticesDirt.push_back(blCoordX);
							verticesDirt.push_back(blCoordY+CUBE_SIZE);
							verticesDirt.push_back(blCoordZ+CUBE_SIZE);
							texturesDirt.push_back(1.0);
							texturesDirt.push_back(1.0);

							verticesDirt.push_back(blCoordX+CUBE_SIZE);
							verticesDirt.push_back(blCoordY+CUBE_SIZE);
							verticesDirt.push_back(blCoordZ+CUBE_SIZE);
							texturesDirt.push_back(0.0);
							texturesDirt.push_back(1.0);

							verticesDirt.push_back(blCoordX+CUBE_SIZE);
							verticesDirt.push_back(blCoordY+CUBE_SIZE);
							verticesDirt.push_back(blCoordZ);
							texturesDirt.push_back(0.0);
							texturesDirt.push_back(0.0);
						}

						//down
						if(blocksTransAround.down==true){
							verticesDirt.push_back(blCoordX+CUBE_SIZE);
							verticesDirt.push_back(blCoordY);
							verticesDirt.push_back(blCoordZ);
							texturesDirt.push_back(1.0);
							texturesDirt.push_back(0.0);

							verticesDirt.push_back(blCoordX+CUBE_SIZE);
							verticesDirt.push_back(blCoordY);
							verticesDirt.push_back(blCoordZ+CUBE_SIZE);
							texturesDirt.push_back(1.0);
							texturesDirt.push_back(1.0);

							verticesDirt.push_back(blCoordX);
							verticesDirt.push_back(blCoordY);
							verticesDirt.push_back(blCoordZ+CUBE_SIZE);
							texturesDirt.push_back(0.0);
							texturesDirt.push_back(1.0);

							verticesDirt.push_back(blCoordX);
							verticesDirt.push_back(blCoordY);
							verticesDirt.push_back(blCoordZ);
							texturesDirt.push_back(0.0);
							texturesDirt.push_back(0.0);
						}

						//front
						if(blocksTransAround.front==true){
							verticesDirt.push_back(blCoordX+CUBE_SIZE);
							verticesDirt.push_back(blCoordY);
							verticesDirt.push_back(blCoordZ+CUBE_SIZE);
							texturesDirt.push_back(1.0);
							texturesDirt.push_back(0.0);

							verticesDirt.push_back(blCoordX+CUBE_SIZE);
							verticesDirt.push_back(blCoordY+CUBE_SIZE);
							verticesDirt.push_back(blCoordZ+CUBE_SIZE);
							texturesDirt.push_back(1.0);
							texturesDirt.push_back(1.0);

							verticesDirt.push_back(blCoordX);
							verticesDirt.push_back(blCoordY+CUBE_SIZE);
							verticesDirt.push_back(blCoordZ+CUBE_SIZE);
							texturesDirt.push_back(0.0);
							texturesDirt.push_back(1.0);

							verticesDirt.push_back(blCoordX);
							verticesDirt.push_back(blCoordY);
							verticesDirt.push_back(blCoordZ+CUBE_SIZE);
							texturesDirt.push_back(0.0);
							texturesDirt.push_back(0.0);
						}

						//back
						if(blocksTransAround.back==true){
							verticesDirt.push_back(blCoordX);
							verticesDirt.push_back(blCoordY);
							verticesDirt.push_back(blCoordZ);
							texturesDirt.push_back(1.0);
							texturesDirt.push_back(0.0);

							verticesDirt.push_back(blCoordX);
							verticesDirt.push_back(blCoordY+CUBE_SIZE);
							verticesDirt.push_back(blCoordZ);
							texturesDirt.push_back(1.0);
							texturesDirt.push_back(1.0);

							verticesDirt.push_back(blCoordX+CUBE_SIZE);
							verticesDirt.push_back(blCoordY+CUBE_SIZE);
							verticesDirt.push_back(blCoordZ);
							texturesDirt.push_back(0.0);
							texturesDirt.push_back(1.0);

							verticesDirt.push_back(blCoordX+CUBE_SIZE);
							verticesDirt.push_back(blCoordY);
							verticesDirt.push_back(blCoordZ);
							texturesDirt.push_back(0.0);
							texturesDirt.push_back(0.0);
						}

						//left
						if(blocksTransAround.left==true){
							verticesDirt.push_back(blCoordX);
							verticesDirt.push_back(blCoordY);
							verticesDirt.push_back(blCoordZ+CUBE_SIZE);
							texturesDirt.push_back(1.0);
							texturesDirt.push_back(0.0);

							verticesDirt.push_back(blCoordX);
							verticesDirt.push_back(blCoordY+CUBE_SIZE);
							verticesDirt.push_back(blCoordZ+CUBE_SIZE);
							texturesDirt.push_back(1.0);
							texturesDirt.push_back(1.0);

							verticesDirt.push_back(blCoordX);
							verticesDirt.push_back(blCoordY+CUBE_SIZE);
							verticesDirt.push_back(blCoordZ);
							texturesDirt.push_back(0.0);
							texturesDirt.push_back(1.0);

							verticesDirt.push_back(blCoordX);
							verticesDirt.push_back(blCoordY);
							verticesDirt.push_back(blCoordZ);
							texturesDirt.push_back(0.0);
							texturesDirt.push_back(0.0);
						}

						//right
						if(blocksTransAround.right==true){
							verticesDirt.push_back(blCoordX+CUBE_SIZE);
							verticesDirt.push_back(blCoordY);
							verticesDirt.push_back(blCoordZ);
							texturesDirt.push_back(1.0);
							texturesDirt.push_back(0.0);

							verticesDirt.push_back(blCoordX+CUBE_SIZE);
							verticesDirt.push_back(blCoordY+CUBE_SIZE);
							verticesDirt.push_back(blCoordZ);
							texturesDirt.push_back(1.0);
							texturesDirt.push_back(1.0);

							verticesDirt.push_back(blCoordX+CUBE_SIZE);
							verticesDirt.push_back(blCoordY+CUBE_SIZE);
							verticesDirt.push_back(blCoordZ+CUBE_SIZE);
							texturesDirt.push_back(0.0);
							texturesDirt.push_back(1.0);

							verticesDirt.push_back(blCoordX+CUBE_SIZE);
							verticesDirt.push_back(blCoordY);
							verticesDirt.push_back(blCoordZ+CUBE_SIZE);
							texturesDirt.push_back(0.0);
							texturesDirt.push_back(0.0);
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

							verticesGrassTop.push_back(blCoordX);
							verticesGrassTop.push_back(blCoordY+CUBE_SIZE);
							verticesGrassTop.push_back(blCoordZ+CUBE_SIZE);
							texturesGrassTop.push_back(1.0);
							texturesGrassTop.push_back(1.0);

							verticesGrassTop.push_back(blCoordX+CUBE_SIZE);
							verticesGrassTop.push_back(blCoordY+CUBE_SIZE);
							verticesGrassTop.push_back(blCoordZ+CUBE_SIZE);
							texturesGrassTop.push_back(0.0);
							texturesGrassTop.push_back(1.0);

							verticesGrassTop.push_back(blCoordX+CUBE_SIZE);
							verticesGrassTop.push_back(blCoordY+CUBE_SIZE);
							verticesGrassTop.push_back(blCoordZ);
							texturesGrassTop.push_back(0.0);
							texturesGrassTop.push_back(0.0);
						}

						//down
						if(blocksTransAround.down==true){
							verticesDirt.push_back(blCoordX+CUBE_SIZE);
							verticesDirt.push_back(blCoordY);
							verticesDirt.push_back(blCoordZ);
							texturesDirt.push_back(1.0);
							texturesDirt.push_back(0.0);

							verticesDirt.push_back(blCoordX+CUBE_SIZE);
							verticesDirt.push_back(blCoordY);
							verticesDirt.push_back(blCoordZ+CUBE_SIZE);
							texturesDirt.push_back(1.0);
							texturesDirt.push_back(1.0);

							verticesDirt.push_back(blCoordX);
							verticesDirt.push_back(blCoordY);
							verticesDirt.push_back(blCoordZ+CUBE_SIZE);
							texturesDirt.push_back(0.0);
							texturesDirt.push_back(1.0);

							verticesDirt.push_back(blCoordX);
							verticesDirt.push_back(blCoordY);
							verticesDirt.push_back(blCoordZ);
							texturesDirt.push_back(0.0);
							texturesDirt.push_back(0.0);
						}

						//front		
						if(blocksTransAround.front==true){
							verticesGrassSide.push_back(blCoordX+CUBE_SIZE);
							verticesGrassSide.push_back(blCoordY);
							verticesGrassSide.push_back(blCoordZ+CUBE_SIZE);
							texturesGrassSide.push_back(1.0);
							texturesGrassSide.push_back(0.0);

							verticesGrassSide.push_back(blCoordX+CUBE_SIZE);
							verticesGrassSide.push_back(blCoordY+CUBE_SIZE);
							verticesGrassSide.push_back(blCoordZ+CUBE_SIZE);
							texturesGrassSide.push_back(1.0);
							texturesGrassSide.push_back(1.0);

							verticesGrassSide.push_back(blCoordX);
							verticesGrassSide.push_back(blCoordY+CUBE_SIZE);
							verticesGrassSide.push_back(blCoordZ+CUBE_SIZE);
							texturesGrassSide.push_back(0.0);
							texturesGrassSide.push_back(1.0);

							verticesGrassSide.push_back(blCoordX);
							verticesGrassSide.push_back(blCoordY);
							verticesGrassSide.push_back(blCoordZ+CUBE_SIZE);
							texturesGrassSide.push_back(0.0);
							texturesGrassSide.push_back(0.0);
						}			

						//back		
						if(blocksTransAround.back==true){
							verticesGrassSide.push_back(blCoordX);
							verticesGrassSide.push_back(blCoordY);
							verticesGrassSide.push_back(blCoordZ);
							texturesGrassSide.push_back(1.0);
							texturesGrassSide.push_back(0.0);

							verticesGrassSide.push_back(blCoordX);
							verticesGrassSide.push_back(blCoordY+CUBE_SIZE);
							verticesGrassSide.push_back(blCoordZ);
							texturesGrassSide.push_back(1.0);
							texturesGrassSide.push_back(1.0);

							verticesGrassSide.push_back(blCoordX+CUBE_SIZE);
							verticesGrassSide.push_back(blCoordY+CUBE_SIZE);
							verticesGrassSide.push_back(blCoordZ);
							texturesGrassSide.push_back(0.0);
							texturesGrassSide.push_back(1.0);

							verticesGrassSide.push_back(blCoordX+CUBE_SIZE);
							verticesGrassSide.push_back(blCoordY);
							verticesGrassSide.push_back(blCoordZ);
							texturesGrassSide.push_back(0.0);
							texturesGrassSide.push_back(0.0);
						}

						//left
						if(blocksTransAround.left==true){
							verticesGrassSide.push_back(blCoordX);
							verticesGrassSide.push_back(blCoordY);
							verticesGrassSide.push_back(blCoordZ+CUBE_SIZE);
							texturesGrassSide.push_back(1.0);
							texturesGrassSide.push_back(0.0);

							verticesGrassSide.push_back(blCoordX);
							verticesGrassSide.push_back(blCoordY+CUBE_SIZE);
							verticesGrassSide.push_back(blCoordZ+CUBE_SIZE);
							texturesGrassSide.push_back(1.0);
							texturesGrassSide.push_back(1.0);

							verticesGrassSide.push_back(blCoordX);
							verticesGrassSide.push_back(blCoordY+CUBE_SIZE);
							verticesGrassSide.push_back(blCoordZ);
							texturesGrassSide.push_back(0.0);
							texturesGrassSide.push_back(1.0);

							verticesGrassSide.push_back(blCoordX);
							verticesGrassSide.push_back(blCoordY);
							verticesGrassSide.push_back(blCoordZ);
							texturesGrassSide.push_back(0.0);
							texturesGrassSide.push_back(0.0);
						}			

						//right		
						if(blocksTransAround.right==true){
							verticesGrassSide.push_back(blCoordX+CUBE_SIZE);
							verticesGrassSide.push_back(blCoordY);
							verticesGrassSide.push_back(blCoordZ);
							texturesGrassSide.push_back(1.0);
							texturesGrassSide.push_back(0.0);

							verticesGrassSide.push_back(blCoordX+CUBE_SIZE);
							verticesGrassSide.push_back(blCoordY+CUBE_SIZE);
							verticesGrassSide.push_back(blCoordZ);
							texturesGrassSide.push_back(1.0);
							texturesGrassSide.push_back(1.0);

							verticesGrassSide.push_back(blCoordX+CUBE_SIZE);
							verticesGrassSide.push_back(blCoordY+CUBE_SIZE);
							verticesGrassSide.push_back(blCoordZ+CUBE_SIZE);
							texturesGrassSide.push_back(0.0);
							texturesGrassSide.push_back(1.0);

							verticesGrassSide.push_back(blCoordX+CUBE_SIZE);
							verticesGrassSide.push_back(blCoordY);
							verticesGrassSide.push_back(blCoordZ+CUBE_SIZE);
							texturesGrassSide.push_back(0.0);
							texturesGrassSide.push_back(0.0);
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

							verticesStone.push_back(blCoordX);
							verticesStone.push_back(blCoordY+CUBE_SIZE);
							verticesStone.push_back(blCoordZ+CUBE_SIZE);
							texturesStone.push_back(1.0);
							texturesStone.push_back(1.0);

							verticesStone.push_back(blCoordX+CUBE_SIZE);
							verticesStone.push_back(blCoordY+CUBE_SIZE);
							verticesStone.push_back(blCoordZ+CUBE_SIZE);
							texturesStone.push_back(0.0);
							texturesStone.push_back(1.0);

							verticesStone.push_back(blCoordX+CUBE_SIZE);
							verticesStone.push_back(blCoordY+CUBE_SIZE);
							verticesStone.push_back(blCoordZ);
							texturesStone.push_back(0.0);
							texturesStone.push_back(0.0);
						}

						//down
						if(blocksTransAround.down==true){
							verticesStone.push_back(blCoordX+CUBE_SIZE);
							verticesStone.push_back(blCoordY);
							verticesStone.push_back(blCoordZ);
							texturesStone.push_back(1.0);
							texturesStone.push_back(0.0);

							verticesStone.push_back(blCoordX+CUBE_SIZE);
							verticesStone.push_back(blCoordY);
							verticesStone.push_back(blCoordZ+CUBE_SIZE);
							texturesStone.push_back(1.0);
							texturesStone.push_back(1.0);

							verticesStone.push_back(blCoordX);
							verticesStone.push_back(blCoordY);
							verticesStone.push_back(blCoordZ+CUBE_SIZE);
							texturesStone.push_back(0.0);
							texturesStone.push_back(1.0);

							verticesStone.push_back(blCoordX);
							verticesStone.push_back(blCoordY);
							verticesStone.push_back(blCoordZ);
							texturesStone.push_back(0.0);
							texturesStone.push_back(0.0);
						}

						//front
						if(blocksTransAround.front==true){
							verticesStone.push_back(blCoordX+CUBE_SIZE);
							verticesStone.push_back(blCoordY);
							verticesStone.push_back(blCoordZ+CUBE_SIZE);
							texturesStone.push_back(1.0);
							texturesStone.push_back(0.0);

							verticesStone.push_back(blCoordX+CUBE_SIZE);
							verticesStone.push_back(blCoordY+CUBE_SIZE);
							verticesStone.push_back(blCoordZ+CUBE_SIZE);
							texturesStone.push_back(1.0);
							texturesStone.push_back(1.0);

							verticesStone.push_back(blCoordX);
							verticesStone.push_back(blCoordY+CUBE_SIZE);
							verticesStone.push_back(blCoordZ+CUBE_SIZE);
							texturesStone.push_back(0.0);
							texturesStone.push_back(1.0);

							verticesStone.push_back(blCoordX);
							verticesStone.push_back(blCoordY);
							verticesStone.push_back(blCoordZ+CUBE_SIZE);
							texturesStone.push_back(0.0);
							texturesStone.push_back(0.0);
						}

						//back
						if(blocksTransAround.back==true){
							verticesStone.push_back(blCoordX);
							verticesStone.push_back(blCoordY);
							verticesStone.push_back(blCoordZ);
							texturesStone.push_back(1.0);
							texturesStone.push_back(0.0);

							verticesStone.push_back(blCoordX);
							verticesStone.push_back(blCoordY+CUBE_SIZE);
							verticesStone.push_back(blCoordZ);
							texturesStone.push_back(1.0);
							texturesStone.push_back(1.0);

							verticesStone.push_back(blCoordX+CUBE_SIZE);
							verticesStone.push_back(blCoordY+CUBE_SIZE);
							verticesStone.push_back(blCoordZ);
							texturesStone.push_back(0.0);
							texturesStone.push_back(1.0);

							verticesStone.push_back(blCoordX+CUBE_SIZE);
							verticesStone.push_back(blCoordY);
							verticesStone.push_back(blCoordZ);
							texturesStone.push_back(0.0);
							texturesStone.push_back(0.0);
						}

						//left
						if(blocksTransAround.left==true){
							verticesStone.push_back(blCoordX);
							verticesStone.push_back(blCoordY);
							verticesStone.push_back(blCoordZ+CUBE_SIZE);
							texturesStone.push_back(1.0);
							texturesStone.push_back(0.0);

							verticesStone.push_back(blCoordX);
							verticesStone.push_back(blCoordY+CUBE_SIZE);
							verticesStone.push_back(blCoordZ+CUBE_SIZE);
							texturesStone.push_back(1.0);
							texturesStone.push_back(1.0);

							verticesStone.push_back(blCoordX);
							verticesStone.push_back(blCoordY+CUBE_SIZE);
							verticesStone.push_back(blCoordZ);
							texturesStone.push_back(0.0);
							texturesStone.push_back(1.0);

							verticesStone.push_back(blCoordX);
							verticesStone.push_back(blCoordY);
							verticesStone.push_back(blCoordZ);
							texturesStone.push_back(0.0);
							texturesStone.push_back(0.0);
						}

						//right
						if(blocksTransAround.right==true){
							verticesStone.push_back(blCoordX+CUBE_SIZE);
							verticesStone.push_back(blCoordY);
							verticesStone.push_back(blCoordZ);
							texturesStone.push_back(1.0);
							texturesStone.push_back(0.0);

							verticesStone.push_back(blCoordX+CUBE_SIZE);
							verticesStone.push_back(blCoordY+CUBE_SIZE);
							verticesStone.push_back(blCoordZ);
							texturesStone.push_back(1.0);
							texturesStone.push_back(1.0);

							verticesStone.push_back(blCoordX+CUBE_SIZE);
							verticesStone.push_back(blCoordY+CUBE_SIZE);
							verticesStone.push_back(blCoordZ+CUBE_SIZE);
							texturesStone.push_back(0.0);
							texturesStone.push_back(1.0);

							verticesStone.push_back(blCoordX+CUBE_SIZE);
							verticesStone.push_back(blCoordY);
							verticesStone.push_back(blCoordZ+CUBE_SIZE);
							texturesStone.push_back(0.0);
							texturesStone.push_back(0.0);
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

							verticesSand.push_back(blCoordX);
							verticesSand.push_back(blCoordY+CUBE_SIZE);
							verticesSand.push_back(blCoordZ+CUBE_SIZE);
							texturesSand.push_back(1.0);
							texturesSand.push_back(1.0);

							verticesSand.push_back(blCoordX+CUBE_SIZE);
							verticesSand.push_back(blCoordY+CUBE_SIZE);
							verticesSand.push_back(blCoordZ+CUBE_SIZE);
							texturesSand.push_back(0.0);
							texturesSand.push_back(1.0);

							verticesSand.push_back(blCoordX+CUBE_SIZE);
							verticesSand.push_back(blCoordY+CUBE_SIZE);
							verticesSand.push_back(blCoordZ);
							texturesSand.push_back(0.0);
							texturesSand.push_back(0.0);
						}

						//down
						if(blocksTransAround.down==true){
							verticesSand.push_back(blCoordX+CUBE_SIZE);
							verticesSand.push_back(blCoordY);
							verticesSand.push_back(blCoordZ);
							texturesSand.push_back(1.0);
							texturesSand.push_back(0.0);

							verticesSand.push_back(blCoordX+CUBE_SIZE);
							verticesSand.push_back(blCoordY);
							verticesSand.push_back(blCoordZ+CUBE_SIZE);
							texturesSand.push_back(1.0);
							texturesSand.push_back(1.0);

							verticesSand.push_back(blCoordX);
							verticesSand.push_back(blCoordY);
							verticesSand.push_back(blCoordZ+CUBE_SIZE);
							texturesSand.push_back(0.0);
							texturesSand.push_back(1.0);

							verticesSand.push_back(blCoordX);
							verticesSand.push_back(blCoordY);
							verticesSand.push_back(blCoordZ);
							texturesSand.push_back(0.0);
							texturesSand.push_back(0.0);
						}

						//front
						if(blocksTransAround.front==true){
							verticesSand.push_back(blCoordX+CUBE_SIZE);
							verticesSand.push_back(blCoordY);
							verticesSand.push_back(blCoordZ+CUBE_SIZE);
							texturesSand.push_back(1.0);
							texturesSand.push_back(0.0);

							verticesSand.push_back(blCoordX+CUBE_SIZE);
							verticesSand.push_back(blCoordY+CUBE_SIZE);
							verticesSand.push_back(blCoordZ+CUBE_SIZE);
							texturesSand.push_back(1.0);
							texturesSand.push_back(1.0);

							verticesSand.push_back(blCoordX);
							verticesSand.push_back(blCoordY+CUBE_SIZE);
							verticesSand.push_back(blCoordZ+CUBE_SIZE);
							texturesSand.push_back(0.0);
							texturesSand.push_back(1.0);

							verticesSand.push_back(blCoordX);
							verticesSand.push_back(blCoordY);
							verticesSand.push_back(blCoordZ+CUBE_SIZE);
							texturesSand.push_back(0.0);
							texturesSand.push_back(0.0);
						}

						//back
						if(blocksTransAround.back==true){
							verticesSand.push_back(blCoordX);
							verticesSand.push_back(blCoordY);
							verticesSand.push_back(blCoordZ);
							texturesSand.push_back(1.0);
							texturesSand.push_back(0.0);

							verticesSand.push_back(blCoordX);
							verticesSand.push_back(blCoordY+CUBE_SIZE);
							verticesSand.push_back(blCoordZ);
							texturesSand.push_back(1.0);
							texturesSand.push_back(1.0);

							verticesSand.push_back(blCoordX+CUBE_SIZE);
							verticesSand.push_back(blCoordY+CUBE_SIZE);
							verticesSand.push_back(blCoordZ);
							texturesSand.push_back(0.0);
							texturesSand.push_back(1.0);

							verticesSand.push_back(blCoordX+CUBE_SIZE);
							verticesSand.push_back(blCoordY);
							verticesSand.push_back(blCoordZ);
							texturesSand.push_back(0.0);
							texturesSand.push_back(0.0);
						}

						//left
						if(blocksTransAround.left==true){
							verticesSand.push_back(blCoordX);
							verticesSand.push_back(blCoordY);
							verticesSand.push_back(blCoordZ+CUBE_SIZE);
							texturesSand.push_back(1.0);
							texturesSand.push_back(0.0);

							verticesSand.push_back(blCoordX);
							verticesSand.push_back(blCoordY+CUBE_SIZE);
							verticesSand.push_back(blCoordZ+CUBE_SIZE);
							texturesSand.push_back(1.0);
							texturesSand.push_back(1.0);

							verticesSand.push_back(blCoordX);
							verticesSand.push_back(blCoordY+CUBE_SIZE);
							verticesSand.push_back(blCoordZ);
							texturesSand.push_back(0.0);
							texturesSand.push_back(1.0);

							verticesSand.push_back(blCoordX);
							verticesSand.push_back(blCoordY);
							verticesSand.push_back(blCoordZ);
							texturesSand.push_back(0.0);
							texturesSand.push_back(0.0);
						}

						//right
						if(blocksTransAround.right==true){
							verticesSand.push_back(blCoordX+CUBE_SIZE);
							verticesSand.push_back(blCoordY);
							verticesSand.push_back(blCoordZ);
							texturesSand.push_back(1.0);
							texturesSand.push_back(0.0);

							verticesSand.push_back(blCoordX+CUBE_SIZE);
							verticesSand.push_back(blCoordY+CUBE_SIZE);
							verticesSand.push_back(blCoordZ);
							texturesSand.push_back(1.0);
							texturesSand.push_back(1.0);

							verticesSand.push_back(blCoordX+CUBE_SIZE);
							verticesSand.push_back(blCoordY+CUBE_SIZE);
							verticesSand.push_back(blCoordZ+CUBE_SIZE);
							texturesSand.push_back(0.0);
							texturesSand.push_back(1.0);

							verticesSand.push_back(blCoordX+CUBE_SIZE);
							verticesSand.push_back(blCoordY);
							verticesSand.push_back(blCoordZ+CUBE_SIZE);
							texturesSand.push_back(0.0);
							texturesSand.push_back(0.0);
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

		//сколько каких видов точек надо рисовать

		pointsOfDirtToDraw=verticesDirt.getElementsCount()/3;
		pointsOfGrassTopToDraw=verticesGrassTop.getElementsCount()/3;
		pointsOfGrassSideToDraw=verticesGrassSide.getElementsCount()/3;
		pointsOfStoneToDraw=verticesStone.getElementsCount()/3;
		pointsOfSandToDraw=verticesSand.getElementsCount()/3;

		//зададим смещение
		offsetOfDirt=0;
		offsetOfGrassTop=offsetOfDirt+pointsOfDirtToDraw;
		offsetOfGrassSide=offsetOfGrassTop+pointsOfGrassTopToDraw;
		offsetOfStone=offsetOfGrassSide+pointsOfGrassSideToDraw;
		offsetOfSand=offsetOfStone+pointsOfStoneToDraw;

		VBOBoxTransferInfo toReturn;

		toReturn.pointsOfDirtToDraw=pointsOfDirtToDraw;
		toReturn.pointsOfGrassTopToDraw=pointsOfGrassTopToDraw;
		toReturn.pointsOfGrassSideToDraw=pointsOfGrassSideToDraw;
		toReturn.pointsOfStoneToDraw=pointsOfStoneToDraw;
		toReturn.pointsOfSandToDraw=pointsOfSandToDraw;

		toReturn.offsetOfDirt=offsetOfDirt;
		toReturn.offsetOfGrassTop=offsetOfGrassTop;
		toReturn.offsetOfGrassSide=offsetOfGrassSide;
		toReturn.offsetOfStone=offsetOfStone;
		toReturn.offsetOfSand=offsetOfSand;

		return toReturn;
}