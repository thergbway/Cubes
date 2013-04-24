#include <cstdlib>
#include <crtdbg.h>
#include <QDebug>
#include "worldlayerholder.h"
#include "defines.h"

WorldLayerHolder::WorldLayerHolder(){
	//инициализируем в некоторой точке
	reloadLayers(15*CUBE_SIZE,-27*CUBE_SIZE);
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
	if(flag1 || flag2){//выдадим, что знаем, остальное по одному уровню
		for(int i0=0,i1=indexX1; i1<indexX2; ++i1, ++i0){
			for(int j0=0, j1=indexZ1; j1<indexZ2; ++j1, ++j0){
				if(i1>=0 && i1<BLOCK_CHAINED_COUNT
					&& j1>=0 && j1<BLOCK_CHAINED_COUNT){
					outputLayerTransfer.stoneLayer[i0][j0]=stoneLayer[i1][j1];
					outputLayerTransfer.dirtLayer[i0][j0]=dirtLayer[i1][j1];
				}
				else{
					outputLayerTransfer.stoneLayer[i0][j0]=8;
					outputLayerTransfer.dirtLayer[i0][j0]=8;
				}
			}
		}
	}
	else{//иначе выдадим один уровень (просто для теста)
		for(int i=0; i<BLOCK_COUNT; ++i){
			for(int j=0; j<BLOCK_COUNT; ++j){
				if(i%2 == 0 && j%2 ==0){
					outputLayerTransfer.stoneLayer[i][j]=5;
					outputLayerTransfer.dirtLayer[i][j]=5;
				}
				else{
					outputLayerTransfer.stoneLayer[i][j]=10;
					outputLayerTransfer.dirtLayer[i][j]=10;
				}
			}
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

	//for (int i = 0; i < 15; i++) {
	for (int i = 0; i < 2; i++) {
		xm7          = x % 7;
		xm13        = x % 13;
		xm1301081 = x % 1301081;
		ym8461     = z % 8461;
		ym105467  = z % 105467;
		ym105943  = z % 105943;
		z += x + seed;
		x += (xm7 + xm13 + xm1301081 + ym8461 + ym105467 + ym105943 + seed*seed);
	}
	int result=abs((xm7 + xm13 + xm1301081 + ym8461 + ym105467 + ym105943) / 1520972.0*10000);
	result%=(max-min+1);//from min to max
	result+=min;
	return result;
}

bool WorldLayerHolder::chance(int first,int ofSecond,int seed, int x, int z){
	int number=random(1,ofSecond,seed,x,z);
	if(number<=first)
		return true;
	else
		return false;
}

void WorldLayerHolder::reloadLayers(int coordMainX, int coordMainZ){
	qDebug()<<"Starting reloading layers";
	//эта функция вызывается из WorldLayerHolder::updateLayerTransferForChunk с новыми координатами, если старые неверны
	coordX=coordMainX;
	coordZ=coordMainZ;

	//создадим предварительный слой для камня
	float stoneTmpLayer[BLOCK_CHAINED_COUNT][BLOCK_CHAINED_COUNT];
	//заполним слой недействительными блоками для отличия
	for(int i=0; i<BLOCK_CHAINED_COUNT; ++i)
		for(int j=0; j<BLOCK_CHAINED_COUNT; ++j)
			stoneTmpLayer[i][j]=-500;
	//угловые значения
	//stoneTmpLayer[0][0]=random(STONE_MIN,STONE_MAX,seed,coordX,coordZ);
	//stoneTmpLayer[0][BLOCK_CHAINED_COUNT-1]=random(STONE_MIN,STONE_MAX,seed,coordX,coordZ+BLOCK_CHAINED_COUNT*CUBE_SIZE);
	//stoneTmpLayer[BLOCK_CHAINED_COUNT-1][BLOCK_CHAINED_COUNT-1]=random(STONE_MIN,STONE_MAX,seed,coordX+BLOCK_CHAINED_COUNT*CUBE_SIZE,coordZ+BLOCK_CHAINED_COUNT*CUBE_SIZE);
	//stoneTmpLayer[BLOCK_CHAINED_COUNT-1][0]=random(STONE_MIN,STONE_MAX,seed,coordX+BLOCK_CHAINED_COUNT*CUBE_SIZE,coordZ);
	stoneTmpLayer[0][0]=10;
	stoneTmpLayer[0][BLOCK_CHAINED_COUNT-1]=50;
	stoneTmpLayer[BLOCK_CHAINED_COUNT-1][BLOCK_CHAINED_COUNT-1]=10;
	stoneTmpLayer[BLOCK_CHAINED_COUNT-1][0]=40;

	//теперь заполним предварительный слой
	int base=BLOCK_CHAINED_COUNT-1;//= square or diamond size -1 for the current iteration
	while(base >= 2){
		//MAKE SQUARE
		//сколько квадратов поместится по стороне
		int sqrForSide=(BLOCK_CHAINED_COUNT-1)/base;
		for(int i=0; i < sqrForSide; ++i){
			for(int j=0; j < sqrForSide; ++j){
				//squareStep
				int indexLeftBackX=i*base;
				int indexLeftBackZ=j*base;
				int indexRightFrontX=i*base+base;
				int indexRightFrontZ=j*base+base;
				stoneMakeSquare(indexLeftBackX,indexLeftBackZ,indexRightFrontX,indexRightFrontZ,stoneTmpLayer);
			}
		}
		//MAKE DIAMOND
		//снова проходим как шаг КВАДРАТ только функция теперь DIAMOND
		//сколько квадратов поместится по стороне (уже инициализирована переменная)
		sqrForSide=(BLOCK_CHAINED_COUNT-1)/base;
		for(int i=0; i < sqrForSide; ++i){
			for(int j=0; j < sqrForSide; ++j){
				//squareStep
				int indexLeftBackX=i*base;
				int indexLeftBackZ=j*base;
				int indexRightFrontX=i*base+base;
				int indexRightFrontZ=j*base+base;
				int indexCentralX=(indexRightFrontX-indexLeftBackX)/2+indexLeftBackX;
				int indexCentralZ=(indexRightFrontZ-indexLeftBackZ)/2+indexLeftBackZ;
				stoneMakeDiamond(indexLeftBackX,indexLeftBackZ,indexCentralX,indexCentralZ,stoneTmpLayer);
				stoneMakeDiamond(indexCentralX,indexCentralZ-base,indexRightFrontX,indexLeftBackZ,stoneTmpLayer);
				stoneMakeDiamond(indexRightFrontX,indexLeftBackZ,indexCentralX+base,indexCentralZ,stoneTmpLayer);
				stoneMakeDiamond(indexCentralX,indexCentralZ,indexRightFrontX,indexRightFrontZ,stoneTmpLayer);
			}
		}
		//уменьшаем базу
		base/=2;
	}
	//сглаживание 3 на 3 с изменением середины только
	qDebug()<<"Smoothing stones 1";
	for(int i=1; i<BLOCK_CHAINED_COUNT-1; ++i)
		for(int j=1; j<BLOCK_CHAINED_COUNT-1; ++j){
			float average=stoneTmpLayer[i-1][j-1]+stoneTmpLayer[i][j-1]+stoneTmpLayer[i+1][j-1]+
				stoneTmpLayer[i-1][j]+stoneTmpLayer[i][j]+stoneTmpLayer[i+1][j]+
				stoneTmpLayer[i-1][j+1]+stoneTmpLayer[i][j+1]+stoneTmpLayer[i+1][j+1];
			average/=9;
			stoneTmpLayer[i][j]=average;
		}

	qDebug()<<"Smoothing stones 2";
	for(int i=1; i<BLOCK_CHAINED_COUNT-1; ++i)
		for(int j=1; j<BLOCK_CHAINED_COUNT-1; ++j){
			float average=stoneTmpLayer[i-1][j-1]+stoneTmpLayer[i][j-1]+stoneTmpLayer[i+1][j-1]+
				stoneTmpLayer[i-1][j]+stoneTmpLayer[i][j]+stoneTmpLayer[i+1][j]+
				stoneTmpLayer[i-1][j+1]+stoneTmpLayer[i][j+1]+stoneTmpLayer[i+1][j+1];
			average/=9;
			stoneTmpLayer[i][j]=average;
		}

	qDebug()<<"Smoothing stones 3";
	for(int i=1; i<BLOCK_CHAINED_COUNT-1; ++i)
		for(int j=1; j<BLOCK_CHAINED_COUNT-1; ++j){
			float average=stoneTmpLayer[i-1][j-1]+stoneTmpLayer[i][j-1]+stoneTmpLayer[i+1][j-1]+
				stoneTmpLayer[i-1][j]+stoneTmpLayer[i][j]+stoneTmpLayer[i+1][j]+
				stoneTmpLayer[i-1][j+1]+stoneTmpLayer[i][j+1]+stoneTmpLayer[i+1][j+1];
			average/=9;
			stoneTmpLayer[i][j]=average;
		}

	qDebug()<<"Smoothing stones 4";
	for(int i=1; i<BLOCK_CHAINED_COUNT-1; ++i)
		for(int j=1; j<BLOCK_CHAINED_COUNT-1; ++j){
			float average=stoneTmpLayer[i-1][j-1]+stoneTmpLayer[i][j-1]+stoneTmpLayer[i+1][j-1]+
				stoneTmpLayer[i-1][j]+stoneTmpLayer[i][j]+stoneTmpLayer[i+1][j]+
				stoneTmpLayer[i-1][j+1]+stoneTmpLayer[i][j+1]+stoneTmpLayer[i+1][j+1];
			average/=9;
			stoneTmpLayer[i][j]=average;
		}

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

	//обрабатываем высоту земли
	//создадим предварительный слой для земли
	float dirtTmpLayer[BLOCK_CHAINED_COUNT][BLOCK_CHAINED_COUNT];
	//заполним слой недействительными блоками для отличия
	for(int i=0; i<BLOCK_CHAINED_COUNT; ++i)
		for(int j=0; j<BLOCK_CHAINED_COUNT; ++j)
			dirtTmpLayer[i][j]=-500;
	//угловые значения
	//dirtTmpLayer[0][0]=random(DIRT_MIN,DIRT_MAX,seed*2,coordX,coordZ);
	//dirtTmpLayer[0][BLOCK_CHAINED_COUNT-1]=random(DIRT_MIN,DIRT_MAX,seed*2,coordX,coordZ+BLOCK_CHAINED_COUNT*CUBE_SIZE);
	//dirtTmpLayer[BLOCK_CHAINED_COUNT-1][BLOCK_CHAINED_COUNT-1]=random(DIRT_MIN,DIRT_MAX,seed*2,coordX+BLOCK_CHAINED_COUNT*CUBE_SIZE,coordZ+BLOCK_CHAINED_COUNT*CUBE_SIZE);
	//dirtTmpLayer[BLOCK_CHAINED_COUNT-1][0]=random(STONE_MIN,STONE_MAX,seed*2,coordX+BLOCK_CHAINED_COUNT*CUBE_SIZE,coordZ);
	
	dirtTmpLayer[0][0]=10;
	dirtTmpLayer[0][BLOCK_CHAINED_COUNT-1]=14;
	dirtTmpLayer[BLOCK_CHAINED_COUNT-1][BLOCK_CHAINED_COUNT-1]=22;
	dirtTmpLayer[BLOCK_CHAINED_COUNT-1][0]=5;

	//теперь заполним предварительный слой
	base=BLOCK_CHAINED_COUNT-1;//= square or diamond size -1 for the current iteration
	while(base >= 2){
		//MAKE SQUARE
		//сколько квадратов поместится по стороне
		int sqrForSide=(BLOCK_CHAINED_COUNT-1)/base;
		for(int i=0; i < sqrForSide; ++i){
			for(int j=0; j < sqrForSide; ++j){
				//squareStep
				int indexLeftBackX=i*base;
				int indexLeftBackZ=j*base;
				int indexRightFrontX=i*base+base;
				int indexRightFrontZ=j*base+base;
				dirtMakeSquare(indexLeftBackX,indexLeftBackZ,indexRightFrontX,indexRightFrontZ,dirtTmpLayer);
			}
		}
		//MAKE DIAMOND
		//снова проходим как шаг КВАДРАТ только функция теперь DIAMOND
		//сколько квадратов поместится по стороне (уже инициализирована переменная)
		sqrForSide=(BLOCK_CHAINED_COUNT-1)/base;
		for(int i=0; i < sqrForSide; ++i){
			for(int j=0; j < sqrForSide; ++j){
				//squareStep
				int indexLeftBackX=i*base;
				int indexLeftBackZ=j*base;
				int indexRightFrontX=i*base+base;
				int indexRightFrontZ=j*base+base;
				int indexCentralX=(indexRightFrontX-indexLeftBackX)/2+indexLeftBackX;
				int indexCentralZ=(indexRightFrontZ-indexLeftBackZ)/2+indexLeftBackZ;
				dirtMakeDiamond(indexLeftBackX,indexLeftBackZ,indexCentralX,indexCentralZ,dirtTmpLayer);
				dirtMakeDiamond(indexCentralX,indexCentralZ-base,indexRightFrontX,indexLeftBackZ,dirtTmpLayer);
				dirtMakeDiamond(indexRightFrontX,indexLeftBackZ,indexCentralX+base,indexCentralZ,dirtTmpLayer);
				dirtMakeDiamond(indexCentralX,indexCentralZ,indexRightFrontX,indexRightFrontZ,dirtTmpLayer);
			}
		}
		//уменьшаем базу
		base/=2;
	}
	//сглаживание 3 на 3 с изменением середины только
	qDebug()<<"Smoothing dirt 1";
	for(int i=1; i<BLOCK_CHAINED_COUNT-1; ++i)
		for(int j=1; j<BLOCK_CHAINED_COUNT-1; ++j){
			float average=dirtTmpLayer[i-1][j-1]+dirtTmpLayer[i][j-1]+dirtTmpLayer[i+1][j-1]+
				dirtTmpLayer[i-1][j]+dirtTmpLayer[i][j]+dirtTmpLayer[i+1][j]+
				dirtTmpLayer[i-1][j+1]+dirtTmpLayer[i][j+1]+dirtTmpLayer[i+1][j+1];
			average/=9;
			dirtTmpLayer[i][j]=average;
		}

	qDebug()<<"Smoothing dirt 2";
	for(int i=1; i<BLOCK_CHAINED_COUNT-1; ++i)
		for(int j=1; j<BLOCK_CHAINED_COUNT-1; ++j){
			float average=dirtTmpLayer[i-1][j-1]+dirtTmpLayer[i][j-1]+dirtTmpLayer[i+1][j-1]+
				dirtTmpLayer[i-1][j]+dirtTmpLayer[i][j]+dirtTmpLayer[i+1][j]+
				dirtTmpLayer[i-1][j+1]+dirtTmpLayer[i][j+1]+dirtTmpLayer[i+1][j+1];
			average/=9;
			dirtTmpLayer[i][j]=average;
		}

	qDebug()<<"Smoothing dirt 3";
	for(int i=1; i<BLOCK_CHAINED_COUNT-1; ++i)
		for(int j=1; j<BLOCK_CHAINED_COUNT-1; ++j){
			float average=dirtTmpLayer[i-1][j-1]+dirtTmpLayer[i][j-1]+dirtTmpLayer[i+1][j-1]+
				dirtTmpLayer[i-1][j]+dirtTmpLayer[i][j]+dirtTmpLayer[i+1][j]+
				dirtTmpLayer[i-1][j+1]+dirtTmpLayer[i][j+1]+dirtTmpLayer[i+1][j+1];
			average/=9;
			dirtTmpLayer[i][j]=average;
		}

	qDebug()<<"Smoothing dirt 4";
	for(int i=1; i<BLOCK_CHAINED_COUNT-1; ++i)
		for(int j=1; j<BLOCK_CHAINED_COUNT-1; ++j){
			float average=dirtTmpLayer[i-1][j-1]+dirtTmpLayer[i][j-1]+dirtTmpLayer[i+1][j-1]+
				dirtTmpLayer[i-1][j]+dirtTmpLayer[i][j]+dirtTmpLayer[i+1][j]+
				dirtTmpLayer[i-1][j+1]+dirtTmpLayer[i][j+1]+dirtTmpLayer[i+1][j+1];
			average/=9;
			dirtTmpLayer[i][j]=average;
		}

	//переведем временный слой в основной слой
	for(int i=0; i<BLOCK_CHAINED_COUNT; ++i){
		for(int j=0; j<BLOCK_CHAINED_COUNT; ++j){
			//округлим правильно
			if(((int)(dirtTmpLayer[i][j]*100))%100 >=50)
				dirtLayer[i][j]=((byte)dirtTmpLayer[i][j])+1;
			else
				dirtLayer[i][j]=(byte)dirtTmpLayer[i][j];
		}
	}

	qDebug()<<"Finished reloading layers";
	qDebug()<<"Stone corners:"<<(byte)stoneLayer[0][0]<<(byte)stoneLayer[0][BLOCK_CHAINED_COUNT-1]<<(byte)stoneLayer[BLOCK_CHAINED_COUNT-1][BLOCK_CHAINED_COUNT-1]<<(byte)stoneLayer[BLOCK_CHAINED_COUNT-1][0];
	qDebug()<<"Dirt corners:"<<(byte)dirtLayer[0][0]<<(byte)dirtLayer[0][BLOCK_CHAINED_COUNT-1]<<(byte)dirtLayer[BLOCK_CHAINED_COUNT-1][BLOCK_CHAINED_COUNT-1]<<(byte)dirtLayer[BLOCK_CHAINED_COUNT-1][0];
}

void WorldLayerHolder::stoneMakeSquare(int indexLeftBackX,int indexLeftBackZ,int indexRightFrontX, int indexRightFrontZ, float stoneTmpLayer[BLOCK_CHAINED_COUNT][BLOCK_CHAINED_COUNT]){
	int sqrSize=indexRightFrontX-indexLeftBackX;
	float factor=sqrSize/STONE_SPARPNESS;
	int sign=1;
	if(random(30,50,56516,indexLeftBackX*indexLeftBackX/indexRightFrontX*indexLeftBackZ,indexLeftBackZ*indexRightFrontX)>40)
		sign=-1;
	float randDispl=sign*random(5,10,56516,indexLeftBackX*indexLeftBackX/indexRightFrontX*indexLeftBackZ,indexLeftBackZ*indexRightFrontX)*factor;
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
}

void WorldLayerHolder::stoneMakeDiamond(int indexBackX,int indexBackZ,int indexRightX, int indexRightZ, float stoneTmpLayer[BLOCK_CHAINED_COUNT][BLOCK_CHAINED_COUNT]){
	int sqrSize=indexRightX-indexBackX;
	float factor=sqrSize/STONE_SPARPNESS;
	int sign=1;
	if(random(30,50,56516,indexBackX*indexBackX/indexRightX*indexBackZ,indexBackZ*indexRightX)>40)
		sign=-1;
	float randDispl=sign*random(5,15,56516,indexBackX*indexBackX/indexRightX*indexBackZ,indexBackZ*indexRightX)*factor;
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

void WorldLayerHolder::dirtMakeSquare(int indexLeftBackX,int indexLeftBackZ,int indexRightFrontX, int indexRightFrontZ, float dirtTmpLayer[BLOCK_CHAINED_COUNT][BLOCK_CHAINED_COUNT]){
	int sqrSize=indexRightFrontX-indexLeftBackX;
	float factor=sqrSize/DIRT_SPARPNESS;
	int sign=1;
	if(random(30,50,99516,indexLeftBackX*indexLeftBackX/indexRightFrontX*indexLeftBackZ,indexLeftBackZ*indexRightFrontX)>40)
		sign=-1;
	float randDispl=sign*random(5,10,122504,indexLeftBackX*indexLeftBackX/indexRightFrontX*indexLeftBackZ,indexLeftBackZ*indexRightFrontX)*factor;
	int currIndexX=(indexRightFrontX-indexLeftBackX)/2+indexLeftBackX;
	int currIndexZ=(indexRightFrontZ-indexLeftBackZ)/2+indexLeftBackZ;
	float average=dirtTmpLayer[indexLeftBackX][indexLeftBackZ]+
		dirtTmpLayer[indexLeftBackX][indexRightFrontZ]+
		dirtTmpLayer[indexRightFrontX][indexRightFrontZ]+
		dirtTmpLayer[indexRightFrontX][indexLeftBackZ];
	average/=4;
	dirtTmpLayer[currIndexX][currIndexZ]=average+randDispl;
	//если вышли за границы
	if(dirtTmpLayer[currIndexX][currIndexZ]<DIRT_MIN)
		dirtTmpLayer[currIndexX][currIndexZ]=DIRT_MIN;
	if(dirtTmpLayer[currIndexX][currIndexZ]>DIRT_MAX)
		dirtTmpLayer[currIndexX][currIndexZ]=DIRT_MAX;
}

void WorldLayerHolder::dirtMakeDiamond(int indexBackX,int indexBackZ,int indexRightX, int indexRightZ, float dirtTmpLayer[BLOCK_CHAINED_COUNT][BLOCK_CHAINED_COUNT]){
	int sqrSize=indexRightX-indexBackX;
	float factor=sqrSize/DIRT_SPARPNESS;
	int sign=1;
	if(random(30,50,455177,indexBackX*indexBackX/indexRightX*indexBackZ,indexBackZ*indexRightX)>40)
		sign=-1;
	float randDispl=sign*random(5,15,401544,indexBackX*indexBackX/indexRightX*indexBackZ,indexBackZ*indexRightX)*factor;
	int currIndexX=indexBackX;
	int currIndexZ=indexRightZ;
	int diamondSize=(indexRightX-indexBackX)*2;
	float average=0;
	int divider=0;

	if(indexBackZ >= 0){
		average+=dirtTmpLayer[indexBackX][indexBackZ];
		++divider;
	}

	if(indexRightX <BLOCK_CHAINED_COUNT){
		average+=dirtTmpLayer[indexRightX][indexRightZ];
		++divider;
	}

	if(indexBackZ+diamondSize < BLOCK_CHAINED_COUNT){
		average+=dirtTmpLayer[indexBackX][indexBackZ+diamondSize];
		++divider;
	}

	if(indexRightX-diamondSize >=0){
		average+=dirtTmpLayer[indexRightX-diamondSize][indexRightZ];
		++divider;
	}

	average/=divider;

	dirtTmpLayer[currIndexX][currIndexZ]=average+randDispl;
	//не вышли ли за границы?
	if(dirtTmpLayer[currIndexX][currIndexZ] <	DIRT_MIN)
		dirtTmpLayer[currIndexX][currIndexZ]=	DIRT_MIN;
	if(dirtTmpLayer[currIndexX][currIndexZ] >	DIRT_MAX)
		dirtTmpLayer[currIndexX][currIndexZ]=	DIRT_MAX;
	//рекурсию эта функция не вызывает
	return;
}