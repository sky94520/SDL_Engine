#添加源文件
aux_source_directory(SDL_Engine SRC_LIST1)
aux_source_directory(SDL_Engine/ui SRC_LIST2)
aux_source_directory(SDL_Engine/extensions SRC_LIST3)
#链接头文件
include_directories(${SDL2_INCLUDE_DIR} 
	${SDL2_IMAGE_INCLUDE_DIR}
	${SDL2_MIXER_INCLUDE_DIR}
	${SDL2_TTF_INCLUDE_DIR}
	${SDL2_GFX_INCLUDE_DIR})
#生成动态库
add_library(SDL_Engine_shared SHARED ${SRC_LIST1} ${SRC_LIST2} ${SRC_LIST3})
#设置目标属性
set_target_properties(SDL_Engine_shared PROPERTIES OUTPUT_NAME "SDL_Engine")
#添加依赖
target_link_libraries(SDL_Engine_shared
	${SDL2_LIBRARY}
	${SDL2_IMAGE_LIBRARY}
	${SDL2_MIXER_LIBRARY}
	${SDL2_TTF_LIBRARY}
	${SDL2_GFX_LIBRARY})
#设置库文件输出路径
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
