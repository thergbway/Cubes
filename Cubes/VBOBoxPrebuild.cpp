#include "VBOBoxPrebuild.h"

//VBOBoxPrebuild ONLY
const int VBOBOXPREBUILD_HEAP_SIZE=VBOBOXPREBUILD_ENTIRE_LENGTH;
bool VBOBoxPrebuild::alloc_map[VBOBOXPREBUILD_HEAP_SIZE];//для динамического выделения памяти
unsigned char VBOBoxPrebuild::pool[VBOBOXPREBUILD_HEAP_SIZE*sizeof(VBOBoxPrebuild)];//для динамического выделения памяти
//~VBOBoxPrebuild ONLY

VBOBoxPrebuild::VBOBoxPrebuild(Chunk* chunkPtr ,Chunk* chBackPreloaded,Chunk* chFrontPreloaded
	,Chunk* chLeftPreloaded,Chunk* chRightPreloaded){

		vBOBoxTransferInfo=new VBOBoxTransferInfo;

		//обновим векторы, получим выходные данные
		*vBOBoxTransferInfo=updateGraphicsArraysForChunk(verticesFinal,normalsFinal,texturesFinal,nullptr
			,chunkPtr,0,0,true,chBackPreloaded,chFrontPreloaded,chLeftPreloaded,chRightPreloaded);
		//update coordinates of appropriate chunk
		coordAppChX=chunkPtr->getCoordX();
		coordAppChZ=chunkPtr->getCoordZ();
}

VBOBoxPrebuild::~VBOBoxPrebuild(){
	delete vBOBoxTransferInfo;
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