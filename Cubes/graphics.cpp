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
	qglClearColor(Qt::cyan);
	glEnable(GL_TEXTURE_2D);//�������� ��������
	glEnable(GL_DEPTH_TEST);//���� �������
	glEnable(GL_NORMALIZE);//����������
	glShadeModel(GL_SMOOTH);//������������ ������
	glFrontFace(GL_CCW);//������� ����� �� ������ ������ ������� �������
	//glEnable(GL_LIGHTING);//��������� ����
	//glLightModelf(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);//������ ���������
	//glEnable(GL_LIGHT0);//�������� 0
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
	for(int i=(CHUNKS_COUNT-1)/2-(CHUNKS_TO_DRAW-1)/2; i<(CHUNKS_COUNT-1)/2-(CHUNKS_TO_DRAW-1)/2+CHUNKS_TO_DRAW; ++i){
		for(int j=(CHUNKS_COUNT-1)/2-(CHUNKS_TO_DRAW-1)/2; j<(CHUNKS_COUNT-1)/2-(CHUNKS_TO_DRAW-1)/2+CHUNKS_TO_DRAW; ++j){
			int currId=gameMain->world->getChunkId(i,j);
			if(newVBOBoxMap.count(currId) == 0){
				newVBOBoxMap[currId]= new VBOBox(i,j,gameMain,textures);
				gameMain->world->setVBOForChunkCreated(i,j);
			}
		}
	}
	//��������� ������ ����� �����
	VBOBoxMap=newVBOBoxMap;

	//������ ������ ���
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	Vector3D lookingPoint=gameMain->player->getVectorOfPlayerView();//������, ���� ����� �������
	gluLookAt(gameMain->player->getCoorX(),gameMain->player->getCoorY(),gameMain->player->getCoorZ(),
		gameMain->player->getCoorX()+lookingPoint.getX(),gameMain->player->getCoorY()+lookingPoint.getY(),gameMain->player->getCoorZ()+lookingPoint.getZ(),
		0,1,0);

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
}

void Graphics::loadAllTextures(){
	//��������� ������ �������
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
	//�������� ������� ��� ��������������
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


	// �������� �������
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
}