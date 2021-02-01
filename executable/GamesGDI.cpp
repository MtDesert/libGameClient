#include"Game.h"
#include<stdio.h>
#include"windows.h"
#include"winuser.h"
#include"wingdi.h"

Game *game=nullptr;//游戏本体
Point2D<int> res,halfRes;//分辨率,半分辨率
HDC hdc=nullptr;//设备环境,直接显示屏幕
RECT clientRect;//客户端绘画区域
PAINTSTRUCT paintStruct;//绘制专用结构体
int timerID;//计时器ID
const int timerInterval=20;

#define CASE(name,keyName) case VK_##name:k=Keyboard::Key_##keyName;break;
void windowsMessage_Key(WPARAM key,bool isPressed){
	Keyboard::KeyboardKey k=Keyboard::Amount_KeyboardKey;
	switch(key){
		CASE(RETURN,Enter)
		CASE(ESCAPE,Esc)
		CASE(UP,Up)
		CASE(DOWN,Down)
		CASE(LEFT,Left)
		CASE(RIGHT,Right)
		default:printf("key %llu????\n",key);
	}
	game->keyboardKey(k,isPressed);
}

//事件回调函数
LRESULT __stdcall windowProcedure(HWND window,unsigned int msg,WPARAM wp,LPARAM lp){
	switch(msg){
		case WM_CREATE:{
			printf("WM_CREATE\n");
			res.setP(Game::resolution);
			halfRes.setXY(res.x/2,res.y/2);
			timerID=SetTimer(window,0,timerInterval,NULL);
			//设置设备环境
			hdc=GetDC(window);
			Texture::deviceContext=ShapeRenderer::deviceContext=CreateCompatibleDC(hdc);
			auto bufferBitmap=CreateCompatibleBitmap(hdc,res.x,res.y);
			SelectObject(Texture::deviceContext,bufferBitmap);
			//设置图像模式为高级,不然矩阵变换无法使用
			SetGraphicsMode(hdc,GM_ADVANCED);
			SetGraphicsMode(Texture::deviceContext,GM_ADVANCED);
			//调整窗口大小
			GetClientRect(window,&clientRect);
			MoveWindow(window,0,0,
				res.x*2-(clientRect.right-clientRect.left),
				res.y*2-(clientRect.bottom-clientRect.top),true);
			//设置绘图中心
			SetViewportOrgEx(Texture::deviceContext,halfRes.x,halfRes.y,NULL);
			clientRect.left=-halfRes.x;
			clientRect.right=halfRes.x;
			clientRect.top=-halfRes.y;
			clientRect.bottom=halfRes.y;
		}break;
		case WM_DESTROY:
			printf("WM_DESTROY\n");
			KillTimer(window,timerID);
			PostQuitMessage(0);
		break;
		case WM_TIMER:{
			game->addTimeSlice(timerInterval);
			//SendMessage(window,WM_PAINT,0,0);
			InvalidateRect(window,NULL,true);
		}break;
		case WM_PAINT:{
			BeginPaint(window,&paintStruct);
			//绘制黑色背景
			FillRect(Texture::deviceContext,&clientRect,(HBRUSH)GetStockObject(BLACK_BRUSH));
			game->render();
			BitBlt(hdc,0,0,res.x,res.x,Texture::deviceContext,-halfRes.x,-halfRes.y,SRCCOPY);
			EndPaint(window,&paintStruct);
		}break;
		//键盘事件
		case WM_KEYDOWN:windowsMessage_Key(wp,true);break;
		case WM_KEYUP:windowsMessage_Key(wp,false);break;
		//鼠标移动
		case WM_MOUSEMOVE:{
			auto xPos = LOWORD(lp);
			auto yPos = HIWORD(lp);
			game->mouseMove(xPos-halfRes.x,halfRes.y-yPos);
		}break;
		//鼠标点击
		case WM_LBUTTONDOWN:game->mouseKey(Game::Mouse_LeftButton,true);break;
		case WM_LBUTTONUP:game->mouseKey(Game::Mouse_LeftButton,false);break;
		case WM_RBUTTONDOWN:game->mouseKey(Game::Mouse_RightButton,true);break;
		case WM_RBUTTONUP:game->mouseKey(Game::Mouse_RightButton,false);break;
		//鼠标滚轮
		case WM_MOUSEWHEEL:{
			short angle=HIWORD(wp);//这里必须用short,WORD其实就是BYTE的两倍
			if(angle)angle/=abs(angle);
			game->mouseWheel(angle);break;
		}default:;
	}
	//printf("msg:%d\n",msg);
	return DefWindowProc(window,msg,wp,lp);
}

void printMSG(const MSG &msg){
	printf("time=%lu ",msg.time);
	printf("message=%u ",msg.message);
	if(msg.wParam)printf("w=%llu ",msg.wParam);
	if(msg.lParam)printf("l=%lld ",msg.lParam);
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
	wc.hbrBackground = 0;
	wc.lpszMenuName = 0;
	wc.lpszClassName = "GamesGDI";
	RegisterClass(&wc);
	//启动游戏
	Game::resolution.setXY(640,480);
	game=Game::newGame();
	game->reset();
	//创建窗口
	HWND window = CreateWindow("GamesGDI",game->gameName().data(),WS_SYSMENU,CW_USEDEFAULT,CW_USEDEFAULT,Game::resolution.x,Game::resolution.y,NULL,NULL,wc.hInstance,0);
	if(!window){
		printf("window error %lu\n",GetLastError());
		return -1;
	}
	ShowWindow(window,SW_SHOWDEFAULT);
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