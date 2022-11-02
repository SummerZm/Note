# dis

## Install

### 制作Docker镜像
> <b>执行make_docker.sh生成制作镜像。</b>

#### 1. ImageService 服务
- 材料：make_docker.sh生成docker文件中dig/ImageService目录、image.dockfile。放在同一目录下：
- 制作镜像命令：docker build -f [制作镜像dockerfile的路径] -t [镜像名字] [文件材料绝对路径] 。
    ```sh
    # pwd: /Users/leaf/Work/yunyan/src/git.topvdn.com/dig/docker/dig
    docker build -f image.dockfile -t image /Users/leaf/Work/yunyan/src/git.topvdn.com/dig/docker/dig
    ```

### 运行Docker镜像
> <b> 注：宿主机的映射端口需要和服务配置文件相对应 </b>

#### 1. ImageService 服务
- 运行容器命令：docker run -d -p宿主机端口：镜像端口 -v宿主机映射文件：镜像映射文件 镜像名 
    ```sh
    # 在宿主机创建目录并给读写权限：mkdir -p /opt/dig/ImageService/logs/
    # 提供 /opt/dig/ImageService/imageservice.yaml 
    docker run -d -p19088:19088 -p19089:19089 -p40000:40000 -v/opt/dig/ImageService/imageservice.yaml:/opt/ImageService/imageservice.yaml -v/opt/dig/ImageService/logs/:/opt/ImageService/logs/ image
    ```

### docker常用命令
- 导出镜像：docker save [镜像ID] > xxx.tar
- 导入镜像：docker load < xxx.tar
- 镜像改名：docker tag [镜像ID] repository:tag
- 查看docker网络： docker network ls
- 查看docker网络详细信息：docker network inspect [网络ID-通过上面命令获取]
- 查看docker制作过程：docker history  [docker-id]
