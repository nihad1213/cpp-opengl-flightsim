CXX = g++
CC = gcc
CXXFLAGS = -std=c++23 -Wall -Wextra -I./src/include
CFLAGS = -Wall -Wextra -I./src/include

LIBS = -lglfw -lGL -lm -ldl

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

CPP_SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
C_SOURCES = $(wildcard $(SRC_DIR)/*.c)

CPP_OBJECTS = $(CPP_SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
C_OBJECTS = $(C_SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
OBJECTS = $(CPP_OBJECTS) $(C_OBJECTS)

TARGET = $(BIN_DIR)/flight_simulator

all: directories $(TARGET)

directories:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LIBS)
	@echo "Build complete: $(TARGET)"

# Compile C++ source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile C source files (for glad.c)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
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