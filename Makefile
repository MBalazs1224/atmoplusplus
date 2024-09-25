
nodes := ./build/statement_list_node.o ./build/body_node.o 

symbols := ./build/symbols/symbolvariable.o ./build/symbols/symbolfunction.o ./build/symbols/symbolclass.o

literals := ./build/literal.o

expressions := ./build/expressions/add_expression.o ./build/expressions/and_expression.o ./build/expressions/assignment_expression.o ./build/expressions/divide_expression.o ./build/expressions/expressionable.o ./build/expressions/function_call.o ./build/expressions/greater_than_expression.o ./build/expressions/greater_than_or_equal_expression.o ./build/expressions/identifier.o  ./build/expressions/less_than_expression.o ./build/expressions/less_than_or_equal_expression.o ./build/expressions/matches_expression.o ./build/expressions/member_access_expression.o ./build/expressions/multiply_expression.o ./build/expressions/not_expression.o ./build/expressions/not_matches_expression.o ./build/expressions/or_expression.o ./build/expressions/subtract_expression.o

attributes := ./build/attribute.o ./build/attribute_private.o ./build/attribute_protected.o ./build/attribute_public.o ./build/attribute_static.o

types := ./build/types/typearray.o ./build/types/type.o 


helper := ./build/helper.o



objects := ./build/main.o  ./build/lex.yy.o ./build/symboltable.o ./build/symboltableelement.o ./build/atmo_driver.o ./build/parser.tab.o ./build/scope.o ./build/error.o ./build/ilocation.o $(nodes) $(literals) $(expressions) $(attributes) $(helper) $(symbols) $(types)

#CXXFLAGS = -g -Wpedantic -Wextra -Wall -fsanitize=address
CXXFLAGS = -g -Wpedantic -Wextra -Wall -Werror
#LDFLAGS = -static-libasan


main: $(objects)
	g++ $(CXXFLAGS) $^ -o $@
	
./build/main.o: main.cc AtmoLexer.hh parser.tab.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/atmo_driver.o: atmo_driver.cc atmo_driver.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/ilocation.o: ./src/location/ilocation.cc ./src/location/ilocation.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/scope.o: ./src/symboltable/scope.cc ./src/symboltable/scope.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/error.o: ./src/error/error.cc ./src/error/error.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/lex.yy.o: lex.yy.cc AtmoLexer.hh parser.tab.hh
	g++ $(CXXFLAGS) -c $< -o $@


./build/attribute.o: ./src/ast/attributes/attribute.cc ./src/ast/attributes/attribute.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/literal.o: ./src/ast/literals/literal.cc ./src/ast/literals/literal.hh
	g++ $(CXXFLAGS) -c $< -o $@


./build/statement_list_node.o: ./src/ast/nodes/statement_list_node.cc ./src/ast/nodes/statement_list_node.hh
	g++ $(CXXFLAGS) -c $< -o $@


./build/body_node.o: ./src/ast/nodes/body_node.cc ./src/ast/nodes/body_node.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/symboltable.o: ./src/symboltable/symboltable.cc ./src/symboltable/symboltable.hh 
	g++ $(CXXFLAGS) -c $< -o $@

./build/symboltableelement.o: ./src/symboltable/symboltableelement.cc ./src/symboltable/symboltableelement.hh 
	g++ $(CXXFLAGS) -c $< -o $@

# Symbols

./build/symbols/symbolvariable.o : ./src/symboltable/symbols/symbolvariable.cc ./src/symboltable/symbols/symbolvariable.cc
#	Symbolvariable will be compiled first, so it needs to check if the symbol folder exists
	@mkdir -p ./build/symbols
	g++ $(CXXFLAGS) -c $< -o $@

./build/symbols/symbolfunction.o : ./src/symboltable/symbols/symbolfunction.cc ./src/symboltable/symbols/symbolfunction.cc
	g++ $(CXXFLAGS) -c $< -o $@

./build/symbols/symbolclass.o : ./src/symboltable/symbols/symbolclass.cc ./src/symboltable/symbols/symbolclass.cc
	g++ $(CXXFLAGS) -c $< -o $@


# Expressions


./build/expressions/add_expression.o: ./src/ast/expressions/add_expression.cc ./src/ast/expressions/add_expression.hh
# Add expression will be compiled first, so it needs to check if the expressions folder exists
	@mkdir -p ./build/expressions
	g++ $(CXXFLAGS) -c $< -o $@

./build/expressions/and_expression.o: ./src/ast/expressions/and_expression.cc ./src/ast/expressions/and_expression.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/expressions/assignment_expression.o: ./src/ast/expressions/assignment_expression.cc ./src/ast/expressions/assignment_expression.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/expressions/divide_expression.o: ./src/ast/expressions/divide_expression.cc ./src/ast/expressions/divide_expression.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/expressions/expressionable.o: ./src/ast/expressions/expressionable.cc ./src/ast/expressions/expressionable.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/expressions/function_call.o: ./src/ast/expressions/function_call.cc ./src/ast/expressions/function_call.hh
	g++ $(CXXFLAGS) -c $< -o $@


./build/expressions/greater_than_expression.o: ./src/ast/expressions/greater_than_expression.cc ./src/ast/expressions/greater_than_expression.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/expressions/greater_than_or_equal_expression.o: ./src/ast/expressions/greater_than_or_equal.cc ./src/ast/expressions/greater_than_or_equal.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/expressions/identifier.o: ./src/ast/expressions/identifier.cc ./src/ast/expressions/identifier.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/expressions/less_than_expression.o: ./src/ast/expressions/less_than_expression.cc ./src/ast/expressions/less_than_expression.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/expressions/less_than_or_equal_expression.o: ./src/ast/expressions/less_than_or_equal.cc ./src/ast/expressions/less_than_or_equal.hh
	g++ $(CXXFLAGS) -c $< -o $@


./build/expressions/matches_expression.o: ./src/ast/expressions/matches_expression.cc ./src/ast/expressions/matches_expression.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/expressions/member_access_expression.o: ./src/ast/expressions/member_access_expression.cc ./src/ast/expressions/member_access_expression.hh
	g++ $(CXXFLAGS) -c $< -o $@
	

./build/expressions/multiply_expression.o: ./src/ast/expressions/multiply_expression.cc ./src/ast/expressions/multiply_expression.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/expressions/not_expression.o: ./src/ast/expressions/not_expression.cc ./src/ast/expressions/not_expression.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/expressions/not_matches_expression.o: ./src/ast/expressions/not_matches_expression.cc ./src/ast/expressions/not_matches_expression.hh
	g++ $(CXXFLAGS) -c $< -o $@


./build/expressions/or_expression.o: ./src/ast/expressions/or_expression.cc ./src/ast/expressions/or_expression.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/expressions/subtract_expression.o: ./src/ast/expressions/subtract_expression.cc ./src/ast/expressions/subtract_expression.hh
	g++ $(CXXFLAGS) -c $< -o $@

# Attributes

./build/attribute_private.o: ./src/ast/attributes/attributeprivate.cc ./src/ast/attributes/attributeprivate.hh 
	g++ $(CXXFLAGS) -c $< -o $@

./build/attribute_protected.o: ./src/ast/attributes/attributeprotected.cc ./src/ast/attributes/attributeprotected.hh 
	g++ $(CXXFLAGS) -c $< -o $@

./build/attribute_public.o: ./src/ast/attributes/attributepublic.cc ./src/ast/attributes/attributepublic.hh 
	g++ $(CXXFLAGS) -c $< -o $@

./build/attribute_static.o: ./src/ast/attributes/attributestatic.cc ./src/ast/attributes/attributestatic.hh 
	g++ $(CXXFLAGS) -c $< -o $@

# Types

./build/types/typearray.o: ./src/ast/types/typearray.cc ./src/ast/types/typearray.hh
	@mkdir -p ./build/types
	g++ $(CXXFLAGS) -c $< -o $@

./build/types/type.o: ./src/ast/types/type.cc ./src/ast/types/type.hh
	@mkdir -p ./build/types
	g++ $(CXXFLAGS) -c $< -o $@

# Helper

./build/helper.o: ./src/helper/helper.cc ./src/helper/helper.hh 
	g++ $(CXXFLAGS) -c $< -o $@

	
./build/parser.tab.o: parser.tab.cc AtmoLexer.hh
	g++ $(CXXFLAGS) -c $< -o $@

lex.yy.cc: lexer.ll
	flex -d -+ $<	
	
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
