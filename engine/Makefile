# Makefile for Termesh project

# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -I./include
TESTFLAGS = -std=c++17 -Wall -Wextra -g -I./include

# Directories
INCLUDE_DIR = include
TEST_DIR = tests
BUILD_DIR = build
TEST_BUILD_DIR = build/tests

# Source files (excluding main.cpp for tests)
CPP_FILES = $(wildcard *.cpp)
SOURCES = $(filter-out main.cpp, $(CPP_FILES))
OBJECTS = $(SOURCES:%.cpp=$(BUILD_DIR)/%.o)
MAIN_OBJECT = $(BUILD_DIR)/main.o

# Test files
TEST_SOURCES = $(wildcard $(TEST_DIR)/*.cpp)
TEST_OBJECTS = $(TEST_SOURCES:$(TEST_DIR)/%.cpp=$(TEST_BUILD_DIR)/%.o)
TEST_EXECUTABLES = $(TEST_SOURCES:$(TEST_DIR)/test_%.cpp=$(TEST_BUILD_DIR)/test_%)

# Main executable
MAIN_TARGET = $(BUILD_DIR)/stl_renderer

# Default target
all: $(MAIN_TARGET)

# Create build directories
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(TEST_BUILD_DIR)

# Build main executable
$(MAIN_TARGET): $(MAIN_OBJECT) $(OBJECTS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lm

# Compile source files
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build tests
tests: $(TEST_EXECUTABLES)

# Build individual test executables
$(TEST_BUILD_DIR)/test_%: $(TEST_DIR)/test_%.cpp $(filter-out $(BUILD_DIR)/main.o, $(OBJECTS)) | $(BUILD_DIR)
	$(CXX) $(TESTFLAGS) -o $@ $^ -lm

# Run all tests
test: tests
	@echo "Running all tests..."
	@for test in $(TEST_EXECUTABLES); do \
		echo "\n=== Running $$(basename $$test) ==="; \
		$$test || exit 1; \
	done
	@echo "\n✅ All tests passed!"

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Clean test files only
clean-tests:
	rm -rf $(TEST_BUILD_DIR)

# Build for WASM (requires Emscripten)
wasm: setup.sh
	@echo "Building WASM version..."
	@if [ -f /etc/profile.d/emscripten.sh ]; then \
		bash setup.sh; \
	else \
		echo "Error: Emscripten not found. Please set up Emscripten first."; \
		exit 1; \
	fi

# Install dependencies (if needed)
install-deps:
	@echo "No external dependencies required for native build."
	@echo "For WASM build, install Emscripten:"
	@echo "  https://emscripten.org/docs/getting_started/downloads.html"

# Help target
help:
	@echo "Available targets:"
	@echo "  all          - Build main executable"
	@echo "  tests        - Build all test executables"
	@echo "  test         - Build and run all tests"
	@echo "  clean        - Remove all build artifacts"
	@echo "  clean-tests  - Remove only test build artifacts"
	@echo "  wasm         - Build WASM version (requires Emscripten)"
	@echo "  help         - Show this help message"

.PHONY: all tests test clean clean-tests wasm install-deps help

