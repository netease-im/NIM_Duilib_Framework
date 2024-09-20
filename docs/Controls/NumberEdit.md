# NumberEdit（数字文本框控件）

`NumberEdit` 继承了 `RichEdit` 的属性和方法，更多可用属性和方法请参考：[RichEdit](RichEdit.md) 控件

## 可用属性

| 属性名称 | 默认值 | 参数类型 | 用途 |
| :--- | :--- | :--- | :--- |
| intonly | false | BOOL | 是否仅支持输入整数,如(true) |
| allownegative | false | BOOL | 是否允许输入负数,如(true) |
| maxvalueenabled | false | BOOL | 是否开启最大值控制,如(true) |
| minvalueenabled | false | BOOL | 是否开启最小值控制,如(true) |
| maxvalue | 0 | INT | 允许输入的最大值 |
| minvalue | 0 | INT | 允许输入的最小值 |

## 可用接口

| 方法名称 | 用途 |
| :--- | :--- |
| [IsIntOnly](#IsIntOnly) | 判断是否仅支持输入整数 |
| [SetIntOnly](#SetIntOnly) | 设置是否仅支持输入整数 |
| [IsAllowNegative](#IsAllowNegative) | 判断是否允许输入负数 |
| [SetAllowNegative](#SetAllowNegative) | 设置是否允许输入负数 |
| [GetIntValue](#GetIntValue) | 获取控件中的整数 |
| [GetValue](#GetValue) | 获取控件中的实数 |
| [SetValue](#SetValue) | 设置控件的数值(整数) |
| [SetValue](#SetValue-1) | 设置控件的数值(实数) |
| [IsMaxValueEnabled](#IsMaxValueEnabled) | 判断是否开启最大值控制 |
| [SetMaxValueEnabled](#SetMaxValueEnabled) | 设置是否是否开启最大值控制 |
| [IsMinValueEnabled](#IsMinValueEnabled) | 判断是否开启最小值控制 |
| [SetMinValueEnabled](#SetMinValueEnabled) | 设置是否是否开启最小值控制 |
| [GetMaxValue](#GetMaxValue) | 获取可输入的最大值 |
| [SetMaxValue](#SetMaxValue) | 设置可输入的最大值 |
| [GetMinValue](#GetMinValue) | 获取可输入的最小值 |
| [SetMinValue](#SetMinValue) | 设置可输入的最小值 |

### IsIntOnly

判断是否仅支持输入整数

```cpp
bool IsIntOnly()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 表示只能输入整数，false 表示可以整数也可以实数

### SetIntOnly

设置是否仅支持输入整数

```cpp
void SetIntOnly(bool bIntOnly = true)
```

 - 参&emsp;数：  
    - `bIntOnly` 为 true 则限制只能输入整数，false 为不限制
 - 返回值：无

### IsAllowNegative

判断是否允许输入负数

```cpp
bool IsAllowNegative()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 表示允许输入负数数值，false 表示只能是正数

### SetAllowNegative

设置是否允许输入负数

```cpp
void SetAllowNegative(bool bAllowNegative = true)
```

 - 参&emsp;数：  
    - `bAllowNegative` 为 true 则允许进行负数输入，false 为不允许
 - 返回值：无

### GetIntValue

获取控件中的整数

```cpp
int GetIntValue(int def = 0)
```

 - 参&emsp;数：  
    - `def` 取值失败时返回的缺省值
 - 返回值：返回控件中的整数

### GetValue

获取控件中的实数

```cpp
double GetValue(double def = 0)
```

 - 参&emsp;数：  
    - `def` 取值失败时返回的缺省值
 - 返回值：返回控件中的实数

### SetValue 

设置控件的数值(整数)

```cpp
void SetValue(int value)
```

 - 参&emsp;数：  
    - `value` 要设置的整数值
 - 返回值：无

### SetValue 

设置控件的数值(实数)

```cpp
void SetValue(double value)
```

 - 参&emsp;数：  
    - `value` 要设置的实数值
 - 返回值：无
 
### IsMaxValueEnabled

判断是否开启最大值控制

```cpp
bool IsMaxValueEnabled()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 表示开启
 
### SetMaxValueEnabled

设置是否是否开启最大值控制

```cpp
void SetMaxValueEnabled(bool bMaxValueEnabled = false)
```

 - 参&emsp;数：  
    - `bMaxValueEnabled` 为 true 开启
 - 返回值：无
  
### IsMinValueEnabled

判断是否开启最小值控制

```cpp
bool IsMinValueEnabled()
```

 - 参&emsp;数：无  
 - 返回值：返回 true 表示开启
 
### SetMinValueEnabled

设置是否是否开启最小值控制

```cpp
void SetMinValueEnabled(bool bMinValueEnabled = false)
```

 - 参&emsp;数：  
    - `bMinValueEnabled` 为 true 开启
 - 返回值：无
 
### GetMaxValue

获取可输入的最大值

```cpp
int GetMaxValue()
```

 - 参&emsp;数：无  
 - 返回值：返回可输入的最大值

### SetMaxValue

设置可输入的最大值

```cpp
void SetMaxValue(int value)
```

 - 参&emsp;数：  
    - `value` 要设置的最大可输入值
 - 返回值：无
 
### GetMinValue

获取可输入的最小值

```cpp
int GetMinValue()
```

 - 参&emsp;数：无  
 - 返回值：返回可输入的最小值

### SetMinValue

设置可输入的最小值

```cpp
void SetMinValue(int value)
```

 - 参&emsp;数：  
    - `value` 要设置的最小可输入值
 - 返回值：无
