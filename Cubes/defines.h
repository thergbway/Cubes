#pragma once
#include "QString"

//start paths
const QString GAME_DIRECTORY="C:\\Users\\AND\\Desktop\\Sam_Cubes_work_with_files\\debug\\";
const QString TEXTURE_DIRT_NAME="dirt.bmp";
const QString TEXTURE_GRASS_TOP_NAME="grass_top.bmp";
const QString TEXTURE_GRASS_SIDE_NAME="grass_side.bmp";
const QString TEXTURE_STONE_NAME="stone.bmp";
const QString TEXTURE_SAND_NAME="sand.bmp";
const QString TEXTURE_WATER_NAME="water.bmp";
const QString TEXTURE_LEAFS_NAME="leafs.bmp";
const QString TEXTURE_WOOD_NAME="wood.bmp";
const QString TEXTURE_SNOW_NAME="snow.bmp";
const QString LOADING_SPLASH_SCREEN_NAME="loading_splash_screen.png";
const int MAX_STRING_LEN_FOR_ARRAYS=200;//def.200. ������ ������ ������� ��� �������� ���� ��������. ����� ��� ���. ��������������
//world numbers
const int CHUNKS_COUNT=41; //def 41 ���-�� ������ �� ����� �������, �� ����� �� CHUNK_COUNT*CHUNK_COUNT
const int CHUNKS_PRELOAD_COUNT=2;//def 2 ���-�� ��������������� ������ �� ����� �������
const int BLOCK_CHAINED_COUNT=4097;//def 4097 ������� 2*2^X+1 ���������� ����������(��� ����) ������
							//������ �� ������ ������� ������ �����
const int BLOCK_COUNT=17;//def 17 ���-�� ������ �� ����� �������, �� ����� �� BLOCK_COUNT*BLOCK_COUNT*BLOCK_HEIGHT_COUNT
const int BLOCK_HEIGHT_COUNT=128;//def 128
const int CUBE_SIZE=256;//def 256
//blocks
const int AIR=1;//def 1
const int DIRT=2;//def 2
const int GRASS=3;//def 3
const int STONE=4;//def 4
const int SAND=5;//def 5
const int WATER=6;//def 6
const int LEAFS=7;//def 7
const int WOOD=8;//def 8
const int SNOW=9;//def 9
//day night values
const int DAY_NIGHT_CYCLE_TIME=140000;//def 140000. ����� ����� ����-����. � ��.
const int DAWN_TIME=20000;//def 20000. ����� �������� � ��.
const int SUNSET_TIME=30000;//def 30000. ����� ������ � ����������
const double RED_SHADE=1.3;//��������� ������� ������ �������
const double BLUE_SHADE=1.15;//�������� ����� ������ �����
const int STARTING_TIME=130000;//������� ����� �� ������ �������
const float SKY_RED_FACTOR=0.0;//������� ������� ����������� ����
const float SKY_GREEN_FACTOR=1.0;//������� ������� ����������� ����
const float SKY_BLUE_FACTOR=1.0;//����� ������� ����������� ����
//player
const int SPEED_OF_AUTO_CHANGING_HEIGHT=3*CUBE_SIZE;//�������� ��������������� ��������� ������ � ��������������� ������
const int PLAYER_HEIGHT=4*CUBE_SIZE;//������, �� ���. ������ ��������� ����� ��� ������������ � �����. ������
const int ALLOWABLE_INTERVAL=1*CUBE_SIZE;//���������� ���������� ��� PLAYER_HEIGHT
//graphics
const int VBOBOX_TO_BUILD_IN_ONE_FRAME=10;//def 1. VBOBox, ������� ����� �������� �� 1 ����
const int MIN_WIDTH=400;//def 400
const int MIN_HEIGHT=400;//def 400
const int CHUNKS_TO_DRAW=33;//def 33 ���_�� ������ ��� ����������. ������� ��� 2*�+1. �� �� ������ CHUNKS_COUNT!!!
const double PARAMETER_LEN=1.0;//def 1.0 �������� ��� ���������� �����, ���� ������� �����
const int SIZE_OF_VERTICES_FINAL_ARRAYS=100000;//def 100000. ���� ���-�� ����� ������� ����� ������� ����� ��� �������
const int SIZE_OF_TEXTURES_FINAL_ARRAYS=70000;//def 70000. ���� ���-�� ����� ������� ����� ������� ����� ��� ��������
const int SIZE_OF_VERTICES_DIRT_ARRAYS=100000;//def 100000 ���� ���-�� �����-������ ��� ��������� �����
const int SIZE_OF_TEXTURES_DIRT_ARRAYS=70000;//def 70000 ���� ���-�� �����-���������� �����. ��� ��������� �����
const int SIZE_OF_VERTICES_GRASS_TOP_ARRAYS=100000;//def 100000 ���� ���-�� �����-������ ��� ��������� ����� �����
const int SIZE_OF_TEXTURES_GRASS_TOP_ARRAYS=70000;//def 70000 ���� ���-�� �����-���������� �����. ��� ��������� ����� �����
const int SIZE_OF_VERTICES_GRASS_SIDE_ARRAYS=100000;//def 100000 ���� ���-�� �����-������ ��� ��������� ������� �����
const int SIZE_OF_TEXTURES_GRASS_SIDE_ARRAYS=70000;//def 70000 ���� ���-�� �����-���������� �����. ��� ��������� ������� �����
const int SIZE_OF_VERTICES_STONE_ARRAYS=100000;//def 100000 ���� ���-�� �����-������ ��� ��������� �����
const int SIZE_OF_TEXTURES_STONE_ARRAYS=70000;//def 70000 ���� ���-�� �����-���������� �����. ��� ��������� �����
const int SIZE_OF_VERTICES_SAND_ARRAYS=100000;//def 100000 ���� ���-�� �����-������ ��� ��������� �����
const int SIZE_OF_TEXTURES_SAND_ARRAYS=70000;//def 70000 ���� ���-�� �����-���������� �����. ��� ��������� �����
const int SIZE_OF_VERTICES_WATER_ARRAYS=100000;//def 100000 ���� ���-�� �����-������ ��� ��������� �����
const int SIZE_OF_TEXTURES_WATER_ARRAYS=70000;//def 70000 ���� ���-�� �����-���������� �����. ��� ��������� �����
const int SIZE_OF_VERTICES_LEAFS_ARRAYS=100000;//def 100000 ���� ���-�� �����-������ ��� ��������� �������
const int SIZE_OF_TEXTURES_LEAFS_ARRAYS=70000;//def 70000 ���� ���-�� �����-���������� �����. ��� ��������� �������
const int SIZE_OF_VERTICES_WOOD_ARRAYS=100000;//def 100000 ���� ���-�� �����-������ ��� ��������� ������
const int SIZE_OF_TEXTURES_WOOD_ARRAYS=70000;//def 70000 ���� ���-�� �����-���������� �����. ��� ��������� ������
const int SIZE_OF_VERTICES_SNOW_ARRAYS=100000;//def 100000 ���� ���-�� �����-������ ��� ��������� �����
const int SIZE_OF_TEXTURES_SNOW_ARRAYS=70000;//def 70000 ���� ���-�� �����-���������� �����. ��� ��������� �����
//graphics-textures
const int TEXTURES_COUNT=50;//def. 50 ������������ ����� �������
const int DIRT_TEX_INDEX=0;//def. 0 ������ �������� ����� � ������� �������. �� ������ TEXTURES_COUNT � �� ��������� � ������� ���������
const int GRASS_TOP_TEX_INDEX=1;//def.1. ��. ����
const int GRASS_SIDE_TEX_INDEX=2;//def.2. ��. ����
const int STONE_TEX_INDEX=3;//def. 3. ��. ����
const int SAND_TEX_INDEX=4;//def. 4. �� ����
const int WATER_TEX_INDEX=5;//def. 5. �� ����
const int LEAFS_TEX_INDEX=6;//def. 6. �� ����
const int WOOD_TEX_INDEX=7;//def. 7. �� ����
const int SNOW_TEX_INDEX=8;//def. 8. �� ����
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