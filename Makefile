# === Directories ===
SRC_DIR := src
INC_DIR := include
OBJ_DIR := build

# === Compiler & Flags ===
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I$(INC_DIR)

# === Source and Object Files ===
CPP_FILES := $(shell find $(SRC_DIR) -name '*.cpp')
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(CPP_FILES))

# === Output Executable ===
TARGET := $(OBJ_DIR)/optio

# === Default Rule ===
all: $(TARGET)

# === Linking ===
$(TARGET): $(OBJ_FILES)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $^ -o $@

# === Compiling ===
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# === Clean ===
clean:
	rm -rf $(OBJ_DIR)

.PHONY: all clean
