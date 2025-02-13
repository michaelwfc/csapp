# C dev env

## C dev enviroments on Windows

- Windows: MS Visual C++
- Windows: mingw-w64
  pros: 
  cons: 好像有些linux 命令不支持

- Windows: wsl
  
- Windows: remote wsl
  Pros:   
    - without worrying about pathing issues, binary compatibility, or other cross-OS challenges
    - 

  
- Windows: vm linux 虚拟机
  pros: 使用linux 的系统
  cons: 需要在虚拟机操作，有时候来回切换比较麻烦，如检索，copy
- Windows: docker container
- Windows: remote dev container

  

## C dev tools

- IDE : [vscode](https://code.visualstudio.com/docs/languages/cpp)
- vscode extension: Install recommended C/C++ extension in VSCode and reload
- compiler : MS Visual C++ /gcc/clang


## Compilers

C/C++ extension does not include a C++ compiler. So, you will need to install one or use which is already installed on your computer.

- Windows:  MS Visual C++  or  GCC C++ compiler (g++) +  GDB debugger(using mingw-w64 or Cygwin )
- Linux: gcc + GDB
- Mac: xcode +  LLDB or GDB
Also, Make sure to add C++ compiler PATH to environment variable of your platform. 

For Windows MinGW64 add: C:\MinGW64\bin 


### Issues:

#### Exec format error with MYSYS2 GCC

- Description: why run bomb readme.txt on Windows UCRT64 said: cannot execute binary file: Exec format error?
- Reason: Incompatible Binary Format: 
The bomb file you're trying to execute is probably not a Windows executable file. It might be a binary compiled for Linux, macOS, or another platform (e.g., ELF format for Linux), which Windows cannot directly execute.
Windows uses the Portable Executable (PE) format, while Linux commonly uses the ELF format for binaries.
- Solution
  1. Check the File Format: Use a tool like file (available on Linux or through MSYS2 on Windows) to inspect the binary format of the file. In MSYS2, you can run the following command in the terminal:
  This will tell you if the file is in the wrong format, such as an ELF binary for Linux or a 32-bit binary when your system is 64-bit.

  2. Run on the Correct Platform:

    If the binary is meant for Linux or another Unix-like system, you will need to run it on the correct platform.
    Option 1: Use a Linux machine or a virtual machine (VM) running Linux.
    Option 2: Use Windows Subsystem for Linux (WSL) to run Linux binaries on your Windows machine.




## Unix-like C compilers in Windows

- Cygwin : Cygwin是模拟 POSIX 系统，源码移植 Linux 应用到 Windows 下
- MinGW & MSYS: MinGW 是用于进行 Windows 应用开发的 GNU 工具链（开发环境），它的编译产物一般是原生 Windows 应用
- MinGW-w64 & MSYS2:  https://www.msys2.org/



## WSL Vscode settings
- c_cpp_properties.json (compiler path and IntelliSense settings)
- tasks.json (build instructions)
- launch.json (debugger settings)


- https://code.visualstudio.com/docs/cpp/config-wsl
- https://code.visualstudio.com/docs/cpp/config-mingw
- https://gourav.io/blog/setup-vscode-to-run-debug-c-cpp-code
- https://code.visualstudio.com/docs/cpp/cpp-debug
- https://code.visualstudio.com/docs/cpp/launch-json-reference
- https://code.visualstudio.com/docs/remote/wsl


# WSL 


## Remote wsl connect to windows proxy(clash) for VS extensions
### 1. Configure Clash to Listen on All Interfaces
Clash for Windows (or similar tools) may be set by default to bind only to 127.0.0.1. To allow WSL to reach Clash via the Windows host IP, you need to change its binding settings.
Enable “Allow LAN” in Clash
Open Clash’s settings:
Look for an option labeled “Allow LAN” or similar. Enabling this option will bind Clash to 0.0.0.0 (all interfaces) instead of just 127.0.0.1.

Restart Clash:
After changing the setting, restart Clash so that it binds on all interfaces. Then, confirm that it is listening on the Windows host IP.

Verify the Binding on Windows
You can check which interfaces Clash is listening on by opening a Command Prompt (run as administrator) and executing:

```bash
netstat -ano | findstr :7897
# You should see an entry like:
TCP    0.0.0.0:7897    0.0.0.0:0    LISTENING    <PID>
# If it shows only 127.0.0.1:7897, then Clash is not listening on the external interface.
```
### 2. Test Proxy Connectivity from Windows:
Testing connectivity from Windows itself could help. They can try using curl on Windows to connect to
curl -I --proxy http://127.0.0.1:7897 https://www.google.com
If this fails, Clash isn't running correctly on port 7897.

### 3. Allow Port in Windows Firewall:
Open Windows Security → Firewall & Network Protection → Advanced Settings.
Create a new Inbound Rule for port 7897 (TCP) to allow connections from WSL.

### 4. Confirm Windows Host IP:
Find Windows Host IP from WSL
Run this in WSL:  # 10.255.255.254 
cat /etc/resolv.conf | grep nameserver | awk '{print $2}'
but this IP is specific to WSL’s virtual network and may not work for proxy connections.

Confirm Windows Host IP instead: 192.168.1.14
```powershell
ipconfig | findstr IPv4
```

### 5. set the proxy for remote wsl
```bash
echo 'export http_proxy="http://192.168.1.14:7897"' >> ~/.bashrc
echo 'export https_proxy="http://192.168.1.14:7897"' >> ~/.bashrc
echo 'export no_proxy="localhost,127.0.0.1,::1"' >> ~/.bashrc
# Run the Fixed Command (replace YOUR_WINDOWS_IP 192.168.1.14):
# sed -i 's/10.255.255.254/192.168.1.14/g' ~/.bashrc
# sed -i 's/192.168.1.14/192.168.1.4/g' ~/.bashrc
source ~/.bashrc
# 检查remote wsl代理是否生效
curl -I --proxy http://192.168.1.14:7897 https://www.google.com
curl -I https://www.google.com
```


## Reference
- [Using C++ and WSL in VS Code](https://code.visualstudio.com/docs/cpp/config-wsl)
- https://code.visualstudio.com/docs/remote/wsl
- https://code.visualstudio.com/api/advanced-topics/remote-extensions


## WSL Ubuntu env Installation
 - install wsl and linux distribution
 - install vscode
 - install vscode for local wsl

 To use REMOTE WSL C
 - install WSL extension



   


### WSL env 

- https://learn.microsoft.com/zh-cn/windows/wsl/setup/environment

#### install wsl and linux distribution
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

#### set user name and password
- open Ubuntu-22.04
- set user_name and password
- show the distribution: wsl -l -v

#### set user name and password manully

```bash
sudo adduser michael

# To change the password for your user, use the following command:
sudo passwd michael

# Give the new user sudo privileges:
sudo usermod -aG sudo michael

# you can remove the old user
sudo deluser oldusername
```
#### set the default wsl distribution user

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
sudo apt update && apt upgrade -y

## 安装必备的库, 执行
sudo apt install -y build-essential gcc gdb make libssl-dev zlib1g-dev libbz2-dev libreadline-dev libsqlite3-dev wget curl llvm libncurses5-dev libncursesw5-dev xz-utils tk-dev libffi-dev liblzma-dev

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




## C debug in vscode

- https://code.visualstudio.com/docs/cpp/launch-json-reference


## wsl operarion 

cope/paste :
- win > ubuntu: ctrl+ c/v > right click
- ubuntu > win : ctrl+shift + c  > ctrl+ v
- ubuntu > ubuntu : left click choose > right click


export HTTP_PROXY=[username]:[password]@[proxy-web-or-IP-address]:[port-number]
export HTTP_PROXY=127.0.0.1:7890








# Make

- preprocessing
- compiling
- assembling
- linking

## windows 下使用make命令编译代码

https://blog.csdn.net/Nicholas_Liu2017/article/details/78323391

在 Windows 操作系统中，没有原生的 "make" 命令，但是您可以安装 GNU 工具链来获得它。  
安装 GNU 工具链的方法有很多，其中一种方法是通过安装 MinGW-w64。这是一个在 Windows 平台上提供了许多 GNU 工具的开源项目。在安装 MinGW-w64 后，您可以使用其提供的 "mingw32-make" 命令来执行类似于 Makefile 的任务。  
以下是使用 MinGW-w64 安装 make 命令的步骤：  
1.前往 MinGW-w64 的官方网站 (mingw-w64.org/doku.php/do…  
2.运行安装程序，并按照提示进行安装。请注意，如果您正在运行 64 位的 Windows 操作系统，则应选择与您的操作系统架构相对应的安装程序。  
3.在安装过程中，请确保选择了 "mingw32-base" 和 "mingw32-make" 以及其他您需要的工具。编译器勾选 C compiler 和 C++ compiler  
4.设置环境变量 ： Path 中增加 MinGW/bin  
5.复制或者重命名  "mingw32-make.exe to make.exe   
6.在命令行中输入 "mingw32-make" or make 来执行 Makefile 文件中的任务。  




