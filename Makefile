
nodes := ./build/nodes/statement_list_node.o ./build/nodes/body_node.o ./build/nodes/variable_definition_node.o ./build/nodes/class_definition_node.o ./build/nodes/constructor_definition_node.o ./build/nodes/destructor_definition_node.o ./build/nodes/do_until_statement_node.o ./build/nodes/else_if_statement_node.o  ./build/nodes/else_statement_node.o ./build/nodes/function_definition_node.o ./build/nodes/if_statement_node.o ./build/nodes/return_statement_node.o ./build/nodes/until_statement_node.o

symbols := ./build/symbols/symbolvariable.o ./build/symbols/symbolfunction.o ./build/symbols/symbolclass.o

expressions := ./build/expressions/add_expression.o ./build/expressions/and_expression.o ./build/expressions/assignment_expression.o ./build/expressions/divide_expression.o ./build/expressions/expressionable.o ./build/expressions/function_call.o ./build/expressions/greater_than_expression.o ./build/expressions/greater_than_or_equal_expression.o ./build/expressions/identifier.o  ./build/expressions/less_than_expression.o ./build/expressions/less_than_or_equal_expression.o ./build/expressions/matches_expression.o ./build/expressions/member_access_expression.o ./build/expressions/multiply_expression.o ./build/expressions/not_expression.o ./build/expressions/not_matches_expression.o ./build/expressions/or_expression.o ./build/expressions/subtract_expression.o ./build/expressions/two_operand_expression.o ./build/expressions/one_operand_expression.o ./build/expressions/array_subscript_expression.o

attributes := ./build/attributes/attribute.o ./build/attributes/attribute_private.o ./build/attributes/attribute_protected.o ./build/attributes/attribute_public.o ./build/attributes/attribute_static.o

types := ./build/types/typearray.o ./build/types/type.o  ./build/types/typeboolean.o  ./build/types/typeinteger.o ./build/types/typefloat.o ./build/types/typestring.o ./build/types/typevoid.o ./build/types/typechar.o  


helper := ./build/helper/helper.o

lexer := ./build/lexer/atmolexer.o ./build/lexer/lex.yy.o

driver := ./build/driver/atmo_driver.o

main := ./build/main/main.o

parser := ./build/parser/parser.tab.o

scope := ./build/scope/scope.o

error := ./build/error/error.o

ilocation := ./build/location/ilocation.o

symboltable:= ./build/symboltable/symboltable.o ./build/symboltable/symboltableelement.o

objects :=   $(main)   $(parser) $(scope) $(error) $(ilocation) $(lexer) $(driver) $(symboltable) $(nodes) $(literals) $(expressions) $(attributes) $(helper) $(symbols) $(types)

#CXXFLAGS = -g -Wpedantic -Wextra -Wall -fsanitize=address
CXXFLAGS = -g3 -Wno-deprecated -pipe -fno-elide-type -fdiagnostics-show-template-tree -Wall -Werror -Wextra -Wpedantic -Wvla -Wextra-semi -Wnull-dereference -fvar-tracking-assignments -Wduplicated-cond -Wduplicated-branches -rdynamic -Wsuggest-override -O0 -Wno-overloaded-virtual
#LDFLAGS = -static-libasan



# Main

main: $(objects)
	g++ $(CXXFLAGS) $^ -o $@
# Echo new line
	@echo "" 
	@echo "Building successful, you can run the program by typing ./main"

./build/main/main.o: ./src/main.cc ./src/lexer/AtmoLexer.hh ./src/parser/parser.tab.hh
	@mkdir -p ./build/main
	g++ $(CXXFLAGS) -c $< -o $@

# Lexer

./build/lexer/atmolexer.o: ./src/lexer/AtmoLexer.cc ./src/lexer/AtmoLexer.hh ./src/parser/parser.tab.hh ./src/lexer/lex.yy.cc
	@mkdir -p ./build/lexer
	g++ $(CXXFLAGS) -c $< -o $@


./build/lexer/lex.yy.o: ./src/lexer/lex.yy.cc ./src/lexer/AtmoLexer.hh ./src/parser/parser.tab.hh
	g++ $(CXXFLAGS) -c $< -o $@


./src/lexer/lex.yy.cc: ./src/lexer/lexer.ll
	flex -o $@ -d -+ $< 

# Driver


./build/driver/atmo_driver.o: ./src/driver/atmo_driver.cc ./src/driver/atmo_driver.hh
	@mkdir -p ./build/driver
	g++ $(CXXFLAGS) -c $< -o $@

# Location

./build/location/ilocation.o: ./src/location/ilocation.cc ./src/location/ilocation.hh
	@mkdir -p ./build/location
	g++ $(CXXFLAGS) -c $< -o $@

# Scope

./build/scope/scope.o: ./src/symboltable/scope.cc ./src/symboltable/scope.hh
	@mkdir -p ./build/scope
	g++ $(CXXFLAGS) -c $< -o $@

# Error

./build/error/error.o: ./src/error/error.cc ./src/error/error.hh
	@mkdir -p ./build/error
	g++ $(CXXFLAGS) -c $< -o $@








# Symboltable

./build/symboltable/symboltable.o: ./src/symboltable/symboltable.cc ./src/symboltable/symboltable.hh
	@mkdir -p ./build/symboltable
	g++ $(CXXFLAGS) -c $< -o $@

./build/symboltable/symboltableelement.o: ./src/symboltable/symboltableelement.cc ./src/symboltable/symboltableelement.hh 
	g++ $(CXXFLAGS) -c $< -o $@

# Nodes

./build/nodes/statement_list_node.o: ./src/ast/nodes/statement_list_node.cc ./src/ast/nodes/statement_list_node.hh
	@mkdir -p ./build/nodes
	g++ $(CXXFLAGS) -c $< -o $@


./build/nodes/body_node.o: ./src/ast/nodes/body_node.cc ./src/ast/nodes/body_node.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/nodes/variable_definition_node.o: ./src/ast/nodes/variable_definition_node.cc ./src/ast/nodes/variable_definition_node.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/nodes/class_definition_node.o: ./src/ast/nodes/class_definition_node.cc ./src/ast/nodes/class_definition_node.hh
	g++ $(CXXFLAGS) -c $< -o $@


./build/nodes/constructor_definition_node.o: ./src/ast/nodes/constructor_definition_node.cc ./src/ast/nodes/constructor_definition_node.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/nodes/destructor_definition_node.o: ./src/ast/nodes/destructor_definition_node.cc ./src/ast/nodes/destructor_definition_node.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/nodes/do_until_statement_node.o: ./src/ast/nodes/do_until_statement_node.cc ./src/ast/nodes/do_until_statement_node.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/nodes/else_if_statement_node.o: ./src/ast/nodes/else_if_statement_node.cc ./src/ast/nodes/else_if_statement_node.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/nodes/else_statement_node.o: ./src/ast/nodes/else_statement_node.cc ./src/ast/nodes/else_statement_node.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/nodes/function_definition_node.o: ./src/ast/nodes/function_definition_node.cc ./src/ast/nodes/function_definition_node.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/nodes/if_statement_node.o: ./src/ast/nodes/if_statement_node.cc ./src/ast/nodes/if_statement_node.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/nodes/return_statement_node.o: ./src/ast/nodes/return_statement_node.cc ./src/ast/nodes/return_statement_node.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/nodes/until_statement_node.o: ./src/ast/nodes/until_statement_node.cc ./src/ast/nodes/until_statement_node.hh
	g++ $(CXXFLAGS) -c $< -o $@

# Symbols

./build/symbols/symbolvariable.o : ./src/symboltable/symbols/symbolvariable.cc ./src/symboltable/symbols/symbolvariable.hh
#	Symbolvariable will be compiled first, so it needs to check if the symbol folder exists
	@mkdir -p ./build/symbols
	g++ $(CXXFLAGS) -c $< -o $@

./build/symbols/symbolfunction.o : ./src/symboltable/symbols/symbolfunction.cc ./src/symboltable/symbols/symbolfunction.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/symbols/symbolclass.o : ./src/symboltable/symbols/symbolclass.cc ./src/symboltable/symbols/symbolclass.hh
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

./build/expressions/two_operand_expression.o: ./src/ast/expressions/two_operand_expression.cc ./src/ast/expressions/two_operand_expression.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/expressions/one_operand_expression.o: ./src/ast/expressions/one_operand_expression.cc ./src/ast/expressions/one_operand_expression.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/expressions/array_subscript_expression.o: ./src/ast/expressions/array_subscript.cc ./src/ast/expressions/array_subscript.hh
	g++ $(CXXFLAGS) -c $< -o $@

# Attributes

./build/attributes/attribute.o: ./src/ast/attributes/attribute.cc ./src/ast/attributes/attribute.hh
	@mkdir -p ./build/attributes
	g++ $(CXXFLAGS) -c $< -o $@

./build/attributes/attribute_private.o: ./src/ast/attributes/attributeprivate.cc ./src/ast/attributes/attributeprivate.hh 
	g++ $(CXXFLAGS) -c $< -o $@

./build/attributes/attribute_protected.o: ./src/ast/attributes/attributeprotected.cc ./src/ast/attributes/attributeprotected.hh 
	g++ $(CXXFLAGS) -c $< -o $@

./build/attributes/attribute_public.o: ./src/ast/attributes/attributepublic.cc ./src/ast/attributes/attributepublic.hh 
	g++ $(CXXFLAGS) -c $< -o $@

./build/attributes/attribute_static.o: ./src/ast/attributes/attributestatic.cc ./src/ast/attributes/attributestatic.hh 
	g++ $(CXXFLAGS) -c $< -o $@

# Types

./build/types/typearray.o: ./src/ast/types/typearray.cc ./src/ast/types/typearray.hh
	@mkdir -p ./build/types
	g++ $(CXXFLAGS) -c $< -o $@

./build/types/type.o: ./src/ast/types/type.cc ./src/ast/types/type.hh
	@mkdir -p ./build/types
	g++ $(CXXFLAGS) -c $< -o $@

./build/types/typeboolean.o: ./src/ast/types/typeboolean.cc ./src/ast/types/typeboolean.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/types/typeinteger.o: ./src/ast/types/typeinteger.cc ./src/ast/types/typeinteger.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/types/typefloat.o: ./src/ast/types/typefloat.cc ./src/ast/types/typefloat.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/types/typechar.o: ./src/ast/types/typechar.cc ./src/ast/types/typechar.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/types/typestring.o: ./src/ast/types/typestring.cc ./src/ast/types/typestring.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/types/typevoid.o: ./src/ast/types/typevoid.cc ./src/ast/types/typevoid.hh
	g++ $(CXXFLAGS) -c $< -o $@

# Helper

./build/helper/helper.o: ./src/helper/helper.cc ./src/helper/helper.hh 
	@mkdir -p ./build/helper
	g++ $(CXXFLAGS) -c $< -o $@

# Literals

./build/literals/literal.o: ./src/ast/literals/literal.cc ./src/ast/literals/literal.hh
	@mkdir -p ./build/literals
	g++ $(CXXFLAGS) -c $< -o $@

./build/literals/bool_literal.o: ./src/ast/literals/bool_literal.cc ./src/ast/literals/bool_literal.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/literals/char_literal.o: ./src/ast/literals/char_literal.cc ./src/ast/literals/char_literal.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/literals/int_literal.o: ./src/ast/literals/int_literal.cc ./src/ast/literals/int_literal.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/literals/float_literal.o: ./src/ast/literals/float_literal.cc ./src/ast/literals/float_literal.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/literals/string_literal.o: ./src/ast/literals/string_literal.cc ./src/ast/literals/string_literal.hh
	g++ $(CXXFLAGS) -c $< -o $@

# Parser
	
./build/parser/parser.tab.o: ./src/parser/parser.tab.cc ./src/lexer/AtmoLexer.hh
	@mkdir -p ./build/parser
	g++ $(CXXFLAGS) -c $< -o $@


	
./src/parser/parser.tab.cc: ./src/parser/parser.yy
	bison -d --debug -t $(DEBUG) $< -o $@

./src/parser/parser.tab.hh: ./src/parser/parser.yy
	bison -d --debug -t $(DEBUG) $< -o $@

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
	rm -f ./src/lexer/lex.yy.cc
	rm -f ./src/parser/parser.tab.cc
	rm -f ./src/parser/parser.tab.hh
	rm -f main

.PHONY: tests
tests: 
	@cd src/tests && make && cd ../../

.PHONY: tests_clean
tests_clean: 
	@cd src/tests && make clean && cd ../../

# https://makefiletutorial.com00
