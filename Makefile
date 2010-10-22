TARGET ?= XC-2
BASE_DIR = .

DEMO_SOURCE_DIRS = src \
		           src/misc \
                   src/config \
                   src/dumper \
		   module_xtcp::src/xtcp_apps/mdns

SOURCE_DIRS += $(DEMO_SOURCE_DIRS)

INCLUDE_DIRS += $(DEMO_SOURCE_DIRS) 

XCC_FLAGS += -Xmapper --map -Xmapper map.out -DMDNS_NETBIOS=1 -O3 -save-temps -g --show-report -DXC2_FIRMWARE_VERSION=1v3

EXCLUDE_FILES = httpd-fsdata.c app.c fsdata.c fsdata_old.c

USED_MODULES = module_xtcp.1v3 module_ethernet.1v3

include $(BASE_DIR)/../module_xmos_common.1v0/build/Makefile.common

all: $(BIN_DIR)/xc2_firmware.xe
	@echo Build complete

clean: clean_common
