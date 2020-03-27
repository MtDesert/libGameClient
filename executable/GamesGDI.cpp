#include"Game.h"
#include<stdio.h>
#include"windows.h"
#include"winuser.h"
#include"wingdi.h"

Game *game=nullptr;//游戏本体
PAINTSTRUCT paintStruct;//绘制专用结构体
int timerID;//计时器ID

//事件回调函数
LRESULT __stdcall windowProcedure(HWND window,unsigned int msg,WPARAM wp,LPARAM lp){
	switch(msg){
		case WM_CREATE:{
			printf("WM_CREATE\n");
			timerID=SetTimer(window,0,250,NULL);
			//设置设备环境
			HDC hdc=GetDC(window);
			Texture::deviceContext=ShapeRenderer::deviceContext=hdc;
			Texture::compatibleDeviceContex=CreateCompatibleDC(hdc);
			//调整窗口大小
			RECT clientRect;
			GetClientRect(window,&clientRect);
			MoveWindow(window,0,0,
				game->resolution.x*2-(clientRect.right-clientRect.left),
				game->resolution.y*2-(clientRect.bottom-clientRect.top),true);
			//设置绘图中心
			SetViewportOrgEx(hdc,game->resolution.x/2,game->resolution.y/2,NULL);
		}break;
		case WM_DESTROY:
			printf("WM_DESTROY\n");
			KillTimer(window,timerID);
			PostQuitMessage(0);
		break;
		case WM_TIMER:
			printf("WM_TIMER\n");
			SendMessage(window,WM_PAINT,0,0);
		break;
		case WM_PAINT:
			BeginPaint(window,&paintStruct);
			game->render();
			EndPaint(window,&paintStruct);
		break;
		default:;
	}
	//printf("msg:%d\n",msg);
	return DefWindowProc(window,msg,wp,lp);
}

void printMSG(const MSG &msg){
	printf("time=%lu ",msg.time);
	printf("message=%u ",msg.message);
	if(msg.wParam)printf("w=%u ",msg.wParam);
	if(msg.lParam)printf("l=%ld ",msg.lParam);
	printf("\n");
	fflush(stdout);
}

int main(int argc,char* argv[]){
	//设置窗体类的信息
	WNDCLASS wc;
	wc.style = 0;
	wc.lpfnWndProc = windowProcedure;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(0);
	wc.hIcon = LoadIcon(0,IDI_APPLICATION);
	wc.hCursor = LoadCursor(0,IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = "GamesGDI";
	RegisterClass(&wc);
	//创建窗体
	Game::resolution.setXY(640,480);
	HWND window = CreateWindow("GamesGDI","title",WS_SYSMENU,CW_USEDEFAULT,CW_USEDEFAULT,Game::resolution.x,Game::resolution.y,NULL,NULL,wc.hInstance,0);
	if(!window){
		printf("window error %lu\n",GetLastError());
		return -1;
	}
	ShowWindow(window,SW_SHOWDEFAULT);
	game=Game::newGame();
	game->reset();
	//事件循环
	MSG msg;
	while(GetMessage(&msg,NULL,0,0)!=-1){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
		if(msg.message==WM_QUIT)break;//退出
	}
	delete game;
	return 0;
}