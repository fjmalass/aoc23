# Compiler
# CXX = clang++
# CXXFlags = -Wall -Wextra -std=c++2b 
CXX = g++
CXXFlags = -Wall -std=c++23 
CXXOpt = -O2

# Executable
EXECUTABLE = aoc
# Sources
SOURCES = aoc.cpp

# Objects
OBJ = $(SOURCES:.cpp=.o)

# DEBUG
debug: CXXFlags += -DDEBUG
debug: CXXOpt = 
debug: $(EXECUTABLE)
	./$(EXECUTABLE)


# Making the target
$(EXECUTABLE): $(OBJ)
	$(CXX) $(CXXFlags) $(CXXOpt) -o $(EXECUTABLE) $(OBJ)

%.o: %.cpp %.h
	$(CXX) $(CXXFlags) $(CXXOpt) -c $< -o $@


all: $(EXECUTABLE)

clean: 
	rm -f $(OBJ) $(EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE)


# Clean
.PHONY: all clean run 


