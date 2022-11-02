#!/usr/bin/env bash
echo "start to rm docker"
version=$1

cd docker/dig
chmod 755 *.dockfile
echo "start to make docker image"
workdir=`pwd`
docker build -f imageserver.dockfile -t sz-harbor.topvdn.com/develop_version/esp/amd64/imageserver:$version $workdir

docker run -d -p21088:21088 -v/opt/dig/nginx_imageserver/conf/nginx.conf:/opt/nginx_imageserver/conf/nginx.conf -v/opt/dig/nginx_imageserver/logs/:/opt/nginx_imageserver/logs/ sz-harbor.topvdn.com/develop_version/esp/amd64/imageserver:$version ./nginx_image -p ..

echo "finished!"
