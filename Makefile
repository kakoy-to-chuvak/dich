SRC_DIR=src
BUILD_DIR=build
TARGET_EXEC=main

SRCS=$(wildcard $(SRC_DIR)/*.c)
HEADERS=$(SRCS:%.c=%.h)


$(BUILD_DIR)/$(TARGET_EXEC): $(SRCS) $(HEADERS)
	@echo on
	@echo $(SRCS)
	@echo $(HEADERS)
	gcc "$(SRC_DIR)/*.c" -o "$(BUILD_DIR)/$(TARGET_EXEC)" -L C:\\SDL\\x86_64-w64-mingw32\\lib -I C:\\SDL\\x86_64-w64-mingw32\\include -lSDL3_ttf -lSDL3_image -lSDL3