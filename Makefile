DIRTYDUCK_VERSION := 0.1.4
SMOKE_VERSION := 0.1.0

KP_DIR ?= $(CURDIR)/KernelPatch

.PHONY: all dirtyduck smoke clean

all: smoke dirtyduck

dirtyduck:
	$(MAKE) -C dirtyduck KP_DIR=$(abspath $(KP_DIR))
	cp dirtyduck/dirtyduck_selinux_$(DIRTYDUCK_VERSION).kpm .

smoke:
	$(MAKE) -C smoke KP_DIR=$(abspath $(KP_DIR))
	cp smoke/dirtyduck_smoke_$(SMOKE_VERSION).kpm .

clean:
	$(MAKE) -C smoke clean KP_DIR=$(abspath $(KP_DIR))
	$(MAKE) -C dirtyduck clean KP_DIR=$(abspath $(KP_DIR))
	rm -f *.kpm
