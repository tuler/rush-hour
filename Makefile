# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++17 -g

SRC_DIR = src
BUILD_DIR = build

# Linker flags (add libraries here)
LDFLAGS = -lriv

# Library paths (optional)
LIBPATHS = -L/usr/local/lib -L./libs

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Executable name
EXECUTABLE = rush

# Default target
all: $(BUILD_DIR) $(EXECUTABLE)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Rule to create the executable
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LIBPATHS) $(LDFLAGS) -o $@

# Rule to create object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(EXECUTABLE)

# Phony targets
.PHONY: all clean
