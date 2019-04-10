# DpiManager（DPI 管理）

| 方法名称 | 用途 |
| :--- | :--- |
| [GetSystemDPI](#GetSystemDPI) | 获取系统DPI，开启DPI感知后有效 |
| [GetMonitorDPI](#GetMonitorDPI) | 获取某个显示器的DPI，开启DPI感知后有效 |
| [GetMainMonitorDPI](#GetMainMonitorDPI) | 获取主显示器DPI，开启DPI感知后有效 |
| [GetDPIFromRegistry](#GetDPIFromRegistry) | 从注册表获取DPI，不开启DPI感知也有效 |
| [IsAdaptDPI](#IsAdaptDPI) | 是否程序自己适配DPI缩放效果 |
| [SetAdaptDPI](#SetAdaptDPI) | 设置是否程序自己适配DPI缩放，默认不自己适配 |
| [GetScale](#GetScale) | 获取当前界面缩放比 |
| [SetScale](#SetScale) | 根据DPI值设置界面缩放比，只有程序自己处理DPI缩放时才有效 |
| [ScaleInt](#ScaleInt) | 根据界面缩放比来缩放整数，只有程序自己处理DPI缩放时才有效 |
| [ScaleSize](#ScaleSize) | 根据界面缩放比来缩放SIZE，只有程序自己处理DPI缩放时才有效 |
| [ScalePoint](#ScalePoint) | 根据界面缩放比来缩放POINT，只有程序自己处理DPI缩放时才有效 |
| [ScaleRect](#ScaleRect) | 根据界面缩放比来缩放RECT，只有程序自己处理DPI缩放时才有效 |

### GetSystemDPI

获取系统DPI，开启DPI感知后有效

```cpp
static UINT GetSystemDPI()
```

 - 参&emsp;数：无  
 - 返回值：UINT DPI

### GetMonitorDPI

获取某个显示器的DPI，开启DPI感知后有效

```cpp
static UINT GetMonitorDPI(HMONITOR hMonitor)
```

 - 参&emsp;数：  
    - `HMONITOR句柄
 - 返回值：UINT DPI

### GetMainMonitorDPI

获取主显示器DPI，开启DPI感知后有效

```cpp
static UINT GetMainMonitorDPI()
```

 - 参&emsp;数：无  
 - 返回值：UINT DPI

### GetDPIFromRegistry

从注册表获取DPI，不开启DPI感知也有效

```cpp
static UINT GetDPIFromRegistry()
```

 - 参&emsp;数：无  
 - 返回值：UINT DPI

### IsAdaptDPI

是否程序自己适配DPI缩放效果

```cpp
bool IsAdaptDPI()
```

 - 参&emsp;数：无  
 - 返回值：bool true 是，false 否

### SetAdaptDPI

设置是否程序自己适配DPI缩放，默认不自己适配

```cpp
bool SetAdaptDPI()
```

 - 参&emsp;数：无  
 - 返回值：bool true 设置成功，false 设置失败

### GetScale

获取当前界面缩放比

```cpp
UINT GetScale()
```

 - 参&emsp;数：无  
 - 返回值：UINT	缩放比

### SetScale

根据DPI值设置界面缩放比，只有程序自己处理DPI缩放时才有效

```cpp
void SetScale(UINT uDPI)
```

 - 参&emsp;数：  
    - `uDPI` DPI值
 - 返回值：void	无返回值

### ScaleInt

根据界面缩放比来缩放整数，只有程序自己处理DPI缩放时才有效

```cpp
int ScaleInt(int &iValue)
```

 - 参&emsp;数：  
    - `iValue` 整数
 - 返回值：int	缩放后的值

### ScaleSize

根据界面缩放比来缩放SIZE，只有程序自己处理DPI缩放时才有效

```cpp
void ScaleSize(SIZE &size)
```

 - 参&emsp;数：  
    - `pSize` SIZE指针
 - 返回值：void	无返回值

### ScalePoint

根据界面缩放比来缩放POINT，只有程序自己处理DPI缩放时才有效

```cpp
void ScalePoint(POINT &point)
```

 - 参&emsp;数：  
    - `pSize` SIZE指针
 - 返回值：void	无返回值

### ScaleRect

根据界面缩放比来缩放RECT，只有程序自己处理DPI缩放时才有效

```cpp
void ScaleRect(RECT &rect)
```

 - 参&emsp;数：  
    - `pSize` SIZE指针
 - 返回值：void	无返回值

