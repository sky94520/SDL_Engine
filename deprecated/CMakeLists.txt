cmake_minimum_required(VERSION 3.10)

#链接头文件
include_directories(/usr/include/SDL2)

#添加源文件
aux_source_directory(SDL_Engine SRC_LIST1)
aux_source_directory(SDL_Engine/extensions SRC_LIST2)
aux_source_directory(SDL_Engine/ui SRC_LIST3)

#message(WARNING "SDL_Engine:${PROJECT_SOURCE_DIR}")
#添加编译选项
#set (CMAKE_CXX_FLAGS "-fPIC")
#创建动态库
add_library(SDL_Engine_shared SHARED ${SRC_LIST1} ${SRC_LIST2} ${SRC_LIST3})

#设置目标属性
set_target_properties(SDL_Engine_shared PROPERTIES OUTPUT_NAME "SDL_Engine")

#添加依赖
#链接
target_link_libraries(SDL_Engine_shared 
	SDL2
	SDL2_image
	SDL2_mixer
	SDL2_ttf
	SDL2_gfx
	libz)
#设置库文件输出路径
set (LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
#设置库目录的简化
set (LIBRARY_DIR "/usr/lib/x86_64-linux-gnu")
#SDL2
add_library(SDL2 SHARED IMPORTED)
set_target_properties(SDL2 PROPERTIES IMPORTED_LOCATION ${LIBRARY_DIR}/libSDL2.so)
#SDL2_image
add_library(SDL2_image SHARED IMPORTED)
set_target_properties(SDL2_image PROPERTIES IMPORTED_LOCATION ${LIBRARY_DIR}/libSDL2_image.so)
#SDL2_mixer
add_library(SDL2_mixer SHARED IMPORTED)
set_target_properties(SDL2_mixer PROPERTIES IMPORTED_LOCATION ${LIBRARY_DIR}/libSDL2_mixer.so)
#SDL2_ttf
add_library(SDL2_ttf SHARED IMPORTED)
set_target_properties(SDL2_ttf PROPERTIES IMPORTED_LOCATION ${LIBRARY_DIR}/libSDL2_ttf.so)
#SDL2_gfx
add_library(SDL2_gfx SHARED IMPORTED)
set_target_properties(SDL2_gfx PROPERTIES IMPORTED_LOCATION ${LIBRARY_DIR}/libSDL2_gfx.so)
#libz
add_library(libz SHARED IMPORTED)
set_target_properties(libz PROPERTIES IMPORTED_LOCATION ${LIBRARY_DIR}/libz.so)
