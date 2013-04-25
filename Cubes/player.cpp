#pragma once
#include <QDebug>
#include <math.h>
#include "player.h"
#include "gameMain.h"

Player::Player(GameMain* gameMainPtr){
	gameMain=gameMainPtr;
	lastFrameTime=clock();
	maxWalkSpeed=gameMain->settings->getMaxWalkSpeed();
	maxRunSpeed=gameMain->settings->getMaxRunSpeed();
	coorX=gameMain->settings->getSpawnPointX();
	coorY=gameMain->settings->getSpawnPointY();
	coorZ=gameMain->settings->getSpawnPointZ();
	horRot=0;
	vertRot=0;
}

void Player::updatePlayerCoord(){
	//���������� ����
	//���� ���� �� ���������, �� �� ������������ �
	if(!gameMain->state->isMouseReleased()){
		horRot+=(double)gameMain->kMController->deltaX * gameMain->settings->getSensitivity();
		vertRot+=(double)gameMain->kMController->deltaY * gameMain->settings->getSensitivity();
		//������������ ���� �� � �������� -90 - +90 �������� �� ������� ���� ��������
		if(vertRot>=90)
			vertRot=89,999999999;
		if(vertRot<=-90)
			vertRot=-89,999999999;

		gameMain->kMController->deltaX=0;
		gameMain->kMController->deltaY=0;
	}

	//������, ������� ������ ������ � ���������� �����
	//������������ ������� �������
	double secFromLastFrame=(double)(clock() - lastFrameTime) / CLOCKS_PER_SEC;
	lastFrameTime=clock();

	if(gameMain->state->isFlyingModOn()){
		//���������� ������� � ������ ������

		//�� ������� ������ �������������?
		double movingWalkDist;
		if(gameMain->kMController->leftShiftPressed)
			movingWalkDist=maxRunSpeed*secFromLastFrame+0.00001;//������� �����, ���� ���� �� ��������
		else
			movingWalkDist=maxWalkSpeed*secFromLastFrame+0.00001;//������� �����, ���� ���� �� ��������

		Vector3D lookingDir=getVectorOfPlayerView();
		lookingDir.setNewLength(movingWalkDist);
		if(gameMain->kMController->wPressed){
			coorX+=lookingDir.getX();
			coorY+=lookingDir.getY();
			coorZ+=lookingDir.getZ();
		}
		if(gameMain->kMController->sPressed){
			coorX-=lookingDir.getX();
			coorY-=lookingDir.getY();
			coorZ-=lookingDir.getZ();
		}
		Vector3D dirRight=lookingDir*Vector3D(0,100,0);
		dirRight.setNewLength(movingWalkDist);
		if(gameMain->kMController->aPressed){
			coorX-=dirRight.getX();
			coorY-=dirRight.getY();
			coorZ-=dirRight.getZ();
		}
		if(gameMain->kMController->dPressed){
			coorX+=dirRight.getX();
			coorY+=dirRight.getY();
			coorZ+=dirRight.getZ();
		}
		if(gameMain->kMController->spacePressed){
			coorY+=movingWalkDist;
		}
		if(gameMain->kMController->leftCtrlPressed){
			coorY-=movingWalkDist;
		}
	}
	else{
		//���������� ������� � ������ � ��������� �� ������

		//�� ������� ������ �������������?
		double movingWalkDist;
		if(gameMain->kMController->leftShiftPressed)
			movingWalkDist=maxRunSpeed*secFromLastFrame+0.00001;//������� �����, ����� ���� �� ��������
		else
			movingWalkDist=maxWalkSpeed*secFromLastFrame+0.00001;//������� �����, ����� ���� �� ��������

		Vector3D lookingDir=getVectorOfPlayerView();
		lookingDir.setNewLength(movingWalkDist);
		if(gameMain->kMController->wPressed){
			coorX+=lookingDir.getX();
			coorY+=lookingDir.getY();
			coorZ+=lookingDir.getZ();
		}
		if(gameMain->kMController->sPressed){
			coorX-=lookingDir.getX();
			coorY-=lookingDir.getY();
			coorZ-=lookingDir.getZ();
		}
		Vector3D dirRight=lookingDir*Vector3D(0,100,0);
		dirRight.setNewLength(movingWalkDist);
		if(gameMain->kMController->aPressed){
			coorX-=dirRight.getX();
			coorY-=dirRight.getY();
			coorZ-=dirRight.getZ();
		}
		if(gameMain->kMController->dPressed){
			coorX+=dirRight.getX();
			coorY+=dirRight.getY();
			coorZ+=dirRight.getZ();
		}
		if(gameMain->kMController->spacePressed){
			coorY+=movingWalkDist;
		}
		if(gameMain->kMController->leftCtrlPressed){
			coorY-=movingWalkDist;
		}

		//�������� ��������� ������, ���� �����
		double autoChangingHeightDist=SPEED_OF_AUTO_CHANGING_HEIGHT*secFromLastFrame+0.00001;//������� �����, ����� ���� �� ��������

		//������� ������ ����������� � ������ ��������
		int currSurfaceHeight=0;
		Chunk* chunkPtr=gameMain->world->getChunkPointer((CHUNKS_COUNT-1)/2, (CHUNKS_COUNT-1)/2);
		int plX=coorX;
		int plZ=coorZ;
		int modX=plX % (BLOCK_COUNT*CUBE_SIZE);
		int modZ=plZ % (BLOCK_COUNT*CUBE_SIZE);
		int blIndX = modX / CUBE_SIZE;
		int blIndZ = modZ / CUBE_SIZE;
		if(blIndX < 0)
			blIndX = BLOCK_COUNT-1-(-blIndX);
		if(blIndZ < 0)
			blIndZ = BLOCK_COUNT-1-(-blIndZ);
		for(int i=0; i<BLOCK_HEIGHT_COUNT; ++i){
			if((chunkPtr->blocks[blIndX][i][blIndZ]).getType() != AIR)
				currSurfaceHeight = (i+1)*CUBE_SIZE;
		}
		currSurfaceHeight+= PLAYER_HEIGHT;
		//���� ���� ���������
		if(coorY < currSurfaceHeight - ALLOWABLE_INTERVAL)
			coorY+=autoChangingHeightDist;
		//���� ���� ���������
		if(coorY > currSurfaceHeight + ALLOWABLE_INTERVAL)
			coorY-=autoChangingHeightDist;
	}
}

Vector3D Player::getVectorOfPlayerView(){
	//��� ���������

	//������ � ���������� � 0-180
	double angHor=abs(horRot);
	angHor=angHor-((int)(angHor/180))*180;
	//������ �������� �� ��� �, ������� � ����
	double ang1=(180-angHor)/2;
	double prX=sin(ang1*PI/180)*sqrt(2*PARAMETER_LEN*PARAMETER_LEN-2*PARAMETER_LEN*PARAMETER_LEN*cos(angHor*PI/180));
	if(horRot>0){
		//��� ������������� �����
		if((abs(horRot)-((int)(abs(horRot))/360)*360)<180)
			prX=-prX;
	}
	else{
		//��� ������������� �����
		if((abs(horRot)-((int)(abs(horRot))/360)*360)>180)
			prX=-prX;
	}
	//������ �������� �� ��� z, ������� � ����
	double prZ=sqrt(PARAMETER_LEN*PARAMETER_LEN-prX*prX);
	if(horRot>0){
		//��� ������������� �����
		if((abs(horRot)-((int)(abs(horRot))/360)*360)>90 && (abs(horRot)-((int)(abs(horRot))/360)*360)<270)
			prZ=-prZ;
	}
	else{
		//��� ������������� �����
		if((abs(horRot)-((int)(abs(horRot))/360)*360)>90 && (abs(horRot)-((int)(abs(horRot))/360)*360)<270)
			prZ=-prZ;
	}

	//��� ���������

	//������ � ���������� � 0-90
	double angVert=abs(vertRot);
	angVert=angVert-((int)(angVert/90))*90;
	//������ �������� �� ��� y, ������� � ����
	double prY=PARAMETER_LEN*sin(angVert*PI/180);
	if(vertRot<0){
		//��� ������������� �����
			prY=-prY;
	}

	//���������� � ������� ������
	return Vector3D(prX,prY,prZ);
}

double Player::getCoorX(){
	return coorX;
}

double Player::getCoorY(){
	return coorY;
}

double Player::getCoorZ(){
	return coorZ;
}

void Player::setCoorX(double _coorX){
	coorX=_coorX;
}

void Player::setCoorY(double _coorY){
	coorY=_coorY;
}

void Player::setCoorZ(double _coorZ){
	coorZ=_coorZ;
}