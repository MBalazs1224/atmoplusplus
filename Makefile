objects:= ./build/main.o ./build/lex.yy.o ./build/parser.tab.o

all: $(objects)
	g++ $^ -o $@ -o main.exe
	
./build/main.o: main.cc AtmoLexer.hh parser.tab.hh
	g++ -c $< -o $@
	
./build/lex.yy.o: lex.yy.cc AtmoLexer.hh parser.tab.hh
	g++ -c $< -o $@

./build/parser.tab.o: parser.tab.cc AtmoLexer.hh
	g++ -c $< -o $@

lex.yy.cc: lexer.ll
	win_flex -+ $<
	
parser.tab.cc: parser.yy
	win_bison $<

parser.tab.hh: parser.yy
	win_bison -d $<
	
	
.PHONY: rebuild
rebuild:
	make clean
	make
	
.PHONY: clean
clean:
	del /Q /S build
	del /Q lex.yy.cc
	del /Q parser.tab.cc
	del /Q parser.tab.hh
	del /Q main.exe
	
# https://makefiletutorial.com00