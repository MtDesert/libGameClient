#常量
Client:=true
clientName:=GamesClient
clientDir:=lib$(clientName)
clientSrcDir:=. Zipper Zipper/FileStructs Image Image/FileStructs Image/ColorSpaces Renderer Font Charset Timer Network Widgets Dialogs Scenes Layers

ifeq ($(GAME_NAME),) #无游戏名,编译通用部分
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
	endif
endif

include ../libGamesEngines/Makefile