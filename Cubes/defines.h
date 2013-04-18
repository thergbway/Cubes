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
const int MAX_STRING_LEN_FOR_ARRAYS=200;//def.200. гЮДЮЕР ПЮГЛЕП ЛЮЯЯХБЮ ДКЪ МЮГБЮМХЪ ОСРХ РЕЙЯРСПШ. мСФМН ДКЪ МЕЙ. ОПЕНАПЮГНБЮМХИ
//world numbers
const int CHUNKS_COUNT=41; //def 65(31) йнк-бн вюмйнб он ндмни ярнпнме, ре бяецн ху CHUNK_COUNT*CHUNK_COUNT
const int CHUNKS_PRELOAD_COUNT=5;//def 5 йнк-бн опедгюцпсфюелшу вюмйнб он ндмни ярнпнме
const int BLOCK_CHAINED_COUNT=4097;//def 4097 тнплскю 2*2^X+1 йнкхвеярбн яжеокеммшу(аег ьбнб) акнйнб
							//пюглеп да анкэье пюглепю ндмнцн вюмйю
const int BLOCK_COUNT=17;//def 17(15) йнк-бн вюмйнб он ндмни ярнпнме, ре бяецн ху BLOCK_COUNT*BLOCK_COUNT*BLOCK_HEIGHT_COUNT
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
const int CHUNKS_TO_DRAW=33;//def 51(13) йнк_бн вюмйнб дкъ опнпхянбйх. явхрюрэ йюй 2*у+1. да ме анкэье CHUNKS_COUNT!!!
const double PARAMETER_LEN=1.0;//def 100.0 оюпюлерп дкъ бшвхякемхъ рнвйх, йсдю ялнрпхр хцпнй
//graphics-textures
const int TEXTURES_COUNT=50;//def. 50 люйяхлюкэмне вхякн рейярсп
const int DIRT_TEX_INDEX=0;//def. 0 хмдейя рейярспш гелкх б люяяхбе рейярсп. да лемэье TEXTURES_COUNT х ме янбоюдюрэ я дпсцхлх хмдейяюлх
const int GRASS_TOP_TEX_INDEX=1;//def.1. ял. бшье
const int GRASS_SIDE_TEX_INDEX=2;//def.2. ял. бшье
const int STONE_TEX_INDEX=3;//def. 3. ял. бшье
const int SAND_TEX_INDEX=4;//def. 4. ял бшье
//chunks generator
//types of landscape
	//йнмярюмрш пюгкхвмшу цемепюрнпнб ял б яннрберярбсчыху тюикюу
const int TYPE1=1;//random landscape that is less smoothed
const int TYPE2=2;//simple sinus landscape
const int TYPE3=3;//real landscape
const int CHOSE_LANDSCAPE=TYPE3;//current landscape
const int SEED=27561005;//seed. Defines landscape
//math
const double PI=3.14159265;//def 3.14159265