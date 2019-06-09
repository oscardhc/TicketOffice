## TTRS - 整体架构

[TOC]

### 架构

![structure](assets/structure.jpg)

通过Flask框架建立前后端分离的网站，并部署至Apache2服务器上，支持且仅支持Https访问，可极大提升安全性。Flask既提供web网页的访问，也提供api，供web前端和iOS前端与后端逻辑与数据库进行json交互。

### 部署方法

#### 环境依赖

- g++，要求支持`-std=c++-17`
- Python3
- flask框架
- Apache2

#### 安装步骤

- Clone 整个仓库至本地

- 配置Apache2生产环境：

  - 在`/etc/apache2/sites-available`目录中创建`ttrs.conf`

    参考内容如下：(请根据实际情况加以修改)

    ```
    <VirtualHost *:443>
    	ErrorLog /home/oscar/dhc/error.log
    	SSLEngine on
    	SSLCertificateFile /home/oscar/.acme.sh/dhc.moe/dhc.moe.cer
    	SSLCertificateKeyFile /home/oscar/.acme.sh/dhc.moe/dhc.moe.key
    	SSLCertificateChainFile /home/oscar/.acme.sh/dhc.moe/fullchain.cer
    	ServerName ttrs.dhc.moe
    	WSGIDaemonProcess project user=oscar threads=5
    	WSGIScriptAlias / /home/oscar/dhc/TicketOffice/web/project/main.wsgi
    	<Directory /home/oscar/dhc/TicketOffice/web/project/>
    		WSGIProcessGroup project
    		WSGIApplicationGroup %{GLOBAL}
    		WSGIScriptReloading On
    		Require all granted
    	</Directory>
    	Alias /static /home/oscar/dhc/TicketOffice/web/project/static
    	<Directory /home/oscar/dhc/TicketOffice/web/project/static/>
    		Order allow,deny
    		Allow from all
    		Require all granted
    	</Directory>
    	Alias /templates /home/oscar/dhc/TicketOffice/web/project/templates
    	<Directory /home/oscar/dhc/TicketOffice/web/project/templates/>
    		Order allow,deny
    		Allow from all
    		Require all granted
    	</Directory>
    </VirtualHost>
    ```

  - 修改`/etc/apache2/ports.conf`
  
    添加`listen 433` 

- 配置flask环境

  修改`TicketOffice/web/project/main.py`中`BackEndPath`变量值为本机运行的绝对路径。

- 重新加载Apache2配置，即可运行整个网站和后端。

  [注]由于第一次运行时服务器会运行一次极限数据，因此重启Apache2后的第一次访问可能较慢，但之后速度会恢复正常。