#include <cstdlib>
#include <crtdbg.h>
#include <QDebug>
#include "worldlayerholder.h"
#include "defines.h"

WorldLayerHolder::WorldLayerHolder(){
	//инициализируем в некоторой точке
	reloadLayers(0*CUBE_SIZE,0*CUBE_SIZE);
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
					outputLayerTransfer.waterLayer[i0][j0]=waterLayer[i1][j1];
					outputLayerTransfer.sandLayer[i0][j0]=sandLayer[i1][j1];
					outputLayerTransfer.snowLayer[i0][j0]=snowLayer[i1][j1];
					outputLayerTransfer.woodLayer[i0][j0]=woodLayer[i1][j1];
				}
				else{
					outputLayerTransfer.stoneLayer[i0][j0]=1;
					outputLayerTransfer.dirtLayer[i0][j0]=1;
					outputLayerTransfer.waterLayer[i0][j0]=0;
					outputLayerTransfer.sandLayer[i0][j0]=0;
					outputLayerTransfer.snowLayer[i0][j0]=0;
					outputLayerTransfer.woodLayer[i0][j0]=0;
				}
			}
		}
	}
	else{//иначе выдадим один уровень (просто для теста)
		for(int i=0; i<BLOCK_COUNT; ++i){
			for(int j=0; j<BLOCK_COUNT; ++j){
				if(i%2 == 0 && j%2 ==0){
					outputLayerTransfer.stoneLayer[i][j]=1;
					outputLayerTransfer.dirtLayer[i][j]=1;
					outputLayerTransfer.waterLayer[i][j]=0;
					outputLayerTransfer.sandLayer[i][j]=0;
					outputLayerTransfer.snowLayer[i][j]=0;
					outputLayerTransfer.woodLayer[i][j]=0;
				}
				else{
					outputLayerTransfer.stoneLayer[i][j]=1;
					outputLayerTransfer.dirtLayer[i][j]=1;
					outputLayerTransfer.waterLayer[i][j]=0;
					outputLayerTransfer.sandLayer[i][j]=0;
					outputLayerTransfer.snowLayer[i][j]=0;
					outputLayerTransfer.woodLayer[i][j]=0;
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
	for (int i = 0; i < 1; i++) {
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
	stoneTmpLayer[0][0]=random(STONE_MIN,STONE_MAX,seed,coordX,coordZ);
	stoneTmpLayer[0][BLOCK_CHAINED_COUNT-1]=random(STONE_MIN,STONE_MAX,seed,coordX,coordZ+BLOCK_CHAINED_COUNT*CUBE_SIZE);
	stoneTmpLayer[BLOCK_CHAINED_COUNT-1][BLOCK_CHAINED_COUNT-1]=random(STONE_MIN,STONE_MAX,seed,coordX+BLOCK_CHAINED_COUNT*CUBE_SIZE,coordZ+BLOCK_CHAINED_COUNT*CUBE_SIZE);
	stoneTmpLayer[BLOCK_CHAINED_COUNT-1][0]=random(STONE_MIN,STONE_MAX,seed,coordX+BLOCK_CHAINED_COUNT*CUBE_SIZE,coordZ);

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
	dirtTmpLayer[0][0]=random(DIRT_MIN,DIRT_MAX,seed*2,coordX,coordZ);
	dirtTmpLayer[0][BLOCK_CHAINED_COUNT-1]=random(DIRT_MIN,DIRT_MAX,seed*2,coordX,coordZ+BLOCK_CHAINED_COUNT*CUBE_SIZE);
	dirtTmpLayer[BLOCK_CHAINED_COUNT-1][BLOCK_CHAINED_COUNT-1]=random(DIRT_MIN,DIRT_MAX,seed*2,coordX+BLOCK_CHAINED_COUNT*CUBE_SIZE,coordZ+BLOCK_CHAINED_COUNT*CUBE_SIZE);
	dirtTmpLayer[BLOCK_CHAINED_COUNT-1][0]=random(STONE_MIN,STONE_MAX,seed*2,coordX+BLOCK_CHAINED_COUNT*CUBE_SIZE,coordZ);

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

	//переведем слой земли в обычную высоту, те если слой высотой 28, то в массиве будут значения от 1 до 28
	for(int i=0; i < BLOCK_CHAINED_COUNT; ++i){
		for(int j=0; j < BLOCK_CHAINED_COUNT; ++j){
			dirtLayer[i][j]-=stoneLayer[i][j];
		}
	}

	//создадим слой воды и песка
	for(int i=0; i < BLOCK_CHAINED_COUNT; ++i){
		for(int j=0; j < BLOCK_CHAINED_COUNT; ++j){
			if(dirtLayer[i][j] + stoneLayer[i][j] >= WATER_LEVEL){
				waterLayer[i][j]=0;
				sandLayer[i][j]=0;
			}
			else{//air->water
				//какой высоты столб воды
				int height = WATER_LEVEL - (dirtLayer[i][j] + stoneLayer[i][j])-SAND_STEAK;
				//write the height
				if(height >= 0){
					waterLayer[i][j]=height;
					sandLayer[i][j]=SAND_STEAK;
				}
				else{
					waterLayer[i][j]=0;
					sandLayer[i][j]=height+SAND_STEAK;
				}
			}
		}
	}

	//создадим слой снега
	for(int i=0; i<BLOCK_CHAINED_COUNT; ++i){
		for(int j=0; j<BLOCK_CHAINED_COUNT; ++j){
			if(dirtLayer[i][j] + stoneLayer[i][j] < SNOW_LEVEL)
				snowLayer[i][j]=0;
			else{
				int height=dirtLayer[i][j] + stoneLayer[i][j]-(SNOW_LEVEL-1);
				snowLayer[i][j]=height;
				dirtLayer[i][j]-=height;
				if(dirtLayer[i][j] < 0)
					dirtLayer[i][j] = 0;
			}
		}
	}

	//добавим стволы деревьев
	for(int i=0 ; i<BLOCK_CHAINED_COUNT; ++i)
		for(int j=0; j<BLOCK_CHAINED_COUNT; ++j)
			woodLayer[i][j]=0;//обнулим
	const int treeStep=TREE_MAX_WIDTH+2*TREE_OFFSET;//стандартный шаг дерева
	for(int i=treeStep; i < BLOCK_CHAINED_COUNT-treeStep; i+=treeStep){
		for(int j=treeStep; j < BLOCK_CHAINED_COUNT-treeStep; j+=treeStep){
			//можно ли ставить дерево?
			const int currHeight=dirtLayer[i][j] + stoneLayer[i][j] + waterLayer[i][j] + sandLayer[i][j] + snowLayer[i][j];
			if(dirtLayer[i][j] == 0 ||
				currHeight < TREE_MIN_HEIGHT ||
				currHeight > TREE_MAX_HEIGHT)
				continue;
			//имеем точку куда можно ставить дерево
			//проверим по вероятности
			bool treeCanBePlaced=chance(TREE_FREQUENCY,100,seed*15,i*225*j,j*9120);
			if(!treeCanBePlaced)
				continue;
			//дерево разрешено ставить
			//вычислим смещение
			int offsetI=random(-1,1,seed/2,i*i*i*i,j*j*j*j*j);
			int offsetJ=random(-1,1,seed/3,i*i*i*i,j*j*j*j*j);
			int indXToPlace=i+offsetI;
			int indZToPlace=j+offsetJ;

			//разместим дерево
			//рассчитаем высоту
			const int currTreeHeight=random(TREE_HEIGHT_MIN,TREE_HEIGHT_MAX,seed/2*22,i*i*i,j*j*j*j/33);
			woodLayer[indXToPlace][indZToPlace]=currTreeHeight;
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