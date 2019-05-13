# linuxcnc-docker
sudo apt install docker docker.io

git clone https://github.com/FirstWithThisName/linuxcnc-docker.git
###### A docker image to run linuxcnc.

### Build it

##### Requirements for the build:
* run docker as current user
* installed git

```
./build.sh # you need to be in the directory of the repository
```

### Get the binary image
```
docker pull aceofdiamonds/linuxcnc-docker

docker pull 154897092/pzxcnc
密码： 123456
账号： linuxcnc

### Run it
if not already done you first need to do `xhost +` to allow docker to use your x-server.

```
docker run --rm -it -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix  linuxcnc-dev

```
DISPLAY=192.168.142.1:0 //这里是xming服务器的ip地址。即windwos的ip
docker exec -it  -e DISPLAY=$DISPLAY cncssh bash
