# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -Iinclude -Wall -Wextra -O2 `sdl2-config --cflags`
LDFLAGS := `sdl2-config --libs` -lSDL2_ttf -lSDL2_image 
# Directories
SRC_DIR := src
BUILD_DIR := build
INCLUDE_DIR := include

# Files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))
MAIN := main.cpp
TARGET := $(BUILD_DIR)/simulation

# Default target
all: $(TARGET)

# Link final binary
$(TARGET): $(OBJS) $(BUILD_DIR)/main.o
	$(CXX) $^ -o $@ $(LDFLAGS)

# Compile main.cpp
$(BUILD_DIR)/main.o: $(MAIN)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile other .cpp files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the built binary
run: $(TARGET)
	./$(TARGET)

# Clean up build files
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all run clean
