# Compiler
CXX = g++
CC  = gcc

# Flags
CXXFLAGS = -std=c++23 -Wall -Wextra -I./src/include
CFLAGS   = -Wall -Wextra -I./src/include

# Libraries
LIBS = -lglfw -lGL -lm -ldl

# Directories
SRC_DIRS = src src/functions
BUILD_DIR = build
BIN_DIR   = bin

# Source files
CPP_SOURCES = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))
C_SOURCES   = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))

# Object files
CPP_OBJECTS = $(patsubst %,$(BUILD_DIR)/%,$(notdir $(CPP_SOURCES:.cpp=.o)))
C_OBJECTS   = $(patsubst %,$(BUILD_DIR)/%,$(notdir $(C_SOURCES:.c=.o)))
OBJECTS = $(CPP_OBJECTS) $(C_OBJECTS)

# Target
TARGET = $(BIN_DIR)/flight_simulator

# Default build
all: directories $(TARGET)

# Create necessary directories
directories:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)

# Link object files into executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LIBS)
	@echo "Build complete: $(TARGET)"

# Compile C++ source files
$(BUILD_DIR)/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: src/functions/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile C source files
$(BUILD_DIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: src/functions/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Run the application
run: all
	./$(TARGET)

# Clean build files
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "Clean complete"

# Rebuild
rebuild: clean all

# Help
help:
	@echo "Available targets:"
	@echo "  all      - Build the project (default)"
	@echo "  run      - Build and run the application"
	@echo "  clean    - Remove build files"
	@echo "  rebuild  - Clean and build"
	@echo "  help     - Show this help message"

.PHONY: all directories run clean rebuild help
