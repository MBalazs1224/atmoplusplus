objects:= ./build/main.o ./build/lex.yy.o ./build/testParser.tab.o
all: $(objects)
	g++ $^ -o $@ -o main.exe
	
./build/main.o: main.cc TestLexer.hh testParser.tab.hh
	g++ -c $< -o $@
	
./build/lex.yy.o: lex.yy.cc TestLexer.hh testParser.tab.hh
	g++ -c $< -o $@

./build/testParser.tab.o: testParser.tab.cc FooLexer.hh
	g++ -c $< -o $@

lex.yy.cc: testLexer.ll
	flex $<
	
testParser.tab.cc testParser.tab.hh: testParser.yy
	bison $<

.PHONY: clean
clean:
	del /Q /S "$(objects)"
	
# https://makefiletutorial.com