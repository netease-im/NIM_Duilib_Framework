#pragma once

#include "web_view_base.h"

namespace nim_comp {

class WkeWebNative : public WebViewBase {
 public:
  WkeWebNative();
  ~WkeWebNative() override;

  void InitNative(wkeWindowType type, HWND parent, int x, int y, int width, int height);

  void MoveWebWindow(int x, int y);

  void MoveWebWindowToCenter();

  void ResizeWebWindow(int width, int height);

  void ShowWebWindow(bool show);

};

}
