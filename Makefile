#客户端目录
clientDir := $(word $(words ${MAKEFILE_LIST}),${MAKEFILE_LIST})
clientDir := $(shell dirname $(clientDir))

Client=true

clientSrcDir =. Zipper Zipper/FileStructs Image Image/FileStructs Image/ColorSpaces Renderer Font Charset Timer Network Widgets	Dialogs	Scenes Layers
#源目录
allSrcDir := $(if $(GAME_NAME),,$(clientSrcDir))
allSrc := $(if $(Executable),$(clientDir)/executable/GamesGDI.cpp,)
#包含目录
allIncDir := $(if $(GAME_NAME)$(Executable),$(foreach name,$(clientSrcDir),$(clientDir)/$(name)),)
#依赖库
userLibs := $(if $(Executable),$(Executable)Client GamesClient $(Executable),)
sysLibs = z iconv gdi32
exeName := $(if $(Executable),GamesGDI,)
include ../libGamesEngines/Makefile