#pragma once

#include "third_party/mini_blink/include/wke.h"

namespace nim_comp {
class WebViewObserver;

typedef std::function<void(const std::wstring&)> TitleChangedCallback;
typedef std::function<void(const std::wstring&)> UrlChangedCallback;
typedef std::function<bool(wkeNavigationType, std::wstring&)> NavigationCallback;
typedef std::function<wkeWebView(wkeNavigationType, const std::wstring&, const wkeWindowFeatures*)> CreateWebViewCallback;
typedef std::function<void(const std::wstring&, wkeLoadingResult, const std::wstring&)> LoadingFinishCallback;
typedef std::function<bool(const std::string&, wkeNetJob)> LoadUrlBeginCallback;
typedef std::function<void(const std::string&, wkeNetJob, void*, int)> LoadUrlEndCallback;
typedef std::function<bool(const std::string&, wkeNetJob)> LoadUrlFailCallback;
typedef std::function<bool()> WindowClosingCallback;
typedef std::function<bool()> WindowDestroyCallback;
typedef std::function<bool(const std::string&, wkeNetJob)> ResponseCallback;

class WebViewBase : public wke::IWebView {
public:
  WebViewBase();
  virtual ~WebViewBase();

  void AddObserver(WebViewObserver* observer);
  void RemoveObserver(WebViewObserver* observer);

  wkeWebView CreateWebView();

  wkeWebView CreateWebWindow(wkeWindowType type, HWND parent, int x, int y, int width, int height);

  // override from wke::IWebView
  // api document: https://miniblink.net/views/doc/APIDocument/API/index.html
  void destroy() override;
  const char* name() const override;
  void setName(const char* name) override;
  bool isTransparent() const override;
  void setTransparent(bool transparent) override;
  void loadURL(const utf8* url) override;
  void loadURL(const wchar_t* url) override;
  void loadHTML(const utf8* html) override;
  void loadHTML(const wchar_t* html) override;
  void loadFile(const utf8* filename) override;
  void loadFile(const wchar_t* filename) override;
  const utf8* url() const override;
  bool isLoading() const override;        /*document load sucessed*/
  bool isLoadingFailed() const override;    /*document load failed*/
  bool isLoadingSucceeded() const override;  /*document load complete*/
  bool isDocumentReady() const override; /*document ready*/
  void stopLoading() override;
  void reload() override;
  const utf8* title() override;
  const wchar_t* titleW() override;
  void resize(int w, int h) override;
  int width() const override;   /*viewport width*/
  int height() const override;  /*viewport height*/
  int contentsWidth() const override;  /*contents width*/
  int contentsHeight() const override; /*contents height*/
  void setDirty(bool dirty) override;
  bool isDirty() const override;
  void addDirtyArea(int x, int y, int w, int h) override;
  void layoutIfNeeded() override;
  void paint(void* bits, int pitch) override;
  bool canGoBack() const override;
  bool goBack() override;
  bool canGoForward() const override;
  bool goForward() override;
  void editorSelectAll() override;
  void editorUnSelect() override;
  void editorCopy() override;
  void editorCut() override;
  void editorPaste() override;
  void editorDelete() override;
  void editorUndo() override;
  void editorRedo() override;
  void setCookieEnabled(bool enable) override;
  bool isCookieEnabled() const override;
  void setMediaVolume(float volume) override;
  float mediaVolume() const override;
  bool fireMouseEvent(unsigned int message, int x, int y, unsigned int flags) override;
  bool fireContextMenuEvent(int x, int y, unsigned int flags) override;
  bool fireMouseWheelEvent(int x, int y, int delta, unsigned int flags) override;
  bool fireKeyUpEvent(unsigned int virtualKeyCode, unsigned int flags, bool systemKey) override;
  bool fireKeyDownEvent(unsigned int virtualKeyCode, unsigned int flags, bool systemKey) override;
  bool fireKeyPressEvent(unsigned int virtualKeyCode, unsigned int flags, bool systemKey) override;
  void setFocus() override;
  void killFocus() override;
  wkeRect getCaret() override;
  jsValue runJS(const utf8* script) override;
  jsValue runJS(const wchar_t* script) override;
  jsExecState globalExec() override;
  void sleep() override; //moveOffscreen
  void wake() override; //moveOnscreen
  bool isAwake() const override;
  void setZoomFactor(float factor) override;
  float zoomFactor() const override;
  void setEditable(bool editable) override;
  void setClientHandler(const wkeClientHandler* handler) override;
  const wkeClientHandler* getClientHandler() const override;

  void NetHookRequest(wkeNetJob job);
  void SetContextMenuEnabled(bool enabled);

  virtual void OnTitleChanged(wkeWebView webView, const wkeString title);

  virtual void OnUrlChanged(wkeWebView webView, const wkeString url);

  virtual bool OnNavigation(wkeWebView webView,  wkeNavigationType navigationType, wkeString url);

  virtual wkeWebView OnCreateView(wkeWebView webView, wkeNavigationType navigationType, const wkeString url, const wkeWindowFeatures* windowFeatures);

  virtual void OnLoadingFinish(wkeWebView webView, const wkeString url, wkeLoadingResult result, const wkeString failedReason);

  virtual bool OnLoadUrlBegin(wkeWebView webView, const utf8* url, wkeNetJob job);

  virtual void OnLoadUrlEnd(wkeWebView webView, const utf8* url, wkeNetJob job, void* buf, int len);

  virtual void OnLoadUrlFail(wkeWebView webView, const utf8* url, wkeNetJob job);

  virtual bool OnWindowClosing(wkeWebView webView);

  virtual void OnWindowDestroy(wkeWebView webView);

  virtual bool NetOnResponse(wkeWebView webView, const utf8* url, wkeNetJob job);

  void AttachOnTitleChanged(const TitleChangedCallback& callback) { m_title_changed_callback = callback; }

  void AttachOnUrlChanged(const UrlChangedCallback& callback) { m_url_changed_callback = callback; }

  void AttachOnNavigation(const NavigationCallback& callback) { m_navigation_callback = callback; }

  void AttachOnCreateView(const CreateWebViewCallback& callback) { m_create_web_view_callback = callback; }

  void AttachOnLoadingFinish(const LoadingFinishCallback& callback) { m_loading_finish_callback = callback; }

  void AttachOnLoadUrlBegin(const LoadUrlBeginCallback& callback) { m_load_url_begin_callback = callback; }

  void AttachOnLoadUrlEnd(const LoadUrlEndCallback& callback) { m_load_url_end_callback = callback; }

  void AttachOnLoadUrlFail(const LoadUrlFailCallback& callback) { m_load_url_fail_callback = callback; }

  void AttachOnWindowClosingCallback(const WindowClosingCallback& callback) { m_window_closing_callback = callback; }

  void AttachOnWindowDestroy(const WindowDestroyCallback& callback) { m_window_destroy_callback = callback; }

  void AttachNetOnResponse(const ResponseCallback& callback) { m_response_callback = callback; }

 protected:
  void InitWebViewBase();

  wkeWebView m_wke_web_view = nullptr;

  std::vector<WebViewObserver*> m_observer_list;

 private:
  TitleChangedCallback m_title_changed_callback;
  UrlChangedCallback m_url_changed_callback;
  NavigationCallback m_navigation_callback;
  CreateWebViewCallback m_create_web_view_callback;
  LoadingFinishCallback m_loading_finish_callback;
  LoadUrlBeginCallback m_load_url_begin_callback;
  LoadUrlEndCallback m_load_url_end_callback;
  LoadUrlFailCallback m_load_url_fail_callback;
  WindowClosingCallback m_window_closing_callback;
  WindowDestroyCallback m_window_destroy_callback;
  ResponseCallback m_response_callback;
};

}
