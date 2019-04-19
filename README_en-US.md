# NIM Duilib

NIM Duilib is a UI development framework for Windows, extend some new features based on the original [Duilib](https://github.com/duilib/duilib). Integrated Google base library with threads/memory/closures. You can use this framework to develop any desktop application you want.

![GitHub](https://img.shields.io/badge/license-MIT-green.svg)
![Build status](https://ci.appveyor.com/api/projects/status/u29yl0j7pasopm3h/branch/master?svg=true)

[中文介绍](README.md)

## Features

 - Multi language support
 - Global style support
 - DPI scaling support
 - GIF animation support
 - CEF control support（based on CEF 2623, support XP）
 - Touch device support（surface, wacom）
 - Abstract rendering interface (support for other rendering engines)

## Preview

![preview](docs/PREVIEW.png)

## Start

Clone the repo into your disk.

```bash
git clone https://github.com/netease-im/NIM_Duilib_Framework
```

Enter the `NIM_Duilib_Framework/samples` directory，Open `samples.sln` with Visual Studio 2013 Update 5 or higher ，press `F7` to compile all projects.

## Documentation

 - [Docs](docs/SUMMARY.md)
    
## Structure

├─`bin` Samples output directories，include skin and language files and CEF dependencies.  
├─`docs` Documentation of Duilib.  
├─`libs` Static library output directories, include CEF static library.  
├─`samples` Samples of Duilib.  
├─`third_party` Third party for CEF.  
├─`toolkits`       
│  ├─`base` Base libraries.  
│  ├─`duilib` Duilib core, depend on base project.  
│  ├─`shared` Utils, such as logs/string/path utils.  
│  ├─`ui_components` UI components based on Duilib, such as `msgbox`,`toast`,`cef_control`  

## Communication

 - Question: Read the [Documentation](docs/SUMMARY.md) and [Samples code](samples/README.md) to help you.
 - Report: File a bug in GitHub issues
 - Suggestion: Request a new feature in Github issues.
