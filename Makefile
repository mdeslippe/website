.DEFAULT_GOAL := all

TOOLCHAIN ?= gcc
BUILD ?= debug

PREFIX ?= $(HOME)/.local
DESTDIR ?=

SRC_DIR := src
VENDOR_DIR := vendor
BUILD_DIR := build
BIN_DIR := bin

EXECUTABLE_NAME ?= ssg
TARGET := $(BIN_DIR)/$(EXECUTABLE_NAME)

ifeq ($(TOOLCHAIN),gcc)
    CC := gcc
    DEBUGGER := gdb
else ifeq ($(TOOLCHAIN),clang)
    CC := clang
    DEBUGGER := lldb
else
    $(error Unknown toolchain: $(TOOLCHAIN))
endif

CPPFLAGS := -Iinclude -Ivendor
CFLAGS := -std=c11 -Wall -Wextra -pedantic -MMD -MP
LDFLAGS :=
LDLIBS :=

ifeq ($(BUILD),release)
    CFLAGS += -O2 -DNDEBUG
else
    CFLAGS += -O0 -g
    ifeq ($(shell uname),Linux)
        CFLAGS += -fsanitize=address,undefined,leak -fno-omit-frame-pointer
        LDFLAGS += -fsanitize=address,undefined,leak
    endif
endif

SRCS := $(shell find $(SRC_DIR) -name '*.c')
VENDOR_SRCS := $(shell find $(VENDOR_DIR) -name '*.c')
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
VENDOR_OBJS := $(patsubst $(VENDOR_DIR)/%.c,$(BUILD_DIR)/$(VENDOR_DIR)/%.o,$(VENDOR_SRCS))
DEPS := $(OBJS:.o=.d) $(VENDOR_OBJS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJS) $(VENDOR_OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(VENDOR_DIR)/%.o: $(VENDOR_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

run: all
	./$(TARGET)

install: all
	install -Dm755 $(TARGET) \
		$(DESTDIR)$(PREFIX)/bin/$(notdir $(TARGET))

-include $(DEPS)

.PHONY: all clean run install
