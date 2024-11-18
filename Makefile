# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++17 -g

SRC_DIR = src
BUILD_DIR = build
IMG_DIR = img

# Linker flags (add libraries here)
LDFLAGS = -lriv

# Library paths (optional)
LIBPATHS = -L/usr/local/lib -L./libs

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

# Object files
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Sprite files
ASE = $(wildcard $(IMG_DIR)/*.ase)

# Sprite processor
LIBRESPRITE = /Applications/libresprite.app/Contents/MacOS/libresprite
JQ = jq

# Executable name
EXECUTABLE = rush

# Sprint Sheet
SPRITE_IMAGE = $(IMG_DIR)/spritesheet.png
SPRITE_DATA = $(IMG_DIR)/spritesheet.txt

# Default target
all: $(BUILD_DIR) $(SPRITE_DATA) $(EXECUTABLE)

sprite: $(SPRITE_DATA)

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Rule to create the executable
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LIBPATHS) $(LDFLAGS) -o $@

# Rules to create the sprite sheet and data file
$(SPRITE_IMAGE): $(ASE)
	$(LIBRESPRITE) --batch $(ASE) --sheet $@ --sheet-width 256 --sheet-height 256 --sheet-pack --data $(basename $@).json --format json-array

$(SPRITE_DATA): $(SPRITE_IMAGE)
	$(JQ) -r '.frames[] | (.filename + " " + (.frame | "\(.x) \(.y) \(.w) \(.h)"))' $(basename $@).json > $@

# Rule to create object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -rf $(BUILD_DIR)
	rm -f $(EXECUTABLE)
	rm -f $(SPRITE_IMAGE)
	rm -f $(SPRITE_DATA)

# Phony targets
.PHONY: all clean sprite
