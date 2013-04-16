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
				//������ ���������
				newVBOBoxMap[currId]=(VBOBoxMap.find(currId))->second;
				VBOBoxMap.erase(currId);
			}
			else{
				//����� �������� �����
				newVBOBoxMap[currId]= new VBOBox(i,j,gameMain);
				gameMain->world->setVBOForChunkCreated(i,j);
			}
		}
	}
	//������� ������ map
	for(std::map<const int,VBOBox*>::iterator iter=VBOBoxMap.begin(); iter!=VBOBoxMap.end(); ++iter){
		delete iter->second;
	}
	VBOBoxMap.clear();

	//��������� ��� �����
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
		iter->second->draw();
	}
}

void Graphics::loadAllTextures(){
	//��������� ������ �������
	QString textureDirtAdress=GAME_DIRECTORY;
	QString textureGrassTopAdress=GAME_DIRECTORY;
	QString textureGrassSideAdress=GAME_DIRECTORY;
	textureDirtAdress.append(TEXTURE_DIRT_NAME);
	textureGrassTopAdress.append(TEXTURE_GRASS_TOP_NAME);
	textureGrassSideAdress.append(TEXTURE_GRASS_SIDE_NAME);
	//�������� ������� ��� ��������������
	wchar_t textureDirtAdressFinalArray[MAX_STRING_LEN_FOR_ARRAYS]={0};
	wchar_t textureGrassTopAdressFinalArray[MAX_STRING_LEN_FOR_ARRAYS]={0};
	wchar_t textureGrassSideAdressFinalArray[MAX_STRING_LEN_FOR_ARRAYS]={0};
	textureDirtAdress.toWCharArray(textureDirtAdressFinalArray);
	textureGrassTopAdress.toWCharArray(textureGrassTopAdressFinalArray);
	textureGrassSideAdress.toWCharArray(textureGrassSideAdressFinalArray);

	// �������� �������
	AUX_RGBImageRec *textureDirt;
	AUX_RGBImageRec *textureGrassTop;
	AUX_RGBImageRec *textureGrassSide;
	textureDirt = auxDIBImageLoad(textureDirtAdressFinalArray);
	textureGrassTop = auxDIBImageLoad(textureGrassTopAdressFinalArray);
	textureGrassSide = auxDIBImageLoad(textureGrassSideAdressFinalArray);

	// ��������� �������� �������
	glGenTextures(TEXTURES_COUNT, &textures[0]);

	//��������� �������� DIRT
	glBindTexture(GL_TEXTURE_2D, textures[DIRT_TEX_INDEX]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, textureDirt->sizeX, textureDirt->sizeY, 0,
		GL_RGB, GL_UNSIGNED_BYTE, textureDirt->data);

	//��������� �������� GRASS_TOP
	glBindTexture(GL_TEXTURE_2D, textures[GRASS_TOP_TEX_INDEX]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, textureGrassTop->sizeX, textureGrassTop->sizeY, 0,
		GL_RGB, GL_UNSIGNED_BYTE, textureGrassTop->data);

	//��������� �������� GRASS_SIDE
	glBindTexture(GL_TEXTURE_2D, textures[GRASS_SIDE_TEX_INDEX]);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, textureGrassSide->sizeX, textureGrassSide->sizeY, 0,
		GL_RGB, GL_UNSIGNED_BYTE, textureGrassSide->data);

}

VBOBox::VBOBox(int chNumX,int chNumZ,GameMain* _gameMain){
	gameMain=_gameMain;

	Chunk* chunkPtr=gameMain->world->getChunkPointer(chNumX,chNumZ);
	std::vector<int> vertices;
	vertices.reserve(4147200);

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
			
				//top
				if(blY<BLOCK_HEIGHT_COUNT-1)
					if(chunkPtr->blocks[blX][blY+1][blZ].getType()==AIR){
						vertices.push_back(blCoordX);
						vertices.push_back(blCoordY+CUBE_SIZE);
						vertices.push_back(blCoordZ);
			
						vertices.push_back(blCoordX);
						vertices.push_back(blCoordY+CUBE_SIZE);
						vertices.push_back(blCoordZ+CUBE_SIZE);
			
						vertices.push_back(blCoordX+CUBE_SIZE);
						vertices.push_back(blCoordY+CUBE_SIZE);
						vertices.push_back(blCoordZ+CUBE_SIZE);
			
						vertices.push_back(blCoordX+CUBE_SIZE);
						vertices.push_back(blCoordY+CUBE_SIZE);
						vertices.push_back(blCoordZ);
					}
			}
		}
	}

	VBO=new QGLBuffer(QGLBuffer::VertexBuffer);
	VBO->setUsagePattern( QGLBuffer::StaticDraw );
	VBO->create();
	VBO->bind();
	VBO->allocate((void *)&vertices[0],vertices.size()*sizeof(int));
	pointsToDraw=vertices.size();
}

VBOBox::~VBOBox(){
	delete VBO;
};

void VBOBox::draw(){
 	glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
 	VBO->bind();
 	glColor3ub(0,240,0);
 	glVertexPointer ( 3, GL_INT, 0, NULL );
 	glDrawArrays(GL_QUADS, 0, pointsToDraw);

	//glTexCoordPointer ( 2, GL_FLOAT, sizeof(TVertex), &tverts.v[0] ); 
	//glVertexPointer ( 3, GL_FLOAT, sizeof(Vertex), &verts.v[0] ); 
	//glDrawArrays ( GL_TRIANGLES, 0, VertexCount );

	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
 	glDisableClientState(GL_VERTEX_ARRAY);
}