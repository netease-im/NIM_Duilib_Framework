# Language （多国语言管理）

| 方法名称 | 用途 |
| :--- | :--- |
| [GetStringViaID](#GetStringViaID) | 根据ID获取指定语言的字符串 |
| [LoadStringTable](#LoadStringTable) | 加载所有语言映射表 |
| [LoadStringTable](#LoadStringTable) | 从内存中加载所有语言映射表 |

### GetStringViaID

根据ID获取指定语言的字符串

```cpp
std::wstring GetStringViaID(const std::wstring& id)
```

 - 参&emsp;数：  
    - `id` 指定字符串 ID
 - 返回值：返回 ID 对应的语言字符串

### LoadStringTable

加载所有语言映射表

```cpp
bool LoadStringTable(const std::wstring& strFilePath)
```

 - 参&emsp;数：  
    - `strFilePath` 语言文件路径
 - 返回值：返回 true 表示成功，返回 false 表示失败

### LoadStringTable

从内存中加载所有语言映射表

```cpp
bool LoadStringTable(const HGLOBAL& hGlobal)
```

 - 参&emsp;数：  
    - `hGlobal` 要加载的语言映射表内存首地址
 - 返回值：返回 true 表示成功，返回 false 表示失败

