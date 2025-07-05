# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -c -Wall -g -I/usr/include/mysql-cppconn

# Linker flags
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lmysqlcppconn -lcurl

# Source files
SRCS = main.cpp parse_input.cpp room.cpp handle_action.cpp player.cpp monster.cpp trap.cpp chest.cpp dungeon.cpp game.cpp api_client.cpp login.cpp auth.cpp

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

# Test the program for memory errors with Valgrind
mem: $(TARGET)
	valgrind --leak-check=full --track-origins=yes ./$(TARGET)

# Clean up the build files
clean:
	rm -f $(OBJS) $(TARGET)

# Debug the program with gdb
gdb: $(TARGET)
	gdb ./$(TARGET)

.PHONY: all clean run gdb mem
