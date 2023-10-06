#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#
# Modified by REIS0

PLUGIN=stegosaurus

include dpf/Makefile.base.mk

all: dgl plugins gen

# --------------------------------------------------------------

dgl:
ifeq ($(HAVE_OPENGL),true)
	$(MAKE) -C dpf/dgl opengl
endif

plugins: dgl
	$(MAKE) all -C plugins/$(PLUGIN)

ifneq ($(CROSS_COMPILING),true)
gen: plugins dpf/utils/lv2_ttl_generator
	@$(CURDIR)/dpf/utils/generate-ttl.sh
ifeq ($(MACOS),true)
	@$(CURDIR)/dpf/utils/generate-vst-bundles.sh
endif

dpf/utils/lv2_ttl_generator:
	$(MAKE) -C dpf/utils/lv2-ttl-generator
else
gen:
endif

# --------------------------------------------------------------

clean:
	$(MAKE) clean -C dpf/dgl
	$(MAKE) clean -C dpf/utils/lv2-ttl-generator
	$(MAKE) clean -C plugins/$(PLUGIN)
	rm -rf bin build

# --------------------------------------------------------------
install:
	cp $(CURDIR)/bin/stegosaurus.lv2 /usr/lib/lv2 -r
	cp $(CURDIR)/bin/stegosaurus-vst.so /usr/lib/vst
	cp $(CURDIR)/plugins/stegosaurus/logo.png /usr/lib/lv2/stegosaurus.lv2
	mkdir /home/$(SUDO_USER)/.lv2/stegosaurus.lv2
	cp $(CURDIR)/plugins/stegosaurus/waves /home/$(SUDO_USER)/.lv2/stegosaurus.lv2/waves -r

# --------------------------------------------------------------
uninstall:
	rm /usr/lib/lv2/stegosaurus.lv2  -rf
	rm /usr/lib/vst/stegosaurus-vst.so -f
	rm /home/$(SUDO_USER)/.lv2/stegosaurus.lv2 -rf


# --------------------------------------------------------------

.PHONY: plugins
