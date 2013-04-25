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
const int MAX_STRING_LEN_FOR_ARRAYS=200;//def.200. Задает размер массива для названия пути текстуры. Нужно для нек. преобразований
//world numbers
const int CHUNKS_COUNT=41; //def 41 КОЛ-ВО ЧАНКОВ ПО ОДНОЙ СТОРОНЕ, ТЕ ВСЕГО ИХ CHUNK_COUNT*CHUNK_COUNT
const int CHUNKS_PRELOAD_COUNT=2;//def 2 КОЛ-ВО ПРЕДЗАГРУЖАЕМЫХ ЧАНКОВ ПО ОДНОЙ СТОРОНЕ
const int BLOCK_CHAINED_COUNT=4097;//def 4097 ФОРМУЛА 2*2^X+1 КОЛИЧЕСТВО СЦЕПЛЕННЫХ(БЕЗ ШВОВ) БЛОКОВ
							//РАЗМЕР ДБ БОЛЬШЕ РАЗМЕРА ОДНОГО ЧАНКА
const int BLOCK_COUNT=17;//def 17 КОЛ-ВО ЧАНКОВ ПО ОДНОЙ СТОРОНЕ, ТЕ ВСЕГО ИХ BLOCK_COUNT*BLOCK_COUNT*BLOCK_HEIGHT_COUNT
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
const int DAY_NIGHT_CYCLE_TIME=140000;//def 140000. Время цикла день-ночь. В мс.
const int DAWN_TIME=20000;//def 20000. Время рассвета в мс.
const int SUNSET_TIME=30000;//def 30000. Время заката с посинением
const double RED_SHADE=1.3;//Насколько красным делать рассвет
const double BLUE_SHADE=1.15;//Насолько синим делать закат
const int STARTING_TIME=130000;//текущее время на момент запуска
const float SKY_RED_FACTOR=0.0;//красный оттенок нормального неба
const float SKY_GREEN_FACTOR=1.0;//зеленый оттенок нормального неба
const float SKY_BLUE_FACTOR=1.0;//синий оттенок нормального неба
//player
const int SPEED_OF_AUTO_CHANGING_HEIGHT=3*CUBE_SIZE;//скорость автоматического изменения высоты в соответствующем режиме
const int PLAYER_HEIGHT=4*CUBE_SIZE;//высота, на кот. должен находится игрок над поверхностью в соотв. режиме
const int ALLOWABLE_INTERVAL=1*CUBE_SIZE;//допустимае отклонение для PLAYER_HEIGHT
//graphics
const int VBOBOX_TO_BUILD_IN_ONE_FRAME=10;//def 1. VBOBox, которое будет строится за 1 кадр
const int MIN_WIDTH=400;//def 400
const int MIN_HEIGHT=400;//def 400
const int CHUNKS_TO_DRAW=33;//def 33 КОЛ_ВО ЧАНКОВ ДЛЯ ПРОРИСОВКИ. СЧИТАТЬ КАК 2*Х+1. ДБ НЕ БОЛЬШЕ CHUNKS_COUNT!!!
const double PARAMETER_LEN=1.0;//def 1.0 ПАРАМЕТР ДЛЯ ВЫЧИСЛЕНИЯ ТОЧКИ, КУДА СМОТРИТ ИГРОК
const int SIZE_OF_VERTICES_FINAL_ARRAYS=100000;//def 100000. Макс кол-во точек которое может хранить класс под вершины
const int SIZE_OF_TEXTURES_FINAL_ARRAYS=70000;//def 70000. Макс кол-во точек которое может хранить класс под текстуры
const int SIZE_OF_VERTICES_DIRT_ARRAYS=100000;//def 100000 Макс кол-во точек-вершин для геометрии земли
const int SIZE_OF_TEXTURES_DIRT_ARRAYS=70000;//def 70000 Макс кол-во точек-текстурных коорд. для геометрии земли
const int SIZE_OF_VERTICES_GRASS_TOP_ARRAYS=100000;//def 100000 Макс кол-во точек-вершин для геометрии верха травы
const int SIZE_OF_TEXTURES_GRASS_TOP_ARRAYS=70000;//def 70000 Макс кол-во точек-текстурных коорд. для геометрии верха травы
const int SIZE_OF_VERTICES_GRASS_SIDE_ARRAYS=100000;//def 100000 Макс кол-во точек-вершин для геометрии стороны травы
const int SIZE_OF_TEXTURES_GRASS_SIDE_ARRAYS=70000;//def 70000 Макс кол-во точек-текстурных коорд. для геометрии стороны травы
const int SIZE_OF_VERTICES_STONE_ARRAYS=100000;//def 100000 Макс кол-во точек-вершин для геометрии камня
const int SIZE_OF_TEXTURES_STONE_ARRAYS=70000;//def 70000 Макс кол-во точек-текстурных коорд. для геометрии камня
const int SIZE_OF_VERTICES_SAND_ARRAYS=100000;//def 100000 Макс кол-во точек-вершин для геометрии песка
const int SIZE_OF_TEXTURES_SAND_ARRAYS=70000;//def 70000 Макс кол-во точек-текстурных коорд. для геометрии песка
const int SIZE_OF_VERTICES_WATER_ARRAYS=100000;//def 100000 Макс кол-во точек-вершин для геометрии песка
const int SIZE_OF_TEXTURES_WATER_ARRAYS=70000;//def 70000 Макс кол-во точек-текстурных коорд. для геометрии песка
const int SIZE_OF_VERTICES_LEAFS_ARRAYS=100000;//def 100000 Макс кол-во точек-вершин для геометрии листьев
const int SIZE_OF_TEXTURES_LEAFS_ARRAYS=70000;//def 70000 Макс кол-во точек-текстурных коорд. для геометрии листьев
const int SIZE_OF_VERTICES_WOOD_ARRAYS=100000;//def 100000 Макс кол-во точек-вершин для геометрии дерева
const int SIZE_OF_TEXTURES_WOOD_ARRAYS=70000;//def 70000 Макс кол-во точек-текстурных коорд. для геометрии дерева
const int SIZE_OF_VERTICES_SNOW_ARRAYS=100000;//def 100000 Макс кол-во точек-вершин для геометрии снега
const int SIZE_OF_TEXTURES_SNOW_ARRAYS=70000;//def 70000 Макс кол-во точек-текстурных коорд. для геометрии снега
//graphics-textures
const int TEXTURES_COUNT=50;//def. 50 МАКСИМАЛЬНОЕ ЧИСЛО ТЕКСТУР
const int DIRT_TEX_INDEX=0;//def. 0 ИНДЕКС ТЕКСТУРЫ ЗЕМЛИ В МАССИВЕ ТЕКСТУР. ДБ МЕНЬШЕ TEXTURES_COUNT И НЕ СОВПАДАТЬ С ДРУГИМИ ИНДЕКСАМИ
const int GRASS_TOP_TEX_INDEX=1;//def.1. СМ. ВЫШЕ
const int GRASS_SIDE_TEX_INDEX=2;//def.2. СМ. ВЫШЕ
const int STONE_TEX_INDEX=3;//def. 3. СМ. ВЫШЕ
const int SAND_TEX_INDEX=4;//def. 4. СМ ВЫШЕ
const int WATER_TEX_INDEX=5;//def. 5. СМ ВЫШЕ
const int LEAFS_TEX_INDEX=6;//def. 6. СМ ВЫШЕ
const int WOOD_TEX_INDEX=7;//def. 7. СМ ВЫШЕ
const int SNOW_TEX_INDEX=8;//def. 8. СМ ВЫШЕ
//chunks generator
//types of landscape
	//КОНСТАНТЫ РАЗЛИЧНЫХ ГЕНЕРАТОРОВ СМ В СООТВЕТСТВУЮЩИХ ФАЙЛАХ
const int TYPE1=1;//random landscape that is less smoothed
const int TYPE2=2;//simple sinus landscape
const int TYPE3=3;//real landscape
const int CHOSE_LANDSCAPE=TYPE3;//current landscape
const int SEED=27561005;//seed. Defines landscape
//math
const double PI=3.14159265;//def 3.14159265