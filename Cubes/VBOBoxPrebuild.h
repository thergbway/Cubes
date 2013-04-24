#pragma once
#include "chunk.h"
#include "gameMain.h"
#include "simpleArray.h"
#include "VBOBoxTransferInfo.h"

class GameMain;
class Chunk;
class VBOBoxTransferInfo;

class VBOBoxPrebuild{
	//��������������� ���������� ��� VBOBox
	//����� ���� ����������� ��� ����������� �������� VBOBox

	//members
public:
	VBOBoxTransferInfo* vBOBoxTransferInfo;//��� ������ ���������� ����� � ������� ��������
	//������� ��������� � ���������� �������� (���������)
	SimpleArray<GLint, SIZE_OF_VERTICES_FINAL_ARRAYS> verticesFinal;
	SimpleArray<GLfloat, SIZE_OF_TEXTURES_FINAL_ARRAYS> texturesFinal;
	int coordAppChX;//���������� ���������������� �����
	int coordAppChZ;//���������� ���������������� �����
private:
	static bool alloc_map[];//��� ������������� ��������� ������
	static unsigned char pool[];//��� ������������� ��������� ������

	//functions
public:
	VBOBoxPrebuild(Chunk* chunk, Chunk* chBackPreloaded, Chunk* chFrontPreloaded
		,Chunk* chLeftPreloaded,Chunk* chRightPreloaded);
	~VBOBoxPrebuild();
	void* operator new(size_t) throw(std::bad_alloc);
	void operator delete(void*);
private:
};