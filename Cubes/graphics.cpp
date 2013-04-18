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

Graphics::Graphics(GameMain* gameMainPtr,QWidget *parent/*= 0*/):QGLWidget(parent)
{
	gameMain=gameMainPtr;
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
	qglClearColor(Qt::cyan);
	glEnable(GL_TEXTURE_2D);//включаем текстуры
	glEnable(GL_DEPTH_TEST);//тест глубины
	glEnable(GL_NORMALIZE);//нормировка
	glShadeModel(GL_SMOOTH);//интерполяция цветов
	glFrontFace(GL_CCW);//лицевые грани по обходу против часовой стрелки
	//glEnable(GL_LIGHTING);//включиить свет
	//glLightModelf(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);//модель освещения
	//glEnable(GL_LIGHT0);//источник 0
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
	for(int i=(CHUNKS_COUNT-1)/2-(CHUNKS_TO_DRAW-1)/2; i<(CHUNKS_COUNT-1)/2-(CHUNKS_TO_DRAW-1)/2+CHUNKS_TO_DRAW; ++i){
		for(int j=(CHUNKS_COUNT-1)/2-(CHUNKS_TO_DRAW-1)/2; j<(CHUNKS_COUNT-1)/2-(CHUNKS_TO_DRAW-1)/2+CHUNKS_TO_DRAW; ++j){
			int currId=gameMain->world->getChunkId(i,j);
			if(newVBOBoxMap.count(currId) == 0){
				newVBOBoxMap[currId]= new VBOBox(i,j,gameMain,textures);
				gameMain->world->setVBOForChunkCreated(i,j);
			}
		}
	}
	//заполняем старую карту новой
	VBOBoxMap=newVBOBoxMap;

	//ТЕПЕРЬ РИСУЕМ МИР
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Vector3D lookingPoint=gameMain->player->getVectorOfPlayerView();//вектор, куда игрок смотрит
	gluLookAt(gameMain->player->getCoorX(),gameMain->player->getCoorY(),gameMain->player->getCoorZ(),
		gameMain->player->getCoorX()+lookingPoint.getX(),gameMain->player->getCoorY()+lookingPoint.getY(),gameMain->player->getCoorZ()+lookingPoint.getZ(),
		0,1,0);

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
}

void Graphics::loadAllTextures(){
	//формируем адреса текстур
	QString textureDirtAdress=GAME_DIRECTORY;
	QString textureGrassTopAdress=GAME_DIRECTORY;
	QString textureGrassSideAdress=GAME_DIRECTORY;
	QString textureStoneAdress=GAME_DIRECTORY;
	QString textureSandAdress=GAME_DIRECTORY;
	textureDirtAdress.append(TEXTURE_DIRT_NAME);
	textureGrassTopAdress.append(TEXTURE_GRASS_TOP_NAME);
	textureGrassSideAdress.append(TEXTURE_GRASS_SIDE_NAME);
	textureStoneAdress.append(TEXTURE_STONE_NAME);
	textureSandAdress.append(TEXTURE_SAND_NAME);
	//создадим массивы для преобразования
	wchar_t textureDirtAdressFinalArray[MAX_STRING_LEN_FOR_ARRAYS]={0};
	wchar_t textureGrassTopAdressFinalArray[MAX_STRING_LEN_FOR_ARRAYS]={0};
	wchar_t textureGrassSideAdressFinalArray[MAX_STRING_LEN_FOR_ARRAYS]={0};
	wchar_t textureStoneAdressFinalArray[MAX_STRING_LEN_FOR_ARRAYS]={0};
	wchar_t textureSandAdressFinalArray[MAX_STRING_LEN_FOR_ARRAYS]={0};
	textureDirtAdress.toWCharArray(textureDirtAdressFinalArray);
	textureGrassTopAdress.toWCharArray(textureGrassTopAdressFinalArray);
	textureGrassSideAdress.toWCharArray(textureGrassSideAdressFinalArray);
	textureStoneAdress.toWCharArray(textureStoneAdressFinalArray);
	textureSandAdress.toWCharArray(textureSandAdressFinalArray);


	// Загрузка текстур
	AUX_RGBImageRec *textureDirt;
	AUX_RGBImageRec *textureGrassTop;
	AUX_RGBImageRec *textureGrassSide;
	AUX_RGBImageRec *textureStone;
	AUX_RGBImageRec *textureSand;
	textureDirt = auxDIBImageLoad(textureDirtAdressFinalArray);
	textureGrassTop = auxDIBImageLoad(textureGrassTopAdressFinalArray);
	textureGrassSide = auxDIBImageLoad(textureGrassSideAdressFinalArray);
	textureStone = auxDIBImageLoad(textureStoneAdressFinalArray);
	textureSand = auxDIBImageLoad(textureSandAdressFinalArray);

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
}