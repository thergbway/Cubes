#pragma once
#include "QString"

//start paths
const QString GAME_DIRECTORY="C:\\Users\\AND\\Desktop\\Sam_Cubes_work_with_files\\debug\\";
const QString TEXTURE_DIRT_NAME="dirt.bmp";
const QString TEXTURE_GRASS_TOP_NAME="grass_top.bmp";
const QString TEXTURE_GRASS_SIDE_NAME="grass_side.bmp";
const QString LOADING_SPLASH_SCREEN_NAME="loading_splash_screen.png";
const int MAX_STRING_LEN_FOR_ARRAYS=200;//def.200. гЮДЮЕР ПЮГЛЕП ЛЮЯЯХБЮ ДКЪ МЮГБЮМХЪ ОСРХ РЕЙЯРСПШ. мСФМН ДКЪ МЕЙ. ОПЕНАПЮГНБЮМХИ
//world numbers
const int CHUNKS_COUNT=31; //def 31 йнк-бн вюмйнб он ндмни ярнпнме, ре бяецн ху CHUNK_COUNT*CHUNK_COUNT
const int BLOCK_COUNT=15;//def 15 йнк-бн вюмйнб он ндмни ярнпнме, ре бяецн ху BLOCK_COUNT*BLOCK_COUNT*BLOCK_HEIGHT_COUNT
const int BLOCK_HEIGHT_COUNT=256;//def 256
const int CUBE_SIZE=256;//def 256
//blocks
const int AIR=1;//def 1
const int DIRT=2;//def 2
//graphics
const int MIN_WIDTH=400;//def 600
const int MIN_HEIGHT=400;//def 600
const int CHUNKS_TO_DRAW=13;//def 13 йнк_бн вюмйнб дкъ опнпхянбйх. явхрюрэ йюй 2*у+1. да ме анкэье CHUNKS_COUNT!!!
const double PARAMETER_LEN=1.0;//def 100.0 оюпюлерп дкъ бшвхякемхъ рнвйх, йсдю ялнрпхр хцпнй

const int TEXTURES_COUNT=50;//def. 50 люйяхлюкэмне вхякн рейярсп
const int DIRT_TEX_INDEX=0;//def. 0 хмдейя рейярспш гелкх б люяяхбе рейярсп. да лемэье TEXTURES_COUNT х ме янбоюдюрэ я дпсцхлх хмдейяюлх
const int GRASS_TOP_TEX_INDEX=1;//def.1. ял. бшье
const int GRASS_SIDE_TEX_INDEX=2;//def.2. ял. бшье
//math
const double PI=3.14159265;//def 3.14159265