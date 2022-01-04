# CefControl（Cef 控件）

`CefControl` 和 `CefNativeControl` 继承了 `Control` 控件属性和方法，更多可用属性和方法请参考：[Control](Control.md) 控件

 - CefControl 是离屏渲染模式的 CEF 控件
 - CefNativeControl 是真窗口模式的 CEF 控件（具有窗口句柄）

CEF 控件使用了最后支持 XP 的 2623 版本，控件依赖 CEF 已经编译好的二进制文件（见 bin 目录中 cef 和 cef_x64）和 CEF 的 C++ 封装层 `third_party\\cef_wrapper`，
如果使用到了 `CefControl` 或 `CefNativeControl`，需要将对应版本的 cef 目录复制到可执行文件相同目录并将 `third_party\\cef_wrapper` 目录设置为公共头目录，更多请参考 samples 目录下的 cef 示例和 multi_browser 示例。

| 方法名称 | 用途 |
| :--- | :--- |
| [LoadURL](#LoadURL) | 加载一个地址 |
| [LoadString](#LoadString) | 给指定地址添加一个字符串资源 |
| [GoBack](#GoBack) | 后退 |
| [GoForward](#GoForward) | 前进 |
| [CanGoBack](#CanGoBack) | 判断是否可以后退 |
| [CanGoForward](#CanGoForward) | 判断是否可以前进 |
| [Refresh](#Refresh) | 刷新 |
| [StopLoad](#StopLoad) | 停止加载 |
| [IsLoading](#IsLoading) | 是否加载中 |
| [StartDownload](#StartDownload) | 开始一个下载任务 |
| [SetZoomLevel](#SetZoomLevel) | 设置页面缩放比例 |
| [GetURL](#GetURL) | 获取页面 URL |
| [GetUTF8URL](#GetUTF8URL) | 获取 UTF8 格式 URL |
| [GetMainURL](#GetMainURL) | 获取网址 # 号前的地址 |
| [RegisterCppFunc](#RegisterCppFunc) | 注册一个 C++ 方法提供前端调用 |
| [UnRegisterCppFunc](#UnRegisterCppFunc) | 反注册一个 C++ 方法 |
| [CallJSFunction](#CallJSFunction) | 调用一个前端已经注册好的方法 |
| [CallJSFunction](#CallJSFunction) | 调用一个前端已经注册好的方法 |
| [AttachDevTools](#AttachDevTools) | 绑定开发者工具到一个 CefControl 控件上 |
| [DettachDevTools](#DettachDevTools) | 解绑开发者工具 |
| [IsAttachedDevTools](#IsAttachedDevTools) | 判断是否已经绑定开发者工具到一个 CefControl |
| [RepairBrowser](#RepairBrowser) | 修复浏览器 |
| [AttachBeforeContextMenu](#AttachBeforeContextMenu) | 绑定一个回调函数用于监听右键菜单弹出 |
| [AttachMenuCommand](#AttachMenuCommand) | 绑定一个回调函数用于监听选择了哪个右键菜单 |
| [AttachTitleChange](#AttachTitleChange) | 绑定一个回调函数用于监听页面 Title 改变 |
| [AttachUrlChange](#AttachUrlChange) | 绑定一个回调函数用于监听页面中 frame URL 地址改变 |
| [AttachMainURLChange](#AttachMainURLChange) | 绑定一个回调函数用于监听主页面 URL 地址改变 |
| [AttachBeforeNavigate](#AttachBeforeNavigate) | 绑定一个回调函数用于监听页面资源全部加载完毕 |
| [AttachLinkClick](#AttachLinkClick) | 绑定一个回调函数用于监听一个弹出窗口弹出的通知 |
| [AttachLoadingStateChange](#AttachLoadingStateChange) | 绑定一个回调函数用于监听页面加载状态改变 |
| [AttachLoadStart](#AttachLoadStart) | 绑定一个回调函数用于监听页面开始加载通知 |
| [AttachLoadEnd](#AttachLoadEnd) | 绑定一个回调函数用于监听页面加载完毕通知 |
| [AttachLoadError](#AttachLoadError) | 绑定一个回调函数用于监听界面加载错误通知 |
| [AttachDevToolAttachedStateChange](#AttachDevToolAttachedStateChange) | 绑定一个回调函数用于监听开发者工具状态改变通知 |

### LoadURL

加载一个地址

```cpp
void LoadURL(const CefString& url)
```

 - 参&emsp;数：  
    - `url` 网站地址
 - 返回值：无

### LoadString

给指定地址添加一个字符串资源

```cpp
void LoadString(const CefString& stringW, const CefString& url)
```

 - 参&emsp;数：  
    - `stringW` 字符串资源
    - `url` 网址
 - 返回值：无

### GoBack

后退

```cpp
void GoBack()
```

 - 参&emsp;数：无  
 - 返回值：无

### GoForward

前进

```cpp
void GoForward()
```

 - 参&emsp;数：无  
 - 返回值：无

### CanGoBack

判断是否可以后退

```cpp
bool CanGoBack()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 表示可以，false 表示不可以

### CanGoForward

判断是否可以前进

```cpp
bool CanGoForward()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 表示可以，false 表示不可以

### Refresh

刷新

```cpp
void Refresh()
```

 - 参&emsp;数：无  
 - 返回值：无

### StopLoad

停止加载

```cpp
void StopLoad()
```

 - 参&emsp;数：无  
 - 返回值：无

### IsLoading

是否加载中

```cpp
bool IsLoading()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 表示加载中，否则为 false

### StartDownload

开始一个下载任务

```cpp
void StartDownload(const CefString& url)
```

 - 参&emsp;数：  
    - `url` 要下载的文件地址
 - 返回值：无

### SetZoomLevel

设置页面缩放比例

```cpp
void SetZoomLevel(float zoom_level)
```

 - 参&emsp;数：  
    - `zoom_level` 比例值
 - 返回值：无

### GetURL

获取页面 URL

```cpp
CefString GetURL()
```

 - 参&emsp;数：无  
 - 返回值：返回 URL 地址

### GetUTF8URL

获取 UTF8 格式 URL

```cpp
std::string GetUTF8URL()
```

 - 参&emsp;数：无  
 - 返回值：返回 URL 地址

### GetMainURL

获取网址 # 号前的地址

```cpp
CefString GetMainURL(const CefString& url)
```

 - 参&emsp;数：  
    - `url` 要获取的完整地址
 - 返回值：返回截取后的地址

### RegisterCppFunc

注册一个 C++ 方法提供前端调用

```cpp
bool RegisterCppFunc(const std::wstring& function_name, nim_cef::CppFunction function, bool global_function = false)
```

 - 参&emsp;数：  
    - `function_name` 方法名称
    - `function` 方法函数体
    - `global_function` 是否是一个全局方法
 - 返回值：返回 true 表示注册成功，false 可能已经注册

### UnRegisterCppFunc

反注册一个 C++ 方法

```cpp
void UnRegisterCppFunc(const std::wstring& function_name)
```

 - 参&emsp;数：  
    - `function_name` 方法名称
 - 返回值：无

### CallJSFunction

调用一个前端已经注册好的方法

```cpp
bool CallJSFunction(const std::wstring& js_function_name, const std::wstring& params, nim_cef::CallJsFunctionCallback callback, const std::wstring& frame_name = L"")
```

 - 参&emsp;数：  
    - `js_function_name` 前端提供的方法名
    - `params` 传递 JSON 字符串格式的参数
    - `callback` 前端执行完成后的回调函数
    - `frame_name` 要调用哪个名称 frame 下的方法，默认使用主 frame
 - 返回值：返回 true 表示成功调用，false 表示调用失败，方法可能不存在

### CallJSFunction

调用一个前端已经注册好的方法

```cpp
bool CallJSFunction(const std::wstring& js_function_name, const std::wstring& params, nim_cef::CallJsFunctionCallback callback, int frame_id)
```

 - 参&emsp;数：  
    - `js_function_name` 前端提供的方法名
    - `params` 传递 JSON 字符串格式的参数
    - `callback` 前端执行完成后的回调函数
    - `frame_id` 要调用哪个 ID frame 下的方法，默认使用主 frame
 - 返回值：返回 true 表示成功调用，false 表示调用失败，方法可能不存在

### AttachDevTools

绑定开发者工具到一个 CefControl 控件上

```cpp
virtual bool AttachDevTools(CefControl* view)
```

 - 参&emsp;数：  
    - `view` 一个 CefControl 控件实例
 - 返回值：成功返回 true，失败返回 false

### DettachDevTools

解绑开发者工具

```cpp
virtual void DettachDevTools()
```

 - 参&emsp;数：无  
 - 返回值：无

### IsAttachedDevTools

判断是否已经绑定开发者工具到一个 CefControl

```cpp
virtual bool IsAttachedDevTools()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 表示已经绑定，false 为未绑定

### RepairBrowser

修复浏览器

```cpp
virtual void RepairBrowser()
```

 - 参&emsp;数：无  
 - 返回值：无

### AttachBeforeContextMenu

绑定一个回调函数用于监听右键菜单弹出

```cpp
void AttachBeforeContextMenu(const OnBeforeMenuEvent& callback)
```

 - 参&emsp;数：  
    - `callback` 一个回调函数，参考 OnBeforeMenuEvent 声明
 - 返回值：无

### AttachMenuCommand

绑定一个回调函数用于监听选择了哪个右键菜单

```cpp
void AttachMenuCommand(const OnMenuCommandEvent& callback)
```

 - 参&emsp;数：  
    - `callback` 一个回调函数，参考 OnMenuCommandEvent 声明
 - 返回值：无

### AttachTitleChange

绑定一个回调函数用于监听页面 Title 改变

```cpp
void AttachTitleChange(const OnTitleChangeEvent& callback)
```

 - 参&emsp;数：  
    - `callback` 一个回调函数，参考 OnTitleChangeEvent 声明
 - 返回值：无

### AttachUrlChange

绑定一个回调函数用于监听页面中 frame URL 地址改变

```cpp
void AttachUrlChange(const OnUrlChangeEvent& callback)
```

 - 参&emsp;数：  
    - `callback` 一个回调函数，参考 OnUrlChangeEvent 声明
 - 返回值：无

### AttachMainURLChange

绑定一个回调函数用于监听主页面 URL 地址改变

```cpp
void AttachMainURLChange(OnMainURLChengeEvent cb)
```

 - 参&emsp;数：  
    - `callback` 一个回调函数，参考 OnMainURLChengeEvent 声明
 - 返回值：无

### AttachBeforeNavigate

绑定一个回调函数用于监听页面资源全部加载完毕

```cpp
void AttachBeforeNavigate(const OnBeforeResourceLoadEvent& callback)
```

 - 参&emsp;数：  
    - `callback` 一个回调函数，参考 OnBeforeResourceLoadEvent 声明
 - 返回值：无

### AttachLinkClick

绑定一个回调函数用于监听一个弹出窗口弹出的通知

```cpp
void AttachLinkClick(const OnLinkClickEvent& callback)
```

 - 参&emsp;数：  
    - `callback` 一个回调函数，参考 OnLinkClickEvent 声明
 - 返回值：无

### AttachLoadingStateChange

绑定一个回调函数用于监听页面加载状态改变

```cpp
void AttachLoadingStateChange(const OnLoadingStateChangeEvent& callback)
```

 - 参&emsp;数：  
    - `callback` 一个回调函数，参考 OnLoadingStateChangeEvent 声明
 - 返回值：无

### AttachLoadStart

绑定一个回调函数用于监听页面开始加载通知

```cpp
void AttachLoadStart(const OnLoadStartEvent& callback)
```

 - 参&emsp;数：  
    - `callback` 一个回调函数，参考 OnLoadStartEvent 声明
 - 返回值：无

### AttachLoadEnd

绑定一个回调函数用于监听页面加载完毕通知

```cpp
void AttachLoadEnd(const OnLoadEndEvent& callback)
```

 - 参&emsp;数：  
    - `callback` 一个回调函数，参考 OnLoadEndEvent 声明
 - 返回值：无

### AttachLoadError

绑定一个回调函数用于监听界面加载错误通知

```cpp
void AttachLoadError(const OnLoadErrorEvent& callback)
```

 - 参&emsp;数：  
    - `callback` 一个回调函数，参考 OnLoadErrorEvent 声明
 - 返回值：无

### AttachDevToolAttachedStateChange

绑定一个回调函数用于监听开发者工具状态改变通知

```cpp
void AttachDevToolAttachedStateChange(const OnDevToolAttachedStateChangeEvent& callback)
```

 - 参&emsp;数：  
    - `callback` 一个回调函数，参考 OnDevToolAttachedStateChangeEvent 声明
 - 返回值：无
