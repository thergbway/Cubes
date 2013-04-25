#pragma once
#pragma comment(lib,"glaux.lib")
#include <gl/GLAux.h>
#include <QtOpenGL/QtOpenGL>
#include <gl/GLU.h>
#include <QGLWidget>
#include <QDebug>
#include <QString>
#include <QGLFormat>
#include <stdlib.h>
#include <crtdbg.h>
#include <vector>
#include <map>
#include "graphics.h"
#include "gameMain.h"
#include "chunk.h"
#include "vector3d.h"

Graphics::Graphics(GameMain* gameMainPtr,GameDataPreloader* _gameDataPreloader,QWidget *parent/*= 0*/):QGLWidget(parent)
{
	gameMain=gameMainPtr;
	gameDataPreloader=_gameDataPreloader;
	setMinimumSize(MIN_WIDTH,MIN_HEIGHT);
	setMouseTracking(true);//постоянный опрос мыши

	//сглаживание
	//QGLFormat format;
	////format.setVersion(3,3);
	////format.setProfile( QGLFormat::CoreProfile );
	//format.setDoubleBuffer(true);
	//format.setDepth(false);
	//format.setAlpha(false);
	////format.setSampleBuffers(true);
	//format.setSamples(16);
	//setFormat(format);
};

void Graphics::initializeGL(){
	qglClearColor(Qt::cyan);//только для инициализации, во время работы используем другой цвет
	glEnable(GL_TEXTURE_2D);//включаем текстуры
	glEnable(GL_DEPTH_TEST);//тест глубины
	glEnable(GL_NORMALIZE);//нормировка
	glShadeModel(GL_SMOOTH);//интерполяция цветов
	glFrontFace(GL_CCW);//лицевые грани по обходу против часовой стрелки
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);//односторонняя модель освещения
	//glEnable(GL_COLOR_MATERIAL) ;// добавить цветные материалы
	glEnable(GL_CULL_FACE);//показывать только внешние поверхности

	loadAllTextures();//создадим текстуры
	glEnable(GL_TEXTURE_2D);//включим текстуры
	//установим фильтры- нужны для правильной работы мипмапов текстурных
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);

	//включим сглаживание
	//для мультисэмплинга
	//glEnable(0x809D);// 0x809D==GL_MULTISAMPLE
};

void Graphics::resizeGL(int nWidth, int nHeight){
	glViewport(0, 0, nWidth, nHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, this->width()/this->height(),1,5000000);
};

void Graphics::paintGL(){
	//обновим VBOBoxMap в новую newVBOBoxMap, если нужно, создадим новые VBOBox, удалим старые VBOBox
	//копируем существующие нужные VBOBox в новый map
	std::map<const int,VBOBox*> newVBOBoxMap;
	for(int i=(CHUNKS_COUNT-1)/2-(CHUNKS_TO_DRAW-1)/2; i<(CHUNKS_COUNT-1)/2-(CHUNKS_TO_DRAW-1)/2+CHUNKS_TO_DRAW; ++i){
		for(int j=(CHUNKS_COUNT-1)/2-(CHUNKS_TO_DRAW-1)/2; j<(CHUNKS_COUNT-1)/2-(CHUNKS_TO_DRAW-1)/2+CHUNKS_TO_DRAW; ++j){
			int currId=gameMain->world->getChunkId(i,j);
			bool needToUpdate=gameMain->world->isChunkUpdated(i,j);
			if(!needToUpdate && VBOBoxMap.count(currId)==1){
				//просто переместим
				newVBOBoxMap[currId]=(VBOBoxMap.find(currId))->second;
				VBOBoxMap.erase(currId);
			}
		}
	}
	//стираем старый map
	for(std::map<const int,VBOBox*>::iterator iter=VBOBoxMap.begin(); iter!=VBOBoxMap.end(); ++iter){
		delete iter->second;
	}
	VBOBoxMap.clear();

	//создаем новые VBOBox
	for(int i=(CHUNKS_COUNT-1)/2-(CHUNKS_TO_DRAW-1)/2, currLoads=0; i<(CHUNKS_COUNT-1)/2-(CHUNKS_TO_DRAW-1)/2+CHUNKS_TO_DRAW; ++i){
		for(int j=(CHUNKS_COUNT-1)/2-(CHUNKS_TO_DRAW-1)/2; j<(CHUNKS_COUNT-1)/2-(CHUNKS_TO_DRAW-1)/2+CHUNKS_TO_DRAW; ++j){
			int currId=gameMain->world->getChunkId(i,j);
			if(newVBOBoxMap.count(currId) == 0){

				newVBOBoxMap[currId]= gameDataPreloader->getNewVBOBoxPtr(i,j,gameMain,textures);
				gameMain->world->setVBOForChunkCreated(i,j);
				++currLoads;
				if(currLoads >= VBOBOX_TO_BUILD_IN_ONE_FRAME)
					goto leaveLoadingCycle;//прекратить постройку графических объектов
			}
		}
	}
	leaveLoadingCycle:
	//заполняем старую карту новой
	VBOBoxMap=newVBOBoxMap;

	//ТЕПЕРЬ РИСУЕМ МИР
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Vector3D lookingPoint=gameMain->player->getVectorOfPlayerView();//вектор, куда игрок смотрит
	gluLookAt(gameMain->player->getCoorX(),gameMain->player->getCoorY(),gameMain->player->getCoorZ(),
		gameMain->player->getCoorX()+lookingPoint.getX(),gameMain->player->getCoorY()+lookingPoint.getY(),gameMain->player->getCoorZ()+lookingPoint.getZ(),
		0,1,0);//правильно расположили камеру

	glEnable(GL_LIGHTING);//включим свет
	glEnable(GL_LIGHT1);//turn on the first light
	glEnable(GL_LIGHT2);//turn on the second light
	glEnable(GL_LIGHT3);//turn on the third light
	glEnable(GL_LIGHT4);//turn on the fourth light
	glEnable(GL_LIGHT5);//turn on the 5th light

	adjustBackLightingAndClearColor(GL_LIGHT1,GL_LIGHT2,GL_LIGHT3,GL_LIGHT4,GL_LIGHT5);//освещение фоновое и цвет неба
	if(gameMain->state->isPlayerLightsOn()){
		glEnable(GL_LIGHT6);//turn on the 6th light
		glEnable(GL_LIGHT7);//turn on the 7th light
		adjustPlayerLight(GL_LIGHT6, GL_LIGHT7);//фонарик игрока и лампочка
	}

	for(std::map<const int,VBOBox*>::iterator iter=VBOBoxMap.begin(); iter!=VBOBoxMap.end(); ++iter){
		//приравняем у вектора lookingPoint координату y к нулю (получив новый вектор), тк иначе будут неправильные вычисления
		Vector3D assumedLookingPoint=lookingPoint;
		assumedLookingPoint.setY(0);
		//если VBOBox(чанк) видит игрок, то рисуем его
		Vector3D playerVec(gameMain->player->getCoorX(),gameMain->player->getCoorY(),gameMain->player->getCoorZ());
		Vector3D leftBackVec=Vector3D(iter->second->getCoorX(),0,iter->second->getCoorZ())-playerVec;
		Vector3D rightBackVec=Vector3D(iter->second->getCoorX()+CUBE_SIZE*BLOCK_COUNT,0,iter->second->getCoorZ())-playerVec;
		Vector3D rightFrontVec=Vector3D(iter->second->getCoorX()+CUBE_SIZE*BLOCK_COUNT,0,iter->second->getCoorZ()+CUBE_SIZE*BLOCK_COUNT)-playerVec;
		Vector3D leftFrontVec=Vector3D(iter->second->getCoorX(),0,iter->second->getCoorZ()+CUBE_SIZE*BLOCK_COUNT)-playerVec;
		//проверяем и рисуем
		if(assumedLookingPoint.scaleMult(leftBackVec)>0 ||
			assumedLookingPoint.scaleMult(rightBackVec)>0 ||
			assumedLookingPoint.scaleMult(rightFrontVec)>0 ||
			assumedLookingPoint.scaleMult(leftFrontVec)>0){
				iter->second->draw();
		}
	}
	glDisable(GL_LIGHT7);//turn off the 7th light
	glDisable(GL_LIGHT6);//turn off the 6th light
	glDisable(GL_LIGHT5);//turn off the 5th light
	glDisable(GL_LIGHT4);//turn off the fourth light
	glDisable(GL_LIGHT3);//turn off the third light
	glDisable(GL_LIGHT2);//turn off the second light
	glDisable(GL_LIGHT1);//turn off the first light
	glDisable(GL_LIGHTING);//включим свет
}

void Graphics::loadAllTextures(){
	//формируем адреса текстур
	QString textureDirtAdress=GAME_DIRECTORY;
	QString textureGrassTopAdress=GAME_DIRECTORY;
	QString textureGrassSideAdress=GAME_DIRECTORY;
	QString textureStoneAdress=GAME_DIRECTORY;
	QString textureSandAdress=GAME_DIRECTORY;
	QString textureWaterAdress=GAME_DIRECTORY;
	QString textureLeafsAdress=GAME_DIRECTORY;
	QString textureWoodAdress=GAME_DIRECTORY;
	QString textureSnowAdress=GAME_DIRECTORY;
	textureDirtAdress.append(TEXTURE_DIRT_NAME);
	textureGrassTopAdress.append(TEXTURE_GRASS_TOP_NAME);
	textureGrassSideAdress.append(TEXTURE_GRASS_SIDE_NAME);
	textureStoneAdress.append(TEXTURE_STONE_NAME);
	textureSandAdress.append(TEXTURE_SAND_NAME);
	textureWaterAdress.append(TEXTURE_WATER_NAME);
	textureLeafsAdress.append(TEXTURE_LEAFS_NAME);
	textureWoodAdress.append(TEXTURE_WOOD_NAME);
	textureSnowAdress.append(TEXTURE_SNOW_NAME);
	//создадим массивы для преобразования
	wchar_t textureDirtAdressFinalArray[MAX_STRING_LEN_FOR_ARRAYS]={0};
	wchar_t textureGrassTopAdressFinalArray[MAX_STRING_LEN_FOR_ARRAYS]={0};
	wchar_t textureGrassSideAdressFinalArray[MAX_STRING_LEN_FOR_ARRAYS]={0};
	wchar_t textureStoneAdressFinalArray[MAX_STRING_LEN_FOR_ARRAYS]={0};
	wchar_t textureSandAdressFinalArray[MAX_STRING_LEN_FOR_ARRAYS]={0};
	wchar_t textureWaterAdressFinalArray[MAX_STRING_LEN_FOR_ARRAYS]={0};
	wchar_t textureLeafsAdressFinalArray[MAX_STRING_LEN_FOR_ARRAYS]={0};
	wchar_t textureWoodAdressFinalArray[MAX_STRING_LEN_FOR_ARRAYS]={0};
	wchar_t textureSnowAdressFinalArray[MAX_STRING_LEN_FOR_ARRAYS]={0};
	textureDirtAdress.toWCharArray(textureDirtAdressFinalArray);
	textureGrassTopAdress.toWCharArray(textureGrassTopAdressFinalArray);
	textureGrassSideAdress.toWCharArray(textureGrassSideAdressFinalArray);
	textureStoneAdress.toWCharArray(textureStoneAdressFinalArray);
	textureSandAdress.toWCharArray(textureSandAdressFinalArray);
	textureWaterAdress.toWCharArray(textureWaterAdressFinalArray);
	textureLeafsAdress.toWCharArray(textureLeafsAdressFinalArray);
	textureWoodAdress.toWCharArray(textureWoodAdressFinalArray);
	textureSnowAdress.toWCharArray(textureSnowAdressFinalArray);

	// Загрузка текстур
	AUX_RGBImageRec *textureDirt;
	AUX_RGBImageRec *textureGrassTop;
	AUX_RGBImageRec *textureGrassSide;
	AUX_RGBImageRec *textureStone;
	AUX_RGBImageRec *textureSand;
	AUX_RGBImageRec *textureWater;
	AUX_RGBImageRec *textureLeafs;
	AUX_RGBImageRec *textureWood;
	AUX_RGBImageRec *textureSnow;
	textureDirt = auxDIBImageLoad(textureDirtAdressFinalArray);
	textureGrassTop = auxDIBImageLoad(textureGrassTopAdressFinalArray);
	textureGrassSide = auxDIBImageLoad(textureGrassSideAdressFinalArray);
	textureStone = auxDIBImageLoad(textureStoneAdressFinalArray);
	textureSand = auxDIBImageLoad(textureSandAdressFinalArray);
	textureWater = auxDIBImageLoad(textureWaterAdressFinalArray);
	textureLeafs = auxDIBImageLoad(textureLeafsAdressFinalArray);
	textureWood = auxDIBImageLoad(textureWoodAdressFinalArray);
	textureSnow = auxDIBImageLoad(textureSnowAdressFinalArray);

	// Выделение индексов текстур
	glGenTextures(TEXTURES_COUNT, &textures[0]);

	//загрузим текстуры через создание мипмапов
	//загружаем текстуру DIRT
	glBindTexture(GL_TEXTURE_2D, textures[DIRT_TEX_INDEX]);
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, textureDirt->sizeX, textureDirt->sizeY, 0,GL_RGB, GL_UNSIGNED_BYTE, textureDirt->data);//просто создание текстуры
	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,textureDirt->sizeX,textureDirt->sizeY,GL_RGB,GL_UNSIGNED_BYTE,textureDirt->data);//создание мипмапа

	//загружаем текстуру GRASS_TOP
	glBindTexture(GL_TEXTURE_2D, textures[GRASS_TOP_TEX_INDEX]);
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, textureGrassTop->sizeX, textureGrassTop->sizeY, 0,GL_RGB, GL_UNSIGNED_BYTE, textureGrassTop->data);
	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,textureGrassTop->sizeX,textureGrassTop->sizeY,GL_RGB,GL_UNSIGNED_BYTE,textureGrassTop->data);//создание мипмапа
	
	//загружаем текстуру GRASS_SIDE
	glBindTexture(GL_TEXTURE_2D, textures[GRASS_SIDE_TEX_INDEX]);
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, textureGrassSide->sizeX, textureGrassSide->sizeY, 0,GL_RGB, GL_UNSIGNED_BYTE, textureGrassSide->data);
	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,textureGrassSide->sizeX,textureGrassSide->sizeY,GL_RGB,GL_UNSIGNED_BYTE,textureGrassSide->data);//создание мипмапа

	//загружаем текстуру STONE
	glBindTexture(GL_TEXTURE_2D, textures[STONE_TEX_INDEX]);
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, textureStone->sizeX, textureStone->sizeY, 0,GL_RGB, GL_UNSIGNED_BYTE, texturStone->data);
	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,textureStone->sizeX,textureStone->sizeY,GL_RGB,GL_UNSIGNED_BYTE,textureStone->data);//создание мипмапа

	//загружаем текстуру SAND
	glBindTexture(GL_TEXTURE_2D, textures[SAND_TEX_INDEX]);
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, textureSand->sizeX, textureSand->sizeY, 0,GL_RGB, GL_UNSIGNED_BYTE, textureSand->data);
	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,textureSand->sizeX,textureSand->sizeY,GL_RGB,GL_UNSIGNED_BYTE,textureSand->data);//создание мипмапа

	//загружаем текстуру WATER
	glBindTexture(GL_TEXTURE_2D, textures[WATER_TEX_INDEX]);
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, textureWater->sizeX, textureWater->sizeY, 0,GL_RGB, GL_UNSIGNED_BYTE, textureWater->data);
	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,textureWater->sizeX,textureWater->sizeY,GL_RGB,GL_UNSIGNED_BYTE,textureWater->data);//создание мипмапа

	//загружаем текстуру LEAFS
	glBindTexture(GL_TEXTURE_2D, textures[LEAFS_TEX_INDEX]);
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, textureLeafs->sizeX, textureLeafs->sizeY, 0,GL_RGB, GL_UNSIGNED_BYTE, textureLeafs->data);
	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,textureLeafs->sizeX,textureLeafs->sizeY,GL_RGB,GL_UNSIGNED_BYTE,textureLeafs->data);//создание мипмапа

	//загружаем текстуру WOOD
	glBindTexture(GL_TEXTURE_2D, textures[WOOD_TEX_INDEX]);
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, textureWood->sizeX, textureWood->sizeY, 0,GL_RGB, GL_UNSIGNED_BYTE, textureWood->data);
	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,textureWood->sizeX,textureWood->sizeY,GL_RGB,GL_UNSIGNED_BYTE,textureWood->data);//создание мипмапа

	//загружаем текстуру SNOW
	glBindTexture(GL_TEXTURE_2D, textures[SNOW_TEX_INDEX]);
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, textureSnow->sizeX, textureSnow->sizeY, 0,GL_RGB, GL_UNSIGNED_BYTE, textureSnow->data);
	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,textureSnow->sizeX,textureSnow->sizeY,GL_RGB,GL_UNSIGNED_BYTE,textureSnow->data);//создание мипмапа
}

void Graphics::adjustBackLightingAndClearColor(int GL_LIGHT_ONE,int GL_LIGHT_TWO,int GL_LIGHT_THREE,int GL_LIGHT_FOUR,int GL_LIGHT_FIVE){
	int currentTime=gameMain->dayNightCycleManager->getCurrentTime();
	//утром освещение краснеет, а ночью синеет
	//c 0 до DAWN_TIME идет рассвет с красным светом, который под конец становится белым.
	//с DAY_NIGHT_CYCLE_TIME идет потемнение с посинением за время SUNSET_TIME.

	//направление освещения
	float light_direction_down[] = {0.0, 1.0, 0.0, 0.0};
	float light_direction_left[] = {-1.0, 0.0, 0.0, 0.0};
	float light_direction_right[] = {1.0, 0.0, 0.0, 0.0};
	float light_direction_front[] = {0.0, 0.0, 1.0, 0.0};
	float light_direction_back[] = {0.0, 0.0, -1.0, 0.0};


	//выясняем какое сейчас время и выбираем освещение согласно правилам
	if(currentTime >= 0 && currentTime < DAWN_TIME){
		//сейчас рассвет
		//compute shade
		float shade=(float)currentTime/(float)DAWN_TIME;
		float shadeRed=(float)currentTime/(float)DAWN_TIME*RED_SHADE;
		if(shadeRed > 1.0)
			shadeRed=1.0;
		float light_color[] = {shadeRed,shade,shade};

		glLightfv(GL_LIGHT_ONE, GL_DIFFUSE, light_color);
		glLightfv(GL_LIGHT_ONE, GL_AMBIENT, light_color);
		glLightfv(GL_LIGHT_ONE, GL_SPECULAR, light_color);
		glLightfv(GL_LIGHT_ONE, GL_POSITION, light_direction_down);

		glLightfv(GL_LIGHT_TWO, GL_DIFFUSE, light_color);
		glLightfv(GL_LIGHT_TWO, GL_AMBIENT, light_color);
		glLightfv(GL_LIGHT_TWO, GL_SPECULAR, light_color);
		glLightfv(GL_LIGHT_TWO, GL_POSITION, light_direction_left);

		glLightfv(GL_LIGHT_THREE, GL_DIFFUSE, light_color);
		glLightfv(GL_LIGHT_THREE, GL_AMBIENT, light_color);
		glLightfv(GL_LIGHT_THREE, GL_SPECULAR, light_color);
		glLightfv(GL_LIGHT_THREE, GL_POSITION, light_direction_right);

		glLightfv(GL_LIGHT_FOUR, GL_DIFFUSE, light_color);
		glLightfv(GL_LIGHT_FOUR, GL_AMBIENT, light_color);
		glLightfv(GL_LIGHT_FOUR, GL_SPECULAR, light_color);
		glLightfv(GL_LIGHT_FOUR, GL_POSITION, light_direction_front);

		glLightfv(GL_LIGHT_FIVE, GL_DIFFUSE, light_color);
		glLightfv(GL_LIGHT_FIVE, GL_AMBIENT, light_color);
		glLightfv(GL_LIGHT_FIVE, GL_SPECULAR, light_color);
		glLightfv(GL_LIGHT_FIVE, GL_POSITION, light_direction_back);

		//небо
		glClearColor(shadeRed*0,shade,shade,1.0);

		return;
	}

	if(currentTime >= DAWN_TIME && currentTime < DAY_NIGHT_CYCLE_TIME/2){
		//сейчас день
		float light_color[] = {1.0,1.0,1.0};

		glLightfv(GL_LIGHT_ONE, GL_DIFFUSE, light_color);
		glLightfv(GL_LIGHT_ONE, GL_AMBIENT, light_color);
		glLightfv(GL_LIGHT_ONE, GL_SPECULAR, light_color);
		glLightfv(GL_LIGHT_ONE, GL_POSITION, light_direction_down);

		glLightfv(GL_LIGHT_TWO, GL_DIFFUSE, light_color);
		glLightfv(GL_LIGHT_TWO, GL_AMBIENT, light_color);
		glLightfv(GL_LIGHT_TWO, GL_SPECULAR, light_color);
		glLightfv(GL_LIGHT_TWO, GL_POSITION, light_direction_left);

		glLightfv(GL_LIGHT_THREE, GL_DIFFUSE, light_color);
		glLightfv(GL_LIGHT_THREE, GL_AMBIENT, light_color);
		glLightfv(GL_LIGHT_THREE, GL_SPECULAR, light_color);
		glLightfv(GL_LIGHT_THREE, GL_POSITION, light_direction_right);

		glLightfv(GL_LIGHT_FOUR, GL_DIFFUSE, light_color);
		glLightfv(GL_LIGHT_FOUR, GL_AMBIENT, light_color);
		glLightfv(GL_LIGHT_FOUR, GL_SPECULAR, light_color);
		glLightfv(GL_LIGHT_FOUR, GL_POSITION, light_direction_front);

		glLightfv(GL_LIGHT_FIVE, GL_DIFFUSE, light_color);
		glLightfv(GL_LIGHT_FIVE, GL_AMBIENT, light_color);
		glLightfv(GL_LIGHT_FIVE, GL_SPECULAR, light_color);
		glLightfv(GL_LIGHT_FIVE, GL_POSITION, light_direction_back);

		//небо
		glClearColor(1.0*0,1.0,1.0,1.0);

		return;
	}

	if(currentTime >= DAY_NIGHT_CYCLE_TIME/2 && currentTime < DAY_NIGHT_CYCLE_TIME/2+SUNSET_TIME){
		//сейчас закат
		//compute shade
		float shadeBlue=((float)currentTime-DAY_NIGHT_CYCLE_TIME/2)/(float)SUNSET_TIME;
		shadeBlue=1.0-shadeBlue;//не с 0 до 1, а с 1 до 0
		float shade=((float)currentTime-DAY_NIGHT_CYCLE_TIME/2)/(float)SUNSET_TIME*BLUE_SHADE;
		if(shade > 1.0)
			shade=1.0;
		shade = 1.0 - shade;
		float light_color[] = {shade,shade,shadeBlue};
		
		glLightfv(GL_LIGHT_ONE, GL_DIFFUSE, light_color);
		glLightfv(GL_LIGHT_ONE, GL_AMBIENT, light_color);
		glLightfv(GL_LIGHT_ONE, GL_SPECULAR, light_color);
		glLightfv(GL_LIGHT_ONE, GL_POSITION, light_direction_down);

		glLightfv(GL_LIGHT_TWO, GL_DIFFUSE, light_color);
		glLightfv(GL_LIGHT_TWO, GL_AMBIENT, light_color);
		glLightfv(GL_LIGHT_TWO, GL_SPECULAR, light_color);
		glLightfv(GL_LIGHT_TWO, GL_POSITION, light_direction_left);

		glLightfv(GL_LIGHT_THREE, GL_DIFFUSE, light_color);
		glLightfv(GL_LIGHT_THREE, GL_AMBIENT, light_color);
		glLightfv(GL_LIGHT_THREE, GL_SPECULAR, light_color);
		glLightfv(GL_LIGHT_THREE, GL_POSITION, light_direction_right);

		glLightfv(GL_LIGHT_FOUR, GL_DIFFUSE, light_color);
		glLightfv(GL_LIGHT_FOUR, GL_AMBIENT, light_color);
		glLightfv(GL_LIGHT_FOUR, GL_SPECULAR, light_color);
		glLightfv(GL_LIGHT_FOUR, GL_POSITION, light_direction_front);

		glLightfv(GL_LIGHT_FIVE, GL_DIFFUSE, light_color);
		glLightfv(GL_LIGHT_FIVE, GL_AMBIENT, light_color);
		glLightfv(GL_LIGHT_FIVE, GL_SPECULAR, light_color);
		glLightfv(GL_LIGHT_FIVE, GL_POSITION, light_direction_back);

		//небо
		glClearColor(shade*0,shade,shadeBlue,1.0);

		return;
	}

	if(currentTime >= DAY_NIGHT_CYCLE_TIME/2+SUNSET_TIME && currentTime < DAY_NIGHT_CYCLE_TIME){
		//сейчас ночь
		float light_color[] = {0.0,0.0,0.0};
		
		glLightfv(GL_LIGHT_ONE, GL_DIFFUSE, light_color);
		glLightfv(GL_LIGHT_ONE, GL_AMBIENT, light_color);
		glLightfv(GL_LIGHT_ONE, GL_SPECULAR, light_color);
		glLightfv(GL_LIGHT_ONE, GL_POSITION, light_direction_down);

		glLightfv(GL_LIGHT_TWO, GL_DIFFUSE, light_color);
		glLightfv(GL_LIGHT_TWO, GL_AMBIENT, light_color);
		glLightfv(GL_LIGHT_TWO, GL_SPECULAR, light_color);
		glLightfv(GL_LIGHT_TWO, GL_POSITION, light_direction_left);

		glLightfv(GL_LIGHT_THREE, GL_DIFFUSE, light_color);
		glLightfv(GL_LIGHT_THREE, GL_AMBIENT, light_color);
		glLightfv(GL_LIGHT_THREE, GL_SPECULAR, light_color);
		glLightfv(GL_LIGHT_THREE, GL_POSITION, light_direction_right);

		glLightfv(GL_LIGHT_FOUR, GL_DIFFUSE, light_color);
		glLightfv(GL_LIGHT_FOUR, GL_AMBIENT, light_color);
		glLightfv(GL_LIGHT_FOUR, GL_SPECULAR, light_color);
		glLightfv(GL_LIGHT_FOUR, GL_POSITION, light_direction_front);

		glLightfv(GL_LIGHT_FIVE, GL_DIFFUSE, light_color);
		glLightfv(GL_LIGHT_FIVE, GL_AMBIENT, light_color);
		glLightfv(GL_LIGHT_FIVE, GL_SPECULAR, light_color);
		glLightfv(GL_LIGHT_FIVE, GL_POSITION, light_direction_back);

		//небо
		glClearColor(0.0,0.0,0.0,1.0);

		return;
	}
	else{
		while(true){
			qDebug()<<"Error in checking time in adjustBackLightingAndClearColor() function in graphics.cpp!";
		}
	}
}

void Graphics::adjustPlayerLight(int GL_LIGHT_NUMBER_PROJECTOR, int GL_LIGHT_NUMBER_USUAL){

	Vector3D lookingDirection=gameMain->player->getVectorOfPlayerView();//вектор, куда игрок смотрит
	int playerPosX=gameMain->player->getCoorX();
	int playerPosY=gameMain->player->getCoorY();
	int playerPosZ=gameMain->player->getCoorZ();

	//ПРОЖЕКТОР
	GLfloat light_color[] = {(GLfloat)1.0, (GLfloat)1.0, (GLfloat)0.7};
	float light_position[] = {playerPosX, playerPosY, playerPosZ, 1.0};
	float light_spot_direction[] = {lookingDirection.getX(), lookingDirection.getY(), lookingDirection.getZ()};
	glLightfv(GL_LIGHT_NUMBER_PROJECTOR, GL_DIFFUSE, light_color);
	glLightfv(GL_LIGHT_NUMBER_PROJECTOR, GL_AMBIENT, light_color);
	glLightfv(GL_LIGHT_NUMBER_PROJECTOR, GL_SPECULAR, light_color);
	glLightfv(GL_LIGHT_NUMBER_PROJECTOR, GL_POSITION, light_position);
	glLightf(GL_LIGHT_NUMBER_PROJECTOR, GL_SPOT_CUTOFF, 40);
	glLightfv(GL_LIGHT_NUMBER_PROJECTOR, GL_SPOT_DIRECTION, light_spot_direction);
	glLightf(GL_LIGHT_NUMBER_PROJECTOR, GL_SPOT_EXPONENT, 13.0);
	//убывание интенсивности с расстоянием
	glLightf(GL_LIGHT_NUMBER_PROJECTOR, GL_CONSTANT_ATTENUATION, 0.0);
	glLightf(GL_LIGHT_NUMBER_PROJECTOR, GL_LINEAR_ATTENUATION, 0.0);
	glLightf(GL_LIGHT_NUMBER_PROJECTOR, GL_QUADRATIC_ATTENUATION, (GLfloat)0.00000005);

	//ОБЫЧНЫЙ ТОЧЕЧНЫЙ ИСТОЧНИК
	float position[4] = {playerPosX, playerPosY, playerPosZ, 1.0};
	GLfloat color[4] = {(GLfloat)1.0, (GLfloat)1.0, (GLfloat)0.7, (GLfloat)1.0};
	glLightfv(GL_LIGHT_NUMBER_USUAL, GL_DIFFUSE, color);
	glLightfv(GL_LIGHT_NUMBER_USUAL, GL_AMBIENT, color);
	glLightfv(GL_LIGHT_NUMBER_USUAL, GL_SPECULAR, color);
	glLightfv(GL_LIGHT_NUMBER_USUAL, GL_POSITION, position);
	//убывание интенсивности с расстоянием
	glLightf(GL_LIGHT_NUMBER_USUAL, GL_CONSTANT_ATTENUATION, 0.0);
	glLightf(GL_LIGHT_NUMBER_USUAL, GL_LINEAR_ATTENUATION, 0.0);
	glLightf(GL_LIGHT_NUMBER_USUAL, GL_QUADRATIC_ATTENUATION, (GLfloat)0.0000008);
}