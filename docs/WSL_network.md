# WSL 


## WSL connect to windows proxy(clash) for VS extensions
##  原因分析
WSL 运行在 NAT 虚拟网络中，它是一个“虚拟子系统”，和 Windows 主机虽然共享文件系统，但网络上是两个不同的 IP：

| 系统            | `localhost` 实际指向         |
| ------------- | ------------------------ |
| Windows       | Windows 自己（127.0.0.1）    |
| WSL（如 Ubuntu） | WSL 自己的子系统，不是 Windows 主机 |

所以你在 Windows 上设置了 127.0.0.1 的代理，比如 Clash、V2Ray、Shadowsocks，那 **WSL 中访问 127.0.0.1 是访问不到 Windows 上的代理服务的**。


## 解决办法

### 1. 在 WSL 中使用 Windows 主机 IP

你可以用 Windows 主机的 IP 地址来代替 `127.0.0.1`，方法如下：

在 WSL 中查找 Windows 主机 IP（通常是 `eth0` 的网关）：

```bash
ip route | grep default
# default via 172.23.160.1 dev eth0 proto kernel
```

那么你可以在 WSL 中将代理地址设置为：

```bash
export http_proxy=http://172.23.160.1:7897
export https_proxy=http://172.23.160.1:7897
```

> ⚠️ 记得把 `7897` 改成你实际代理监听的端口。

---

set the proxy for wsl
你也可以写一个 `.bashrc` 脚本来自动检测主机 IP，并设置代理：

```bash
HOST_IP=$(ip route | awk '/default/ { print $3 }')
export http_proxy="http://$HOST_IP:7897"
export https_proxy="http://$HOST_IP:7897"
export no_proxy="localhost,127.0.0.1,::1"
source ~/.bashrc




```



### 2. Configure Clash to Listen on All Interfaces

Clash for Windows (or similar tools) may be set by default to bind only to 127.0.0.1. 
To allow WSL to reach Clash via the Windows host IP, you need to change its binding settings:  Enable “Allow LAN” in Clash

- Open Clash’s settings:
- Look for an option labeled “Allow LAN” or similar. Enabling this option will bind Clash to 0.0.0.0 (all interfaces) instead of just 127.0.0.1.
- Restart Clash:
After changing the setting, restart Clash so that it binds on all interfaces. Then, confirm that it is listening on the Windows host IP.
- Verify the Binding on Windows
You can check which interfaces Clash is listening on by opening a Command Prompt (run as administrator) and executing:

```Powershell
netstat -ano | findstr :7897
# You should see an entry like:
  TCP    0.0.0.0:7897           0.0.0.0:0              LISTENING       27272
  TCP    127.0.0.1:7897         127.0.0.1:59889        FIN_WAIT_2      39188
  TCP    127.0.0.1:7897         127.0.0.1:59947        FIN_WAIT_2      39188
  TCP    127.0.0.1:7897         127.0.0.1:60179        TIME_WAIT       0
# If it shows only 127.0.0.1:7897, then Clash is not listening on the external interface.
```


### 3. Test Proxy Connectivity from Windows:
Testing connectivity from Windows itself could help. They can try using curl on Windows to connect to
```bash
# 检查remote wsl代理是否生效

curl -I https://www.google.com
curl -I --proxy http://172.23.160.1:7897 https://www.google.com

```
If this fails, Clash isn't running correctly on port 7897.

### (optional)3. Allow Port in Windows Firewall:
Open Windows Security → Firewall & Network Protection → Advanced Settings.
Create a new Inbound Rule for port 7897 (TCP) to allow connections from WSL.







