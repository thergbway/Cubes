#pragma once
#include "QString"

//start paths
const QString GAME_DIRECTORY="C:\\Users\\AND\\Desktop\\Sam_Cubes_work_with_files\\debug\\";
const QString TEXTURE_DIRT_NAME="dirt.bmp";
const QString TEXTURE_GRASS_TOP_NAME="grass_top.bmp";
const QString TEXTURE_GRASS_SIDE_NAME="grass_side.bmp";
const QString TEXTURE_STONE_NAME="stone.bmp";
const QString TEXTURE_SAND_NAME="sand.bmp";
const QString LOADING_SPLASH_SCREEN_NAME="loading_splash_screen.png";
const int MAX_STRING_LEN_FOR_ARRAYS=200;//def.200. ������ ������ ������� ��� �������� ���� ��������. ����� ��� ���. ��������������
//world numbers
const int CHUNKS_COUNT=41; //def 65(31) ���-�� ������ �� ����� �������, �� ����� �� CHUNK_COUNT*CHUNK_COUNT
const int CHUNKS_PRELOAD_COUNT=5;//def 5 ���-�� ��������������� ������ �� ����� �������
const int BLOCK_CHAINED_COUNT=4097;//def 4097 ������� 2*2^X+1 ���������� ����������(��� ����) ������
							//������ �� ������ ������� ������ �����
const int BLOCK_COUNT=17;//def 17(15) ���-�� ������ �� ����� �������, �� ����� �� BLOCK_COUNT*BLOCK_COUNT*BLOCK_HEIGHT_COUNT
const int BLOCK_HEIGHT_COUNT=128;//def 128(256)
const int CUBE_SIZE=256;//def 256
//blocks
const int AIR=1;//def 1
const int DIRT=2;//def 2
const int GRASS=3;//def 3
const int STONE=4;//def 4
const int SAND=5;//def 5
//graphics
const int MIN_WIDTH=400;//def 600
const int MIN_HEIGHT=400;//def 600
const int CHUNKS_TO_DRAW=33;//def 51(13) ���_�� ������ ��� ����������. ������� ��� 2*�+1. �� �� ������ CHUNKS_COUNT!!!
const double PARAMETER_LEN=1.0;//def 100.0 �������� ��� ���������� �����, ���� ������� �����
//graphics-textures
const int TEXTURES_COUNT=50;//def. 50 ������������ ����� �������
const int DIRT_TEX_INDEX=0;//def. 0 ������ �������� ����� � ������� �������. �� ������ TEXTURES_COUNT � �� ��������� � ������� ���������
const int GRASS_TOP_TEX_INDEX=1;//def.1. ��. ����
const int GRASS_SIDE_TEX_INDEX=2;//def.2. ��. ����
const int STONE_TEX_INDEX=3;//def. 3. ��. ����
const int SAND_TEX_INDEX=4;//def. 4. �� ����
//chunks generator
//types of landscape
	//��������� ��������� ����������� �� � ��������������� ������
const int TYPE1=1;//random landscape that is less smoothed
const int TYPE2=2;//simple sinus landscape
const int TYPE3=3;//real landscape
const int CHOSE_LANDSCAPE=TYPE3;//current landscape
const int SEED=27561005;//seed. Defines landscape
//math
const double PI=3.14159265;//def 3.14159265