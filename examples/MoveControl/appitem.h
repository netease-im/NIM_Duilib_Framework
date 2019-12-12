#pragma once

#define APP_HEIGHT 90
#define APP_WIDTH  90
#define EACH_LINE  6

#include <string>
//app的具体信息，这里假定有id，name，_icon,_isFrequent自行拓展
struct AppItem
{
	std::string _id;
	std::wstring _name;
	std::wstring _icon;
	bool _isFrequent=false;
};


//App UI类
class AppItemUi : public ui::VBox
{
public:
	static AppItemUi* Create(const AppItem& item);
	virtual void DoInit();
	void SetAppdata(const AppItem& item,bool refresh);
	void FixPos(int step,int index=-1);   //前进/后退多少步  目前应该有-1 0 1    
	inline int getIndex() const { return index_; }
	inline const AppItem& getAppData() const { return app_data_; }
private:
	AppItem app_data_;
	int index_ = 0;    //第几个
	ui::Control* app_icon_ = nullptr;
	ui::Label* app_name_ = nullptr;
};


//AppWindow 拖动显示窗口类
//最好半透明
class AppWindow : public ui::WindowImplBase
{
public:
	AppWindow();
	~AppWindow();

	static AppWindow* CreateAppWindow(HWND hParent, POINT pt, const AppItem& Item)
	{
		AppWindow* ret = new AppWindow;
		ret->SetBeforeCreate(Item, pt);
		ret->Create(hParent, L"", WS_POPUP, WS_EX_TOOLWINDOW);
		pThis_ = ret;
		//需要改变下pos,延后到initWindows
		return ret;
	}

	/**
	* 一下三个接口是必须要覆写的接口，父类会调用这三个接口来构建窗口
	* GetSkinFolder		接口设置你要绘制的窗口皮肤资源路径
	* GetSkinFile			接口设置你要绘制的窗口的 xml 描述文件
	* GetWindowClassName	接口设置窗口唯一的类名称
	*/
	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;

	/**
	* 收到 WM_CREATE 消息时该函数会被调用，通常做一些控件初始化的操作
	*/
	virtual void InitWindow() override;
	/**
	* 收到 WM_CLOSE 消息时该函数会被调用
	*/
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	//其他的功能函数
	void SetBeforeCreate(const AppItem& Item, POINT pt){ item_ = Item; pt_ = pt; }
	void AdjustPos();
	void InstallHook();
	void UnInstallHook();
	static LRESULT CALLBACK LowLevelMouseProc(int nCode, WPARAM wParam, LPARAM lParam);
private:
	AppItem item_;
	ui::Box* origin_owner = nullptr;  //
	POINT pt_;
	static HHOOK mouse_Hook_ ;
	static AppWindow* pThis_;
};

