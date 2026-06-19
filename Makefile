# Compiler settings
CXX = g++
# Debugging flags: -g adds debug symbols, -O0 disables all optimizations
CXXFLAGS = -std=c++17 -Wall -Wextra -g -O0

TARGET = chess

# Tell Make to look in the root folder AND the 'engine' subdirectory
SRCS = $(wildcard *.cpp) $(wildcard engine/*.cpp)

OBJS = $(SRCS:.cpp=.o)

# Default rule
all: $(TARGET)

# Rule to link object files into the final executable
# The 'rm -f $(OBJS)' command at the end deletes the intermediate files immediately after linking
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	rm -f $(OBJS)

# Rule to compile every .cpp file into a .o object file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean rule just in case you ever need to manually clear the executable
clean:
	rm -f $(TARGET)