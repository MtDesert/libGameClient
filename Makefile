#常量
Client:=true
clientName:=GamesClient
clientDir:=lib$(clientName)
clientSrcDir:=. Zipper Zipper/FileStructs Image Image/FileStructs Image/ColorSpaces Renderer Font Charset Timer Network Widgets Dialogs Scenes Layers

ifeq ($(GAME_NAME),) #无游戏名,编译通用部分
ifeq ($(Executable),) #无执行程序名,编译通用客户端库
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
else #DEST_PLATFORM
sysLibs:=iconv msimg32 gdi32 opengl32
endif #DEST_PLATFORM

else #Executable带有游戏名,编译客户端执行程序

ifeq ($(DEST_PLATFORM),Linux)
allSrcCpp:=../$(clientDir)/executable/GamesGLUT.cpp
allIncDir:=$(foreach name,$(clientSrcDir),../$(clientDir)/$(name))
userLibs:=$(Executable)Client $(clientName) $(Executable)
sysLibs:=gtk-3 gobject-2.0 glut GL
exeName:=GamesGLUT
else ifeq ($(DEST_PLATFORM),$(findstring $(DEST_PLATFORM),MinGW Windows))
#allSrcCpp:=../$(clientDir)/executable/GamesGDI.cpp
allSrcCpp:=../$(clientDir)/executable/GamesGLUT.cpp
allIncDir:=$(foreach name,$(clientSrcDir),../$(clientDir)/$(name)) ../freeglut/include
userLibs:=$(Executable)Client $(clientName) $(Executable) freeglut
sysLibs:=gdi32 iconv msimg32 opengl32
exeName:=GamesGLUT
else #DEST_PLATFORM
$(error 未知目标$(DEST_PLATFORM))
endif #DEST_PLATFORM

endif #Executable
else #有游戏名,编译游戏客户端库
allSrcDir:=$(foreach name,$(CLIENT_DIR),Client/$(name))
allIncDir:=$(foreach name,$(clientSrcDir),../$(clientDir)/$(name))
userLibs:=$(clientName) $(GAME_NAME)
libName:=$(GAME_NAME)Client
endif #GAME_NAME

include ../libGamesEngines/Makefile