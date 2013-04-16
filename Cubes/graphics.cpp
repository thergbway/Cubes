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

VBOBox::VBOBox(int chNumX,int chNumZ,GameMain* _gameMain,GLuint* _texturesArrayPtr){
	gameMain=_gameMain;

	texturesPtr=_texturesArrayPtr;
	Chunk* chunkPtr=gameMain->world->getChunkPointer(chNumX,chNumZ);

	//�������� ���� ��� ��������� VBOBox
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
	}

	//��������� ������� ������ � ���������� ���������
	std::vector<GLint> verticesFinal;
	std::vector<GLfloat> texturesFinal;
	verticesDirt.reserve(10000000);
	texturesDirt.reserve(10000000);
	//�������� verticesFinal
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
	//�������� texturesFinal
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

	//������� ����� ����� ����� ���� ��������
	pointsOfDirtToDraw=verticesDirt.size()/3;
	pointsOfGrassTopToDraw=verticesGrassTop.size()/3;
	pointsOfGrassSideToDraw=verticesGrassSide.size()/3;
	pointsOfStoneToDraw=verticesStone.size()/3;
	pointsOfSandToDraw=verticesSand.size()/3;

	//������� ��������
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