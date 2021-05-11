#include "stdafx.h"

#include "wke_web_native.h"

namespace nim_comp {
WkeWebNative::WkeWebNative() {}

WkeWebNative::~WkeWebNative() {
  if (m_wke_web_view) {
    wkeDestroyWebWindow(m_wke_web_view);
  }
}

void WkeWebNative::InitNative(wkeWindowType type, HWND parent, int x, int y, int width, int height) {
  m_wke_web_view = CreateWebWindow(type, parent, x, y, width, height);

  InitWebViewBase();
}

void WkeWebNative::MoveWebWindow(int x, int y) {
  if (m_wke_web_view) {
    wkeMoveWindow(m_wke_web_view, x, y, width(), height());
  }
}

void WkeWebNative::MoveWebWindowToCenter() {
  if (m_wke_web_view) {
    wkeMoveToCenter(m_wke_web_view);
  }
}

void WkeWebNative::ResizeWebWindow(int width, int height) {
  if (m_wke_web_view) {
    wkeResizeWindow(m_wke_web_view, width, height);
  }
}

void WkeWebNative::ShowWebWindow(bool show) {
  if (m_wke_web_view) {
    wkeShowWindow(m_wke_web_view, show);
  }
}

}
