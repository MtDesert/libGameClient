#include"GameInputBox.h"
#include"Game.h"
#include"Number.h"
#include"Thread.h"

GameInputBox::GameInputBox():whenInputConfirm(nullptr){
	//点击时候触发输入
	onClicked=[&](){startInput();};
}
GameInputBox_Bool::GameInputBox_Bool():boolValue(false),falseStr("false"),trueStr("true"){setValue(boolValue);}
GameInputBox_String::GameInputBox_String():passwordChar('\0'){}
GameInputBox_Integer::GameInputBox_Integer():mInteger(0),minInteger(0),maxInteger(0x7FFFFFFF){}

void GameInputBox_Bool::setValue(bool b){
	boolValue=b;
	GameInputBox::setString(b ? trueStr : falseStr,true);
}
void GameInputBox_String::setValue(const string &str){
	if(passwordChar){//以密码的方式显示
		char buffer[str.size()+1];
		memset(buffer,passwordChar,str.size());
		buffer[str.size()]='\0';
		GameButton_String::setString(buffer);
	}else{
		GameButton_String::setString(str);
	}
	mString=str;
	if(whenInputConfirm)whenInputConfirm();
}
void GameInputBox_Integer::setValue(int num){
	GameButton_String::setString(Number::toString(num));
	mInteger=num;
	if(whenInputConfirm)whenInputConfirm();
}

//GamesEngines的输入控件
static GameInputBox *gInputBox=nullptr;
static int gInputInt=0;
static const char *gInputStr=nullptr;
//线程实现输入
#ifndef __MINGW32__
#ifdef __ANDROID__
void GameStringInputBox::startInput(){}
#else

#include<gtk-3.0/gtk/gtk.h>
static GtkWidget *window=nullptr,*entry=nullptr,*spinBox=nullptr;//GTK控件:窗体,文本输入,数字输入

//隐藏窗体
static gboolean hideWindow(){
	gtk_widget_hide(window);
	gtk_main_quit();//必须用此函数退出gtk_main(),否则后续代码不会执行
	entry=nullptr;
	spinBox=nullptr;
	return FALSE;//如果不返回TRUE,则GTK会发射destroy信号,然后把控件销毁
}
//窗体事件
gboolean whenWidgetEvent(GtkWidget *widget,GdkEvent *event,gpointer user_data){
	switch(event->type){
		case GDK_DELETE:return hideWindow();break;
		case GDK_KEY_RELEASE:{
			auto eventKey=reinterpret_cast<GdkEventKey*>(event);
			switch(eventKey->keyval){
				case GDK_KEY_Escape:return hideWindow();//ESC退出
				case GDK_KEY_Return:case GDK_KEY_KP_Enter:
					if(entry){
						gInputStr=gtk_entry_get_text(GTK_ENTRY(entry));//确认输入
						return hideWindow();//输入完毕后隐藏控件
					}
					if(spinBox){//获取spinBox的输入信息
						gInputStr=gtk_entry_get_text(GTK_ENTRY(spinBox));
						gInputInt=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinBox));
						return hideWindow();
					}
				break;
				default:;
			}
		}break;
		default:;
	}
	return FALSE;
}
//创建窗体
static void createWindow(){
	if(!window)gtk_init(nullptr,nullptr);//注:不清空window变量,使得gtk_init在整个进程中只执行一次
	window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(window,"delete_event",G_CALLBACK(whenWidgetEvent),window);//关闭窗口触发
	g_signal_connect(window,"key-release-event",G_CALLBACK(whenWidgetEvent),window);//键盘按键松开时触发
}
//等待输入完成
static void waitInputFinish(GameInputBox *pInputBox){
	gInputStr=nullptr;
	gtk_widget_show_all(window);
	gtk_main();//事件循环,等待输入结束,靠gtk_main_quit来结束执行
	//输入结束
	if(gInputStr){
		gInputBox=pInputBox;//updateInput负责更新
	}
}
static Thread inputBoxThread;//输入过程用线程处理,保证在用户输入数据的同时后台游戏也能运行
static void* inputStringThreadFunc(void *box){
	auto pInputBox=reinterpret_cast<GameInputBox_String*>(box);
	createWindow();
	//根据类型创建控件
	entry=gtk_entry_new();
	gtk_container_add(GTK_CONTAINER(window),GTK_WIDGET(entry));
	//设置文字内容
	gtk_entry_set_visibility(GTK_ENTRY(entry),!pInputBox->passwordChar);
	gtk_entry_set_invisible_char(GTK_ENTRY(entry),pInputBox->passwordChar);
	gtk_entry_set_text(GTK_ENTRY(entry),pInputBox->mString.data());
	//显示输入窗口,等待输入
	waitInputFinish(pInputBox);
	return nullptr;
}

static void* inputIntegerThreadFunc(void *box){
	auto pInputBox=reinterpret_cast<GameInputBox_Integer*>(box);
	createWindow();
	//根据类型创建控件
	spinBox=gtk_spin_button_new_with_range(pInputBox->minInteger,pInputBox->maxInteger,1);
	gtk_container_add(GTK_CONTAINER(window),spinBox);
	gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinBox),pInputBox->mInteger);
	//显示输入窗口,等待输入
	waitInputFinish(pInputBox);
	return nullptr;
}

void GameInputBox_Bool::startInput(){setValue(!boolValue);}
void GameInputBox_String::startInput(){inputBoxThread.start(inputStringThreadFunc,this);}
void GameInputBox_Integer::startInput(){inputBoxThread.start(inputIntegerThreadFunc,this);}

#endif //__ANDROID__
#else //__MINGW32__

#include<stdio.h>
#include<windows.h>

HWND window,editBox;
static const char *inputStr=nullptr;
char strInput[BUFSIZ];

LRESULT __stdcall WindowProcedure(HWND window, unsigned int msg, WPARAM wp, LPARAM lp){
	switch(msg){
		case WM_CREATE:
			editBox=CreateWindow("edit","text",WS_BORDER|WS_CHILD|WS_VISIBLE,0,0,400,20,window,0,0,0);
		break;
		case WM_DESTROY:
			PostQuitMessage(0);
		default:return DefWindowProc(window,msg,wp,lp);
	}
	return 0;
}

void inputOK(const char *str){
	printf("input %s\n",str);
	fflush(stdout);
}

static Thread inputBoxThread;
static void* inputBoxThreadFunc(void *box){
	auto inputBox=reinterpret_cast<GameInputBox_String*>(box);
	window=editBox=0;
	//注册WindowClass
	WNDCLASS wc;
	wc.style = CS_DBLCLKS;
	wc.lpfnWndProc = WindowProcedure;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = GetModuleHandle(0);
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.lpszMenuName = 0;
	wc.lpszClassName = "myclass";
	RegisterClass(&wc);
	//window
	window = CreateWindow("myclass","title",WS_POPUPWINDOW|WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,420,60,0,0,GetModuleHandle(0),0);
	if(window){
		ShowWindow(window,SW_SHOWDEFAULT) ;
		MSG msg;
		//事件循环
		while(GetMessage(&msg,0,0,0)){
			if(msg.message==WM_CHAR){
				if(msg.wParam==VK_RETURN){
					GetWindowText(editBox,strInput,BUFSIZ);
					PostQuitMessage(0);
				}
				if(msg.wParam==VK_ESCAPE){
					PostQuitMessage(0);
				}
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}else{
		printf("window error %lu\n",GetLastError());
	}
	UnregisterClass("myclass",wc.hInstance);
	//输入结束
	if(inputStr){
		if(inputBox)inputBox->setString(inputStr);
	}
	return nullptr;
}

void GameInputBox_Bool::startInput(){}
void GameInputBox_String::startInput(){
	inputBoxThread.start(inputBoxThreadFunc,this);
}
void GameInputBox_Integer::startInput(){}

#endif

void GameInputBox::updateInput(){
	if(gInputBox){
		auto inputBoxString=dynamic_cast<GameInputBox_String*>(gInputBox);
		if(inputBoxString){
			inputBoxString->setValue(gInputStr);
			gInputBox=nullptr;
		}
		auto inputBoxInteger=dynamic_cast<GameInputBox_Integer*>(gInputBox);
		if(inputBoxInteger){
			inputBoxInteger->setValue(gInputInt);
			gInputBox=nullptr;
		}
	}
}

bool GameAttr_InputBoxBool::getValue()const{return inputBox.boolValue;}
string GameAttr_InputBoxString::getValue()const{return inputBox.mString;}
int GameAttr_InputBoxInteger::getValue()const{return inputBox.mInteger;}
unsigned GameAttr_InputBoxInteger::getUnsignedValue()const{return inputBox.mInteger;}