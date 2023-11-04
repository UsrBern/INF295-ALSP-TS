CXXFLAGS = -std=c++20 -Wall -Wextra

# Directories
SRC_DIR = src
INC_DIR = include
OBJ_DIR = src/obj

# Name of the executable
EXEC = ALSP

# Source and object files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Rule to link the executable
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

# Rule to create the obj directory
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Rule to clean intermediate files
.PHONY: clean
clean:
	rm -f $(OBJ_DIR)/*.o $(EXEC)