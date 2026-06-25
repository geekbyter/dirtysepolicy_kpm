DIRTYDUCK_VERSION := 0.1.4
SMOKE_VERSION := 0.1.0
DIRTYDUCK_HOTPATCH_KP0112 ?= 0

KP_DIR ?= $(CURDIR)/KernelPatch
SUBMAKE_VARS := KP_DIR=$(abspath $(KP_DIR)) \
	ANDROID_NDK_HOME="$(ANDROID_NDK_HOME)" \
	ANDROID_NDK="$(ANDROID_NDK)" \
	ANDROID_NDK_LATEST_HOME="$(ANDROID_NDK_LATEST_HOME)" \
	TARGET_COMPILE="$(TARGET_COMPILE)"

.PHONY: all dirtyduck smoke clean

all: smoke dirtyduck

dirtyduck:
	$(MAKE) -C dirtyduck $(SUBMAKE_VARS) DIRTYDUCK_VERSION=$(DIRTYDUCK_VERSION) DIRTYDUCK_HOTPATCH_KP0112=$(DIRTYDUCK_HOTPATCH_KP0112)
	cp dirtyduck/dirtyduck_selinux_$(DIRTYDUCK_VERSION).kpm .

smoke:
	$(MAKE) -C smoke $(SUBMAKE_VARS) SMOKE_VERSION=$(SMOKE_VERSION)
	cp smoke/dirtyduck_smoke_$(SMOKE_VERSION).kpm .

clean:
	$(MAKE) -C smoke clean $(SUBMAKE_VARS)
	$(MAKE) -C dirtyduck clean $(SUBMAKE_VARS)
	rm -f *.kpm
