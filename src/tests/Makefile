# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++14 -g -I/usr/include/gtest -I/usr/include

# Google Test libraries
GTEST_LIBS = -lgtest -lgmock -lgtest_main -pthread

# Source files
SRCS = main.cc ../lexer/AtmoLexer.cc ../lexer/lex.yy.cc ../error/error.cc

BUILD_DIR = ../../build

# Object files
OBJS = ../../build/test_main.o $(wildcard ../../build/attributes/*.o) $(wildcard ../../build/driver/*.o) $(wildcard ../../build/error/*.o) $(wildcard ../../build/expressions/*.o) $(wildcard ../../build/helper/*.o) $(wildcard ../../build/lexer/*.o) $(wildcard ../../build/literals/*.o) $(wildcard ../../build/location/*.o) $(wildcard ../../build/nodes/*.o) $(wildcard ../../build/parser/*.o) $(wildcard ../../build/scope/*.o) $(wildcard ../../build/symbols/*.o) $(wildcard ../../build/symboltable/*.o) $(wildcard ../../build/types/*.o) $(wildcard ../../build/ir/*/*.o) $(wildcard ../../build/frame/*.o) $(wildcard ../../build/translate/*.o)

# Executable name
EXEC = ../../runTests

# Default target
all: $(EXEC)

# Link the executable
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(GTEST_LIBS)

# Compile source files into object files
../../build/test_main.o: main.cc lexer_tests.hh parser_tests.hh
	@mkdir -p ./build
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f ../../build/test_main.o $(EXEC)

# Phony targets
.PHONY: all clean