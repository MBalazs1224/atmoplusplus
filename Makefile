objects:= ./build/main.o ./build/parser.tab.o ./build/lex.yy.o ./build/symboltable.o

CXXFLAGS = -g -Wpedantic -Wextra -Wall


main: $(objects)
	g++ $(CXXFLAGS) $^ -o $@
	
./build/main.o: main.cc AtmoLexer.hh parser.tab.hh
	g++ $(CXXFLAGS) -c $< -o $@
	
./build/lex.yy.o: lex.yy.cc AtmoLexer.hh parser.tab.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/symboltable.o: ./src/symboltable/symboltable.cc ./src/symboltable/symboltable.hh 
	g++ $(CXXFLAGS) -c $< -o $@

./build/parser.tab.o: parser.tab.cc AtmoLexer.hh
ifdef DEBUG
	g++ $(CXXFLAGS) -DYYDEBUG=1 -c $< -o $@
endif
ifndef DEBUG
	g++ $(CXXFLAGS) -c $< -o $@
endif

lex.yy.cc: lexer.ll
ifdef DEBUG
	flex -d -+ $<
endif
ifndef DEBUG
	flex -+ $<
endif
	
	
parser.tab.cc: parser.yy
	bison -d $(DEBUG) $<

parser.tab.hh: parser.yy
	bison -d -t $(DEBUG) $<

debug:
	make clean
	make DEBUG="--debug"
	
	
.PHONY: rebuild
rebuild:
	make clean
	make
	
.PHONY: clean
clean:
	rm -rf $(objects)
	rm -f lex.yy.cc
	rm -f parser.tab.cc
	rm -f parser.tab.hh
	rm -f main

# https://makefiletutorial.com00
