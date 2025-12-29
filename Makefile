# =============================================================================
#  Keyping Build System
# =============================================================================

# --- 1. 项目配置 (Project Config) ---
TARGET      := keyping
PREFIX      ?= /usr/local
BIN_DIR     := $(PREFIX)/bin

# 目录定义
BUILD_DIR   := ./build
SRC_DIR     := ./src
INC_DIR     := ./include
VENDOR_DIR  := ./vendor
OBJ_DIR     := $(BUILD_DIR)/objs

# 依赖库目录
SQLITE_DIR  := $(VENDOR_DIR)/sqlite3
SODIUM_DIR  := $(VENDOR_DIR)/libsodium_install

# --- 2. 编译器设置 (Compiler Settings) ---
CC          := gcc

# 基础编译选项 (通用)
CFLAGS_BASE := -Wall -Wextra -Werror -std=gnu11
CPPFLAGS    := -I$(INC_DIR) -I$(SQLITE_DIR) -I$(SODIUM_DIR)/include

# 链接选项
LDLIBS      := $(SODIUM_DIR)/lib/libsodium.a -pthread -ldl
LDFLAGS     := 

# Debug 模式 (默认)
CFLAGS_DBG  := -g -O0 -DDEBUG

# Release 模式
CFLAGS_REL  := -O3 -march=native -DNDEBUG

# 依赖管理 (.d 文件)
DEPFLAGS    := -MMD -MP

# --- 3. 源文件与目标文件 (Sources & Objects) ---
# 自动查找 src 下的所有 .c 文件，并加上 sqlite3.c
SRCS := $(shell find $(SRC_DIR) -name '*.c') \
        $(SQLITE_DIR)/sqlite3.c

# 将所有 .c 文件名转换为 .o 文件名 (展平路径，全部放到 build/objs 下)
OBJS := $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(SRCS)))
DEPS := $(OBJS:.o=.d)

# 告诉 make 去哪里找 .c 文件 (因为 OBJS 只有文件名没有路径)
vpath %.c $(SRC_DIR) $(SQLITE_DIR)

# [新增] 针对 SQLite 的特殊豁免：
# 1. filter-out: 移除 -Werror (不报错) 和 -Wall -Wextra (不警告)
# 2. -w: 彻底关闭该文件的所有警告 (Quiet 模式)
$(OBJ_DIR)/sqlite3.o: CFLAGS := $(filter-out -Werror -Wall -Wextra, $(CFLAGS)) -w

# --- 4. 辅助变量 (美化输出) ---
# 颜色代码
COLOR_RESET := \033[0m
COLOR_BOLD  := \033[1m
COLOR_GREEN := \033[32m
COLOR_BLUE  := \033[34m

# 打印函数
define print_status
	@echo "$(COLOR_BLUE)[$(1)]$(COLOR_RESET) $(2)"
endef

# =============================================================================
#  构建规则 (Build Targets)
# =============================================================================

.PHONY: all debug release clean install uninstall deps help

# 默认目标：Debug 构建
all: debug

# --- Debug 构建 ---
debug: CFLAGS := $(CFLAGS_BASE) $(CFLAGS_DBG)
debug: deps $(BUILD_DIR)/$(TARGET)

# --- Release 构建 ---
# Release 模式下，链接时去除符号表 (-s) 以减小体积
release: CFLAGS := $(CFLAGS_BASE) $(CFLAGS_REL)
release: LDFLAGS += -s
release: clean deps $(BUILD_DIR)/$(TARGET)

# --- 链接主程序 ---
$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(call print_status,LINK,Linking executable $@...)
	@$(CC) $(OBJS) -o $@ $(LDFLAGS) $(LDLIBS)
	@echo "$(COLOR_GREEN)$(COLOR_BOLD)Build successful: $(BUILD_DIR)/$(TARGET)$(COLOR_RESET)"

# --- 编译 .c 到 .o ---
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(call print_status,CC,Compiling $<...)
	@$(CC) $(CPPFLAGS) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

# --- 处理依赖库 ---
deps:
	$(call print_status,DEPS,Checking and preparing dependencies...)
	@chmod +x scripts/get_libsodium.sh
	@./scripts/get_libsodium.sh

# 包含头文件依赖
-include $(DEPS)

# =============================================================================
#  管理命令 (Management)
# =============================================================================

# 清理构建产物
clean:
	$(call print_status,CLEAN,Cleaning build directory...)
	@rm -rf $(BUILD_DIR)

# 安装到系统
install: release
	$(call print_status,INSTALL,Installing to $(BIN_DIR)...)
	@mkdir -p $(BIN_DIR)
	@cp $(BUILD_DIR)/$(TARGET) $(BIN_DIR)/
	@chmod 755 $(BIN_DIR)/$(TARGET)
	@echo "$(COLOR_GREEN)Installation complete.$(COLOR_RESET)"
	@echo "Data location: ~/.local/share/keyping/"

	@$(MAKE) clean

# 卸载
uninstall:
	$(call print_status,UNINSTALL,Removing $(TARGET)...)
	@rm -f $(BIN_DIR)/$(TARGET)
	@echo "$(COLOR_GREEN)Uninstalled.$(COLOR_RESET)"
	@echo "Note: User data in ~/.local/share/keyping/ was kept."

# 帮助信息
help:
	@echo "$(COLOR_BOLD)Keyping Build System$(COLOR_RESET)"
	@echo ""
	@echo "Targets:"
	@echo "  $(COLOR_GREEN)make$(COLOR_RESET)           Build debug version (default)"
	@echo "  $(COLOR_GREEN)make release$(COLOR_RESET)   Build optimized release version"
	@echo "  $(COLOR_GREEN)make install$(COLOR_RESET)   Build release version and install to system"
	@echo "  $(COLOR_GREEN)make uninstall$(COLOR_RESET) Remove installed executable"
	@echo "  $(COLOR_GREEN)make clean$(COLOR_RESET)     Remove build artifacts"
	@echo ""