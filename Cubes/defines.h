#pragma once
#include "QString"

//start paths
const QString GAME_DIRECTORY="C:\\Users\\AND\\Desktop\\Sam_Cubes_work_with_files\\debug\\";
const QString TEXTURE_DIRT_NAME="dirt.bmp";
const QString TEXTURE_GRASS_TOP_NAME="grass_top.bmp";
const QString TEXTURE_GRASS_SIDE_NAME="grass_side.bmp";
const QString LOADING_SPLASH_SCREEN_NAME="loading_splash_screen.png";
const int MAX_STRING_LEN_FOR_ARRAYS=200;//def.200. ������ ������ ������� ��� �������� ���� ��������. ����� ��� ���. ��������������
//world numbers
const int CHUNKS_COUNT=31; //def 31 ���-�� ������ �� ����� �������, �� ����� �� CHUNK_COUNT*CHUNK_COUNT
const int BLOCK_COUNT=15;//def 15 ���-�� ������ �� ����� �������, �� ����� �� BLOCK_COUNT*BLOCK_COUNT*BLOCK_HEIGHT_COUNT
const int BLOCK_HEIGHT_COUNT=256;//def 256
const int CUBE_SIZE=256;//def 256
//blocks
const int AIR=1;//def 1
const int DIRT=2;//def 2
//graphics
const int MIN_WIDTH=400;//def 600
const int MIN_HEIGHT=400;//def 600
const int CHUNKS_TO_DRAW=13;//def 13 ���_�� ������ ��� ����������. ������� ��� 2*�+1. �� �� ������ CHUNKS_COUNT!!!
const double PARAMETER_LEN=1.0;//def 100.0 �������� ��� ���������� �����, ���� ������� �����

const int TEXTURES_COUNT=50;//def. 50 ������������ ����� �������
const int DIRT_TEX_INDEX=0;//def. 0 ������ �������� ����� � ������� �������. �� ������ TEXTURES_COUNT � �� ��������� � ������� ���������
const int GRASS_TOP_TEX_INDEX=1;//def.1. ��. ����
const int GRASS_SIDE_TEX_INDEX=2;//def.2. ��. ����
//math
const double PI=3.14159265;//def 3.14159265