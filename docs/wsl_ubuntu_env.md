
# WSL env 

- https://learn.microsoft.com/zh-cn/windows/wsl/setup/environment

## install wsl and linux distribution
```bash
# 如果在线安装较慢，可以使用离线安装， 所在仓库为https://github.com/microsoft/WSL，只需要下载其中的msi文件安装即可，文件下载地址：https://github.com/microsoft/WSL/releases 
wsl --install

wsl --list --online 
# 离线安装  Linux 发行版  https://learn.microsoft.com/zh-cn/windows/wsl/install-manual#step-6---install-your-linux-distribution-of-choice
wsl --install -d Ubuntu-20.04

# To see all installed WSL distributions:
wsl -l -v

# Set a Default Distribution (Optional)
wsl --set-default Ubuntu-20.04

# To start a specific distribution, use: wsl or
wsl -d Ubuntu-20.04
# Codename:       focal

# To check the version of Ubuntu you are running in a Bash shell,
lsb_release -a
```

## set user name and password
- open Ubuntu-22.04
- set user_name and password
- show the distribution: wsl -l -v

### set user name and password manully

```bash
sudo adduser michael

# To change the password for your user, use the following command:
sudo passwd michael

# Give the new user sudo privileges:
sudo usermod -aG sudo michael

# you can remove the old user
sudo deluser oldusername
```

## set the default wsl distribution user

```Powershell
# Launch your WSL instance as root:
wsl  -d Ubuntu -u root
# Set the default user by editing /etc/wsl.conf:
nano /etc/wsl.conf
# Add the following lines to specify the default user:
[user]
default=michael
# Save the file (Ctrl+O, Enter, then Ctrl+X to exit).
# Restart WSL:
wsl --shutdown
wsl
```

## install wsl software
```bash
# enter into wsl from cmd
wsl

sudo cp /etc/apt/sources.list /etc/apt/sources.list.bak
sudo vim /etc/apt/sources.list
# Ubuntu 18.04 Bionic Beaver（仿生海狸）
# Ubuntu 20.04 Focal Fossa（马岛长尾狸猫，马达加斯加岛上最大的食肉性哺乳动物）
# Ubuntu 22.04 代号为 Jammy Jellyfish(幸运水母)
deb http://mirrors.aliyun.com/ubuntu/ jammy main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ jammy main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ jammy-security main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ jammy-security main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ jammy-updates main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ jammy-updates main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ jammy-proposed main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ jammy-proposed main restricted universe multiverse
deb http://mirrors.aliyun.com/ubuntu/ jammy-backports main restricted universe multiverse
deb-src http://mirrors.aliyun.com/ubuntu/ jammy-backports main restricted universe multiverse

#5.更新软件列表 && 更新软件包
sudo apt update && sudo apt upgrade -y

## 安装必备的库, 执行
sudo apt install -y build-essential gcc gdb make libssl-dev zlib1g-dev libbz2-dev libreadline-dev libsqlite3-dev wget curl llvm libncurses5-dev libncursesw5-dev xz-utils tk-dev libffi-dev liblzma-dev libcurl4-openssl-dev

whereis gcc
whereis gdb

# Python openssl bindings are available in 22.04 in python3-openssl
sudo apt install -y python3-openssl

# 在 WSL 下如何访问 Windows 下的所有目录，所以只需要在 WSL 的开发目录下构建一个软链接即可以在避免直接访问 /mnt 目录的同时访问到 Windows 下的目录
mkdir projects
ln -s /mnt/e/projects projects

# Navigate to your helloworld project folder and launch VS Code from the WSL terminal with code .
cd $Home/projects/helloworld/
code . 
```



##  Remote WSL C developing with VS Code

### Open a remote folder or workspace 

```shell
# open wsl
wsl 

# Navigate to your helloworld project folder and launch VS Code from the WSL terminal with code .
# Windows filesystem mounts like /mnt/c

cd $Home/projects/helloworld/
code .
```
