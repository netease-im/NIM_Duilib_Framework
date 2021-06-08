#include "stdafx.h"

#include "web_view_base.h"

namespace {

void OnTitleChangedCallback(wkeWebView webView, void* param, const wkeString title) {
  nim_comp::WebViewBase* web_view = (nim_comp::WebViewBase*)param;
  web_view->OnTitleChanged(webView, title);
}

void OnUrlChangedCallback(wkeWebView webView, void* param, const wkeString url) {
  nim_comp::WebViewBase* web_view = (nim_comp::WebViewBase*)param;
  web_view->OnUrlChanged(webView, url);
}

bool OnNavigationCallback(wkeWebView webView, void* param, wkeNavigationType navigationType, wkeString url) {
  nim_comp::WebViewBase* web_view = (nim_comp::WebViewBase*)param;
  return web_view->OnNavigation(webView, navigationType, url);
}

wkeWebView OnCreateViewCallback(wkeWebView webView, void* param, wkeNavigationType navigationType, const wkeString url, const wkeWindowFeatures* windowFeatures) {
  nim_comp::WebViewBase* web_view = (nim_comp::WebViewBase*)param;
  return web_view->OnCreateView(webView, navigationType, url, windowFeatures);
}

void OnLoadingFinishCallback(wkeWebView webView, void* param, const wkeString url, wkeLoadingResult result, const wkeString failedReason) {
  nim_comp::WebViewBase* web_view = (nim_comp::WebViewBase*)param;
  web_view->OnLoadingFinish(webView, url, result, failedReason);
}

bool OnLoadUrlBeginCallback(wkeWebView webView, void* param, const utf8* url, wkeNetJob job) {
  nim_comp::WebViewBase* web_view = (nim_comp::WebViewBase*)param;
  return web_view->OnLoadUrlBegin(webView, url, job);
}

void OnLoadUrlEndCallback(wkeWebView webView, void* param, const utf8* url, wkeNetJob job, void* buf, int len) {
  nim_comp::WebViewBase* web_view = (nim_comp::WebViewBase*)param;
  web_view->OnLoadUrlEnd(webView, url, job, buf, len);
}

void OnLoadUrlFailCallback(wkeWebView webView, void* param, const utf8* url, wkeNetJob job) {
  nim_comp::WebViewBase* web_view = (nim_comp::WebViewBase*)param;
  web_view->OnLoadUrlFail(webView, url, job);
}

bool OnWindowClosingCallback(wkeWebView webView, void* param) {
  nim_comp::WebViewBase* web_view = (nim_comp::WebViewBase*)param;
  return web_view->OnWindowClosing(webView);
}

void OnWindowDestroyCallback(wkeWebView webView, void* param) {
  nim_comp::WebViewBase* web_view = (nim_comp::WebViewBase*)param;
  web_view->OnWindowDestroy(webView);
}

bool NetOnResponseCallback(wkeWebView webView, void* param, const utf8* url, wkeNetJob job) {
  nim_comp::WebViewBase* web_view = (nim_comp::WebViewBase*)param;
  return web_view->NetOnResponse(webView, url, job);
}

}

namespace nim_comp {

WebViewBase::WebViewBase() {}

WebViewBase::~WebViewBase() {}

void WebViewBase::AddObserver(WebViewObserver* observer) {
  if (!observer) {
    return;
  }

  m_observer_list.emplace_back(observer);
}

void WebViewBase::RemoveObserver(WebViewObserver* observer) {
  auto item = std::find(m_observer_list.begin(), m_observer_list.end(), observer);
  if (item != m_observer_list.end()) {
    m_observer_list.erase(item);
  }
}

wkeWebView WebViewBase::CreateWebView() {
  return wkeCreateWebView();
}

wkeWebView WebViewBase::CreateWebWindow(wkeWindowType type, HWND parent, int x, int y, int width, int height) {
  return wkeCreateWebWindow(type, parent, x, y, width, height);
}

void WebViewBase::destroy() {
  if (m_wke_web_view) {
    wkeDestroyWebView(m_wke_web_view);
  }
}

const char* WebViewBase::name() const {
  return m_wke_web_view ? wkeWebViewName(m_wke_web_view) : "";
}

void WebViewBase::setName(const char* name) {
  if (m_wke_web_view) {
    wkeSetWebViewName(m_wke_web_view, name);
  }
}

bool WebViewBase::isTransparent() const {
  return m_wke_web_view ? wkeIsTransparent(m_wke_web_view) : false;
}

void WebViewBase::setTransparent(bool transparent) {
  if (m_wke_web_view) {
    wkeSetTransparent(m_wke_web_view, transparent);
  }
}

void WebViewBase::loadURL(const utf8* url) {
  if (m_wke_web_view) {
    wkeLoadURL(m_wke_web_view, url);
  }
}

void WebViewBase::loadURL(const wchar_t* url) {
  if (m_wke_web_view) {
    wkeLoadURLW(m_wke_web_view, url);
  }
}

void WebViewBase::loadHTML(const utf8* html) {
  if (m_wke_web_view) {
    wkeLoadHTML(m_wke_web_view, html);
  }
}

void WebViewBase::loadHTML(const wchar_t* html) {
  if (m_wke_web_view) {
    wkeLoadHTMLW(m_wke_web_view, html);
  }
}

void WebViewBase::loadFile(const utf8* filename) {
  if (m_wke_web_view) {
    wkeLoadFile(m_wke_web_view, filename);
  }
}

void WebViewBase::loadFile(const wchar_t* filename) {
  if (m_wke_web_view) {
    wkeLoadFileW(m_wke_web_view, filename);
  }
}

const utf8* WebViewBase::url() const {
  if (m_wke_web_view) {
    return wkeGetURL(m_wke_web_view);
  }
  return "";
}

bool WebViewBase::isLoading() const {
  return m_wke_web_view ? wkeIsLoading(m_wke_web_view) : false;
}

bool WebViewBase::isLoadingFailed() const {
  return m_wke_web_view ? wkeIsLoadingFailed(m_wke_web_view) : false;
}

bool WebViewBase::isLoadingSucceeded() const {
  return m_wke_web_view ? wkeIsLoadingSucceeded(m_wke_web_view) : false;
}

bool WebViewBase::isDocumentReady() const {
  return m_wke_web_view ? wkeIsDocumentReady(m_wke_web_view) : false;
}

void WebViewBase::stopLoading() {
  if (m_wke_web_view) {
    wkeStopLoading(m_wke_web_view);
  }
}

void WebViewBase::reload() {
  if (m_wke_web_view) {
    wkeReload(m_wke_web_view);
  }
}

const utf8* WebViewBase::title() {
  return m_wke_web_view ? wkeGetTitle(m_wke_web_view) : "";
}

const wchar_t* WebViewBase::titleW() {
  return m_wke_web_view ? wkeGetTitleW(m_wke_web_view) : L"";
}

void WebViewBase::resize(int w, int h) {
  if (m_wke_web_view) {
    wkeResize(m_wke_web_view, w, h);
  }
}

int WebViewBase::width() const {
  return m_wke_web_view ? wkeGetWidth(m_wke_web_view) : 0;
}

int WebViewBase::height() const {
  return m_wke_web_view ? wkeGetHeight(m_wke_web_view) : 0;
}

int WebViewBase::contentsWidth() const {
  return m_wke_web_view ? wkeGetContentWidth(m_wke_web_view) : 0;
}

int WebViewBase::contentsHeight() const {
  return m_wke_web_view ? wkeGetContentHeight(m_wke_web_view) : 0;
}

void WebViewBase::setDirty(bool dirty) {
  if (m_wke_web_view) {
    wkeSetDirty(m_wke_web_view, dirty);
  }
}

bool WebViewBase::isDirty() const {
  return m_wke_web_view ? wkeIsDirty(m_wke_web_view) : false;
}

void WebViewBase::addDirtyArea(int x, int y, int w, int h) {
  if (m_wke_web_view) {
    wkeAddDirtyArea(m_wke_web_view, x, y, w, h);
  }
}

void WebViewBase::layoutIfNeeded() {
  if (m_wke_web_view) {
    wkeLayoutIfNeeded(m_wke_web_view);
  }
}

void WebViewBase::paint(void* bits, int pitch) {
  if (m_wke_web_view) {
    wkePaint(m_wke_web_view, bits, pitch);
  }
}

bool WebViewBase::canGoBack() const {
  return m_wke_web_view ? wkeCanGoBack(m_wke_web_view) : false;
}

bool WebViewBase::goBack() {
  return m_wke_web_view ? wkeGoBack(m_wke_web_view) : false;
}

bool WebViewBase::canGoForward() const {
  return m_wke_web_view ? wkeCanGoForward(m_wke_web_view) : false;
}

bool WebViewBase::goForward() {
  return m_wke_web_view ? wkeGoForward(m_wke_web_view) : false;
}

void WebViewBase::editorSelectAll() {
  if (m_wke_web_view) {
    wkeEditorSelectAll(m_wke_web_view);
  }
}

void WebViewBase::editorUnSelect() {
  if (m_wke_web_view) {
    wkeEditorUnSelect(m_wke_web_view);
  }
}

void WebViewBase::editorCopy() {
  if (m_wke_web_view) {
    wkeEditorCopy(m_wke_web_view);
  }
}

void WebViewBase::editorCut() {
  if (m_wke_web_view) {
    wkeEditorCut(m_wke_web_view);
  }
}

void WebViewBase::editorPaste() {
  if (m_wke_web_view) {
    wkeEditorPaste(m_wke_web_view);
  }
}

void WebViewBase::editorDelete() {
  if (m_wke_web_view) {
    wkeEditorDelete(m_wke_web_view);
  }
}

void WebViewBase::editorUndo() {
  if (m_wke_web_view) {
    wkeEditorUndo(m_wke_web_view);
  }
}

void WebViewBase::editorRedo() {
  if (m_wke_web_view) {
    wkeEditorRedo(m_wke_web_view);
  }
}

void WebViewBase::setCookieEnabled(bool enable) {
  if (m_wke_web_view) {
    wkeSetCookieEnabled(m_wke_web_view, enable);
  }
}

bool WebViewBase::isCookieEnabled() const {
  return m_wke_web_view ? wkeIsCookieEnabled(m_wke_web_view) : false;
}

void WebViewBase::setMediaVolume(float volume) {
  if (m_wke_web_view) {
    wkeSetMediaVolume(m_wke_web_view, volume);
  }
}

float WebViewBase::mediaVolume() const {
  return m_wke_web_view ? wkeGetMediaVolume(m_wke_web_view) : false;
}

bool WebViewBase::fireMouseEvent(unsigned int message, int x, int y, unsigned int flags) {
  return m_wke_web_view ? wkeFireMouseEvent(m_wke_web_view, message, x, y, flags) : false;
}

bool WebViewBase::fireContextMenuEvent(int x, int y, unsigned int flags) {
  return m_wke_web_view ? wkeFireContextMenuEvent(m_wke_web_view, x, y, flags) : false;
}

bool WebViewBase::fireMouseWheelEvent(int x, int y, int delta, unsigned int flags) {
  return m_wke_web_view ? wkeFireMouseWheelEvent(m_wke_web_view, x, y, delta, flags) : false;
}

bool WebViewBase::fireKeyUpEvent(unsigned int virtualKeyCode, unsigned int flags, bool systemKey) {
  return m_wke_web_view ? wkeFireKeyUpEvent(m_wke_web_view, virtualKeyCode, flags, systemKey) : false;
}

bool WebViewBase::fireKeyDownEvent(unsigned int virtualKeyCode, unsigned int flags, bool systemKey) {
  return m_wke_web_view ? wkeFireKeyDownEvent(m_wke_web_view, virtualKeyCode, flags, systemKey) : false;
}

bool WebViewBase::fireKeyPressEvent(unsigned int virtualKeyCode, unsigned int flags, bool systemKey) {
  return m_wke_web_view ? wkeFireKeyPressEvent(m_wke_web_view, virtualKeyCode, flags, systemKey) : false;
}

void WebViewBase::setFocus() {
  if (m_wke_web_view) {
    wkeSetFocus(m_wke_web_view);
  }
}

void WebViewBase::killFocus() {
  if (m_wke_web_view) {
    wkeKillFocus(m_wke_web_view);
  }
}

wkeRect WebViewBase::getCaret() {
  wkeRect rect = { 0,0,0,0 };
  if (m_wke_web_view) {
    rect = wkeGetCaret(m_wke_web_view);
  }

  return rect;
}

jsValue WebViewBase::runJS(const utf8* script) {
  jsValue value = 0;
  if (m_wke_web_view) {
    wkeRunJS(m_wke_web_view, script);
  }

  return value;
}

jsValue WebViewBase::runJS(const wchar_t* script) {
  jsValue value = 0;
  if (m_wke_web_view) {
    wkeRunJSW(m_wke_web_view, script);
  }

  return value;
}

jsExecState WebViewBase::globalExec() {
  return m_wke_web_view ? wkeGlobalExec(m_wke_web_view) : nullptr;
}

void WebViewBase::sleep() {
  if (m_wke_web_view) {
    wkeSleep(m_wke_web_view);
  }
}
void WebViewBase::wake() {
  if (m_wke_web_view) {
    wkeWake(m_wke_web_view);
  }
}

bool WebViewBase::isAwake() const {
  return m_wke_web_view ? wkeIsAwake(m_wke_web_view) : false;
}

void WebViewBase::setZoomFactor(float factor) {
  if (m_wke_web_view) {
    wkeSetZoomFactor(m_wke_web_view, factor);
  }
}

float WebViewBase::zoomFactor() const {
  return m_wke_web_view ? wkeGetZoomFactor(m_wke_web_view) : 0.f;
}

void WebViewBase::setEditable(bool editable) {
  if (m_wke_web_view) {
    wkeSetEditable(m_wke_web_view, editable);
  }
}

void WebViewBase::setClientHandler(const wkeClientHandler* handler) {
  if (m_wke_web_view) {
    wkeSetClientHandler(m_wke_web_view, handler);
  }
}

const wkeClientHandler* WebViewBase::getClientHandler() const {
  return m_wke_web_view ? wkeGetClientHandler(m_wke_web_view) : nullptr;
}

void WebViewBase::NetHookRequest(wkeNetJob job) {
  if (m_wke_web_view) {
    wkeNetHookRequest(job);
  }
}

void WebViewBase::SetContextMenuEnabled(bool enabled) {
  if (m_wke_web_view) {
    wkeSetContextMenuEnabled(m_wke_web_view, enabled);
  }
}

void WebViewBase::OnTitleChanged(wkeWebView webView, const wkeString title) {
  std::wstring title_temp = wkeGetStringW(title);
  if (m_title_changed_callback) {
    m_title_changed_callback(title_temp);
  }
}

void WebViewBase::OnUrlChanged(wkeWebView webView, const wkeString url) {
  std::wstring url_temp = wkeGetStringW(url);
  if (m_url_changed_callback) {
    m_url_changed_callback(url_temp);
  }
}

bool WebViewBase::OnNavigation(wkeWebView webView, wkeNavigationType navigationType, wkeString url) {
  std::wstring url_temp = wkeGetStringW(url);
  if (m_navigation_callback) {
    return m_navigation_callback(navigationType, url_temp);
  }

  return true;
}

wkeWebView WebViewBase::OnCreateView(wkeWebView webView, wkeNavigationType navigationType, const wkeString url, const wkeWindowFeatures* windowFeatures) {
  std::wstring url_temp = wkeGetStringW(url);
  if (m_create_web_view_callback) {
    return m_create_web_view_callback(navigationType, url_temp, windowFeatures);
  }

  return nullptr;
}

void WebViewBase::OnLoadingFinish(wkeWebView webView, const wkeString url, wkeLoadingResult result, const wkeString failedReason) {
  std::wstring url_temp = wkeGetStringW(url);
  std::wstring failed_reason_temp = wkeGetStringW(failedReason);
  if (m_loading_finish_callback) {
    m_loading_finish_callback(url_temp, result, failed_reason_temp);
  }
}

bool WebViewBase::OnLoadUrlBegin(wkeWebView webView, const utf8* url, wkeNetJob job) {
  if (m_load_url_begin_callback) {
    return m_load_url_begin_callback(url, job);
  }

  return false;
}

void WebViewBase::OnLoadUrlEnd(wkeWebView webView, const utf8* url, wkeNetJob job, void* buf, int len) {
  if (m_load_url_end_callback) {
    m_load_url_end_callback(url, job, buf, len);
  }
}

void WebViewBase::OnLoadUrlFail(wkeWebView webView, const utf8* url, wkeNetJob job) {
  if (m_load_url_fail_callback)  {
    m_load_url_fail_callback(url, job);
  }
}

bool WebViewBase::OnWindowClosing(wkeWebView webView) {
  if (m_window_closing_callback) {
    return m_window_closing_callback();
  }

  return true;
}

void WebViewBase::OnWindowDestroy(wkeWebView webView) {
  if (m_window_destroy_callback) {
    m_window_destroy_callback();
  }
}

bool WebViewBase::NetOnResponse(wkeWebView webView, const utf8* url, wkeNetJob job) {
  if (m_response_callback) {
    return m_response_callback(url, job);
  }
  return false;
}

void WebViewBase::InitWebViewBase() {
  wkeOnTitleChanged(m_wke_web_view, OnTitleChangedCallback, this);
  wkeOnURLChanged(m_wke_web_view, OnUrlChangedCallback, this);
  wkeOnNavigation(m_wke_web_view, OnNavigationCallback, this);
  wkeOnCreateView(m_wke_web_view, OnCreateViewCallback, this);
  wkeOnLoadingFinish(m_wke_web_view, OnLoadingFinishCallback, this);
  wkeOnLoadUrlBegin(m_wke_web_view, OnLoadUrlBeginCallback, this);
  wkeOnLoadUrlEnd(m_wke_web_view, OnLoadUrlEndCallback, this);
  wkeOnLoadUrlFail(m_wke_web_view, OnLoadUrlFailCallback, this);
  wkeOnWindowClosing(m_wke_web_view, OnWindowClosingCallback, this);
  wkeOnWindowDestroy(m_wke_web_view, OnWindowDestroyCallback, this);
  wkeNetOnResponse(m_wke_web_view, NetOnResponseCallback, this);

  wkeSetNavigationToNewWindowEnable(m_wke_web_view, true);
}

}
