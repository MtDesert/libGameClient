#include"GameStringInputBox.h"
#include"Game.h"
#include"extern.h"
#include"Thread.h"

static ColorRGBA borderColor(0xFFFFFFFF);//边框颜色

static void startInput(GameButton *button){
	auto inputBox=dynamic_cast<GameStringInputBox*>(button);
	if(inputBox){
		inputBox->startInput();
	}
}

GameStringInputBox::GameStringInputBox():passwordChar('\0'){
	//尺寸
	size.x=280;size.y=40;
	//文字
	mGameString.position.x=8;
	mGameString.anchorPoint.x=0;
	//开始输入事件
	onClicked=::startInput;
}
void GameStringInputBox::setString(const string &str){
	GameButton_String::setString(str);
	mString=str;
}

//线程实现输入
#ifdef __linux__

#include<gtk-3.0/gtk/gtk.h>
static GtkWidget *window=nullptr,*entry=nullptr;
static const char *inputStr=nullptr;

static void whenWindowClose(){
	inputStr=nullptr;
	entry=window=nullptr;//取消输入
	gtk_main_quit();
}
static void whenEnterKeyPressed(GtkEntry *entry,gpointer user_data){
	inputStr=gtk_entry_get_text(entry);//确认输入
	gtk_widget_hide(window);
	gtk_main_quit();
}

static Thread inputBoxThread;
static void* inputBoxThreadFunc(void *box){
	auto inputBox=reinterpret_cast<GameStringInputBox*>(box);
	if(!window){
		gtk_init(nullptr,nullptr);
		//创建窗口
		window=gtk_window_new(GTK_WINDOW_TOPLEVEL);
		g_signal_connect(window,"delete_event",G_CALLBACK(whenWindowClose),NULL);
	}
	if(!entry){
		//创建输入框
		entry=gtk_entry_new();
		gtk_container_add(GTK_CONTAINER(window),entry);
		g_signal_connect(entry,"activate",G_CALLBACK(whenEnterKeyPressed),NULL);
	}
	//设置文字内容
	gtk_entry_set_visibility(GTK_ENTRY(entry),!inputBox->passwordChar);
	gtk_entry_set_invisible_char(GTK_ENTRY(entry),inputBox->passwordChar);
	gtk_entry_set_text(GTK_ENTRY(entry),inputBox->mString.data());
	//显示输入窗口
	gtk_widget_show_all(window);
	gtk_main();
	//输入结束
	if(inputStr){
		if(inputBox)inputBox->setString(inputStr);
	}
	return nullptr;
}
void GameStringInputBox::startInput(){
	inputBoxThread.create(inputBoxThreadFunc,this);
}
#endif