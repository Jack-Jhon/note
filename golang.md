- sync 中的所有模块: Mutex, RWmutex, Once, Pool, Cond, WaitGroup 等,
- 反射包中的 Value 和 Type， 及接口类型源码 eface和 iface
- 还有 一些关键字的源码 ： select、defer、panic、recover等
- 还有一些常用的 slice  array 和 map 、timer 、ticker 的源码
- 最后需要看下  go的调度模型 G 、M、 P 以及 go的gc (三色标志) 和 go 的内存管理模型 (TCmalloc)

### linux 安装
  - 下载 https://golang.org/dl/
  - 解压
  - 添加以下环境变量, source /etc/profile 
  ```
  vim /etc/profile
  PATH=$PATH:/go/bin
  GOPATH=/go
  GOPROXY=https://goproxy.cn
  GOBIN=/go/bin
  ```
     
    
   