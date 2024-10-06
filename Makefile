# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++11

# Linker flags (add -lncurses to link the ncurses library)
LDFLAGS = -lncurses

# Target executable
TARGET = game

# Source files
SRC = Node.cpp Grid.cpp Stack.cpp game.cpp

# Object files (corresponding to each .cpp file)
OBJ = $(SRC:.cpp=.o)

# Default target: Compile and link the program
all: $(TARGET)

# Rule to link object files and create the executable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

# Rule to compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up object files and the executable
clean:
	rm -f $(OBJ) $(TARGET)
