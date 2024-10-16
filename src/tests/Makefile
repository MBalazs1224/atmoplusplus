# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++14 -g -I/usr/include/gtest -I/usr/include

# Google Test libraries
GTEST_LIBS = -lgtest -lgmock -lgtest_main -pthread

# Source files
SRCS = main.cc ../lexer/AtmoLexer.cc ../lexer/lex.yy.cc ../error/error.cc

# Object files
OBJS = ./build/main.o ./build/atmo_lexer.o ./build/lexer.o ./build/error.o

# Executable name
EXEC = ../../runTests

# Default target
all: $(EXEC)

# Link the executable
$(EXEC): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(GTEST_LIBS)

# Compile source files into object files
./build/main.o: main.cc lexer_tests.hh
	@mkdir -p ./build
	$(CXX) $(CXXFLAGS) -c $< -o $@

./build/atmo_lexer.o: ../lexer/AtmoLexer.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

./build/lexer.o: ../lexer/lex.yy.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

./build/error.o: ../error/error.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(EXEC)

# Phony targets
.PHONY: all clean