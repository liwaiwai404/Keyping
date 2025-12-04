# Executable file name
TARGET_EXEC := keyping
# Build directory path
BUILD_DIR := ./build
# Source code directory path
SRC_DIR := ./src
# Header files directory path
INC_DIR := ./include
# Object files path
OBJ_DIR := $(BUILD_DIR)/objs

# Find all C files
SRCS := $(shell find $(SRC_DIR) -name '*.c')
# All object files
OBJS := $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(SRCS)))
# All dependent files
DEPS := $(OBJS:.o=.d)

# Compiler
CC := gcc
# C Preprocess flags
CPPFLAGS := -I$(INC_DIR)
# Compile flags
CFLAGS ?= -O2 -Wall -Wextra -Werror -std=c11
# Link library flags
LDLIBS := -lsodium -lsqlite3
DEPFLAGS ?= -MMD -MP
LDFLAGS ?= 

.PHONY: all clean

# Normal compile=======================================
all: $(BUILD_DIR)/$(TARGET_EXEC)

# Link all object files
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS) $(LDLIBS)
	@echo "Build successfully"

# Compile all c files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

-include $(DEPS)

# Release compile======================================
release: CFLAGS += -O3 -march=native -DNDEBUG
release: LDFLAGS += -s
release: clean all

# Remove build directory
clean:
	rm -rf $(BUILD_DIR)

print:
	echo $(OBJS)
	echo $(OBJ_DIR)

