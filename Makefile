#Qayam Damji 2024

# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++20 -O3 -march=native -Wall -Wextra -pedantic

# Target executable
TARGET = benchmark

# Source files
SRCS = benchmark.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(TARGET)

# Link the object files to create the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Compile the source files into object files
%.o: %.cpp eytzinger.hpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(TARGET) *.o search_benchmark.csv

# Run the benchmark
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run