#include <cstdlib>
#include <crtdbg.h>
#include <QDebug>
#include "worldlayerholder.h"
#include "defines.h"

WorldLayerHolder::WorldLayerHolder(GameMain* gameMainPtr)
	:gameMain(gameMainPtr){
	//инициализируем в некоторой точке
	reloadLayers(0,0);
}

void WorldLayerHolder::updateLayerTransferForChunk(int coorChX,int coorChZ,LayerTransfer& outputLayerTransfer){
	//если требуемая геометрия уже загружена - выдаем её
	//будем проверять по двум точкам- блокам
	int coorX1=coorChX;
	int	coorX2=coorChX+BLOCK_COUNT*CUBE_SIZE;
	int	coorZ1=coorChZ;
	int	coorZ2=coorChZ+BLOCK_COUNT*CUBE_SIZE;
	//вычислим предполагаемые индексы этих двух точек- блоков
	int indexX1=(coorX1-coordX)/CUBE_SIZE;
	int indexZ1=(coorZ1-coordZ)/CUBE_SIZE;
	int indexX2=(coorX2-coordX)/CUBE_SIZE;
	int indexZ2=(coorZ2-coordZ)/CUBE_SIZE;
	//теперь проверим, не нужно ли подгружать
	bool flag1=(indexX1>=0 && indexZ1>=0);
	bool flag2=(indexX2<BLOCK_CHAINED_COUNT && indexZ2<BLOCK_CHAINED_COUNT);
	//if(flag1 || flag2){//выдадим, что знаем, остальное по одному уровню
	//	for(int i0=0,i1=indexX1; i1<indexX2; ++i1, ++i0){
	//		for(int j0=0, j1=indexZ1; j1<indexZ2; ++j1, ++j0){
	//			if(i1>=0 && i1<BLOCK_CHAINED_COUNT
	//				&& j1>=0 && j1<BLOCK_CHAINED_COUNT)
	//				outputLayerTransfer.stoneLayer[i0][j0]=stoneLayer[i1][j1];
	//			else
	//				outputLayerTransfer.stoneLayer[i0][j0]=8;
	//		}
	//	}
	//}
	//else{//иначе выдадим один уровень (просто для теста)
	//	for(int i=0; i<BLOCK_COUNT; ++i){
	//		for(int j=0; j<BLOCK_COUNT; ++j){
	//			outputLayerTransfer.stoneLayer[i][j]=5;
	//		}
	//	}
	//}

	//для теста
	for(int i0=0,i1=indexX1; i1<indexX2; ++i1, ++i0){
		for(int j0=0, j1=indexZ1; j1<indexZ2; ++j1, ++j0){
			if(i1>=0 && i1<BLOCK_CHAINED_COUNT
				&& j1>=0 && j1<BLOCK_CHAINED_COUNT)
				outputLayerTransfer.stoneLayer[i0][j0]=stoneLayer[i1][j1];
			else
				outputLayerTransfer.stoneLayer[i0][j0]=8;
		}
	}
}

int WorldLayerHolder::random(int min,int max,int seed,int x,int z){
	int xm7            =234;       
	int xm13           =166;      
	int xm1301081      =545; 
	int ym8461         =180;    
	int ym105467       =609;  
	int ym105943       =554;  

	for (int i = 0; i < 15; i++) {
		xm7          = x % 7;
		xm13        = x % 13;
		xm1301081 = x % 1301081;
		ym8461     = z % 8461;
		ym105467  = z % 105467;
		ym105943  = z % 105943;
		z += x + seed;
		x += (xm7 + xm13 + xm1301081 + ym8461 + ym105467 + ym105943);
	}
	int result=abs((xm7 + xm13 + xm1301081 + ym8461 + ym105467 + ym105943) / 1520972.0*10000);
	result%=(max-min+1);//from min to max
	result+=min;
	return result;
}

void WorldLayerHolder::reloadLayers(int coordMainX, int coordMainZ){
	//эта функция вызывается из WorldLayerHolder::updateLayerTransferForChunk с новыми координатами, если старые неверны
	coordX=coordMainX;
	coordZ=coordMainZ;

	//создадим предварительный слой для камня
	float stoneTmpLayer[BLOCK_CHAINED_COUNT][BLOCK_CHAINED_COUNT];
	//заполним слой недействительными блоками для отличия
	for(int i=0; i<BLOCK_CHAINED_COUNT; ++i)
		for(int j=0; j<BLOCK_CHAINED_COUNT; ++j)
			stoneTmpLayer[i][j]=0;
	//угловые значения
	//stoneTmpLayer[0][0]=random(STONE_MAX,STONE_MAX,seed,coordX,coordZ);
	//stoneTmpLayer[0][BLOCK_CHAINED_COUNT-1]=random(STONE_MAX,STONE_MAX,seed,coordX,coordZ+BLOCK_CHAINED_COUNT*CUBE_SIZE);
	//stoneTmpLayer[BLOCK_CHAINED_COUNT-1][BLOCK_CHAINED_COUNT-1]=random(STONE_MAX,STONE_MAX,seed,coordX+BLOCK_CHAINED_COUNT*CUBE_SIZE,coordZ+BLOCK_CHAINED_COUNT*CUBE_SIZE);
	//stoneTmpLayer[BLOCK_CHAINED_COUNT-1][0]=random(STONE_MAX,STONE_MAX,seed,coordX+BLOCK_CHAINED_COUNT*CUBE_SIZE,coordZ);
	stoneTmpLayer[0][0]=10;
	stoneTmpLayer[0][BLOCK_CHAINED_COUNT-1]=20;
	stoneTmpLayer[BLOCK_CHAINED_COUNT-1][BLOCK_CHAINED_COUNT-1]=30;
	stoneTmpLayer[BLOCK_CHAINED_COUNT-1][0]=40;

	//начинаем рекурсивное заполнение предварительного слоя
	stoneMakeSquare(0,0,BLOCK_CHAINED_COUNT-1,BLOCK_CHAINED_COUNT-1,stoneTmpLayer);
	//переведем временный слой в основной слой
	for(int i=0; i<BLOCK_CHAINED_COUNT; ++i){
		for(int j=0; j<BLOCK_CHAINED_COUNT; ++j){
			//округлим правильно
			if(((int)(stoneTmpLayer[i][j]*100))%100 >=50)
				stoneLayer[i][j]=((byte)stoneTmpLayer[i][j])+1;
			else
				stoneLayer[i][j]=(byte)stoneTmpLayer[i][j];
		}
	}

	qDebug()<<(byte)stoneLayer[0][0]<<(byte)stoneLayer[0][BLOCK_CHAINED_COUNT-1]<<(byte)stoneLayer[BLOCK_CHAINED_COUNT-1][BLOCK_CHAINED_COUNT-1]<<(byte)stoneLayer[BLOCK_CHAINED_COUNT-1][0];
}

void WorldLayerHolder::stoneMakeSquare(int indexLeftBackX,int indexLeftBackZ,int indexRightFrontX, int indexRightFrontZ, float stoneTmpLayer[BLOCK_CHAINED_COUNT][BLOCK_CHAINED_COUNT]){
	float randDispl=0;
	int currIndexX=(indexRightFrontX-indexLeftBackX)/2+indexLeftBackX;
	int currIndexZ=(indexRightFrontZ-indexLeftBackZ)/2+indexLeftBackZ;
	float average=stoneTmpLayer[indexLeftBackX][indexLeftBackZ]+
		stoneTmpLayer[indexLeftBackX][indexRightFrontZ]+
		stoneTmpLayer[indexRightFrontX][indexRightFrontZ]+
		stoneTmpLayer[indexRightFrontX][indexLeftBackZ];
	average/=4;
	stoneTmpLayer[currIndexX][currIndexZ]=average+randDispl;
	//если вышли за границы
	if(stoneTmpLayer[currIndexX][currIndexZ]<STONE_MIN)
		stoneTmpLayer[currIndexX][currIndexZ]=STONE_MIN;
	if(stoneTmpLayer[currIndexX][currIndexZ]>STONE_MAX)
		stoneTmpLayer[currIndexX][currIndexZ]=STONE_MAX;
	//makeDiamondStep
	stoneMakeDiamond(indexLeftBackX,indexLeftBackZ,currIndexX,currIndexZ,stoneTmpLayer);
	stoneMakeDiamond(currIndexX,currIndexZ-(indexRightFrontZ-indexLeftBackZ),indexRightFrontX,indexLeftBackZ,stoneTmpLayer);
	stoneMakeDiamond(indexRightFrontX,indexLeftBackZ,indexRightFrontX+(indexRightFrontX-indexLeftBackX)/2,currIndexZ,stoneTmpLayer);
	stoneMakeDiamond(currIndexX,currIndexZ,indexRightFrontX,indexRightFrontZ,stoneTmpLayer);
	//если это шаг на самом малом квадрате- выйдем из рекурсии
	if(indexRightFrontX-indexLeftBackX == 2)
		return;
	//иначе рекурсия малых квадратов
	stoneMakeSquare(indexLeftBackX,indexLeftBackZ,currIndexX,currIndexZ,stoneTmpLayer);
	stoneMakeSquare(currIndexX,indexLeftBackZ,indexRightFrontX,currIndexZ,stoneTmpLayer);
	stoneMakeSquare(currIndexX,currIndexZ,indexRightFrontX,indexRightFrontZ,stoneTmpLayer);
	stoneMakeSquare(indexLeftBackX,currIndexZ,currIndexX,indexRightFrontZ,stoneTmpLayer);
}

void WorldLayerHolder::stoneMakeDiamond(int indexBackX,int indexBackZ,int indexRightX, int indexRightZ, float stoneTmpLayer[BLOCK_CHAINED_COUNT][BLOCK_CHAINED_COUNT]){
	float randDispl=0;
	int currIndexX=indexBackX;
	int currIndexZ=indexRightZ;
	int diamondSize=(indexRightX-indexBackX)*2;
	float average=0;
	int divider=0;

	if(indexBackZ >= 0){
		average+=stoneTmpLayer[indexBackX][indexBackZ];
		++divider;
	}

	if(indexRightX <BLOCK_CHAINED_COUNT){
		average+=stoneTmpLayer[indexRightX][indexRightZ];
		++divider;
	}

	if(indexBackZ+diamondSize < BLOCK_CHAINED_COUNT){
		average+=stoneTmpLayer[indexBackX][indexBackZ+diamondSize];
		++divider;
	}

	if(indexRightX-diamondSize >=0){
		average+=stoneTmpLayer[indexRightX-diamondSize][indexRightZ];
		++divider;
	}

	average/=divider;

	stoneTmpLayer[currIndexX][currIndexZ]=average+randDispl;
	//не вышли ли за границы?
	if(stoneTmpLayer[currIndexX][currIndexZ] < STONE_MIN)
		stoneTmpLayer[currIndexX][currIndexZ]=STONE_MIN;
	if(stoneTmpLayer[currIndexX][currIndexZ] > STONE_MAX)
		stoneTmpLayer[currIndexX][currIndexZ]=STONE_MAX;
	//рекурсию эта функция не вызывает
	return;
}