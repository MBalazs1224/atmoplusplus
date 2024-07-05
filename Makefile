objects:= ./build/main.o ./build/lex.yy.o ./build/parser.tab.o




main.exe: $(objects)
	g++ $^ -o $@
	
./build/main.o: main.cc AtmoLexer.hh parser.tab.hh
	g++ -c $< -o $@
	
./build/lex.yy.o: lex.yy.cc AtmoLexer.hh parser.tab.hh
	g++ -c $< -o $@

./build/parser.tab.o: parser.tab.cc AtmoLexer.hh
ifdef DEBUG
	g++ -DYYDEBUG=1 -c $< -o $@
endif
ifndef DEBUG
	g++ -c $< -o $@
endif

lex.yy.cc: lexer.ll
ifdef DEBUG
	win_flex -d -+ $<
endif
ifndef DEBUG
	win_flex -+ $<
endif
	
	
parser.tab.cc: parser.yy
	win_bison $(DEBUG) $<

parser.tab.hh: parser.yy
	win_bison -t $(DEBUG) $<

debug:
	make clean
	make DEBUG="--debug"
	
	
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