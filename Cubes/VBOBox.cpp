#include "VBOBox.h"

//VBOBox ONLY
const int VBOBOX_HEAP_SIZE=CHUNKS_TO_DRAW*CHUNKS_TO_DRAW;
bool VBOBox::alloc_map[VBOBOX_HEAP_SIZE];//для динамического выделения памяти
unsigned char VBOBox::pool[VBOBOX_HEAP_SIZE*sizeof(VBOBox)];//для динамического выделения памяти
//~VBOBox ONLY

//VBOBoxPrebuild ONLY
const int VBOBOXPREBUILD_HEAP_SIZE=CHUNKS_PRELOAD_ENTIRE_SIDE_LENGTH*CHUNKS_PRELOAD_BORDER;
bool VBOBoxPrebuild::alloc_map[VBOBOXPREBUILD_HEAP_SIZE];//для динамического выделения памяти
unsigned char VBOBoxPrebuild::pool[VBOBOXPREBUILD_HEAP_SIZE*sizeof(VBOBox)];//для динамического выделения памяти
//~VBOBoxPrebuild ONLY

VBOBoxTransferInfo updateGraphicsArraysForChunk(std::vector<GLint> &verticesFinal,std::vector<GLfloat> &texturesFinal,GameMain* gameMain,Chunk* _chunkPtr,bool isPreloading,int chNumX,int chNumZ){
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

	std::vector<GLint> verticesDirt;
	std::vector<GLfloat> texturesDirt;
	verticesDirt.reserve(3000000);
	texturesDirt.reserve(3000000);

	std::vector<GLint> verticesGrassTop;
	std::vector<GLfloat> texturesGrassTop;
	verticesGrassTop.reserve(500000);
	texturesGrassTop.reserve(500000);

	std::vector<GLint> verticesGrassSide;
	std::vector<GLfloat> texturesGrassSide;
	verticesGrassSide.reserve(1500000);
	texturesGrassSide.reserve(1500000);

	std::vector<GLint> verticesStone;
	std::vector<GLfloat> texturesStone;
	verticesStone.reserve(1500000);
	texturesStone.reserve(1500000);

	std::vector<GLint> verticesSand;
	std::vector<GLfloat> texturesSand;
	verticesSand.reserve(1500000);
	texturesSand.reserve(1500000);

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
				BlockTransparencyAround blocksTransAround=gameMain->world->getBlockTransparencyAround(chNumX,chNumZ,blX,blY,blZ);

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
	verticesFinal.reserve(10000000);
	texturesFinal.reserve(10000000);
	//заполним verticesFinal
	for(std::vector<GLint>::iterator iter=verticesDirt.begin();iter != verticesDirt.end();++iter)
		verticesFinal.push_back(*iter);
	for(std::vector<GLint>::iterator iter=verticesGrassTop.begin();iter != verticesGrassTop.end();++iter)
		verticesFinal.push_back(*iter);
	for(std::vector<GLint>::iterator iter=verticesGrassSide.begin();iter != verticesGrassSide.end();++iter)
		verticesFinal.push_back(*iter);
	for(std::vector<GLint>::iterator iter=verticesStone.begin();iter != verticesStone.end();++iter)
		verticesFinal.push_back(*iter);
	for(std::vector<GLint>::iterator iter=verticesSand.begin();iter != verticesSand.end();++iter)
		verticesFinal.push_back(*iter);
	//заполним texturesFinal
	for(std::vector<GLfloat>::iterator iter=texturesDirt.begin();iter != texturesDirt.end();++iter)
		texturesFinal.push_back(*iter);
	for(std::vector<GLfloat>::iterator iter=texturesGrassTop.begin();iter != texturesGrassTop.end();++iter)
		texturesFinal.push_back(*iter);
	for(std::vector<GLfloat>::iterator iter=texturesGrassSide.begin();iter != texturesGrassSide.end();++iter)
		texturesFinal.push_back(*iter);
	for(std::vector<GLfloat>::iterator iter=texturesStone.begin();iter != texturesStone.end();++iter)
		texturesFinal.push_back(*iter);
	for(std::vector<GLfloat>::iterator iter=texturesSand.begin();iter != texturesSand.end();++iter)
		texturesFinal.push_back(*iter);

	//сколько каких видов точек надо рисовать
	pointsOfDirtToDraw=verticesDirt.size()/3;
	pointsOfGrassTopToDraw=verticesGrassTop.size()/3;
	pointsOfGrassSideToDraw=verticesGrassSide.size()/3;
	pointsOfStoneToDraw=verticesStone.size()/3;
	pointsOfSandToDraw=verticesSand.size()/3;

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

VBOBox::VBOBox(int chNumX,int chNumZ,GameMain* _gameMain,GLuint* _texturesArrayPtr){
	gameMain=_gameMain;

	texturesPtr=_texturesArrayPtr;
	Chunk* chunkPtr=gameMain->world->getChunkPointer(chNumX,chNumZ);

	//заполним поля для координат VBOBox
	coorX=chunkPtr->getCoordX();
	coorZ=chunkPtr->getCoordZ();

	pointsOfDirtToDraw=0;
	pointsOfGrassTopToDraw=0;
	pointsOfGrassSideToDraw=0;
	pointsOfStoneToDraw=0;
	pointsOfSandToDraw=0;

	offsetOfDirt=0;
	offsetOfGrassTop=0;
	offsetOfGrassSide=0;
	offsetOfStone=0;
	offsetOfSand=0;

	std::vector<GLint> verticesDirt;
	std::vector<GLfloat> texturesDirt;
	verticesDirt.reserve(3000000);
	texturesDirt.reserve(3000000);

	std::vector<GLint> verticesGrassTop;
	std::vector<GLfloat> texturesGrassTop;
	verticesGrassTop.reserve(500000);
	texturesGrassTop.reserve(500000);

	std::vector<GLint> verticesGrassSide;
	std::vector<GLfloat> texturesGrassSide;
	verticesGrassSide.reserve(1500000);
	texturesGrassSide.reserve(1500000);

	std::vector<GLint> verticesStone;
	std::vector<GLfloat> texturesStone;
	verticesStone.reserve(1500000);
	texturesStone.reserve(1500000);

	std::vector<GLint> verticesSand;
	std::vector<GLfloat> texturesSand;
	verticesSand.reserve(1500000);
	texturesSand.reserve(1500000);

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
				BlockTransparencyAround blocksTransAround=gameMain->world->getBlockTransparencyAround(chNumX,chNumZ,blX,blY,blZ);

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
	std::vector<GLint> verticesFinal;
	std::vector<GLfloat> texturesFinal;
	verticesDirt.reserve(10000000);
	texturesDirt.reserve(10000000);
	//заполним verticesFinal
	for(std::vector<GLint>::iterator iter=verticesDirt.begin();iter != verticesDirt.end();++iter)
		verticesFinal.push_back(*iter);
	for(std::vector<GLint>::iterator iter=verticesGrassTop.begin();iter != verticesGrassTop.end();++iter)
		verticesFinal.push_back(*iter);
	for(std::vector<GLint>::iterator iter=verticesGrassSide.begin();iter != verticesGrassSide.end();++iter)
		verticesFinal.push_back(*iter);
	for(std::vector<GLint>::iterator iter=verticesStone.begin();iter != verticesStone.end();++iter)
		verticesFinal.push_back(*iter);
	for(std::vector<GLint>::iterator iter=verticesSand.begin();iter != verticesSand.end();++iter)
		verticesFinal.push_back(*iter);
	//заполним texturesFinal
	for(std::vector<GLfloat>::iterator iter=texturesDirt.begin();iter != texturesDirt.end();++iter)
		texturesFinal.push_back(*iter);
	for(std::vector<GLfloat>::iterator iter=texturesGrassTop.begin();iter != texturesGrassTop.end();++iter)
		texturesFinal.push_back(*iter);
	for(std::vector<GLfloat>::iterator iter=texturesGrassSide.begin();iter != texturesGrassSide.end();++iter)
		texturesFinal.push_back(*iter);
	for(std::vector<GLfloat>::iterator iter=texturesStone.begin();iter != texturesStone.end();++iter)
		texturesFinal.push_back(*iter);
	for(std::vector<GLfloat>::iterator iter=texturesSand.begin();iter != texturesSand.end();++iter)
		texturesFinal.push_back(*iter);

	//сколько каких видов точек надо рисовать
	pointsOfDirtToDraw=verticesDirt.size()/3;
	pointsOfGrassTopToDraw=verticesGrassTop.size()/3;
	pointsOfGrassSideToDraw=verticesGrassSide.size()/3;
	pointsOfStoneToDraw=verticesStone.size()/3;
	pointsOfSandToDraw=verticesSand.size()/3;

	//зададим смещение
	offsetOfDirt=0;
	offsetOfGrassTop=offsetOfDirt+pointsOfDirtToDraw;
	offsetOfGrassSide=offsetOfGrassTop+pointsOfGrassTopToDraw;
	offsetOfStone=offsetOfGrassSide+pointsOfGrassSideToDraw;
	offsetOfSand=offsetOfStone+pointsOfStoneToDraw;

	VBO=QGLBuffer(QGLBuffer::VertexBuffer);
	VBO.setUsagePattern( QGLBuffer::StaticDraw );
	VBO.create();
	VBO.bind();
	VBO.allocate((void *)&verticesFinal[0],verticesFinal.size()*sizeof(GLint));

	TBO=QGLBuffer(QGLBuffer::VertexBuffer);
	TBO.setUsagePattern( QGLBuffer::StaticDraw );
	TBO.create();
	TBO.bind();
	TBO.allocate((void *)&texturesFinal[0],texturesFinal.size()*sizeof(GLint));
}

VBOBox::~VBOBox(){
};

void VBOBox::draw(){
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//VBO->bind();
	//glColor3ub(0,240,0);
	//glVertexPointer ( 3, GL_INT, 0, NULL );
	//glDrawArrays(GL_QUADS, 0, pointsToDraw);

	VBO.bind();
	glVertexPointer ( 3, GL_INT, 0, NULL );
	TBO.bind();
	glTexCoordPointer ( 2, GL_FLOAT, 0, NULL );

	//binding and drawing DIRT
	glBindTexture(GL_TEXTURE_2D, *(texturesPtr+DIRT_TEX_INDEX));
	glDrawArrays(GL_QUADS, offsetOfDirt, pointsOfDirtToDraw);

	//binding and drawing GRASS_TOP
	glBindTexture(GL_TEXTURE_2D, *(texturesPtr+GRASS_TOP_TEX_INDEX));
	glDrawArrays(GL_QUADS, offsetOfGrassTop, pointsOfGrassTopToDraw);

	//binding and drawing GRASS_SIDE
	glBindTexture(GL_TEXTURE_2D, *(texturesPtr+GRASS_SIDE_TEX_INDEX));
	glDrawArrays(GL_QUADS, offsetOfGrassSide, pointsOfGrassSideToDraw);

	//binding and drawing STONE
	glBindTexture(GL_TEXTURE_2D, *(texturesPtr+STONE_TEX_INDEX));
	glDrawArrays(GL_QUADS, offsetOfStone, pointsOfStoneToDraw);

	//binding and drawing SAND
	glBindTexture(GL_TEXTURE_2D, *(texturesPtr+SAND_TEX_INDEX));
	glDrawArrays(GL_QUADS, offsetOfSand, pointsOfSandToDraw);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

int VBOBox::getCoorX(){
	return coorX;
}

int VBOBox::getCoorZ(){
	return coorZ;
}

void* VBOBox::operator new(size_t) throw(std::bad_alloc){
	for(int i=0; i<VBOBOX_HEAP_SIZE; ++i){
		if(!alloc_map[i]){
			alloc_map[i]=true;//mark as in use
			return pool+i*sizeof(VBOBox);
		}
	}
	//if space has not been found
	qDebug()<<"No more free memory to allocate from pool. See implementation of class VBOBox. Throwing std::bad_alloc.";
	throw std::bad_alloc();
}

void VBOBox::operator delete(void* ptr){
	if(!ptr){
		qDebug()<<"function VBOBox::operator delete was called with NULL parameter. Check for error.";
		return;
	}
	//вычислим номер блока из пула
	unsigned long long block=(unsigned long long) ptr- (unsigned long long)pool;
	block /= sizeof(VBOBox);
	//mark it free
	alloc_map[block]=false;
}

void* VBOBoxPrebuild::operator new(size_t) throw(std::bad_alloc){
	for(int i=0; i<VBOBOXPREBUILD_HEAP_SIZE; ++i){
		if(!alloc_map[i]){
			alloc_map[i]=true;//mark as in use
			return pool+i*sizeof(VBOBoxPrebuild);
		}
	}
	//if space has not been found
	qDebug()<<"No more free memory to allocate from pool. See implementation of class VBOBoxPrebuild. Throwing std::bad_alloc.";
	throw std::bad_alloc();
}

void VBOBoxPrebuild::operator delete(void* ptr){
	if(!ptr){
		qDebug()<<"function VBOBoxPrebuild::operator delete was called with NULL parameter. Check for error.";
		return;
	}
	//вычислим номер блока из пула
	unsigned long long block=(unsigned long long) ptr- (unsigned long long)pool;
	block /= sizeof(VBOBoxPrebuild);
	//mark it free
	alloc_map[block]=false;
}