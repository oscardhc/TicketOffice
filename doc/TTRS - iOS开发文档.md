## TTRS - iOS开发文档

### 简介

开发环境：macOS 10.14 + Xcode 10

测试环境：macOS 10.14 + Xcode 10

开发技术：Swift + Cocoa Touch + 各种各样的库

开发人员：董海辰，吴怀瑾

###文件结构

- AboutUsViewController.swift
	
	「关于」视图
	
- Assets.xcassets

  使用到的图片等资源

- Base.lproj

  工程信息

- Fonts

  自定义字体

- AlertControl.swift

  自定义警告弹窗视图

- AppDelegate.swift

  程序入口

- BuyViewController.swift

  「购票」视图

- ConfirmViewController.swift

  「确认购票」视图

- DateFormat.swift

  日期与数字相互转化工具类

- DemoCell.swift

  「查询」页面中主视图表格的单元视图

- LaunchViewController.swift

  初始化界面视图

- LoginViewController.swift

  「登录」视图

- MainTableViewController.swift

  「查询」页面主视图

- ManageTrainViewController.swift

  「管理火车」视图

- ManageUserViewController.swift

  「管理用户」视图

- ManageViewController.swift

  「管理」页面主视图

- NetworkManager.swift

  网络通信工具类，应作为单例使用：`NetworkManager.default.xxxxx()`

- OrderViewController.swift

  「订单」视图

- RecordView.swift

  「订单」列表中单位视图

- RefundViewController.swift

  「退票」视图

- RegisterViewController.swift

  「注册」视图

- TBViewController.swift

  整个程序Tab导航栏视图

- UserViewController.swift

  「用户」视图

- Info.plist

  程序信息

- tkk.gif

  加载界面动图，不可描述

### 一些实现细节

- 以卡片式界面为主要设计思想，其中融合了部分Material风格和iOS UI Guideline思想，通过阴影实现不同层次，以体现空间之间的关系，富有设计感。
- 主要使用的第三方库为`FoldingCell`和`Card`。
- `NetworkManager`中提供同步请求`postS`和异步请求`postA`，在实际使用中主要使用异步请求。
- 在`order`界面中使用了自制封装过的`UIView`子类`orderView`，可以更方便而规范批量处理空间。
- 在UI / UX设计中严格遵守Human Interface Guideline, 对每一个用户操作均有明确而及时的反馈，所有界面的转换皆有得体的动画过渡。

### 其他

- 感觉不同熟练程度对于开发的效率影响还是蛮大的，我也还在摸索的路上。
- Swift大法好

