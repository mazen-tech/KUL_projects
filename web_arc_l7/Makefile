# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -g

# Source files
SRC = Channel.cpp Chat.cpp User.cpp main.cpp

# Object files
OBJ = $(SRC:.cpp=.o)

# Output executable
TARGET = chat_server

# Default rule to compile and link everything
all: $(TARGET)

# Rule to create the executable
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

# Rule to compile .cpp files into .o files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up compiled files and the executable
clean:
	rm -f $(OBJ) $(TARGET)

# Rule to clean and rebuild
rebuild: clean all

# Rule to run the program
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean rebuild run
