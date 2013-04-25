#include "VBOBox.h"

//VBOBox ONLY
const int VBOBOX_HEAP_SIZE=CHUNKS_TO_DRAW*CHUNKS_TO_DRAW;
bool VBOBox::alloc_map[VBOBOX_HEAP_SIZE];//для динамического выделения памяти
unsigned char VBOBox::pool[VBOBOX_HEAP_SIZE*sizeof(VBOBox)];//для динамического выделения памяти
//~VBOBox ONLY

VBOBox::VBOBox(int chNumX,int chNumZ,GameMain* _gameMain,GLuint* _texturesArrayPtr,VBOBoxPrebuild* vBOBoxPrebuild /*= nullptr*/){
	gameMain=_gameMain;

	texturesPtr=_texturesArrayPtr;
	Chunk* chunkPtr=gameMain->world->getChunkPointer(chNumX,chNumZ);

	//заполним поля для координат VBOBox
	coorX=chunkPtr->getCoordX();
	coorZ=chunkPtr->getCoordZ();

	//финальные массивы вершин и текстурных координат
	SimpleArray<GLint,SIZE_OF_VERTICES_FINAL_ARRAYS> verticesFinal;
	SimpleArray<GLfloat,SIZE_OF_TEXTURES_FINAL_ARRAYS> texturesFinal;
	//количества точек и смещения
	VBOBoxTransferInfo vBOBoxTransferInfo;
	//количество точек в массивах
	int verticesFinalElementsCount=0;
	int texturesFinalElementsCount=0;

	if(vBOBoxPrebuild == nullptr){//если обычная медленная загрузка
		//заполним массивы
		vBOBoxTransferInfo=updateGraphicsArraysForChunk(verticesFinal,texturesFinal,gameMain,chunkPtr,
			chNumX,chNumZ,false,nullptr,nullptr,nullptr,nullptr);
		verticesFinalElementsCount=verticesFinal.getElementsCount();
		texturesFinalElementsCount=texturesFinal.getElementsCount();
	}
	else{//если используем предзагрузку
		vBOBoxTransferInfo=	*(vBOBoxPrebuild->vBOBoxTransferInfo);

		verticesFinal=vBOBoxPrebuild->verticesFinal;
		texturesFinal=vBOBoxPrebuild->texturesFinal;

		verticesFinalElementsCount=vBOBoxPrebuild->verticesFinal.getElementsCount();
		texturesFinalElementsCount=vBOBoxPrebuild->texturesFinal.getElementsCount();
	}

	//сколько каких видов точек надо рисовать
	pointsOfDirtToDraw=			vBOBoxTransferInfo.pointsOfDirtToDraw;
	pointsOfGrassTopToDraw=		vBOBoxTransferInfo.pointsOfGrassTopToDraw;
	pointsOfGrassSideToDraw=	vBOBoxTransferInfo.pointsOfGrassSideToDraw;
	pointsOfStoneToDraw=		vBOBoxTransferInfo.pointsOfStoneToDraw;
	pointsOfSandToDraw=			vBOBoxTransferInfo.pointsOfSandToDraw;
	pointsOfWaterToDraw=		vBOBoxTransferInfo.pointsOfWaterToDraw;
	pointsOfLeafsToDraw=		vBOBoxTransferInfo.pointsOfLeafsToDraw;
	pointsOfWoodToDraw=			vBOBoxTransferInfo.pointsOfWoodToDraw;
	pointsOfSnowToDraw=			vBOBoxTransferInfo.pointsOfSnowToDraw;

	//зададим смещение
	offsetOfDirt=				vBOBoxTransferInfo.offsetOfDirt;
	offsetOfGrassTop=			vBOBoxTransferInfo.offsetOfGrassTop;
	offsetOfGrassSide=			vBOBoxTransferInfo.offsetOfGrassSide;
	offsetOfStone=				vBOBoxTransferInfo.offsetOfStone;
	offsetOfSand=				vBOBoxTransferInfo.offsetOfSand;
	offsetOfWater=				vBOBoxTransferInfo.offsetOfWater;
	offsetOfLeafs=				vBOBoxTransferInfo.offsetOfLeafs;
	offsetOfWood=				vBOBoxTransferInfo.offsetOfWood;
	offsetOfSnow=				vBOBoxTransferInfo.offsetOfSnow;

	VBO=QGLBuffer(QGLBuffer::VertexBuffer);
	VBO.setUsagePattern( QGLBuffer::StaticDraw );
	VBO.create();
	VBO.bind();
	VBO.allocate((void *)&verticesFinal[0],verticesFinalElementsCount*sizeof(GLint));

	TBO=QGLBuffer(QGLBuffer::VertexBuffer);
	TBO.setUsagePattern( QGLBuffer::StaticDraw );
	TBO.create();
	TBO.bind();
	TBO.allocate((void *)&texturesFinal[0],texturesFinalElementsCount*sizeof(GLfloat));	
}

VBOBox::~VBOBox(){
};

void VBOBox::draw(){
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

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

	//binding and drawing WATER
	glBindTexture(GL_TEXTURE_2D, *(texturesPtr+WATER_TEX_INDEX));
	glDrawArrays(GL_QUADS, offsetOfWater, pointsOfWaterToDraw);

	//binding and drawing LEAFS
	glBindTexture(GL_TEXTURE_2D, *(texturesPtr+LEAFS_TEX_INDEX));
	glDrawArrays(GL_QUADS, offsetOfLeafs, pointsOfLeafsToDraw);

	//binding and drawing WOOD
	glBindTexture(GL_TEXTURE_2D, *(texturesPtr+WOOD_TEX_INDEX));
	glDrawArrays(GL_QUADS, offsetOfWood, pointsOfWoodToDraw);

	//binding and drawing SNOW
	glBindTexture(GL_TEXTURE_2D, *(texturesPtr+SNOW_TEX_INDEX));
	glDrawArrays(GL_QUADS, offsetOfSnow, pointsOfSnowToDraw);

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