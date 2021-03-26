#pragma once

class PopoverForm : public ui::WindowImplBase
{
public:
  PopoverForm();
  ~PopoverForm();

  /**
   * 一下三个接口是必须要覆写的接口，父类会调用这三个接口来构建窗口
   * GetSkinFolder		接口设置你要绘制的窗口皮肤资源路径
   * GetSkinFile			接口设置你要绘制的窗口的 xml 描述文件
   * GetWindowClassName	接口设置窗口唯一的类名称
   */
  virtual std::wstring GetSkinFolder() override;
  virtual std::wstring GetSkinFile() override;
  virtual std::wstring GetWindowClassName() const override;
  virtual ui::Control* CreateControl(const std::wstring& pstrClass) override;

  /**
   * 收到 WM_CREATE 消息时该函数会被调用，通常做一些控件初始化的操作
   */
  virtual void InitWindow() override;

  /**
   * 收到 WM_CLOSE 消息时该函数会被调用
   */
  virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

  static const std::wstring kClassName;

private:
  void TestAllPlacement();
  void TestOthers();

private:
  nim_comp::PopoverLayer* m_pPopoverLayer;

  ui::Button* btn_updatetitle;
  ui::Button* btn_updatecontent;
  ui::Button* btn_tl;
  ui::Button* btn_top;
  ui::Button* btn_tr;
  ui::Button* btn_lt;
  ui::Button* btn_rt;
  ui::Button* btn_left;
  ui::Button* btn_right;
  ui::Button* btn_lb;
  ui::Button* btn_rb;
  ui::Button* btn_bl;
  ui::Button* btn_bottom;
  ui::Button* btn_br;
};

