DIRTYDUCK_VERSION := 0.1.4

ifndef KP_DIR
    KP_DIR = ../KernelPatch
endif

OS_NAME = $(shell uname | tr A-Z a-z)
MACHINE = $(shell uname -m)
NDK_BIN_DIR := toolchains/llvm/prebuilt/$(OS_NAME)-$(MACHINE)/bin
ifdef ANDROID_NDK_LATEST_HOME
    NDK_PATH ?= $(ANDROID_NDK_LATEST_HOME)/$(NDK_BIN_DIR)
else ifdef ANDROID_NDK
    NDK_PATH ?= $(ANDROID_NDK)/$(NDK_BIN_DIR)
else ifdef ANDROID_NDK_HOME
    NDK_PATH ?= $(ANDROID_NDK_HOME)/$(NDK_BIN_DIR)
endif

ifdef TARGET_COMPILE
    CC := $(TARGET_COMPILE)gcc
    LD := $(TARGET_COMPILE)ld
else ifdef NDK_PATH
    CC := $(NDK_PATH)/aarch64-linux-android31-clang
    LD := $(NDK_PATH)/ld.lld
endif

CFLAGS = -Wall -Wno-typedef-redefinition -O2 -fno-PIC -fno-asynchronous-unwind-tables -fno-stack-protector -fno-common
CFLAGS += -DDIRTYDUCK_VERSION_VALUE=$(DIRTYDUCK_VERSION) -std=gnu99

INCLUDE_DIRS := . include patch/include linux linux/include linux/security/selinux/include linux/arch/arm64/include linux/tools/arch/arm64/include

INCLUDE_FLAGS := $(foreach dir,$(INCLUDE_DIRS),-I$(KP_DIR)/kernel/$(dir))

objs := dirtyduck_selinux.o

all: dirtyduck_selinux_$(DIRTYDUCK_VERSION).kpm

dirtyduck_selinux_$(DIRTYDUCK_VERSION).kpm: ${objs}
	${CC} -r -nostdlib -o $@ $^

%.o: %.c
ifndef CC
	$(error No compiler found. Set ANDROID_NDK_HOME, ANDROID_NDK, ANDROID_NDK_LATEST_HOME, or TARGET_COMPILE)
endif
	${CC} $(CFLAGS) $(INCLUDE_FLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -rf *.kpm
	find . -name "*.o" | xargs rm -f
