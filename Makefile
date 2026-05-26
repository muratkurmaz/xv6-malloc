CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -pedantic -Iinclude
BUILD_DIR := build
BIN_DIR := bin

ALLOCATOR_OBJ := $(BUILD_DIR)/memory_management.o

.PHONY: all demo test clean

all: demo

$(BUILD_DIR) $(BIN_DIR):
	mkdir -p $@

$(ALLOCATOR_OBJ): src/memory_management.c include/memory_management.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR)/demo: examples/demo.c $(ALLOCATOR_OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(BIN_DIR)/test_allocator: tests/test_allocator.c $(ALLOCATOR_OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

demo: $(BIN_DIR)/demo
	./$(BIN_DIR)/demo

test: $(BIN_DIR)/test_allocator
	./$(BIN_DIR)/test_allocator

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
