ROOT_DIR := .
BUILD_DIR := $(ROOT_DIR)/build

define cmake_build
	@cmake --build $(BUILD_DIR) -j $(shell nproc) --target $(1)
endef

.PHONY: configure
configure:
	@cmake -S $(ROOT_DIR) -B $(BUILD_DIR)

.PHONY: all
all: configure
	$(call cmake_build,all)

.PHONY: run
run: configure
	$(call cmake_build,core)
	@$(BUILD_DIR)/core/core
