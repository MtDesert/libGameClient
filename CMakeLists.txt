cmake_minimum_required(VERSION 2.8)
include(../CompileScripts/CMakeLists.txt)

#指定存放源码和头文件的所有路径(游戏工程文件可能会include此文件)
set(GAMECLIENT_DIR
	. #当前目录
	Zipper Zipper/FileStructs #压缩模块,负责解压
	Image Image/FileStructs Image/ColorSpaces #图像模块,主要用于图像解码和处理
	Renderer #渲染模块,主要用于控制显存和绘制
	Font #字体处理模块,文字可以当作一种特殊的图像
	Charset #字符集,处理字符转换过程
	Timer #计时器,主要用于控制物体动作
	Network #网络模块
	Widgets #控件集,类似于各种平台特有的控件
	Dialogs #对话框
	Scenes #场景,提供公共的常见场景(如选择文件场景)
	Layers #图层,提供公共功能的图层(如游戏中的对话)
)

if(${TARGET_SYSTEM_NAME} STREQUAL Windows)
	include_directories(c:/MinGW/include/GL)
endif()

if(GAME_NAME)
	include(../lib${GAME_NAME}/CMakeLists.txt)#读取游戏的CMakeLists.txt
	includeAll(./ ${GAMECLIENT_DIR})
	set(INCLUDE_DIR_ONLY ON)
	include(../libGamesEngines/CMakeLists.txt)
	#编译执行程序或库
	if(COMPILE_GAME_EXE)
		message(编译执行程序)
		project(GamesExe)
		#设置共用库,给不同平台的可执行文件用
		set(LIBS_LIST ${GAME_NAME}Client GamesClient ${GAME_NAME} GamesEngines ${LIBS_LIST} z)#把编译好的引擎添加到库中
		if(${TARGET_SYSTEM_NAME} STREQUAL Android)
			include_directories(${NDK_ROOT}/sources/android/native_app_glue)
			add_library(main SHARED ./executable/GamesNativeActivity.cpp)
			target_link_libraries(main android log EGL GLESv1_CM)
		else()
			#GLUT库
			set(GLUT_NAME glut)
			if(${TARGET_SYSTEM_NAME} STREQUAL Windows)
				set(GLUT_NAME freeglut)
			endif()
			set(LIBS_LIST ${LIBS_LIST} ${GLUT_NAME})
			#linux相关依赖
			if(${TARGET_SYSTEM_NAME} STREQUAL Linux)
				set(LIBS_LIST ${LIBS_LIST} GLU GL gtk-3 gobject-2.0)
			endif()
			#以下为各种可执行文件的编译过程,根据情况选择编译自己需要的可执行文件即可
			add_executable(GamesGLUT ./executable/GamesGLUT.cpp)
			target_link_libraries(GamesGLUT ${LIBS_LIST})
		endif()
	else()
		message(编译客户端游戏库${GAME_NAME})
		project(${GAME_NAME}Client)
		srcIncludeAll(../lib${GAME_NAME}/Client/ ${CLIENT_DIR})
		#库输出
		add_library(${PROJECT_NAME} SHARED ${SRC_LIST})
		target_link_libraries(${PROJECT_NAME} GamesClient ${GAME_NAME} GamesEngines ${LIBS_LIST})
	endif()
else()
	message(编译客户端库)
	project(GamesClient)
	#添加所有paths下的源码和头文件
	srcIncludeAll(./ ${GAMECLIENT_DIR})
	#包含依赖库的目录
	set(INCLUDE_DIR_ONLY ON)
	include(../libGamesEngines/CMakeLists.txt)
	if(${TARGET_SYSTEM_NAME} STREQUAL Linux)
		include_directories(
			/usr/include/gtk-3.0
			/usr/include/atk-1.0
			/usr/include/glib-2.0
			/usr/include/pango-1.0
			/usr/include/cairo
			/usr/include/gdk-pixbuf-2.0/
			/usr/lib/x86_64-linux-gnu/glib-2.0/include)
	endif()
	#库输出
	add_library(${PROJECT_NAME} SHARED ${SRC_LIST})
	target_link_libraries(${PROJECT_NAME} GamesEngines ${LIBS_LIST})
	#执行程序输出
	add_executable(tools/PngJson ./executable/PngJson.cpp)
	target_link_libraries(tools/PngJson GamesClient GamesEngines GLU GL gtk-3)
endif()