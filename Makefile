
nodes := ./build/nodes/statement_list_node.o ./build/nodes/body_node.o ./build/nodes/variable_definition_node.o ./build/nodes/class_definition_node.o ./build/nodes/constructor_definition_node.o ./build/nodes/destructor_definition_node.o ./build/nodes/do_until_statement_node.o ./build/nodes/else_if_statement_node.o  ./build/nodes/else_statement_node.o ./build/nodes/function_definition_node.o ./build/nodes/if_statement_node.o ./build/nodes/return_statement_node.o ./build/nodes/until_statement_node.o

literals := ./build/literals/literal.o ./build/literals/bool_literal.o ./build/literals/char_literal.o ./build/literals/integer_literal.o ./build/literals/float_literal.o ./build/literals/string_literal.o

symbols := ./build/symbols/symbolvariable.o ./build/symbols/symbolfunction.o ./build/symbols/symbolclass.o

expressions := ./build/expressions/add_expression.o ./build/expressions/and_expression.o ./build/expressions/assignment_expression.o ./build/expressions/divide_expression.o ./build/expressions/expressionable.o ./build/expressions/function_call.o ./build/expressions/greater_than_expression.o ./build/expressions/greater_than_or_equal_expression.o ./build/expressions/identifier.o  ./build/expressions/less_than_expression.o ./build/expressions/less_than_or_equal_expression.o ./build/expressions/matches_expression.o ./build/expressions/member_access_expression.o ./build/expressions/multiply_expression.o ./build/expressions/not_expression.o ./build/expressions/not_matches_expression.o ./build/expressions/or_expression.o ./build/expressions/subtract_expression.o ./build/expressions/two_operand_expression.o ./build/expressions/one_operand_expression.o ./build/expressions/array_subscript_expression.o

attributes := ./build/attributes/attribute.o ./build/attributes/attribute_private.o ./build/attributes/attribute_protected.o ./build/attributes/attribute_public.o ./build/attributes/attribute_static.o ./build/attributes/attribute_virtual.o ./build/attributes/attribute_overriding.o

types := ./build/types/typearray.o ./build/types/type.o  ./build/types/typeboolean.o  ./build/types/typeinteger.o ./build/types/typefloat.o ./build/types/typestring.o ./build/types/typevoid.o ./build/types/typechar.o

ir_statements := ./build/ir/statements/ir_cjump.o ./build/ir/statements/ir_evaluate_expression.o ./build/ir/statements/ir_jump.o ./build/ir/statements/ir_label.o ./build/ir/statements/ir_move.o ./build/ir/statements/ir_sequence.o ./build/ir/statements/ir_statement_list.o ./build/ir/statements/ir_statement.o ./build/ir/statements/ir_return.o ./build/ir/statements/ir_pop.o ./build/ir/statements/ir_push.o ./build/ir/statements/ir_syscall.o ./build/ir/statements/ir_leave.o ./build/ir/statements/ir_enter.o

ir_expressions := ./build/ir/expressions/ir_binary_operator.o ./build/ir/expressions/ir_call.o ./build/ir/expressions/ir_const.o ./build/ir/expressions/ir_const_float.o ./build/ir/expressions/ir_eseq.o ./build/ir/expressions/ir_mem.o ./build/ir/expressions/ir_name.o ./build/ir/expressions/ir_temp.o ./build/ir/expressions/ir_expression.o ./build/ir/expressions/ir_expression_list.o

ir_block_trace := ./build/ir/blocks/ir_block.o ./build/ir/traces/ir_trace.o

ir_normalizer := ./build/ir/ir_normalizer.o

frame := ./build/frame/x86_frame.o ./build/frame/boollist.o ./build/frame/inframe.o ./build/frame/inreg.o ./build/frame/label.o ./build/frame/temp.o ./build/frame/accesslist.o ./build/frame/offset_from_object.o ./build/frame/printedlabel.o ./build/frame/access.o ./build/frame/global_frame.o

translate := ./build/translate/translate_conditional_expression.o ./build/translate/translate_value_expression.o ./build/translate/translate_no_value_expression.o

canonical :=  ./build/ir/canonical/move_call.o ./build/ir/canonical/exp_call.o ./build/ir/canonical/ir_canonical.o

reserved_ir_registers := ./build/ir/reserved_ir_registers.o

assembly := ./build/asm/assembly_label.o ./build/asm/assembly_move.o ./build/asm/assembly_oper.o  ./build/asm/assembly_instruction.o

code_generator := ./build/codegen/codegen.o ./build/codegen/x86_codegen.o ./build/codegen/global_strings.o

reg_allocator := ./build/codegen/linear_scan.o

garbage_collector := ./src/garbage_collector/atmo_gc.c

helper := ./build/helper/helper.o

lexer := ./build/lexer/atmolexer.o ./build/lexer/lex.yy.o

driver := ./build/driver/atmo_driver.o

main := ./build/main/main.o

parser := ./build/parser/parser.tab.o

scope := ./build/scope/scope.o

error := ./build/error/error.o

ilocation := ./build/location/ilocation.o

symboltable := ./build/symboltable/symboltable.o ./build/symboltable/symboltableelement.o


objects :=  $(main)   $(parser) $(scope) $(error) $(ilocation) $(lexer) $(driver) $(symboltable) $(nodes) $(literals) $(expressions) $(attributes) $(helper) $(symbols) $(types) $(ir_statements) $(ir_expressions) $(reserved_ir_registers) $(frame) $(translate) $(canonical) $(ir_normalizer) $(ir_block_trace) $(assembly) $(code_generator) $(reg_allocator)

#CXXFLAGS = -g -Wpedantic -Wextra -Wall -fsanitize=address
CXXFLAGS = -g3 -Wno-deprecated -pipe -fno-elide-type -fdiagnostics-show-template-tree -Wall -Werror -Wextra -Wpedantic -Wvla -Wextra-semi -Wnull-dereference -fvar-tracking-assignments -Wduplicated-cond -Wduplicated-branches -rdynamic -Wsuggest-override -O0 -Wno-overloaded-virtual -Wno-unused-parameter
#LDFLAGS = -static-libasan



# Main

main: $(objects)
	g++ $(CXXFLAGS) $^ -o $@
	@echo ""
	@echo "Assembling the garbage collector!"
	@gcc -c -g $(garbage_collector) -o atmo_gc.o
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

./build/attributes/attribute_virtual.o: ./src/ast/attributes/attributevirtual.cc ./src/ast/attributes/attributevirtual.hh 
	g++ $(CXXFLAGS) -c $< -o $@

./build/attributes/attribute_overriding.o: ./src/ast/attributes/attributeoverriding.cc ./src/ast/attributes/attributeoverriding.hh 
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

./build/literals/integer_literal.o: ./src/ast/literals/integer_literal.cc ./src/ast/literals/integer_literal.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/literals/float_literal.o: ./src/ast/literals/float_literal.cc ./src/ast/literals/float_literal.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/literals/string_literal.o: ./src/ast/literals/string_literal.cc ./src/ast/literals/string_literal.hh
	g++ $(CXXFLAGS) -c $< -o $@


# IR Statements

./build/ir/statements/ir_cjump.o: ./src/ir/statements/ir_cjump.cc ./src/ir/statements/ir_cjump.hh
	@mkdir -p ./build/ir/statements
	g++ $(CXXFLAGS) -c $< -o $@

./build/ir/statements/ir_evaluate_expression.o: ./src/ir/statements/ir_evaluate_expression.cc ./src/ir/statements/ir_evaluate_expression.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/ir/statements/ir_jump.o: ./src/ir/statements/ir_jump.cc ./src/ir/statements/ir_jump.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/ir/statements/ir_label.o: ./src/ir/statements/ir_label.cc ./src/ir/statements/ir_label.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/ir/statements/ir_move.o: ./src/ir/statements/ir_move.cc ./src/ir/statements/ir_move.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/ir/statements/ir_sequence.o: ./src/ir/statements/ir_sequence.cc ./src/ir/statements/ir_sequence.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/ir/statements/ir_statement_list.o: ./src/ir/statements/ir_statement_list.cc ./src/ir/statements/ir_statement_list.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/ir/statements/ir_statement.o: ./src/ir/statements/ir_statement.cc ./src/ir/statements/ir_statement.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/ir/statements/ir_return.o: ./src/ir/statements/ir_return.cc ./src/ir/statements/ir_return.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/ir/statements/ir_pop.o: ./src/ir/statements/ir_pop.cc ./src/ir/statements/ir_pop.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/ir/statements/ir_push.o: ./src/ir/statements/ir_push.cc ./src/ir/statements/ir_push.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/ir/statements/ir_syscall.o: ./src/ir/statements/ir_syscall.cc ./src/ir/statements/ir_syscall.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/ir/statements/ir_leave.o: ./src/ir/statements/ir_leave.cc ./src/ir/statements/ir_leave.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/ir/statements/ir_enter.o: ./src/ir/statements/ir_enter.cc ./src/ir/statements/ir_enter.hh
	g++ $(CXXFLAGS) -c $< -o $@
./build/ir/ir_normalizer.o: ./src/ir/ir_normalizer.cc ./src/ir/ir_normalizer.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/ir/blocks/ir_block.o: ./src/ir/blocks/ir_block.cc ./src/ir/blocks/ir_block.hh
	@mkdir -p ./build/ir/blocks
	g++ $(CXXFLAGS) -c $< -o $@

./build/ir/traces/ir_trace.o: ./src/ir/traces/ir_trace.cc ./src/ir/traces/ir_trace.hh
	@mkdir -p ./build/ir/traces
	g++ $(CXXFLAGS) -c $< -o $@


# IR Expressions

./build/ir/expressions/ir_binary_operator.o: ./src/ir/expressions/ir_binary_operator.cc ./src/ir/expressions/ir_binary_operator.hh
	@mkdir -p ./build/ir/expressions
	g++ $(CXXFLAGS) -c $< -o $@

./build/ir/expressions/ir_call.o: ./src/ir/expressions/ir_call.cc ./src/ir/expressions/ir_call.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/ir/expressions/ir_const.o: ./src/ir/expressions/ir_const.cc ./src/ir/expressions/ir_const.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/ir/expressions/ir_const_float.o: ./src/ir/expressions/ir_const_float.cc ./src/ir/expressions/ir_const_float.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/ir/expressions/ir_eseq.o: ./src/ir/expressions/ir_eseq.cc ./src/ir/expressions/ir_eseq.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/ir/expressions/ir_mem.o: ./src/ir/expressions/ir_mem.cc ./src/ir/expressions/ir_mem.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/ir/expressions/ir_name.o: ./src/ir/expressions/ir_name.cc ./src/ir/expressions/ir_name.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/ir/expressions/ir_temp.o: ./src/ir/expressions/ir_temp.cc ./src/ir/expressions/ir_temp.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/ir/expressions/ir_expression.o: ./src/ir/expressions/ir_expression.cc ./src/ir/expressions/ir_expression.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/ir/expressions/ir_expression_list.o: ./src/ir/expressions/ir_expression_list.cc ./src/ir/expressions/ir_expression_list.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/ir/reserved_ir_registers.o: ./src/ir/reserved_ir_registers.cc ./src/ir/reserved_ir_registers.hh
	g++ $(CXXFLAGS) -c $< -o $@

# Frame

./build/frame/x86_frame.o: ./src/frame/x86_frame.cc ./src/frame/x86_frame.hh
	@mkdir -p ./build/frame
	g++ $(CXXFLAGS) -c $< -o $@

./build/frame/boollist.o: ./src/frame/boollist.cc ./src/frame/boollist.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/frame/inframe.o: ./src/frame/inframe.cc ./src/frame/inframe.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/frame/inreg.o: ./src/frame/inreg.cc ./src/frame/inreg.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/frame/label.o: ./src/frame/label.cc ./src/frame/label.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/frame/temp.o: ./src/frame/temp.cc ./src/frame/temp.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/frame/accesslist.o: ./src/frame/accesslist.cc ./src/frame/accesslist.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/frame/offset_from_object.o: ./src/frame/offset_from_object.cc ./src/frame/offset_from_object.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/frame/printedlabel.o: ./src/frame/printedlabel.cc ./src/frame/printedlabel.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/frame/access.o: ./src/frame/access.cc ./src/frame/access.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/frame/global_frame.o: ./src/frame/global_frame.cc ./src/frame/global_frame.hh
	g++ $(CXXFLAGS) -c $< -o $@

# Translate

./build/translate/translate_conditional_expression.o: ./src/translate/translate_conditional_expression.cc ./src/translate/translate_conditional_expression.hh
	@mkdir -p ./build/translate
	g++ $(CXXFLAGS) -c $< -o $@

./build/translate/translate_value_expression.o: ./src/translate/translate_value_expression.cc ./src/translate/translate_value_expression.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/translate/translate_no_value_expression.o: ./src/translate/translate_no_value_expression.cc ./src/translate/translate_no_value_expression.hh
	g++ $(CXXFLAGS) -c $< -o $@


# Canonical

./build/ir/canonical/move_call.o: ./src/ir/canonical/move_call.cc ./src/ir/canonical/move_call.hh
	@mkdir -p ./build/ir/canonical
	g++ $(CXXFLAGS) -c $< -o $@

./build/ir/canonical/exp_call.o: ./src/ir/canonical/exp_call.cc ./src/ir/canonical/exp_call.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/ir/canonical/ir_canonical.o: ./src/ir/canonical/ir_canonical.cc ./src/ir/canonical/ir_canonical.hh
	g++ $(CXXFLAGS) -c $< -o $@

# Assembly

./build/asm/assembly_oper.o: ./src/asm/assembly_oper.cc ./src/asm/assembly_oper.hh
	@mkdir -p ./build/asm
	g++ $(CXXFLAGS) -c $< -o $@

./build/asm/assembly_move.o: ./src/asm/assembly_move.cc ./src/asm/assembly_move.hh
	g++ $(CXXFLAGS) -c $< -o $@

./build/asm/assembly_label.o: ./src/asm/assembly_label.cc ./src/asm/assembly_label.hh
	@mkdir -p ./build/asm
	g++ $(CXXFLAGS) -c $< -o $@

./build/asm/assembly_instruction.o: ./src/asm/assembly_instruction.cc ./src/asm/assembly_instruction.hh
	g++ $(CXXFLAGS) -c $< -o $@

# Code Generator

./build/codegen/codegen.o: ./src/codegen/codegen.cc ./src/codegen/codegen.hh
	@mkdir -p ./build/codegen
	g++ $(CXXFLAGS) -c $< -o $@

./build/codegen/x86_codegen.o: ./src/codegen/x86_codegen.cc ./src/codegen/x86_codegen.hh
	@mkdir -p ./build/codegen
	g++ $(CXXFLAGS) -c $< -o $@

./build/codegen/global_strings.o: ./src/codegen/global_strings.cc ./src/codegen/global_strings.hh
	@mkdir -p ./build/codegen
	g++ $(CXXFLAGS) -c $< -o $@

./build/codegen/linear_scan.o: ./src/codegen/reg_allocator/linear_scan.cc ./src/codegen/reg_allocator/linear_scan.hh
	@mkdir -p ./build/codegen
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
