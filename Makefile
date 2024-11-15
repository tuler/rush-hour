# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++17 -g

# Linker flags (add libraries here)
LDFLAGS = -lriv

# Library paths (optional)
LIBPATHS = -L/usr/local/lib -L./libs

# Source files
SOURCES = board.cpp piece.cpp main.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Executable name
EXECUTABLE = rush

# Default target
all: $(EXECUTABLE)

# Rule to create the executable
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LIBPATHS) $(LDFLAGS) -o $(EXECUTABLE)

# Rule to create object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up
clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

# Phony targets
.PHONY: all clean
