# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -c -Wall

# Linker flags
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Source files
SRCS = main.cpp parse_input.cpp room.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable
TARGET = sfml-app

# Default target
all: $(TARGET)

# Link the object files to create the executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile the source files to create the object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean up the build files
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean run
