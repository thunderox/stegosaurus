#

VST2 ?= true
LV2 ?= true

# --------------------------------------------------------------
# Project name, used for binaries

NAME = stegosaurus

# --------------------------------------------------------------
# Files to build

FILES_DSP = \
	stegosaurus.cpp \
	nix-echo.cpp \
	voice.cpp \
	oscillator.cpp \
	fastmaths.cpp \
	eq.cpp \
	
FILES_UI = \
	../../delirium_ui/delirium_ui.cpp \
	../../delirium_ui/delirium_ui_widget_base.cpp \
	../../delirium_ui/delirium_ui_widget_label.cpp \
	../../delirium_ui/delirium_ui_widget_fader.cpp \
	../../delirium_ui/delirium_ui_widget_switch.cpp \
	../../delirium_ui/delirium_ui_widget_logo.cpp \
	../../delirium_ui/delirium_ui_widget_knob.cpp \
	../../delirium_ui/delirium_ui_widget_button.cpp \
	../../delirium_ui/delirium_ui_widget_panel.cpp \
	../../delirium_ui/delirium_ui_widget_adsr.cpp \
	../../delirium_ui/delirium_ui_widget_selector.cpp \
	../../delirium_ui/delirium_ui_widget_filter.cpp \
	../../delirium_ui/delirium_ui_widget_fader_route.cpp \
	../../delirium_ui/delirium_ui_widget_list.cpp \
	../../delirium_ui/delirium_ui_widget_tabbed_navigator.cpp \
	stegosaurus_ui.cpp \

# --------------------------------------------------------------
# Do some magic

UI_TYPE = cairo
#USE_CLAP_BUNDLE = true
USE_VST2_BUNDLE = true
include ../../dpf/Makefile.plugins.mk

# --------------------------------------------------------------
# Enable all possible plugin types

ifeq ($(HAVE_CAIRO),true)

TARGETS += jack

ifneq ($(MACOS_OR_WINDOWS),true)
ifeq ($(HAVE_LIBLO),true)
TARGETS += dssi
endif # HAVE_LIBLO
endif # MACOS_OR_WINDOWS

TARGETS += lv2_sep
TARGETS += vst
#TARGETS += vst3
#TARGETS += clap

endif # HAVE_CAIRO

all: $(TARGETS)

# --------------------------------------------------------------
