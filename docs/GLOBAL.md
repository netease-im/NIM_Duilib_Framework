# Global（全局样式）

Global 全局样式提供了通用的样式列表，避免在多个不同的 XML 中出现相同的描述而产生冗余的代码和消耗开发人员在界面设上的时间。

在调用 GlobalManager::Startup 方法后，会根据设定的皮肤资源路径下查找 `global.xml` 作为全局的样式资源。在现有的 samples 示例代码中，
包含了一些预设的全局样式，如字体、颜色和一些通用样式。

## 字体（Font）

如果你想添加一个字体，则在 `global.xml` 中添加如下代码，程序启动后会加载所有字体列表到缓存中，以 ID 作为区分（注意，老版本没有 ID 属性，缓存时是根据字体的顺序依次编号放入内存中，一旦指定了 ID 属性，索引将不再生效！）。

```xml
<!-- name 代表字体名称，size 代表字体大小，bold 代表是否粗体，underline 代表是否包含下划线 -->
<Font id="system_12" name="system" size="10" bold="true" underline="true"/>
```

当需要使用时，指定字体的编号即可。比如你希望一个 Button 按钮使用 ID 为 system_12 的字体，可以这样写：

```xml
<Button text="Hello Button" font="system_12"/>
```

### Font 所有可用属性

| 属性名称 | 默认值 | 参数类型 | 用途 |
| :--- | :--- | :--- | :--- |
| id | | STRING | 字体ID（一旦使用 ID 属性，字体索引将不再生效） |
| default | false | STRING | 是否是默认字体，如果未给控件指定字体，则使用该字体 |
| name | | STRING | 字体在系统中的名称 |
| size | 12 | INT | 字体大小 |
| bold | false | BOOL | 是否粗体 |
| underline | false | BOOL | 是否下划线 |
| italic | false | BOOL | 是否斜体 |

## 颜色（TextColor）

你可以添加常用的颜色到 `global.xml` 中，如下所示：

```xml
<!-- name 是颜色的名称，value 是颜色的具体数值 -->
<TextColor name="textdefaultcolor" value="#ff333333"/>
```

这样当你需要使用这个颜色给一个 Label 设置文字颜色时，可以这样写：

```xml
<Label text="Hello Label" normaltextcolor="textdefaultcolor"/>
```

### TextColor 所有可用属性

| 属性名称 | 默认值 | 参数类型 | 用途 |
| :--- | :--- | :--- | :--- |
| name | | STRING | 颜色名称 |
| value | | STRING | 颜色值如 0xFFFFFFFF |

## 通用样式（Class）

通用样式可以让我们预设一些经常使用到的样式集合，比如一个高度为 34，宽度自动拉伸、使用 caption.png 作为背景的标题栏。
又或者一个宽度为 80，高度为 30 的通用样式按钮等等。我们都可以通过通用样式来解决。以下示例演示了一个通用样式按钮：

```xml
<!-- name 是通用样式的名称，value 是通用样式的具体样式值 -->
<Class name="btn_global_blue_80x30" value="font=&quot;7&quot; normaltextcolor=&quot;white&quot; normalimage=&quot;file='../public/button/btn_global_blue_80x30_normal.png'&quot; hotimage=&quot;file='../public/button/btn_global_blue_80x30_hovered.png'&quot; pushedimage=&quot;file='../public/button/btn_global_blue_80x30_pushed.png'&quot; disabledimage=&quot;file='../public/button/btn_global_blue_80x30_normal.png' fade='80'&quot;"/>
```

以上代码定义了一个按钮通用样式，命名为 `btn_global_blue_80x30`，使用字体编号为7，普通样式下字体颜色为 `white`，
并分别设置了普通状态、焦点状态和按下状态不同的背景图片，最后还启用了动画效果。当我们需要给一个按钮应用这个通用样式时，可以这样写：

```xml
<Button class="btn_global_blue_80x30" text="classic button"/>
```

要注意的是，`class` 属性必须在所有属性最前面，当你需要覆盖一个通用样式中指定过的属性时，只需要在 `class` 属性之后再重新定义这个属性就可以了。比如我希望我的按钮不使用通用样式的字体样色，可以这样写：

```xml
<Button class="btn_global_blue_80x30" font="2" text="classic button"/>
```

### Class 所有可用属性

| 属性名称 | 默认值 | 参数类型 | 用途 |
| :--- | :--- | :--- | :--- |
| name | | STRING | 通用样式名称 |
| value | | STRING | 通用样式值，必须经过XML转义 |

## 可用接口

| 方法名称 | 用途 |
| :--- | :--- |
| [Startup](#Startup) | 初始化全局设置函数 |
| [Shutdown](#Shutdown) | 释放资源 |
| [GetCurrentPath](#GetCurrentPath) | 获取当前程序所在目录 |
| [GetResourcePath](#GetResourcePath) | 获取当前资源所在目录 |
| [SetCurrentPath](#SetCurrentPath) | 设置程序当前目录 |
| [SetResourcePath](#SetResourcePath) | 设置皮肤资源所在目录 |
| [ReloadSkin](#ReloadSkin) | 重新加载皮肤资源 |
| [GetRenderFactory](#GetRenderFactory) | 获取绘制接口类对象 |
| [CreateRenderContext](#CreateRenderContext) | 创建全局的绘制上下文区域 |
| [CreatePen](#CreatePen) | 创建一个画笔 |
| [CreateBrush](#CreateBrush) | 创建一个画刷 |
| [CreateMatrix](#CreateMatrix) | 创建一个矩阵 |
| [CreatePath](#CreatePath) | 创建一个绘制路径 |
| [AddClass](#AddClass) | 添加一个全局 class 属性 |
| [GetClassAttributes](#GetClassAttributes) | 获取一个全局 class 属性的值 |
| [RemoveAllClasss](#RemoveAllClasss) | 从全局属性中删除所有 class 属性 |
| [AddTextColor](#AddTextColor) | 添加一个全局颜色值提供程序使用 |
| [GetTextColor](#GetTextColor) | 根据名称获取一个颜色的具体数值 |
| [RemoveAllTextColors](#RemoveAllTextColors) | 删除所有全局颜色属性 |
| [IsImageCached](#IsImageCached) | 检查指定图片是否已经被缓存 |
| [AddImageCached](#AddImageCached) | 添加一个图片到缓存中 |
| [RemoveFromImageCache](#RemoveFromImageCache) | 从缓存中一处一个图片 |
| [OnImageInfoDestroy](#OnImageInfoDestroy) | 图片被销毁的回调 |
| [GetImage](#GetImage) | 获取图片 ImageInfo 对象 |
| [RemoveAllImages](#RemoveAllImages) | 从缓存中删除所有图片 |
| [GetDefaultFontName](#GetDefaultFontName) | 获取默认字体名称 |
| [AddFont](#AddFont) | 添加一个字体 |
| [GetTFontInfo](#GetTFontInfo) | 根据索引返回一个字体信息 |
| [GetFont](#GetFont) | 根据索引返回一个字体对象 |
| [GetFont](#GetFont) | 根据字体属性获取字体对象 |
| [GetFontInfo](#GetFontInfo) | 获取字体信息 |
| [GetFontInfo](#GetFontInfo) | 获取字体信息 |
| [FindFont](#FindFont) | 根据字体对象查找指定字体是否存在 |
| [FindFont](#FindFont) | 根据字体信息查找字体是否存在 |
| [RemoveFontAt](#RemoveFontAt) | 根据字体索引删除字体 |
| [RemoveAllFonts](#RemoveAllFonts) | 删除所有字体 |
| [GetDefaultDisabledTextColor](#GetDefaultDisabledTextColor) | 获取默认禁用状态下字体颜色 |
| [SetDefaultDisabledTextColor](#SetDefaultDisabledTextColor) | 设置默认禁用状态下的字体颜色 |
| [GetDefaultTextColor](#GetDefaultTextColor) | 获取默认字体颜色 |
| [SetDefaultTextColor](#SetDefaultTextColor) | 设置默认字体颜色 |
| [GetDefaultLinkFontColor](#GetDefaultLinkFontColor) | 获取默认链接字体颜色 |
| [SetDefaultLinkFontColor](#SetDefaultLinkFontColor) | 设置默认链接字体颜色 |
| [GetDefaultLinkHoverFontColor](#GetDefaultLinkHoverFontColor) | 获取焦点链接的默认字体颜色 |
| [SetDefaultLinkHoverFontColor](#SetDefaultLinkHoverFontColor) | 设置焦点链接的默认字体颜色 |
| [GetDefaultSelectedBkColor](#GetDefaultSelectedBkColor) | 获取默认选择状态字体颜色 |
| [SetDefaultSelectedBkColor](#SetDefaultSelectedBkColor) | 设置默认选择状态的字体颜色 |
| [CreateBox](#CreateBox) | 根据 XML 创建一个 Box |
| [CreateBoxWithCache](#CreateBoxWithCache) | 根据 XML 在缓存中查找指定 Box，如果没有则创建 |
| [FillBox](#FillBox) | 使用 XML 填充指定 Box |
| [FillBoxWithCache](#FillBoxWithCache) | 使用构建过的缓存填充指定 Box，如果没有则重新构建 |
| [CreateControl](#CreateControl) | 自定义控件创建后的全局回调函数 |
| [IsUseZip](#IsUseZip) | 判断当前是否使用了 zip 压缩包 |
| [OpenResZip](#OpenResZip) | 打开一个内存压缩包资源 |
| [OpenResZip](#OpenResZip) | 打开一个本地文件压缩包资源 |
| [GetData](#GetData) | 获取压缩包中的内容到内存 |
| [GetZipFilePath](#GetZipFilePath) | 获取文件在压缩包中的位置 |
| [LoadGlobalResource](#LoadGlobalResource) | 加载全局资源 |


### Startup

初始化全局设置函数

```cpp
static void Startup(const std::wstring& strResourcePath, const CreateControlCallback& callback, bool bAdaptDpi, const std::wstring& theme = L"themes\\default", const std::wstring& language = L"lang\\zh_CN")
```

 - 参&emsp;数：  
    - `strResourcePath` 资源路径位置
    - `callback` 创建自定义控件时的全局回调函数
    - `bAdaptDpi` 是否启用 DPI 适配
    - `theme` 主题目录名，默认为 themes\\default
    - `language` 使用语言，默认为 lang\\zh_CN
 - 返回值：无

### Shutdown

释放资源

```cpp
static void Shutdown()
```

 - 参&emsp;数：无  
 - 返回值：无

### GetCurrentPath

获取当前程序所在目录

```cpp
static std::wstring GetCurrentPath()
```

 - 参&emsp;数：无  
 - 返回值：返回当前程序所在目录

### GetResourcePath

获取当前资源所在目录

```cpp
static std::wstring GetResourcePath()
```

 - 参&emsp;数：无  
 - 返回值：返回当前资源所在目录

### SetCurrentPath

设置程序当前目录

```cpp
static void SetCurrentPath(const std::wstring& strPath)
```

 - 参&emsp;数：  
    - `strPath` 要设置的路径
 - 返回值：无

### SetResourcePath

设置皮肤资源所在目录

```cpp
static void SetResourcePath(const std::wstring& strPath)
```

 - 参&emsp;数：  
    - `strPath` 要设置的路径
 - 返回值：无

### ReloadSkin

重新加载皮肤资源

```cpp
static void ReloadSkin(const std::wstring& resourcePath)
```

 - 参&emsp;数：  
    - `resourcePath` 资源路径
 - 返回值：无

### GetRenderFactory

获取绘制接口类对象

```cpp
static IRenderFactory* GetRenderFactory()
```

 - 参&emsp;数：无  
 - 返回值：返回接口类对象指针

### CreateRenderContext

创建全局的绘制上下文区域

```cpp
static std::unique_ptr<IRenderContext> CreateRenderContext()
```

 - 参&emsp;数：无  
 - 返回值：返回绘制区域对象

### CreatePen

创建一个画笔

```cpp
static std::unique_ptr<IPen> CreatePen(DWORD color, int width = 1)
```

 - 参&emsp;数：  
    - `color` 画笔颜色
    - `width` 画笔宽度
 - 返回值：返回画笔对象

### CreateBrush

创建一个画刷

```cpp
static std::unique_ptr<IBrush> CreateBrush(DWORD color)
```

 - 参&emsp;数：  
    - `color` 画刷颜色
 - 返回值：返回画刷对象

### CreateMatrix

创建一个矩阵

```cpp
static std::unique_ptr<IMatrix> CreateMatrix()
```

 - 参&emsp;数：无  
 - 返回值：返回矩阵对象

### CreatePath

创建一个绘制路径

```cpp
static std::unique_ptr<IPath> CreatePath()
```

 - 参&emsp;数：无  
 - 返回值：返回绘制路径对象

### AddClass

添加一个全局 class 属性

```cpp
static void AddClass(const std::wstring& strClassName, const std::wstring& strControlAttrList)
```

 - 参&emsp;数：  
    - `strClassName` 全局 class 名称
    - `strControlAttrList` 属性列表，需要做 XML 转义
 - 返回值：无

### GetClassAttributes

获取一个全局 class 属性的值

```cpp
static std::wstring GetClassAttributes(const std::wstring& strClassName)
```

 - 参&emsp;数：  
    - `strClassName` 全局 class 名称
 - 返回值：返回字符串形式的 class 属性值

### RemoveAllClasss

从全局属性中删除所有 class 属性

```cpp
static void RemoveAllClasss()
```

 - 参&emsp;数：无  
 - 返回值：返回绘制区域对象

### AddTextColor

添加一个全局颜色值提供程序使用

```cpp
static void AddTextColor(const std::wstring& strName, const std::wstring& strValue)
```

 - 参&emsp;数：  
    - `strName` 颜色名称（如 white）
    - `strValue` 颜色具体数值（如 #FFFFFFFF）
 - 返回值：无

### GetTextColor

根据名称获取一个颜色的具体数值

```cpp
static DWORD GetTextColor(const std::wstring& strName)
```

 - 参&emsp;数：  
    - `strName` 要获取的颜色名称
 - 返回值：返回 DWORD 格式的颜色描述值

### RemoveAllTextColors

删除所有全局颜色属性

```cpp
static void RemoveAllTextColors()
```

 - 参&emsp;数：无  
 - 返回值：无

### IsImageCached

检查指定图片是否已经被缓存

```cpp
static std::shared_ptr<ImageInfo> IsImageCached(const std::wstring& bitmap)
```

 - 参&emsp;数：  
    - `bitmap` 图片路径
 - 返回值：如果已经被缓存，则返回 ImageInfo 的智能指针对象

### AddImageCached

添加一个图片到缓存中

```cpp
static std::shared_ptr<ImageInfo> AddImageCached(const std::shared_ptr<ImageInfo>& shared_image)
```

 - 参&emsp;数：  
    - `shared_image
 - 返回值：返回被缓存的 ImageInfo 智能指针对象

### RemoveFromImageCache

从缓存中一处一个图片

```cpp
static void RemoveFromImageCache(const std::wstring& imageFullPath)
```

 - 参&emsp;数：  
    - `imageFullPath` 图片路径
 - 返回值：无

### OnImageInfoDestroy

图片被销毁的回调

```cpp
static void OnImageInfoDestroy(ImageInfo *pImageInfo)
```

 - 参&emsp;数：  
    - `pImageInfo` 图片对应的 ImageInfo 对象
 - 返回值：无

### GetImage

获取图片 ImageInfo 对象

```cpp
static std::shared_ptr<ImageInfo> GetImage(const std::wstring& bitmap)
```

 - 参&emsp;数：  
    - `bitmap` 图片路径
 - 返回值：返回图片 ImageInfo 对象的智能指针

### RemoveAllImages

从缓存中删除所有图片

```cpp
static void RemoveAllImages()
```

 - 参&emsp;数：无  
 - 返回值：无

### GetDefaultFontName

获取默认字体名称

```cpp
static std::wstring GetDefaultFontName()
```

 - 参&emsp;数：无  
 - 返回值：字体名称

### AddFont

添加一个字体

```cpp
static HFONT AddFont(const std::wstring& strFontName, int nSize, bool bBold, bool bUnderline, bool bItalic)
```

 - 参&emsp;数：  
    - `strFontName` 字体名称
    - `nSize` 字体大小
    - `bBold` 是否粗体
    - `bUnderline` 是否有下划线
    - `bItalic` 是否倾斜
 - 返回值：返回字体的 HFONT 句柄

### GetTFontInfo

根据索引返回一个字体信息

```cpp
static TFontInfo* GetTFontInfo(std::size_t index)
```

 - 参&emsp;数：  
    - `index` 字体索引
 - 返回值：返回字体的 TFontInfo 信息

### GetFont

根据索引返回一个字体对象

```cpp
static HFONT GetFont(std::size_t index)
```

 - 参&emsp;数：  
    - `index` 字体索引
 - 返回值：返回字体的 HFONT 句柄

### GetFont

根据字体属性获取字体对象

```cpp
static HFONT GetFont(const std::wstring& strFontName, int nSize, bool bBold, bool bUnderline, bool bItalic)
```

 - 参&emsp;数：  
    - `strFontName` 字体名称
    - `nSize` 字体大小
    - `bBold` 是否粗体
    - `bUnderline` 是否有下划线
    - `bItalic` 是否倾斜
 - 返回值：返回字体的 HFONT 句柄

### GetFontInfo

获取字体信息

```cpp
static TFontInfo* GetFontInfo(std::size_t index, HDC hDcPaint)
```

 - 参&emsp;数：  
    - `index` 字体索引
    - `hDcPaint` 设备句柄
 - 返回值：返回字体的 TFontInfo 信息

### GetFontInfo

获取字体信息

```cpp
static TFontInfo* GetFontInfo(HFONT hFont, HDC hDcPaint)
```

 - 参&emsp;数：  
    - `hFont` 字体对象句柄
    - `hDcPaint` 设备句柄
 - 返回值：返回字体的 TFontInfo 信息

### FindFont

根据字体对象查找指定字体是否存在

```cpp
static bool FindFont(HFONT hFont)
```

 - 参&emsp;数：  
    - `hFont` 字体对象句柄
 - 返回值：返回是否存在

### FindFont

根据字体信息查找字体是否存在

```cpp
static bool FindFont(const std::wstring& strFontName, int nSize, bool bBold, bool bUnderline, bool bItalic)
```

 - 参&emsp;数：  
    - `strFontName` 字体名称
    - `nSize` 字体大小
    - `bBold` 是否粗体
    - `bUnderline` 是否有下划线
    - `bItalic` 是否倾斜
 - 返回值：返回是否存在

### RemoveFontAt

根据字体索引删除字体

```cpp
static bool RemoveFontAt(std::size_t index)
```

 - 参&emsp;数：  
    - `index` 字体索引
 - 返回值：返回是否删除成功

### RemoveAllFonts

删除所有字体

```cpp
static void RemoveAllFonts()
```

 - 参&emsp;数：无  
 - 返回值：无

### GetDefaultDisabledTextColor

获取默认禁用状态下字体颜色

```cpp
static std::wstring GetDefaultDisabledTextColor()
```

 - 参&emsp;数：无  
 - 返回值：默认禁用状态颜色的字符串表示，对应 global.xml 中指定颜色值

### SetDefaultDisabledTextColor

设置默认禁用状态下的字体颜色

```cpp
static void SetDefaultDisabledTextColor(const std::wstring& strColor)
```

 - 参&emsp;数：  
    - `dwColor` 字体颜色字符串值（如 white）
 - 返回值：无

### GetDefaultTextColor

获取默认字体颜色

```cpp
static std::wstring GetDefaultTextColor()
```

 - 参&emsp;数：无  
 - 返回值：默认字体颜色字符串表示，对应 global.xml 中指定颜色值

### SetDefaultTextColor

设置默认字体颜色

```cpp
static void SetDefaultTextColor(const std::wstring& strColor)
```

 - 参&emsp;数：  
    - `字体颜色的字符串值，对应` global.xml 中指定颜色值
 - 返回值：无

### GetDefaultLinkFontColor

获取默认链接字体颜色

```cpp
static DWORD GetDefaultLinkFontColor()
```

 - 参&emsp;数：无  
 - 返回值：链接字体颜色 DWORD 格式的色值

### SetDefaultLinkFontColor

设置默认链接字体颜色

```cpp
static void SetDefaultLinkFontColor(DWORD dwColor)
```

 - 参&emsp;数：  
    - `dwColor` 默认链接字体颜色
 - 返回值：无

### GetDefaultLinkHoverFontColor

获取焦点链接的默认字体颜色

```cpp
static DWORD GetDefaultLinkHoverFontColor()
```

 - 参&emsp;数：无  
 - 返回值：返回焦点链接的默认字体颜色

### SetDefaultLinkHoverFontColor

设置焦点链接的默认字体颜色

```cpp
static void SetDefaultLinkHoverFontColor(DWORD dwColor)
```

 - 参&emsp;数：  
    - `dwColor` 焦点链接的默认字体颜色
 - 返回值：无

### GetDefaultSelectedBkColor

获取默认选择状态字体颜色

```cpp
static DWORD GetDefaultSelectedBkColor()
```

 - 参&emsp;数：无  
 - 返回值：返回默认选择状态的字体颜色

### SetDefaultSelectedBkColor

设置默认选择状态的字体颜色

```cpp
static void SetDefaultSelectedBkColor(DWORD dwColor)
```

 - 参&emsp;数：  
    - `dwColor` 字体颜色值
 - 返回值：无

### CreateBox

根据 XML 创建一个 Box

```cpp
static Box* CreateBox(const std::wstring& strXmlPath, CreateControlCallback callback = CreateControlCallback()
```

 - 参&emsp;数：  
    - `strXmlPath` XML 文件路径
    - `callback` 自定义控件的回调处理函数
 - 返回值：指定布局模块的对象指针

### CreateBoxWithCache

根据 XML 在缓存中查找指定 Box，如果没有则创建

```cpp
static Box* CreateBoxWithCache(const std::wstring& strXmlPath, CreateControlCallback callback = CreateControlCallback()
```

 - 参&emsp;数：  
    - `strXmlPath` XML 文件路径
    - `callback` 自定义控件的回调处理函数
 - 返回值：指定布局模块的对象指针

### FillBox

使用 XML 填充指定 Box

```cpp
static void FillBox(Box* pUserDefinedBox, const std::wstring& strXmlPath, CreateControlCallback callback = CreateControlCallback()
```

 - 参&emsp;数：  
    - `pUserDefinedBox` 要填充的 box 指针
    - `strXmlPath` XML 文件路径
    - `callback` 自定义控件的回调处理函数
 - 返回值：无

### FillBoxWithCache

使用构建过的缓存填充指定 Box，如果没有则重新构建

```cpp
static void FillBoxWithCache(Box* pUserDefinedBox, const std::wstring& strXmlPath, CreateControlCallback callback = CreateControlCallback()
```

 - 参&emsp;数：  
    - `pUserDefinedBox` 要填充的 box 指针
    - `strXmlPath` XML 文件路径
    - `callback` 自定义控件的回调处理函数
 - 返回值：无

### CreateControl

自定义控件创建后的全局回调函数

```cpp
static Control* CreateControl(const std::wstring& strControlName)
```

 - 参&emsp;数：  
    - `strControlName` 自定义控件名称
 - 返回值：返回一个自定义控件的对象指针

### IsUseZip

判断当前是否使用了 zip 压缩包

```cpp
static bool IsUseZip()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 表示使用了 zip 压缩包作为资源，false 为普通目录模式

### OpenResZip

打开一个内存压缩包资源

```cpp
static bool OpenResZip(LPCTSTR  resource_name, LPCTSTR  resource_type, const std::string& password)
```

 - 参&emsp;数：  
    - `resource_name` 资源名称，由 MAKEINTRESOURCE 宏生成
    - `resource_type` 资源类型，自定义导入的资源类型名称
    - `password` 压缩包密码
 - 返回值：返回 true 打开成功，返回 false 为打开失败

### OpenResZip

打开一个本地文件压缩包资源

```cpp
static bool OpenResZip(const std::wstring& path, const std::string& password)
```

 - 参&emsp;数：  
    - `path` 压缩包文件路径
    - `password` 压缩包密码
 - 返回值：返回 true 打开成功，返回 false 为打开失败

### GetData

获取压缩包中的内容到内存

```cpp
static HGLOBAL GetData(const std::wstring& path)
```

 - 参&emsp;数：  
    - `path` 要获取的文件的路径
 - 返回值：返回文件的内存地址

### GetZipFilePath

获取文件在压缩包中的位置

```cpp
static std::wstring GetZipFilePath(const std::wstring& path)
```

 - 参&emsp;数：  
    - `path` 要获取的文件路径
 - 返回值：返回在压缩包中的文件位置

### LoadGlobalResource

加载全局资源

```cpp
static void LoadGlobalResource()
```

 - 参&emsp;数：无  
 - 返回值：无
