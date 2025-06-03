# Ubuntu 20.04 Dev Env

## APT sources
```bash
# /etc/apt/sources.list
deb [arch=amd64] https://mirrors.ustc.edu.cn/ubuntu/ focal main restricted
deb [arch=amd64] https://mirrors.ustc.edu.cn/ubuntu/ focal-updates main restricted
deb [arch=amd64] https://mirrors.ustc.edu.cn/ubuntu/ focal universe
deb [arch=amd64] https://mirrors.ustc.edu.cn/ubuntu/ focal-updates universe
deb [arch=amd64] https://mirrors.ustc.edu.cn/ubuntu/ focal multiverse
deb [arch=amd64] https://mirrors.ustc.edu.cn/ubuntu/ focal-updates multiverse
deb [arch=amd64] https://mirrors.ustc.edu.cn/ubuntu/ focal-backports main restricted universe multiverse
deb [arch=amd64] https://mirrors.ustc.edu.cn/ubuntu focal-security main restricted
deb [arch=amd64] https://mirrors.ustc.edu.cn/ubuntu focal-security universe
deb [arch=amd64] https://mirrors.ustc.edu.cn/ubuntu focal-security multiverse

# /etc/apt/sources.list.d/ubuntu-ports.list
deb [arch=arm64,armhf] https://mirrors.ustc.edu.cn/ubuntu-ports/ focal main restricted
deb [arch=arm64,armhf] https://mirrors.ustc.edu.cn/ubuntu-ports/ focal-updates main restricted
deb [arch=arm64,armhf] https://mirrors.ustc.edu.cn/ubuntu-ports/ focal universe
deb [arch=arm64,armhf] https://mirrors.ustc.edu.cn/ubuntu-ports/ focal-updates universe
deb [arch=arm64,armhf] https://mirrors.ustc.edu.cn/ubuntu-ports/ focal multiverse
deb [arch=arm64,armhf] https://mirrors.ustc.edu.cn/ubuntu-ports/ focal-updates multiverse
deb [arch=arm64,armhf] https://mirrors.ustc.edu.cn/ubuntu-ports/ focal-backports main restricted universe multiverse
deb [arch=arm64,armhf] https://mirrors.ustc.edu.cn/ubuntu-ports focal-security main restricted
deb [arch=arm64,armhf] https://mirrors.ustc.edu.cn/ubuntu-ports focal-security universe
deb [arch=arm64,armhf] https://mirrors.ustc.edu.cn/ubuntu-ports focal-security multiverse
```

## Base libraries

```bash
# Examples to install build-essential
sudo apt install build-essential autoconf libtool automake bison flex \
libssh-dev libgmp3-dev libmpfr-dev texinfo libisl-dev pkg-config cmake \
minicom crossbuild-essential-arm64 crossbuild-essential-armhf libgl1-mesa-dev \
mesa-common-dev libxcb-cursor-dev libsdl2-dev libsysfs-dev valgrind net-tools \
libicu-dev
```

## Debian-based (Such as Ubuntu)

```bash
sudo apt update
# Bare metal
sudo apt install gcc-arm-none-eabi

# ARM 32
sudo apt install crossbuild-essential-armhf
# ARM 64
# For armv8-64, zlg: gcc-aarch64-linux-gnu g++-aarch64-linux-gnu 9.4.0
# virtual box/vmware:ubuntu-20.04:zlg:zlg
# rs232:root:zlg
# ssh:zlg:zlg
sudo apt install crossbuild-essential-arm64

# To confirm the installation is successful
aarch64-linux-gnu-gcc --version
```

## Qt install on line

```bash
# Download tslib
# https://libts/tslib

# Compile tslib
./configure --prefix=`pwd`/output --host=arm-linux-gnueabihf CC=arm-linux-gnueabihf-gcc CXX=arm-linux-gnueabihf-g++

# https://mirror.nju.edu.cn/ubuntu/pool/main/q/
sudo apt install qt5-default qtcreator qttools5-dev qtdeclarative5-dev

# https://wiki.qt.io/Building_Qt_5_from_Git
# https://wiki.qt.io/Building_Qt_5_from_Git/zh
# git clone  http://code.qt.io/qt/qt.git
[git clone https://code.qt.io/qt/qt5.git -b 5.12.8](https://code.qt.io/qt/qt5.git)
./init-repository --module-subset all

# https://mirrors.tuna.tsinghua.edu.cn/qt/
# http://mirrors.ustc.edu.cn/qtproject
# https://mirrors.sjtug.sjtu.edu.cn/qt/snapshots/qt/5.12/latest/
./qt-online-installer-linux-x64-4.9.0.run --mirror http://mirrors.ustc.edu.cn/qtproject

# Modifies
qtbase/mkspecs/linux-arm-gnueabi-g++/qmake.conf
./configure -prefix /home/ivan/Downloads/qt/qt-everywhere-src-5.12.12/output \
    -opensource -confirm-license -release \
    -strip -shared \
    -xplatform linux-arm-gnueabi-g++ \
    -optimized-qmake \
    -c++std c++17 \
    -sysroot /home/ivan/armv7-sysroot \
    -nomake examples -nomake tests \
    -no-opengl \
    -tslib \
    -I /home/ivan/Downloads/tslib-1.23/install/include \
    -L /home/ivan/Downloads/tslib-1.23/install/lib \
    -recheck-all

# Make environment
export TSLIB_ROOT=/usr/local/arm-tslib
    export TSLIB_CONSOLEDEVICE=none
    export TSLIB_FBDEVICE=/dev/fb0
    export TSLIB_TSDEVICE=/dev/input/event1
    export TSLIB_CONFFILE=$TSLIB_ROOT/etc/ts.conf
    export TSLIB_PLUGINDIR=$TSLIB_ROOT/lib/ts
    export TSLIB_CALIBFILE=/etc/pointercal
    export LD_PRELOAD=$TSLIB_ROOT/lib/

export QT_ROOT=/usr/local/arm-qt
    export QT_QPA_GENERIC_PLUGINS=tslib:/dev/input/event1
    export QT_QPA_FONTDIR=/usr/share/fonts
    export QT_QPA_PLATFORM_PLUGIN_PATH=$QT_ROOT/plugins
    export QT_QPA_PLATFORM=linuxfb:tty=/dev/fb0
    export QT_PLUGIN_PATH=$QT_ROOT/plugins
    export LD_LIBRARY_PATH=$QT_ROOT/lib:$QT_ROOT/plugins/platforms
    export QML2_IMPORT_PATH=$QT_ROOT/qml
    export QT_QPA_FB_TSLIB=1

```

## 致远电子SUPPORT服务系统

http://support.zlg.cn/redmine/projects/epcsupport40

用户：ivan.yu
密码：pass123

## 调试串口

波特率：115200
数据位：8
奇偶校验：无
停止位：1
流控：无
用户: root
密码：zlg

## Ether

IP：192.168.1.136
用户：zlg
密码：zlg

## 蜂鸣器

设备名称：/sys/class/leds/beep/
```bash
# 开启蜂鸣器
echo 50 > /sys/class/leds/beep/brightness
# 关闭蜂鸣器
echo 0 > /sys/class/leds/beep/brightness
```

## RTC

设备名称：/dev/rtc0

```bash
# 读取RTC时间
hwclock -r
# 写入RTC时间，写入的时间当前的系统时间，所以需要先设置系统时间再写入RTC
date -s "2020-08-13 14:21:30"
hwclock -w
```

## NTP

```bash
timedatectl
#                       Local time: Thu 2024-05-30 09:18:56 CST  # 当地时间，指当前时区的时间
#                   Universal time: Thu 2024-05-30 01:18:56 UTC  # UTC时间
#                         RTC time: Thu 2024-05-30 01:18:56      # RTC时间，指保存在RTC芯片内的时间
#                        Time zone: Asia/Shanghai (CST, +0800)   # 当前时区
#        System clock synchronized: yes                          # 是否开启NTP时间同步服务
# systemd-timesyncd.service active: yes                          # systemd-timesynccd服务是否启动
#                  RTC in local TZ: no

# 关闭NTP服务
timedatectl set-ntp false

# 修改NTP服务器
cat /etc/systemd/timesyncd.conf
#  This file is part of systemd.
#
#  systemd is free software; you can redistribute it and/or modify it
#  under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation; either version 2.1 of the License, or
#  (at your option) any later version.
#
# Entries in this file show the compile time defaults.
# You can change settings by editing this file.
# Defaults can be restored by simply deleting this file.
#
# See timesyncd.conf(5) for details.
[Time]
NTP=ntp.aliyun.com ntp1.aliyun.com ntp2.aliyun.com ntp3.aliyun.com # 修改此行
FallbackNTP=ntp.ntsc.ac.cn
#RootDistanceMaxSec=5
#PollIntervalMinSec=32
#PollIntervalMaxSec=2048
```

## M.2

### 分区

```bash
# 分3个分区，大小分别为512MB、8GB以及剩余空

# 创建分区表
parted /dev/nvme0n1 -s mklabel gpt
# 创建第1个分区，大小为512MB
parted /dev/nvme0n1 -s mkpart primary 0% 512MB
# 创建第2个分区，大小为8GB
parted /dev/nvme0n1 -s mkpart primary 512MB 8.5GB
# 创建第3个分区，大小为剩下的所有空间
parted /dev/nvme0n1 -s mkpart primary 8.5GB 100%

# 分1个分区

# 创建分区表
parted /dev/nvme0n1 -s mklabel gpt
# 创建第1个分区，大小为固态硬盘的所有空间
parted /dev/nvme0n1 -s mkpart primary 0% 100%

# 格式化

# FAT32格式
mkfs.fat -F 32 -n part_name /dev/nvme0n1p1
# exFAT格式
mkfs.exfat -n part_name /dev/nvme0n1p1
# EXT4格式
mkfs.ext4 -L part_name /dev/nvme0n1p1

# 挂载

# 挂载第一个分区
udisksctl mount -b /dev/nvme0n1p1
# 挂载第二个分区
udisksctl mount -b /dev/nvme0n1p2
# 卸载第一个分区
udisksctl unmount -b /dev/nvme0n1p1
# 卸载第二个分区
udisksctl unmount -b /dev/nvme0n1p2

# 自动挂载

cat /etc/fstab
# 分区名称       挂载目录      分区格式
/dev/nvme0n1p1 /opt         ext4     defaults 0 2 # 挂载第1个分区
/dev/nvme0n1p2 /userdata    ext4     defaults 0 2 # 挂载第2个分区
```

## Network

```bash
# net1-static/net1-dhcp

# 使能静态IP配置
nmcli connection modify net1-static connection.autoconnect-priority 2
nmcli connection up net1-static
# 失能静态IP配置
nmcli connection modify net1-static connection.autoconnect-priority -1
nmcli connection up net1-dhcp

# 设置静态IP地址
nmcli connection modify net1-static ipv4.addresses 192.168.1.136/24
# 设置静态IP网关
nmcli connection modify net1-static ipv4.gateway 192.168.1.1
# 设置静态IP的DNS服务器地址
nmcli connection modify net1-static ipv4.dns 114.114.114.114

# 重载网络设备配置
# 注意：此处不是net1-static，而是net1。若需替换为其他网络设备，应是net2、net3、net4
nmcli device reapply net1
```

## Wi-Fi

```bash
# 安装驱动
modprobe asr595x

# 列出WIFI列表
nmcli device wifi list ifname wlan0
# 连接无密码网络，<ssid>是连接WIFI的SSID
nmcli device wifi connect <ssid> ifname wlan0
# 连接有密码网络，<ssid>是连接WIFI的SSID，<psk>是连接WIFI的密码
nmcli device wifi connect <ssid> password <psk> ifname wlan0
# 查看是否连接成功
nmcli device status

# 在连接成功后，会自动记录以及连接的WIFI的SSID与密码，重新连接已记录WIFI请使用nmcli con up SSID命令即可
# 若WIFI模块支持STA，可使用以下命令配置热点
# 创建connection
# 以下命令中的WIFI_NAME、WIFI_SSID、WIFI_PSK，可根据需求修改
nmcli device wifi hotspot ifname wlan0 con-name WIFI_NAME ssid WIFI_SSID password WIFI_PSK
nmcli connnection modify WIFI_NAME connection.autoconnect yes

# 若不想使用NetworkManager，也可使用/etc/network/interfaces文件配置网络
# 一旦在/etc/network/interfaces文件中配置了相应的网络接口，NetworkManager将不再接管该网络接口。
```

## CAN

### 电阻配置

```bash
# 开启终端电阻

# CAN1
gpioset 3 27=1
# CAN2
gpioset 3 28=1
# CAN3
gpioset 3 29=1
# 也可通过以下命令同时开启CAN1、CAN2、CAN3的终端电阻
gpioset 3 27=1 28=1 29=1

# 关闭终端电阻

# CAN1
gpioset 3 27=0
# CAN2
gpioset 3 28=0
# CAN3
gpioset 3 29=0
# 也可通过以下命令同时关闭CAN1、CAN2、CAN3的终端电阻
gpioset 3 27=0 28=0 29=0
```

### CAN配置方法

```bash
# 3路CAN接口，支持CANRAW和CAN FD模式，其中CANRAW模式支持波特率5k、10k、100k、125k、250k、500k、1M
# 而CAN FD最大支持波特率高达5M

# 关闭CAN
# 必须先把CAN接口关闭才能配置波特率
# CAN1
ip link set can0 down
# CAN2
ip link set can1 down
# CAN3
ip link set can2 down

# 配置为CAN RAW，并设置波特率
# CAN1
ip link set can0 type can bitrate 1000000
# CAN2
ip link set can1 type can bitrate 1000000
# CAN3
ip link set can2 type can bitrate 1000000

# 配置为CAN FD，并设置波特率
# CAN1
ip link set can0 type can bitrate 1000000 dbitrate 2000000 fd on
# CAN2
ip link set can1 type can bitrate 1000000 dbitrate 2000000 fd on
# CAN3
ip link set can2 type can bitrate 1000000 dbitrate 2000000 fd on

# 启动CAN
# CAN1
ip link set can0 up
# CAN2
ip link set can1 up
# CAN3
ip link set can2 up

# CAN测试方法

## 发送CAN/CAN FD报文
# 发送CAN的标准帧报文 --- CAN ID: 0x123，DLC: 2 bytes，Data: [ 0x12 0x34 ]
cansend can0 123#1234
# 发送CAN的扩展帧报文 --- CAN ID: 0x123，DLC: 2 bytes，Data: [ 0x12 0x34 ]
cansend can0 00000123#1234
# 发送CAN FD的标准帧报文 --- CAN ID: 0x123，DLC: 2 bytes，Data: [ 0x12 0x34 ]
cansend can0 123##01234
# 发送CAN FD的扩展帧报文 --- CAN ID: 0x123，DLC: 2 bytes，Data: [ 0x12 0x34 ]
cansend can0 00000123##01234
# 发送CAN FD(BRS)的标准帧报文 --- CAN ID: 0x123，DLC: 2 bytes，Data: [ 0x12 0x34 ]
cansend can0 123##11234
# 发送CAN FD(BRS)的扩展帧报文 --- CAN ID: 0x123，DLC: 2 bytes，Data: [ 0x12 0x34 ]
cansend can0 00000123##11234

## 接收CAN FD报文
candump can0
```

## DIDO

```bash
zlg_dido get
zlg_dido get 1
zlg_dido get 1,2,3,4

zlg_dido on
zlg_dido on 1
zlg_dido on 1,2,3,4

zlg_dido off
zlg_dido off 1
zlg_dido off 1,2,3,4
```

## UART

### RS232

/dev/ttyRS232-1

### RS485

/dev/ttyRS485-1
/dev/ttyRS485-2
/dev/ttyRS485-3
/dev/ttyRS485-4
/dev/ttyRS485-5
/dev/ttyRS485-6
/dev/ttyRS485-7
/dev/ttyRS485-8

```bash
# UART 测试工具

# 发送数据
# --test-write可指定发送的数据，此处是发送长度为3个字节的字符串"123"
zlg_uart -d /dev/ttyRS232-1 --test-write=123
```