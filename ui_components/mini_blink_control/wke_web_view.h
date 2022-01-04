#pragma once

#include "web_view_base.h"

class WkeMemoryDC;

namespace nim_comp {
class WkeWebView : public ui::Control,
                   public WebViewBase,
                   public ui::IUIMessageFilter {
 public:
  WkeWebView();
  ~WkeWebView() override;

  // override form ui::Control
  void DoInit() override;
  void Paint(ui::IRenderContext* pRender, const ui::UiRect& rcPaint) override;
  void SetWindow(ui::Window* pManager, ui::Box* pParent, bool bInit) override;

  // override from ui::IUIMessageFilter
  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled) override; // 处理窗体消息，转发到miniblink

  void OnPaint(wkeWebView webView, const void* buffer, const wkeRect* r, int width, int height);

  void SetWheelEnabled(bool enabled) { m_bWheelEnabled = enabled; }

 private:
  void OnCursorChange();
  bool SetCursorInfoTypeByCache();
  bool GetWebViewPos(ui::CPoint& point);

  bool m_bInit = false;
  bool m_bWheelEnabled = true;
  std::unique_ptr<WkeMemoryDC> m_web_view_dc;
  int m_cursorInfoType = 0;
};

}
