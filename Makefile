.DEFAULT_GOAL := all

TOOLCHAIN ?= gcc
BUILD ?= debug

PREFIX ?= $(HOME)/.local
DESTDIR ?=

SRC_DIR := src
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

CPPFLAGS := -Iinclude
CFLAGS := -std=c23 -Wall -Wextra -pedantic -MMD -MP
LDFLAGS :=
LDLIBS :=

ifeq ($(BUILD),release)
    CFLAGS += -O2 -DNDEBUG
else
    CFLAGS += -O0 -g
endif

SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
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
