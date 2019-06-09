## TTRS - web开发文档

[TOC]

### 简介

开发环境：macOS 10.14 / macOS 10.15 + PyCharm

测试环境：Ubuntu 16.04

开发技术：(python) + Flask + (html + css + javascript) + MDUI + 其他各式各样的库

开发人员：董海辰

###文件结构

- `main.py` / `main.wsgi`

  Flask服务器主文件

- `static`

  Flask储存静态文件（javascript, css 和 字体文件等）的目录

- `template`

	Flask储存页面文件（html）的目录

- `interaction.py`

	前后端交互库，沟通`c++`后端和`python`服务器

### web具体内容

- `/index`

	主页

- `/query`

	车票查询购买页面

- `/manage`
	
	管理页面
	
- `/order`

	订单页面

- `/account`

	账户信息页面
	
- `/exec` (仅支持POST)

	作为api实现前后端交互
	
### 一些实现细节

- 整个UI遵从 Material 设计风格，并加以定制。

- 利用 Jinja2 模板渲染，`index.html`、`query.html`等页面均继承于`base.html`文件，同时通过 Jinja2 中的变量来控制显示内容，例如导航栏中哪一栏改显示为被激活状态。
- 所有请求均使用 ajax 异步请求，不会阻塞前端。
- 使用了 cookie 来维护登录会话，渲染时将 cookie 作为参数传入 Jinja2 以控制渲染内容（如登录与否的界面差异）。
- 所有动态内容大多使用 JQuery 更新，如查询结果的表格等。

### 其他

- 强烈安利MDUI作为前端风格库，文档丰富且十分轻量级，魔改也很方便。
- 自建了 live2d 看板娘的api，专供本站访问。
- <del>我觉得这网站做的还挺好看的。</del>

