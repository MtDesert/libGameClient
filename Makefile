#常量
Client:=true
clientName:=GamesClient
clientDir:=lib$(clientName)
clientSrcDir:=. Renderer Font Charset Timer Sound Network Widgets Dialogs Scenes Layers

ifeq ($(DEST_PLATFORM),$(findstring $(DEST_PLATFORM),MinGW Windows))
clientSrcDir+=WinAPI
sysLibs:=iconv msimg32 gdi32 opengl32 winmm comctl32
endif

ifeq ($(projectPath),lib$(clientName)/executable)#编译执行程序

allSrcCpp:=../$(clientDir)/executable/GamesGLUT.cpp
allIncDir:=$(foreach name,$(clientSrcDir),../$(clientDir)/$(name))
userLibs:=$(GAME_NAME)Client $(clientName) $(GAME_NAME)
exeName:=GamesGLUT

ifeq ($(DEST_PLATFORM),Linux) #不同环境,执行程序的依赖也不同
sysLibs:=gtk-3 gobject-2.0 glut GL
else ifeq ($(DEST_PLATFORM),$(findstring $(DEST_PLATFORM),MinGW Windows))
allIncDir+=../freeglut/include
userLibs+=freeglut
else #DEST_PLATFORM
$(error 未知目标$(DEST_PLATFORM))
endif #DEST_PLATFORM

else ifeq ($(GAME_NAME),) #无游戏名,编译客户端通用库

allSrcDir:=$(clientSrcDir)
libName:=$(clientName)
#Linux的情况
ifeq ($(DEST_PLATFORM),Linux)
allIncDir:=/usr/include/gtk-3.0\
/usr/include/atk-1.0\
/usr/include/glib-2.0\
/usr/include/pango-1.0\
/usr/include/harfbuzz\
/usr/include/cairo\
/usr/include/gdk-pixbuf-2.0\
/usr/lib/x86_64-linux-gnu/glib-2.0/include
endif #DEST_PLATFORM

else #GAME_NAME

#编译特定游戏的客户端库
allSrcDir:=$(foreach name,$(CLIENT_DIR),Client/$(name))
allIncDir:=$(foreach name,$(clientSrcDir),../$(clientDir)/$(name))
userLibs:=$(clientName) $(GAME_NAME)
libName:=$(GAME_NAME)Client

endif #GAME_NAME

include ../libGamesEngines/Makefile