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
	setMouseTracking(true);//���������� ����� ����

	//�����������
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
	qglClearColor(Qt::cyan);//������ ��� �������������, �� ����� ������ ���������� ������ ����
	glEnable(GL_TEXTURE_2D);//�������� ��������
	glEnable(GL_DEPTH_TEST);//���� �������
	glEnable(GL_NORMALIZE);//����������
	glShadeModel(GL_SMOOTH);//������������ ������
	glFrontFace(GL_CCW);//������� ����� �� ������ ������ ������� �������
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE,GL_FALSE);//������������� ������ ���������
	//glEnable(GL_COLOR_MATERIAL) ;// �������� ������� ���������
	glEnable(GL_CULL_FACE);//���������� ������ ������� �����������

	loadAllTextures();//�������� ��������
	glEnable(GL_TEXTURE_2D);//������� ��������
	//��������� �������- ����� ��� ���������� ������ �������� ����������
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);

	//������� �����������
	//��� ���������������
	//glEnable(0x809D);// 0x809D==GL_MULTISAMPLE
};

void Graphics::resizeGL(int nWidth, int nHeight){
	glViewport(0, 0, nWidth, nHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(90, this->width()/this->height(),1,5000000);
};

void Graphics::paintGL(){
	//������� VBOBoxMap � ����� newVBOBoxMap, ���� �����, �������� ����� VBOBox, ������ ������ VBOBox
	//�������� ������������ ������ VBOBox � ����� map
	std::map<const int,VBOBox*> newVBOBoxMap;
	for(int i=(CHUNKS_COUNT-1)/2-(CHUNKS_TO_DRAW-1)/2; i<(CHUNKS_COUNT-1)/2-(CHUNKS_TO_DRAW-1)/2+CHUNKS_TO_DRAW; ++i){
		for(int j=(CHUNKS_COUNT-1)/2-(CHUNKS_TO_DRAW-1)/2; j<(CHUNKS_COUNT-1)/2-(CHUNKS_TO_DRAW-1)/2+CHUNKS_TO_DRAW; ++j){
			int currId=gameMain->world->getChunkId(i,j);
			bool needToUpdate=gameMain->world->isChunkUpdated(i,j);
			if(!needToUpdate && VBOBoxMap.count(currId)==1){
				//������ ����������
				newVBOBoxMap[currId]=(VBOBoxMap.find(currId))->second;
				VBOBoxMap.erase(currId);
			}
		}
	}
	//������� ������ map
	for(std::map<const int,VBOBox*>::iterator iter=VBOBoxMap.begin(); iter!=VBOBoxMap.end(); ++iter){
		delete iter->second;
	}
	VBOBoxMap.clear();

	//������� ����� VBOBox
	for(int i=(CHUNKS_COUNT-1)/2-(CHUNKS_TO_DRAW-1)/2, currLoads=0; i<(CHUNKS_COUNT-1)/2-(CHUNKS_TO_DRAW-1)/2+CHUNKS_TO_DRAW; ++i){
		for(int j=(CHUNKS_COUNT-1)/2-(CHUNKS_TO_DRAW-1)/2; j<(CHUNKS_COUNT-1)/2-(CHUNKS_TO_DRAW-1)/2+CHUNKS_TO_DRAW; ++j){
			int currId=gameMain->world->getChunkId(i,j);
			if(newVBOBoxMap.count(currId) == 0){

				newVBOBoxMap[currId]= gameDataPreloader->getNewVBOBoxPtr(i,j,gameMain,textures);
				gameMain->world->setVBOForChunkCreated(i,j);
				++currLoads;
				if(currLoads >= VBOBOX_TO_BUILD_IN_ONE_FRAME)
					goto leaveLoadingCycle;//���������� ��������� ����������� ��������
			}
		}
	}
	leaveLoadingCycle:
	//��������� ������ ����� �����
	VBOBoxMap=newVBOBoxMap;

	//������ ������ ���
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Vector3D lookingPoint=gameMain->player->getVectorOfPlayerView();//������, ���� ����� �������
	gluLookAt(gameMain->player->getCoorX(),gameMain->player->getCoorY(),gameMain->player->getCoorZ(),
		gameMain->player->getCoorX()+lookingPoint.getX(),gameMain->player->getCoorY()+lookingPoint.getY(),gameMain->player->getCoorZ()+lookingPoint.getZ(),
		0,1,0);//��������� ����������� ������

	glEnable(GL_LIGHTING);//������� ����
	glEnable(GL_LIGHT1);//turn on the first light
	glEnable(GL_LIGHT2);//turn on the second light
	glEnable(GL_LIGHT3);//turn on the third light
	glEnable(GL_LIGHT4);//turn on the fourth light
	glEnable(GL_LIGHT5);//turn on the 5th light

	adjustBackLightingAndClearColor(GL_LIGHT1,GL_LIGHT2,GL_LIGHT3,GL_LIGHT4,GL_LIGHT5);//��������� ������� � ���� ����
	if(gameMain->state->isPlayerLightsOn()){
		glEnable(GL_LIGHT6);//turn on the 6th light
		glEnable(GL_LIGHT7);//turn on the 7th light
		adjustPlayerLight(GL_LIGHT6, GL_LIGHT7);//������� ������ � ��������
	}

	for(std::map<const int,VBOBox*>::iterator iter=VBOBoxMap.begin(); iter!=VBOBoxMap.end(); ++iter){
		//���������� � ������� lookingPoint ���������� y � ���� (������� ����� ������), �� ����� ����� ������������ ����������
		Vector3D assumedLookingPoint=lookingPoint;
		assumedLookingPoint.setY(0);
		//���� VBOBox(����) ����� �����, �� ������ ���
		Vector3D playerVec(gameMain->player->getCoorX(),gameMain->player->getCoorY(),gameMain->player->getCoorZ());
		Vector3D leftBackVec=Vector3D(iter->second->getCoorX(),0,iter->second->getCoorZ())-playerVec;
		Vector3D rightBackVec=Vector3D(iter->second->getCoorX()+CUBE_SIZE*BLOCK_COUNT,0,iter->second->getCoorZ())-playerVec;
		Vector3D rightFrontVec=Vector3D(iter->second->getCoorX()+CUBE_SIZE*BLOCK_COUNT,0,iter->second->getCoorZ()+CUBE_SIZE*BLOCK_COUNT)-playerVec;
		Vector3D leftFrontVec=Vector3D(iter->second->getCoorX(),0,iter->second->getCoorZ()+CUBE_SIZE*BLOCK_COUNT)-playerVec;
		//��������� � ������
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
	glDisable(GL_LIGHTING);//������� ����
}

void Graphics::loadAllTextures(){
	//��������� ������ �������
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
	//�������� ������� ��� ��������������
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

	// �������� �������
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

	// ��������� �������� �������
	glGenTextures(TEXTURES_COUNT, &textures[0]);

	//�������� �������� ����� �������� ��������
	//��������� �������� DIRT
	glBindTexture(GL_TEXTURE_2D, textures[DIRT_TEX_INDEX]);
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, textureDirt->sizeX, textureDirt->sizeY, 0,GL_RGB, GL_UNSIGNED_BYTE, textureDirt->data);//������ �������� ��������
	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,textureDirt->sizeX,textureDirt->sizeY,GL_RGB,GL_UNSIGNED_BYTE,textureDirt->data);//�������� �������

	//��������� �������� GRASS_TOP
	glBindTexture(GL_TEXTURE_2D, textures[GRASS_TOP_TEX_INDEX]);
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, textureGrassTop->sizeX, textureGrassTop->sizeY, 0,GL_RGB, GL_UNSIGNED_BYTE, textureGrassTop->data);
	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,textureGrassTop->sizeX,textureGrassTop->sizeY,GL_RGB,GL_UNSIGNED_BYTE,textureGrassTop->data);//�������� �������
	
	//��������� �������� GRASS_SIDE
	glBindTexture(GL_TEXTURE_2D, textures[GRASS_SIDE_TEX_INDEX]);
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, textureGrassSide->sizeX, textureGrassSide->sizeY, 0,GL_RGB, GL_UNSIGNED_BYTE, textureGrassSide->data);
	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,textureGrassSide->sizeX,textureGrassSide->sizeY,GL_RGB,GL_UNSIGNED_BYTE,textureGrassSide->data);//�������� �������

	//��������� �������� STONE
	glBindTexture(GL_TEXTURE_2D, textures[STONE_TEX_INDEX]);
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, textureStone->sizeX, textureStone->sizeY, 0,GL_RGB, GL_UNSIGNED_BYTE, texturStone->data);
	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,textureStone->sizeX,textureStone->sizeY,GL_RGB,GL_UNSIGNED_BYTE,textureStone->data);//�������� �������

	//��������� �������� SAND
	glBindTexture(GL_TEXTURE_2D, textures[SAND_TEX_INDEX]);
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, textureSand->sizeX, textureSand->sizeY, 0,GL_RGB, GL_UNSIGNED_BYTE, textureSand->data);
	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,textureSand->sizeX,textureSand->sizeY,GL_RGB,GL_UNSIGNED_BYTE,textureSand->data);//�������� �������

	//��������� �������� WATER
	glBindTexture(GL_TEXTURE_2D, textures[WATER_TEX_INDEX]);
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, textureWater->sizeX, textureWater->sizeY, 0,GL_RGB, GL_UNSIGNED_BYTE, textureWater->data);
	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,textureWater->sizeX,textureWater->sizeY,GL_RGB,GL_UNSIGNED_BYTE,textureWater->data);//�������� �������

	//��������� �������� LEAFS
	glBindTexture(GL_TEXTURE_2D, textures[LEAFS_TEX_INDEX]);
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, textureLeafs->sizeX, textureLeafs->sizeY, 0,GL_RGB, GL_UNSIGNED_BYTE, textureLeafs->data);
	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,textureLeafs->sizeX,textureLeafs->sizeY,GL_RGB,GL_UNSIGNED_BYTE,textureLeafs->data);//�������� �������

	//��������� �������� WOOD
	glBindTexture(GL_TEXTURE_2D, textures[WOOD_TEX_INDEX]);
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, textureWood->sizeX, textureWood->sizeY, 0,GL_RGB, GL_UNSIGNED_BYTE, textureWood->data);
	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,textureWood->sizeX,textureWood->sizeY,GL_RGB,GL_UNSIGNED_BYTE,textureWood->data);//�������� �������

	//��������� �������� SNOW
	glBindTexture(GL_TEXTURE_2D, textures[SNOW_TEX_INDEX]);
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, textureSnow->sizeX, textureSnow->sizeY, 0,GL_RGB, GL_UNSIGNED_BYTE, textureSnow->data);
	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,textureSnow->sizeX,textureSnow->sizeY,GL_RGB,GL_UNSIGNED_BYTE,textureSnow->data);//�������� �������
}

void Graphics::adjustBackLightingAndClearColor(int GL_LIGHT_ONE,int GL_LIGHT_TWO,int GL_LIGHT_THREE,int GL_LIGHT_FOUR,int GL_LIGHT_FIVE){
	int currentTime=gameMain->dayNightCycleManager->getCurrentTime();
	//����� ��������� ��������, � ����� ������
	//c 0 �� DAWN_TIME ���� ������� � ������� ������, ������� ��� ����� ���������� �����.
	//� DAY_NIGHT_CYCLE_TIME ���� ���������� � ���������� �� ����� SUNSET_TIME.

	//����������� ���������
	float light_direction_down[] = {0.0, 1.0, 0.0, 0.0};
	float light_direction_left[] = {-1.0, 0.0, 0.0, 0.0};
	float light_direction_right[] = {1.0, 0.0, 0.0, 0.0};
	float light_direction_front[] = {0.0, 0.0, 1.0, 0.0};
	float light_direction_back[] = {0.0, 0.0, -1.0, 0.0};


	//�������� ����� ������ ����� � �������� ��������� �������� ��������
	if(currentTime >= 0 && currentTime < DAWN_TIME){
		//������ �������
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

		//����
		glClearColor(shadeRed*0,shade,shade,1.0);

		return;
	}

	if(currentTime >= DAWN_TIME && currentTime < DAY_NIGHT_CYCLE_TIME/2){
		//������ ����
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

		//����
		glClearColor(1.0*0,1.0,1.0,1.0);

		return;
	}

	if(currentTime >= DAY_NIGHT_CYCLE_TIME/2 && currentTime < DAY_NIGHT_CYCLE_TIME/2+SUNSET_TIME){
		//������ �����
		//compute shade
		float shadeBlue=((float)currentTime-DAY_NIGHT_CYCLE_TIME/2)/(float)SUNSET_TIME;
		shadeBlue=1.0-shadeBlue;//�� � 0 �� 1, � � 1 �� 0
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

		//����
		glClearColor(shade*0,shade,shadeBlue,1.0);

		return;
	}

	if(currentTime >= DAY_NIGHT_CYCLE_TIME/2+SUNSET_TIME && currentTime < DAY_NIGHT_CYCLE_TIME){
		//������ ����
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

		//����
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

	Vector3D lookingDirection=gameMain->player->getVectorOfPlayerView();//������, ���� ����� �������
	int playerPosX=gameMain->player->getCoorX();
	int playerPosY=gameMain->player->getCoorY();
	int playerPosZ=gameMain->player->getCoorZ();

	//���������
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
	//�������� ������������� � �����������
	glLightf(GL_LIGHT_NUMBER_PROJECTOR, GL_CONSTANT_ATTENUATION, 0.0);
	glLightf(GL_LIGHT_NUMBER_PROJECTOR, GL_LINEAR_ATTENUATION, 0.0);
	glLightf(GL_LIGHT_NUMBER_PROJECTOR, GL_QUADRATIC_ATTENUATION, (GLfloat)0.00000005);

	//������� �������� ��������
	float position[4] = {playerPosX, playerPosY, playerPosZ, 1.0};
	GLfloat color[4] = {(GLfloat)1.0, (GLfloat)1.0, (GLfloat)0.7, (GLfloat)1.0};
	glLightfv(GL_LIGHT_NUMBER_USUAL, GL_DIFFUSE, color);
	glLightfv(GL_LIGHT_NUMBER_USUAL, GL_AMBIENT, color);
	glLightfv(GL_LIGHT_NUMBER_USUAL, GL_SPECULAR, color);
	glLightfv(GL_LIGHT_NUMBER_USUAL, GL_POSITION, position);
	//�������� ������������� � �����������
	glLightf(GL_LIGHT_NUMBER_USUAL, GL_CONSTANT_ATTENUATION, 0.0);
	glLightf(GL_LIGHT_NUMBER_USUAL, GL_LINEAR_ATTENUATION, 0.0);
	glLightf(GL_LIGHT_NUMBER_USUAL, GL_QUADRATIC_ATTENUATION, (GLfloat)0.0000008);
}