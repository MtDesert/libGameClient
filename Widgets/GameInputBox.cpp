#include"GameInputBox.h"
#include"Game.h"
#include"Number.h"
#include"Thread.h"
#include"PrintF.h"

GameInputBox::GameInputBox():whenInputConfirm(nullptr){
	//点击时候触发输入
	onClicked=[&](){startInput(this);};
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
#ifdef __MINGW32__
		auto old=GameString::charset.srcCharset;//备份
		GameString::charset.srcCharset=Charset::GBK;//改变字符集
		GameButton_String::setString(str);//输入
		GameString::charset.srcCharset=old;//还原字符集
#else
		GameButton_String::setString(str);
#endif
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

//特定平台的输入方案
#ifdef __MINGW32__
#include<stdio.h>
#include<windows.h>

static HWND window=NULL;
static int dwStyle=0;//决定输入内容限制
static char strInputBuffer[BUFSIZ];

static void showInputBox(GameInputBox* pInputBox,const char* str){
	//输入框
	if(!window){//防止频繁创建
		window = CreateWindow("edit",str,WS_POPUPWINDOW|WS_OVERLAPPEDWINDOW|dwStyle,CW_USEDEFAULT,CW_USEDEFAULT,420,64,0,0,GetModuleHandle(0),0);
	}
	if(window)ShowWindow(window,SW_SHOWDEFAULT);
	else PRINT_ERROR("window error %lu\n",GetLastError());
}
void GameInputBox::finishInput(){
	gInputBox=NULL;
	//关闭窗口
	CloseWindow(window);
	window=NULL;
}
#else //Unix类系统
#ifdef __ANDROID__
void GameInputBox_String::startInput(){}
void GameInputBox_Integer::startInput(){}
#else //Linux系统,用gtk作为解决方案

#include<gtk-3.0/gtk/gtk.h>
static GtkWidget *window=nullptr,*entry=nullptr,*spinBox=nullptr;//GTK控件:窗体,文本输入,数字输入
gboolean hideWindow(){
	gtk_widget_hide(window);
	return TRUE;
}
//窗体事件
gboolean whenWidgetEvent(GtkWidget *widget,GdkEvent *event,gpointer user_data){
	switch(event->type){
		case GDK_DELETE:return hideWindow();break;
		case GDK_KEY_RELEASE:{
			auto eventKey=reinterpret_cast<GdkEventKey*>(event);
			switch(eventKey->keyval){
				case GDK_KEY_Escape:return hideWindow();//ESC退出
				case GDK_KEY_Return:case GDK_KEY_KP_Enter:{
					//字符串
					auto strInputBox=dynamic_cast<GameInputBox_String*>(gInputBox);
					if(strInputBox){
						strInputBox->setValue(gtk_entry_get_text(GTK_ENTRY(entry)));
					}
					//数字
					auto intInputBox=dynamic_cast<GameInputBox_Integer*>(gInputBox);
					if(intInputBox){
						auto i=gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spinBox));
						intInputBox->setValue(i);
					}
					return hideWindow();
				}break;
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
#endif//__ANDROID__
#endif//__MINGW__
//特定平台的输入方案

//----------------------------------------------
//开始输入
void GameInputBox::startInput(GameInputBox *pInputBox){
	//字符串
	auto strInputBox=dynamic_cast<GameInputBox_String*>(pInputBox);
	if(strInputBox){
		gInputBox=strInputBox;
#ifdef __MINGW32__
		dwStyle=0;
		showInputBox(strInputBox,strInputBox->mString.data());
#else
		createWindow();
		entry=gtk_entry_new();
		gtk_container_add(GTK_CONTAINER(window),GTK_WIDGET(entry));
		//设置文字内容
		gtk_entry_set_visibility(GTK_ENTRY(entry),!strInputBox->passwordChar);
		gtk_entry_set_invisible_char(GTK_ENTRY(entry),strInputBox->passwordChar);
		gtk_entry_set_text(GTK_ENTRY(entry),strInputBox->mString.data());
		gtk_widget_show_all(window);//显示输入窗口,等待输入
#endif
		return;
	}
	//数字
	auto intInputBox=dynamic_cast<GameInputBox_Integer*>(pInputBox);
	if(intInputBox){
		gInputBox=intInputBox;
#ifdef __MINGW32__
		dwStyle=ES_NUMBER;
		showInputBox(intInputBox,Number::toString(intInputBox->mInteger).data());
#else
		createWindow();
		spinBox=gtk_spin_button_new_with_range(intInputBox->minInteger,intInputBox->maxInteger,1);
		gtk_container_add(GTK_CONTAINER(window),spinBox);
		gtk_spin_button_set_value(GTK_SPIN_BUTTON(spinBox),intInputBox->mInteger);
		gtk_widget_show_all(window);//显示输入窗口,等待输入
#endif
		return;
	}
	//布尔型
	auto boolInputBox=dynamic_cast<GameInputBox_Bool*>(pInputBox);
	if(boolInputBox){
		boolInputBox->setValue(!boolInputBox->boolValue);
		return;
	}
}
void GameInputBox::inputing(){
#ifdef __MINGW32__
	if(!window)return;
	MSG msg;
	if(!PeekMessage(&msg,window,0,0,PM_REMOVE))return;
	TranslateMessage(&msg);
	DispatchMessage(&msg);
	if(msg.message==WM_KEYUP){
		if(msg.wParam==VK_RETURN){
			GetWindowText(window,strInputBuffer,BUFSIZ);//获得输入内容
			//字符串
			auto strInputBox=dynamic_cast<GameInputBox_String*>(gInputBox);
			if(strInputBox){
				strInputBox->setValue(strInputBuffer);
			}
			//数字
			auto intInputBox=dynamic_cast<GameInputBox_Integer*>(gInputBox);
			if(intInputBox){
				intInputBox->setValue(atoi(strInputBuffer));
			}
			finishInput();
		}
		if(msg.wParam==VK_ESCAPE){
			finishInput();
		}
	}
#else
	if(!window)return;
	if(gtk_events_pending())gtk_main_iteration();
#endif
}
bool GameAttr_InputBoxBool::getValue()const{return inputBox.boolValue;}
string GameAttr_InputBoxString::getValue()const{return inputBox.mString;}
int GameAttr_InputBoxInteger::getValue()const{return inputBox.mInteger;}
unsigned GameAttr_InputBoxInteger::getUnsignedValue()const{return inputBox.mInteger;}