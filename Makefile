
# If RACK_DIR is not defined when calling the Makefile, default to two directories above
RACK_DIR ?= /media/bigdrive/source/C++/Rack-SDK
# FLAGS will be passed to both the C and C++ compiler
FLAGS +=
CFLAGS +=
CXXFLAGS +=

# Careful about linking to shared libraries, since you can't assume much about the user's environment and library search path.
# Static libraries are fine, but they should be added to this plugin's build system.
LDFLAGS +=

# Add .cpp files to the build
SOURCES += $(wildcard src/*.cpp)

# Add files to the ZIP package when running `make dist`
# The compiled plugin and "plugin.json" are automatically added.
DISTRIBUTABLES += res
DISTRIBUTABLES += $(wildcard LICENSE*)
DISTRIBUTABLES += $(wildcard presets)

# Include the Rack plugin Makefile framework
include $(RACK_DIR)/plugin.mk

# Install directories
RACK_USER_DIR ?= $(HOME)/.local/share/Rack2
RACK_PRO_DIR ?= $(HOME)/.local/share/VCV/Rack2Pro

INSTALL_DIR1 ?= $(RACK_USER_DIR)/plugins-lin-x64
INSTALL_DIR2 ?= $(RACK_PRO_DIR)/plugins

install: all
	mkdir -p "$(INSTALL_DIR1)" "$(INSTALL_DIR2)"
	cp -r dist/* "$(INSTALL_DIR1)"
	cp -r dist/* "$(INSTALL_DIR2)"

