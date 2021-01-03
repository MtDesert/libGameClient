clientSrcDir =. Zipper Zipper/FileStructs Image Image/FileStructs Image/ColorSpaces Renderer Font Charset Timer Network Widgets Dialogs Scenes Layers
Client=true
#源目录
allSrcDir := $(if $(GAME_NAME),,$(clientSrcDir))
allSrc := $(if $(Executable),executable/GamesGDI.cpp,)
#包含目录
allIncDir := $(if $(GAME_NAME),\
	$(foreach name,$(clientSrcDir),../libGamesClient/$(name))\
	,)
#依赖库
userLibs := $(if $(Executable),$(Executable)Client GamesClient $(Executable),)
sysLibs = z iconv gdi32
exeName := $(if $(Executable),GamesGDI,)
include ../libGamesEngines/Makefile