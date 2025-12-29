# Keyping 🔐

Keyping 是一个基于 C 语言编写的轻量、安全的命令行密码管理工具。

## ✨ 特性 (Features)

* **高安全性**：使用 XChaCha20-Poly1305 加密和 Argon2id 密钥派生。
* **内存安全**：敏感数据使用完即刻擦除。
* **剪贴板集成**：支持 Linux (xclip/wl-copy), macOS (pbcopy) 和 Windows/WSL (clip.exe)。
* **本地存储**：数据存储在本地 SQLite 数据库中。

## 🛠️ 依赖 (Dependencies)

在编译之前，请确保安装了以下开发库：

* **GCC** (支持 C11)
* **Make**

**Ubuntu/Debian:**
```bash
sudo apt update
sudo apt install build-essential
```

## 🚀 编译安装 (Compile and install)
```bash
# 普通编译 -普通编译选项且会产生中间文件
make 
# 编译发布版 -更高级的优化编译选项
make release

# 安装 -将编译生成的可执行文件keyping放到 /usr/local/bin 中
sudo make install

# 删除 -从 /usr/local/bin 中删除可执行文件keyping
sudo make uninstall
```

## 🛎 使用方法
```bash
# 1. 添加新账号 (Add)
keyping -a

# 2. 查询账号并复制密码置粘贴板 (Query)
keyping -q

# 3. 列出所有账号 (List)
keyping -l
# 搜索特定账号
keyping -l "google"

# 4. 修改账号密码 (Modify)
keyping -m

# 5. 删除账号 (Delete)
keyping -d
```
