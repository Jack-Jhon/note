### win
- python 37以上版本
- 安装 pip install supervisor-win
- 生成配置 echo_supervisord_conf.exe > ./supervisord.conf
```
修改 supervisord.conf 以下4个配置项
[supervisord]
logfile=F:/supervisorwin/log/supervisord.log ; (main log file;default $CWD/supervisord.log)
pidfile=F:/supervisorwin/supervisord.pid ; (supervisord pidfile;default supervisord.pid)
;这里注意路径不能带有空格，括号等特殊符号(eg: "Program Files (x86)")，尽量符合linux的格式，不然会有各种奇怪的错误

[supervisorctl]
serverurl=http://127.0.0.1:9998 ; use an http:// url to specify an inet socket
username=JackJhon              ; should be same as in [*_http_server] if set
password=jackjhon442592431                ; should be same as in [*_http_server] if set
; windows的ctl命令行只能和http链接，启动 supervisorctl.exe -c "supervisord.conf" status，需要制定配置文件(因为修改了端口，用户名和密码)

[inet_http_server]         ; inet (TCP) server disabled by default
port=127.0.0.1:9998        ; ip_address:port specifier, *:port for all iface
username=JackJhon              ; default is no username (open server)
password=jackjhon442592431               ; default is no password (open server)
; web管理控制台的相关配置

[include]
files=F:/supervisorwin/confd/*.conf ; (main log file;default $CWD/supervisord.log)
;这里的路径，同样不要带特殊符号(eg: conf.d, 其中的.)，不然也会有各种奇怪的错误
;supervisor启动后，去这里配置的路径读取所有子进程配置，并启动子进程。
```
- 子进程配置
```
[program:frpc_mstsc]
command=F:/frpc/frpc.exe ;
directory=F:/frpc/ ;
autorestart=true ;
autostart=true ;
user=administrator;
stderr_logfile=F:/supervisorwin/log/frpc_mstsc_err.log ;
stdout_logfile=F:/supervisorwin/log/frpc_mstsc_out.log ;
stdout_logfile_maxbytes=20MB ;
stdout_logfile_backups=3 ;
startsecs=1 ;
; 这里command在windows下配置启动命令时，附带的参数格式，如: -c 会导致程序无法启动，原因暂时不名，猜测可能是supervisor的正则匹配出现问题，所以尽量使配置的启动命令简洁(可以进来使用默认参数)。
; 采坑，配置frpc启动。原命令：F:/frpc/frpc.exe -c F:/frpc/frpc_mstsc.ini，报错"open F:/frpc/frpc_mstsc.ini;: The system cannot find the file specified."，把配置当成启动程序了，原因不明。最后把配置改成当前目录默认frpc.ini，启动命令 F:/frpc/frpc.exe，启动成功。 
```
- 启动
```
配置windows服务，开机自启动supervisord
- 保存以下启动命令为supervisord_run.bat
``supervisord_run.bat
start supervisord.exe -c F:\supervisorwin\supervisord.conf -n
pause
``
- win+R 输入 shell:startup 打开开机自动启动目录，复制supervisord_run.bat到该目录下
- 开始->设置->应用->启动，可以在右边菜单列表看到刚才新添加的supervisord_run.bat启动项，设置为"开"即可实现supervisor随开机自启动
```
- supervisor自带的web控制台查看日志存在延迟，并且windows下启动的supervisor访问web的tail -f会出现一些奇怪的问题，因此自己编写一个简单的在线http日志服务器
```
python,http,读取本地日志,返回。。。
```


### linux
- 基本和win配置一致

### golang 版本
- https://github.com/ochinchina/supervisord