#!/bin/bash

# 1. 设置：脚本里遇到任何错误，立即停止
set -e 

# 2. 定义路径
VENDOR_DIR="$(pwd)/vendor" # vendor 目录路径
SOURCE_DIR="$VENDOR_DIR/libsodium_source" # vendor目录下存放解压后得到的源代码
INSTALL_DIR="$VENDOR_DIR/libsodium_install" # vendor目录下存放编译后得到的.a静态库文件和.h头文件

# 3. 定义libsodium版本号 与 压缩包文件名称
VERSION="1.0.20-stable" 
TAR_FILE="libsodium-$VERSION.tar.gz"

# 4. 检查静态库是否存在
if [ -f "$INSTALL_DIR/lib/libsodium.a" ]; then
    echo "Libsodium static library exist, skip compile step."
    exit 0
fi

# 5. 创建vendor目录并进入
mkdir -p "$VENDOR_DIR"
cd "$VENDOR_DIR"

# 6. 检查压缩包文件是否存在，不存在则获取压缩包文件
if [ ! -f "$TAR_FILE" ]; then
    echo "Downloading libsodium..."

    # 检查系统是否拥有wget curl工具并下载该压缩包
    if command -v wget >/dev/null 2>&1; then
        wget "https://download.libsodium.org/libsodium/releases/$TAR_FILE"
    elif command -v curl >/dev/null 2>&1; then
        curl -L -O "https://download.libsodium.org/libsodium/releases/$TAR_FILE"
    else
        echo "Error: need wget or curl to download files"
        exit 1
    fi
fi

# 7. 解压缩
echo "Unziping..."
rm -rf "$SOURCE_DIR" # 删除libsodium_source目录以及内部的所有文件
tar -xf "$TAR_FILE" # 解压缩压缩包得到libsodium-stable
mv "libsodium-stable" "$SOURCE_DIR" # 重命名 libsodium-stable 为 libsodium_source


# 8. 编译源码
echo "Compiling..."
cd "$SOURCE_DIR" # 进入源码目录

./configure --prefix="$INSTALL_DIR" --disable-shared --enable-static --with-pic > /dev/null # 配置脚本文件，生成Makefile
make -j$(nproc) > /dev/null # 执行Makefile
make install > /dev/null


# 9. 清理源码文件
cd "$VENDOR_DIR"
rm -rf "$SOURCE_DIR"
rm -f "$TAR_FILE" # 删除压缩包

echo "Libsodium is ready."
echo "Static position: $INSTALL_DIR/lib/libsodium.a"
