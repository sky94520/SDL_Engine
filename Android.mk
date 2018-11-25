LOCAL_PATH := $(call my-dir)
################################
#
#SDL Engine
#need SDL2 SDL2_image SDL2_mixer SDL2_ttf
#
################################
include $(CLEAR_VARS)
LOCAL_MODULE := SDL_Engine
#needed shared libraries
LOCAL_SHARED_LIBRARIES := SDL2 SDL2_image SDL2_mixer SDL2_ttf SDL2_gfx
#needed static libraries Box2D
LOCAL_STATIC_LIBRARIES := box2d_static zlib
#needed SDL
SDL_PATH := ../SDL2
SDL_IMAGE_PATH := ../SDL2_image
SDL_MIXER_PATH := ../SDL2_mixer
SDL_TTF_PATH := ../SDL2_ttf
SDL_GFX_PATH := ../SDL2_gfx
ZLIB_PATH = ../zlib/include

LOCAL_C_INCLUDES := $(LOCAL_PATH)/$(SDL_PATH)/include \
$(LOCAL_PATH)/$(SDL_IMAGE_PATH) \
$(LOCAL_PATH)/$(SDL_MIXER_PATH) \
$(LOCAL_PATH)/$(SDL_TTF_PATH)  \
$(LOCAL_PATH)/$(SDL_GFX_PATH)  \
$(LOCAL_PATH)/rapidxml        

LOCAL_SRC_FILES := $(subst $(LOCAL_PATH)/,, $(wildcard $(LOCAL_PATH)/SDL_Engine/*.cpp))\
                   $(subst $(LOCAL_PATH)/,,$(wildcard $(LOCAL_PATH)/SDL_Engine/ui/*.cpp))
				
LOCAL_CPPFLAGS += -std=c++11 -frtti
LOCAL_LDLIBS := -ldl -lGLESv1_CM -lGLESv2 -llog -landroid

include $(BUILD_SHARED_LIBRARY)
