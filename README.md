# NIM Duilib

NIM Duilib 包含了一整套桌面软件的开发部件，与其说这是一个界面库，不如说它是一个开发框架。从原有 Duilib 基础上拓展了一些新的能力，并整合了 Google base 基础类库（线程、内存、闭包等）。您完全可以直接使用这个框架来开发任何你想要的桌面应用。

![GitHub](https://img.shields.io/badge/license-MIT-green.svg)
[![Build status](https://ci.appveyor.com/api/projects/status/u29yl0j7pasopm3h?svg=true)](https://ci.appveyor.com/project/nmgwddj/nim-duilib-framework) [![Join the chat at https://gitter.im/NIM_Duilib_Framework/community](https://badges.gitter.im/NIM_Duilib_Framework/community.svg)](https://gitter.im/NIM_Duilib_Framework/community?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge&utm_content=badge)

[English](README_en-US.md)

## 特色

 - 多国语言支持
 - 通用样式支持
 - DPI 缩放支持
 - GIF 动画支持
 - CEF 控件支持（CEF 2623 支持 XP）
 - 触控设备支持（Surface、Wacom）
 - 抽象渲染接口（为其他渲染引擎提供支持）

## 预览

![preview](docs/PREVIEW.gif)

## 开始

克隆项目到你的磁盘中

```bash
git clone https://github.com/netease-im/NIM_Duilib_Framework
```

进入 `NIM_Duilib_Framework/examples` 目录，使用 Visual Studio 2013 Update 5 以上版本 IDE 打开 `examples.sln`，按下 F7 即可编译所有示例程序

## 文档

 - [快速上手](docs/GETTING-STARTED.md)
 - [中文文档](docs/SUMMARY.md)

## 目录

├─`base` 基础类库  
├─`bin` 各个示例程序输出目录，包含预设的皮肤和语言文件以及 CEF 依赖  
├─`docs` duilib 接口的说明文档  
├─`duilib` duilib 核心代码，依赖 base 但不依赖 shared  
├─`libs` 静态库编译后的输出目录，包含预设的一些 CEF 组件静态库  
├─`examples` 各类示例程序源代码  
├─`third_party` 第三方库，目前仅有 cef_control 有依赖  
├─`ui_components` 基于 duilib 封装的常用组件库如 `msgbox`、`toast`、`cef_control` 等  

## 交流

 - 遇到问题：欢迎查看我们整理过的[中文文档](docs/SUMMARY.md)参考[示例程序](examples/README.md)来帮助你解决疑惑
 - 提交缺陷：在确保使用最新版本依然存在问题时请尽量以简洁的语言描述清楚复现该问题的步骤并提交 issue
 - 功能建议：如果你有什么好的想法或者提案，欢迎提交 issue 与我们交流

#推荐好友使用云信，即享1500元京东卡奖励# 点击参与https://yunxin.163.com/promotion/recommend?channel=github
[![main]( https://yx-web-nosdn.netease.im/quickhtml%2Fassets%2Fyunxin%2Fdefault%2F2022%2Fgithub%2F233.png)](https://yunxin.163.com/promotion/recommend?channel=github)
