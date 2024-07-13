# C dev env

- IDE : [vscode](https://code.visualstudio.com/docs/languages/cpp)
- vscode extension: Install recommended C/C++ extension in VSCode and reload
- compiler : MS Visual C++ /gcc/clang
   


## vscode setting on wsl

https://code.visualstudio.com/docs/cpp/config-wsl
https://code.visualstudio.com/docs/cpp/config-mingw
https://gourav.io/blog/setup-vscode-to-run-debug-c-cpp-code
https://code.visualstudio.com/docs/cpp/cpp-debug
https://code.visualstudio.com/docs/cpp/launch-json-reference
https://code.visualstudio.com/docs/remote/wsl


## Compilers

C/C++ extension does not include a C++ compiler. So, you will need to install one or use which is already installed on your computer.

- Windows:  MS Visual C++  or  GCC C++ compiler (g++) +  GDB debugger(using mingw-w64 or Cygwin )
- Linux: gcc + GDB
- Mac: xcode +  LLDB or GDB
Also, Make sure to add C++ compiler PATH to environment variable of your platform. 

For Windows MinGW64 add: C:\MinGW64\bin

## Unix-like C compilers in Windows
- Cygwin : Cygwin是模拟 POSIX 系统，源码移植 Linux 应用到 Windows 下
- MinGW & MSYS: MinGW 是用于进行 Windows 应用开发的 GNU 工具链（开发环境），它的编译产物一般是原生 Windows 应用
- MinGW-w64 & MSYS2:  https://www.msys2.org/

# GCC on Windows WSL

[Using C++ and WSL in VS Code](https://code.visualstudio.com/docs/cpp/config-wsl)

## wsl env 

```shell
wsl --install
wsl --list --online 
wsl --install Ubuntu-22.04

wsl
sudo cp /etc/apt/sources.list /etc/apt/sources.list.bak
sudo vim /etc/apt/sources.list
# Ubuntu 20.04 代号为 jammy
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
# Python openssl bindings are available in 22.04 in python3-openssl
sudo apt install -y python-openssl

# Navigate to your helloworld project folder and launch VS Code from the WSL terminal with code .
cd $Home/projects/helloworld/
code . 
```

## Run VS Code in WSL

```shell
# Navigate to your helloworld project folder and launch VS Code from the WSL terminal with code .
cd $Home/projects/helloworld/
code .
```

## WSL vscode settings
- c_cpp_properties.json (compiler path and IntelliSense settings)
- tasks.json (build instructions)
- launch.json (debugger settings)
- 



## wsl operarion 
cope/paste :
- win > ubuntu: ctrl+ c/v > right click
- ubuntu > win : ctrl+shift + c  > ctrl+ v
- ubuntu > ubuntu : left click choose > right click


export HTTP_PROXY=[username]:[password]@[proxy-web-or-IP-address]:[port-number]
export HTTP_PROXY=127.0.0.1:7890





### run

### C debug in vscode

launch.json
{
  "version": "0.2.0",
  "configurations": [
    {
      "name": "g++.exe build and debug active file",
      "type": "cppdbg",
      "request": "launch",
      "program": "${fileDirname}\${fileBasenameNoExtension}.exe",
      "args": [],
      "stopAtEntry": false,
      "cwd": "${workspaceFolder}",
      "environment": [],
      "externalConsole": false, //set to true to see output in cmd instead
      "MIMode": "gdb",
      "miDebuggerPath": "C:\MinGW64\bin\gdb.exe",
      "setupCommands": [
        {
          "description": "Enable pretty-printing for gdb",
          "text": "-enable-pretty-printing",
          "ignoreFailures": true
        }
      ],
      "preLaunchTask": "g++.exe build active file"
    },
    {
      "name": "g++ build & run active file",
      "type": "cppdbg",
      "request": "launch",
      "program": "${fileDirname}\${fileBasenameNoExtension}.exe",
      "args": [],
      "stopAtEntry": false,
      "cwd": "${workspaceFolder}",
      "environment": [],
      "externalConsole": false, //set to true to see output in cmd instead
      "MIMode": "gdb",
      "miDebuggerPath": "C:\MinGW64\bin\gdb.exe",
      "setupCommands": [
        {
          "description": "Enable pretty-printing for gdb",
          "text": "-enable-pretty-printing",
          "ignoreFailures": true
        }
      ],
      "preLaunchTask": "g++ build & run active file"
    }
    ]
}

# make

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