### docker简易安装使用
- 在线安装 curl -sSL https://get.daocloud.io/docker | sh

docker images
docker run -t -i inputoutput/cardano-db-sync

查看当前运行的镜像实例(容器) docker ps

docker info
...
 Storage Driver: overlay2
 Docker Root Dir: /var/lib/docker
...

- ----



==================postgres==========================
sudo su - postgres
psql
create user jack with password 'jackjhon';
CREATE ROLE caigp superuser PASSWORD 'jackjhon' login;


配置远程连接
vim /etc/postgresql/10/main/pg_hba.conf
host    all             all             0.0.0.0/0               md5

vim /etc/postgresql/10/main/postgresql.conf
listen_addresses = 'localhost'         # what IP address(es) to listen on;
改为
listen_addresses = '*' 

重启
/etc/init.d/postgresql restart


================
- docker安装
  - 安装教程 https://www.runoob.com/docker/ubuntu-docker-install.html
  - docker包搜索路径 https://hub.docker.com/u/inputoutput
  - 查看当前所有docker映像，docker ps -a 可以查看到容器id，容器名称等信息
  - 通过容器id，进入相应容器 docker exec -it 容器id /bin/bash
  - 查看指定容器的控制台输出 docker logs 容器id
  - 启动，停止指定容器 docker start/stop 容器id  
  - 删除指定容器 docker rm -f 容器id
  - 清理掉所有处于终止状态的容器 docker container prune
  - 挂载 -v 宿主机目录:/容器内容目录
  - 查看挂载目录 docker inspect a76dca964e02 | grep Mounts -A 20
  - ipc通讯--ipc
  
- docker配置参考链接：https://github.com/input-output-hk/cardano-wallet/blob/master/docker-compose.yml 