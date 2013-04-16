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
				//просто скопируем
				newVBOBoxMap[currId]=(VBOBoxMap.find(currId))->second;
				VBOBoxMap.erase(currId);
			}
			else{
				//иначе создадим новый
				newVBOBoxMap[currId]= new VBOBox(i,j,gameMain,textures);
				gameMain->world->setVBOForChunkCreated(i,j);
			}
		}
	}
	//стираем старый map
	for(std::map<const int,VBOBox*>::iterator iter=VBOBoxMap.begin(); iter!=VBOBoxMap.end(); ++iter){
		delete iter->second;
	}
	VBOBoxMap.clear();

	//заполняем его новым
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
		iter->second->draw();
	}
}

void Graphics::loadAllTextures(){
	//формируем адреса текстур
	QString textureDirtAdress=GAME_DIRECTORY;
	QString textureGrassTopAdress=GAME_DIRECTORY;
	QString textureGrassSideAdress=GAME_DIRECTORY;
	textureDirtAdress.append(TEXTURE_DIRT_NAME);
	textureGrassTopAdress.append(TEXTURE_GRASS_TOP_NAME);
	textureGrassSideAdress.append(TEXTURE_GRASS_SIDE_NAME);
	//создадим массивы для преобразования
	wchar_t textureDirtAdressFinalArray[MAX_STRING_LEN_FOR_ARRAYS]={0};
	wchar_t textureGrassTopAdressFinalArray[MAX_STRING_LEN_FOR_ARRAYS]={0};
	wchar_t textureGrassSideAdressFinalArray[MAX_STRING_LEN_FOR_ARRAYS]={0};
	textureDirtAdress.toWCharArray(textureDirtAdressFinalArray);
	textureGrassTopAdress.toWCharArray(textureGrassTopAdressFinalArray);
	textureGrassSideAdress.toWCharArray(textureGrassSideAdressFinalArray);

	// Загрузка текстур
	AUX_RGBImageRec *textureDirt;
	AUX_RGBImageRec *textureGrassTop;
	AUX_RGBImageRec *textureGrassSide;
	textureDirt = auxDIBImageLoad(textureDirtAdressFinalArray);
	textureGrassTop = auxDIBImageLoad(textureGrassTopAdressFinalArray);
	textureGrassSide = auxDIBImageLoad(textureGrassSideAdressFinalArray);

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
}

VBOBox::VBOBox(int chNumX,int chNumZ,GameMain* _gameMain,GLuint* _texturesArrayPtr){
	gameMain=_gameMain;

	texturesPtr=_texturesArrayPtr;
	Chunk* chunkPtr=gameMain->world->getChunkPointer(chNumX,chNumZ);

	pointsOfDirtToDraw=0;
	pointsOfGrassTopToDraw=0;
	pointsOfGrassSideToDraw=0;

	offsetOfDirt=0;
	offsetOfGrassTop=0;
	offsetOfGrassSide=0;

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
				BlockTransparencyAround blocksTransAround=chunkPtr->getBlockTransparencyAround(blX,blY,blZ);

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
				}
			}
		}
	}

	//объединим массивы вершин и текстурных координат
	std::vector<GLint> verticesFinal;
	std::vector<GLfloat> texturesFinal;
	verticesDirt.reserve(10000000);
	texturesDirt.reserve(10000000);
	//заполним verticesFinal
	for(std::vector<GLint>::iterator iter=verticesDirt.begin();iter != verticesDirt.end();++iter)
		verticesFinal.push_back(*iter);
	for(std::vector<GLint>::iterator iter=verticesGrassTop.begin();iter != verticesGrassTop.end();++iter)
		verticesFinal.push_back(*iter);
	for(std::vector<GLint>::iterator iter=verticesGrassSide.begin();iter != verticesGrassSide.end();++iter)
		verticesFinal.push_back(*iter);
	//заполним texturesFinal
	for(std::vector<GLfloat>::iterator iter=texturesDirt.begin();iter != texturesDirt.end();++iter)
		texturesFinal.push_back(*iter);
	for(std::vector<GLfloat>::iterator iter=texturesGrassTop.begin();iter != texturesGrassTop.end();++iter)
		texturesFinal.push_back(*iter);
	for(std::vector<GLfloat>::iterator iter=texturesGrassSide.begin();iter != texturesGrassSide.end();++iter)
		texturesFinal.push_back(*iter);

	//сколько каких видов точек надо рисовать
	pointsOfDirtToDraw=verticesDirt.size()/3;
	pointsOfGrassTopToDraw=verticesGrassTop.size()/3;
	pointsOfGrassSideToDraw=verticesGrassSide.size()/3;

	//зададим смещение
	offsetOfDirt=0;
	offsetOfGrassTop=offsetOfDirt+pointsOfDirtToDraw;
	offsetOfGrassSide=offsetOfGrassTop+pointsOfGrassTopToDraw;

	VBO=new QGLBuffer(QGLBuffer::VertexBuffer);
	VBO->setUsagePattern( QGLBuffer::StaticDraw );
	VBO->create();
	VBO->bind();
	VBO->allocate((void *)&verticesFinal[0],verticesFinal.size()*sizeof(GLint));

	TBO=new QGLBuffer(QGLBuffer::VertexBuffer);
	TBO->setUsagePattern( QGLBuffer::StaticDraw );
	TBO->create();
	TBO->bind();
	TBO->allocate((void *)&texturesFinal[0],texturesFinal.size()*sizeof(GLint));
}

VBOBox::~VBOBox(){
	delete VBO;
	delete TBO;
};

void VBOBox::draw(){
 	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
 	//VBO->bind();
 	//glColor3ub(0,240,0);
 	//glVertexPointer ( 3, GL_INT, 0, NULL );
 	//glDrawArrays(GL_QUADS, 0, pointsToDraw);

	VBO->bind();
	glVertexPointer ( 3, GL_INT, 0, NULL );
	TBO->bind();
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

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}