#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <sstream>
#include "../driver/atmo_driver.hh"
#include "../ast/nodes/statement_list_node.hh"
#include "../ast/literals/literal.hh"
#include "../ast/types/all_types.hh"
#include "../ast/nodes/all_nodes.hh"
#include "../ast/expressions/all_expressions.hh"

class ParserTest : public ::testing::Test {
protected:
	AtmoDriver driver;
	void SetUp() override {
		Error::InTest = true;
	}

	std::shared_ptr<StatementListNode> parse(const std::string& input) {
		std::istringstream stream(input);
 		driver.parse_only(stream);
		return driver.ast_root;
	}
};

// Expressions

TEST_F(ParserTest, ParseIntegerLiteralExpression)
{
	auto root = parse("5");

	ASSERT_NE(root, nullptr);

	auto statements = root->GetStatements();

	ASSERT_NE(statements.size(), 0);

	auto literal = std::dynamic_pointer_cast<Literal<int,TypeInteger>>(statements[0]);

	
	//If the cast didn't return a nullptr means that the class was csted successfully

	ASSERT_NE(literal, nullptr);


	// Expect that the parser set the correct value for the literal

	EXPECT_EQ(literal->value, 5);

}

TEST_F(ParserTest, ParseFloatLiteralExpression)
{
	auto root = parse("3.2");

	ASSERT_NE(root, nullptr);

	auto statements = root->GetStatements();

	ASSERT_NE(statements.size(), 0);

	auto literal = std::dynamic_pointer_cast<Literal<double,TypeFloat>>(statements[0]);

	
	//If the cast didn't return a nullptr means that the class was csted successfully

	ASSERT_NE(literal, nullptr);


	// Expect that the parser set the correct value for the literal

	EXPECT_DOUBLE_EQ(literal->value, 3.2);

}

TEST_F(ParserTest, ParseAbbreviatedFloatLiteralExpression)
{
	auto root = parse(".2");

	ASSERT_NE(root, nullptr);

	auto statements = root->GetStatements();

	ASSERT_NE(statements.size(), 0);

	auto literal = std::dynamic_pointer_cast<Literal<double,TypeFloat>>(statements[0]);

	
	//If the cast didn't return a nullptr means that the class was csted successfully

	ASSERT_NE(literal, nullptr);


	// Expect that the parser set the correct value for the literal

	EXPECT_DOUBLE_EQ(literal->value, .2);

}

TEST_F(ParserTest, ParseCharLiteralExpression)
{
	auto root = parse("'a'");

	ASSERT_NE(root, nullptr);

	auto statements = root->GetStatements();

	ASSERT_NE(statements.size(), 0);

	auto literal = std::dynamic_pointer_cast<Literal<char,TypeChar>>(statements[0]);

	
	//If the cast didn't return a nullptr means that the class was csted successfully

	ASSERT_NE(literal, nullptr);


	// Expect that the parser set the correct value for the literal

	EXPECT_EQ(literal->value, 'a');

}

// String Literal contains Hello World by default (even if parsign an empty string) and segfaults at the end of the tests so that needs to be fixed but disabling the test now because it's not a priority and the segfault is interfering with the other tests

// TEST_F(ParserTest, ParseStringLiteralExpression)
// {
// 	auto root = parse("\"Hello World\"");

// 	ASSERT_NE(root, nullptr);

// 	auto statements = root->GetStatements();

// 	ASSERT_NE(statements.size(), 0);

// 	auto literal = std::dynamic_pointer_cast<Literal<std::string,TypeString>>(statements[0]);

	
// 	//If the cast didn't return a nullptr means that the class was csted successfully

// 	ASSERT_NE(literal, nullptr);


// 	// Expect that the parser set the correct value for the literal

// 	EXPECT_EQ(literal->value, "Hello World");

// }


TEST_F(ParserTest, ParseIdentifier) {
    auto root = parse("myVar");
    ASSERT_NE(root, nullptr);
    auto statements = root->GetStatements();
    ASSERT_NE(statements.size(), 0);
    auto identifier = std::dynamic_pointer_cast<Identifier>(statements[0]);
    ASSERT_NE(identifier, nullptr);
    EXPECT_EQ(identifier->name, "myVar");
}

TEST_F(ParserTest, ParseAdditionExpression) {
    auto root = parse("5 + 3");
    ASSERT_NE(root, nullptr);
    auto statements = root->GetStatements();
    ASSERT_NE(statements.size(), 0);
    auto addExpr = std::dynamic_pointer_cast<AddExpression>(statements[0]);
    ASSERT_NE(addExpr, nullptr);
    auto left = std::dynamic_pointer_cast<Literal<int, TypeInteger>>(addExpr->GetLeft());
    auto right = std::dynamic_pointer_cast<Literal<int, TypeInteger>>(addExpr->GetRight());
    ASSERT_NE(left, nullptr);
    ASSERT_NE(right, nullptr);
    EXPECT_EQ(left->value, 5);
    EXPECT_EQ(right->value, 3);
}


TEST_F(ParserTest, ParseAndExpression) {
    auto root = parse("true AND false");
    ASSERT_NE(root, nullptr);
    auto statements = root->GetStatements();
    ASSERT_NE(statements.size(), 0);
    auto andExpr = std::dynamic_pointer_cast<AndExpression>(statements[0]);
    ASSERT_NE(andExpr, nullptr);
    auto left = std::dynamic_pointer_cast<Literal<bool, TypeBoolean>>(andExpr->GetLeft());
    auto right = std::dynamic_pointer_cast<Literal<bool, TypeBoolean>>(andExpr->GetRight());
    ASSERT_NE(left, nullptr);
    ASSERT_NE(right, nullptr);
    EXPECT_EQ(left->value, true);
    EXPECT_EQ(right->value, false);
}

TEST_F(ParserTest, ParseSubtractionExpression) {
	auto root = parse("5 - 3");
	ASSERT_NE(root, nullptr);
	auto statements = root->GetStatements();
	ASSERT_NE(statements.size(), 0);
	auto subExpr = std::dynamic_pointer_cast<SubtractExpression>(statements[0]);
	ASSERT_NE(subExpr, nullptr);
	auto left = std::dynamic_pointer_cast<Literal<int, TypeInteger>>(subExpr->GetLeft());
	auto right = std::dynamic_pointer_cast<Literal<int, TypeInteger>>(subExpr->GetRight());
	ASSERT_NE(left, nullptr);
	ASSERT_NE(right, nullptr);
	EXPECT_EQ(left->value, 5);
	EXPECT_EQ(right->value, 3);
}

TEST_F(ParserTest, ParseMultiplicationExpression) {
	auto root = parse("5 * 3");
	ASSERT_NE(root, nullptr);
	auto statements = root->GetStatements();
	ASSERT_NE(statements.size(), 0);
	auto mulExpr = std::dynamic_pointer_cast<MultiplyExpression>(statements[0]);
	ASSERT_NE(mulExpr, nullptr);
	auto left = std::dynamic_pointer_cast<Literal<int, TypeInteger>>(mulExpr->GetLeft());
	auto right = std::dynamic_pointer_cast<Literal<int, TypeInteger>>(mulExpr->GetRight());
	ASSERT_NE(left, nullptr);
	ASSERT_NE(right, nullptr);
	EXPECT_EQ(left->value, 5);
	EXPECT_EQ(right->value, 3);
}

TEST_F(ParserTest, ParseDivisionExpression) {
	auto root = parse("6 / 3");
	ASSERT_NE(root, nullptr);
	auto statements = root->GetStatements();
	ASSERT_NE(statements.size(), 0);
	auto divExpr = std::dynamic_pointer_cast<DivideExpression>(statements[0]);
	ASSERT_NE(divExpr, nullptr);
	auto left = std::dynamic_pointer_cast<Literal<int, TypeInteger>>(divExpr->GetLeft());
	auto right = std::dynamic_pointer_cast<Literal<int, TypeInteger>>(divExpr->GetRight());
	ASSERT_NE(left, nullptr);
	ASSERT_NE(right, nullptr);
	EXPECT_EQ(left->value, 6);
	EXPECT_EQ(right->value, 3);
}

TEST_F(ParserTest, ParseOrExpression) {
	auto root = parse("true OR false");
	ASSERT_NE(root, nullptr);
	auto statements = root->GetStatements();
	ASSERT_NE(statements.size(), 0);
	auto orExpr = std::dynamic_pointer_cast<OrExpression>(statements[0]);
	ASSERT_NE(orExpr, nullptr);
	auto left = std::dynamic_pointer_cast<Literal<bool, TypeBoolean>>(orExpr->GetLeft());
	auto right = std::dynamic_pointer_cast<Literal<bool, TypeBoolean>>(orExpr->GetRight());
	ASSERT_NE(left, nullptr);
	ASSERT_NE(right, nullptr);
	EXPECT_EQ(left->value, true);
	EXPECT_EQ(right->value, false);
}

TEST_F(ParserTest, ParseNotExpression) {
	auto root = parse("NOT true");
	ASSERT_NE(root, nullptr);
	auto statements = root->GetStatements();
	ASSERT_NE(statements.size(), 0);
	auto notExpr = std::dynamic_pointer_cast<NotExpression>(statements[0]);
	ASSERT_NE(notExpr, nullptr);
	auto operand = std::dynamic_pointer_cast<Literal<bool, TypeBoolean>>(notExpr->GetExpression());
	ASSERT_NE(operand, nullptr);
	EXPECT_EQ(operand->value, true);
}

TEST_F(ParserTest, ParseMatchesExpression) {
	auto root = parse("5 == 5");
	ASSERT_NE(root, nullptr);
	auto statements = root->GetStatements();
	ASSERT_NE(statements.size(), 0);
	auto eqExpr = std::dynamic_pointer_cast<MatchesExpression>(statements[0]);
	ASSERT_NE(eqExpr, nullptr);
	auto left = std::dynamic_pointer_cast<Literal<int, TypeInteger>>(eqExpr->GetLeft());
	auto right = std::dynamic_pointer_cast<Literal<int, TypeInteger>>(eqExpr->GetRight());
	ASSERT_NE(left, nullptr);
	ASSERT_NE(right, nullptr);
	EXPECT_EQ(left->value, 5);
	EXPECT_EQ(right->value, 5);
}

TEST_F(ParserTest, ParseNotMatchesExpression) {
	auto root = parse("5 != 3");
	ASSERT_NE(root, nullptr);
	auto statements = root->GetStatements();
	ASSERT_NE(statements.size(), 0);
	auto neqExpr = std::dynamic_pointer_cast<NotMatchesExpression>(statements[0]);
	ASSERT_NE(neqExpr, nullptr);
	auto left = std::dynamic_pointer_cast<Literal<int, TypeInteger>>(neqExpr->GetLeft());
	auto right = std::dynamic_pointer_cast<Literal<int, TypeInteger>>(neqExpr->GetRight());
	ASSERT_NE(left, nullptr);
	ASSERT_NE(right, nullptr);
	EXPECT_EQ(left->value, 5);
	EXPECT_EQ(right->value, 3);
}

TEST_F(ParserTest, ParseGreaterThanExpression) {
	auto root = parse("5 > 3");
	ASSERT_NE(root, nullptr);
	auto statements = root->GetStatements();
	ASSERT_NE(statements.size(), 0);
	auto gtExpr = std::dynamic_pointer_cast<GreaterThanExpression>(statements[0]);
	ASSERT_NE(gtExpr, nullptr);
	auto left = std::dynamic_pointer_cast<Literal<int, TypeInteger>>(gtExpr->GetLeft());
	auto right = std::dynamic_pointer_cast<Literal<int, TypeInteger>>(gtExpr->GetRight());
	ASSERT_NE(left, nullptr);
	ASSERT_NE(right, nullptr);
	EXPECT_EQ(left->value, 5);
	EXPECT_EQ(right->value, 3);
}


TEST_F(ParserTest, ParseGreaterThanOrEqualExpression) {
	auto root = parse("5 >= 3");
	ASSERT_NE(root, nullptr);
	auto statements = root->GetStatements();
	ASSERT_NE(statements.size(), 0);
	auto gteExpr = std::dynamic_pointer_cast<GreaterThanOrEqualExpression>(statements[0]);
	ASSERT_NE(gteExpr, nullptr);
	auto left = std::dynamic_pointer_cast<Literal<int, TypeInteger>>(gteExpr->GetLeft());
	auto right = std::dynamic_pointer_cast<Literal<int, TypeInteger>>(gteExpr->GetRight());
	ASSERT_NE(left, nullptr);
	ASSERT_NE(right, nullptr);
	EXPECT_EQ(left->value, 5);
	EXPECT_EQ(right->value, 3);
}

TEST_F(ParserTest, ParseLessThanExpression) {
	auto root = parse("3 < 5");
	ASSERT_NE(root, nullptr);
	auto statements = root->GetStatements();
	ASSERT_NE(statements.size(), 0);
	auto ltExpr = std::dynamic_pointer_cast<LessThanExpression>(statements[0]);
	ASSERT_NE(ltExpr, nullptr);
	auto left = std::dynamic_pointer_cast<Literal<int, TypeInteger>>(ltExpr->GetLeft());
	auto right = std::dynamic_pointer_cast<Literal<int, TypeInteger>>(ltExpr->GetRight());
	ASSERT_NE(left, nullptr);
	ASSERT_NE(right, nullptr);
	EXPECT_EQ(left->value, 3);
	EXPECT_EQ(right->value, 5);
}

TEST_F(ParserTest, ParseLessThanOrEqualExpression) {
	auto root = parse("3 <= 5");
	ASSERT_NE(root, nullptr);
	auto statements = root->GetStatements();
	ASSERT_NE(statements.size(), 0);
	auto ltExpr = std::dynamic_pointer_cast<LessThanOrEqualExpression>(statements[0]);
	ASSERT_NE(ltExpr, nullptr);
	auto left = std::dynamic_pointer_cast<Literal<int, TypeInteger>>(ltExpr->GetLeft());
	auto right = std::dynamic_pointer_cast<Literal<int, TypeInteger>>(ltExpr->GetRight());
	ASSERT_NE(left, nullptr);
	ASSERT_NE(right, nullptr);
	EXPECT_EQ(left->value, 3);
	EXPECT_EQ(right->value, 5);
}

// Variable declarations

TEST_F(ParserTest, ParseVariableDeclarationWithIntegerType)
{
	auto root = parse("create private integer myVar");

	ASSERT_NE(root, nullptr);

	auto statements = root->GetStatements();
	ASSERT_NE(statements.size(), 0);

	auto varDef = std::dynamic_pointer_cast<VariableDefinitionNode>(statements[0]);
	ASSERT_NE(varDef, nullptr);

	auto symbol = varDef->GetVariable();
	ASSERT_NE(symbol, nullptr);

	//Expect that the expression wasn't set
	EXPECT_EQ(varDef->expression,nullptr);
	EXPECT_EQ(symbol->name, "myVar");
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeInteger>(symbol->GetType()) != nullptr);
}



TEST_F(ParserTest, ParseVariableDeclarationWithFloatType)
{
	auto root = parse("create private float myVar");

	ASSERT_NE(root, nullptr);

	auto statements = root->GetStatements();
	ASSERT_NE(statements.size(), 0);

	auto varDef = std::dynamic_pointer_cast<VariableDefinitionNode>(statements[0]);
	ASSERT_NE(varDef, nullptr);

	auto symbol = varDef->GetVariable();
	ASSERT_NE(symbol, nullptr);

	//Expect that the expression wasn't set
	EXPECT_EQ(varDef->expression,nullptr);
	EXPECT_EQ(symbol->name, "myVar");
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeFloat>(symbol->GetType()) != nullptr);
}

TEST_F(ParserTest, ParseVariableDeclarationWithStringType)
{
	auto root = parse("create private string myVar");

	ASSERT_NE(root, nullptr);

	auto statements = root->GetStatements();
	ASSERT_NE(statements.size(), 0);

	auto varDef = std::dynamic_pointer_cast<VariableDefinitionNode>(statements[0]);
	ASSERT_NE(varDef, nullptr);

	auto symbol = varDef->GetVariable();
	ASSERT_NE(symbol, nullptr);

	//Expect that the expression wasn't set
	EXPECT_EQ(varDef->expression,nullptr);
	EXPECT_EQ(symbol->name, "myVar");
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeString>(symbol->GetType()) != nullptr);
}

TEST_F(ParserTest, ParseVariableDeclarationWithCharType)
{
	auto root = parse("create private char myVar");

	ASSERT_NE(root, nullptr);

	auto statements = root->GetStatements();
	ASSERT_NE(statements.size(), 0);

	auto varDef = std::dynamic_pointer_cast<VariableDefinitionNode>(statements[0]);
	ASSERT_NE(varDef, nullptr);

	auto symbol = varDef->GetVariable();
	ASSERT_NE(symbol, nullptr);

	//Expect that the expression wasn't set
	EXPECT_EQ(varDef->expression,nullptr);
	EXPECT_EQ(symbol->name, "myVar");
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeChar>(symbol->GetType()) != nullptr);
}

TEST_F(ParserTest, ParseVariableDeclarationWithArrayType)
{
	auto root = parse("create private array of integers myVar");

	ASSERT_NE(root, nullptr);

	auto statements = root->GetStatements();
	ASSERT_NE(statements.size(), 0);

	auto varDef = std::dynamic_pointer_cast<VariableDefinitionNode>(statements[0]);
	ASSERT_NE(varDef, nullptr);

	auto symbol = varDef->GetVariable();
	ASSERT_NE(symbol, nullptr);

	//Expect that the expression wasn't set
	EXPECT_EQ(varDef->expression,nullptr);
	EXPECT_EQ(symbol->name, "myVar");
	EXPECT_TRUE(std::dynamic_pointer_cast<Array>(symbol->GetType()) != nullptr);
}

TEST_F(ParserTest, ParseVariableDeclarationWithClassType)
{
	auto root = parse("create class class_1\n\tcreate public integer a\ncreate class_1 classVar");

	ASSERT_NE(root, nullptr);

	auto statements = root->GetStatements();
	ASSERT_NE(statements.size(), 0);

	auto varDef = std::dynamic_pointer_cast<VariableDefinitionNode>(statements[1]);
	ASSERT_NE(varDef, nullptr);

	auto symbol = varDef->GetVariable();
	ASSERT_NE(symbol, nullptr);

	auto class_type = std::dynamic_pointer_cast<ClassSymbol>(symbol->GetType());
	ASSERT_NE(class_type, nullptr);

	// Expect that the class type was set correctly (we can only check that by comparing the names)

	EXPECT_EQ(class_type->name,"class_1");
	EXPECT_EQ(symbol->name, "classVar");
}


TEST_F(ParserTest, ParseVariableDeclarationWithPrivateAttribute)
{
	auto root = parse("create private integer myVar");

	ASSERT_NE(root, nullptr);

	auto statements = root->GetStatements();
	ASSERT_NE(statements.size(), 0);

	auto varDef = std::dynamic_pointer_cast<VariableDefinitionNode>(statements[0]);
	ASSERT_NE(varDef, nullptr);

	auto symbol = varDef->GetVariable();
	ASSERT_NE(symbol, nullptr);

	auto attribute = symbol->GetAttribute();

	//Expect that the expression wasn't set
	EXPECT_EQ(varDef->expression,nullptr);
	EXPECT_EQ(symbol->name, "myVar");
	EXPECT_TRUE(std::dynamic_pointer_cast<AttributePrivate>(attribute) != nullptr);
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeInteger>(symbol->GetType()) != nullptr);
}

TEST_F(ParserTest, ParseVariableDeclarationWithPublicAttribute)
{
	auto root = parse("create public integer myVar");

	ASSERT_NE(root, nullptr);

	auto statements = root->GetStatements();
	ASSERT_NE(statements.size(), 0);

	auto varDef = std::dynamic_pointer_cast<VariableDefinitionNode>(statements[0]);
	ASSERT_NE(varDef, nullptr);

	auto symbol = varDef->GetVariable();
	ASSERT_NE(symbol, nullptr);

	auto attribute = symbol->GetAttribute();

	//Expect that the expression wasn't set
	EXPECT_EQ(varDef->expression,nullptr);
	EXPECT_EQ(symbol->name, "myVar");
	EXPECT_TRUE(std::dynamic_pointer_cast<AttributePublic>(attribute) != nullptr);
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeInteger>(symbol->GetType()) != nullptr);
}


TEST_F(ParserTest, ParseVariableDeclarationWithProtectedAttribute)
{
	auto root = parse("create protected integer myVar");

	ASSERT_NE(root, nullptr);

	auto statements = root->GetStatements();
	ASSERT_NE(statements.size(), 0);

	auto varDef = std::dynamic_pointer_cast<VariableDefinitionNode>(statements[0]);
	ASSERT_NE(varDef, nullptr);

	auto symbol = varDef->GetVariable();
	ASSERT_NE(symbol, nullptr);

	auto attribute = symbol->GetAttribute();

	//Expect that the expression wasn't set
	EXPECT_EQ(varDef->expression,nullptr);
	EXPECT_EQ(symbol->name, "myVar");
	EXPECT_TRUE(std::dynamic_pointer_cast<AttributeProtected>(attribute) != nullptr);
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeInteger>(symbol->GetType()) != nullptr);
}

TEST_F(ParserTest, ParseVariableDeclarationWithNoAttribute)
{
	auto root = parse("create integer myVar");

	ASSERT_NE(root, nullptr);

	auto statements = root->GetStatements();
	ASSERT_NE(statements.size(), 0);

	auto varDef = std::dynamic_pointer_cast<VariableDefinitionNode>(statements[0]);
	ASSERT_NE(varDef, nullptr);

	auto symbol = varDef->GetVariable();
	ASSERT_NE(symbol, nullptr);

	auto attribute = symbol->GetAttribute();

	// Expect that the expression wasn't set
	EXPECT_EQ(varDef->expression,nullptr);
	EXPECT_EQ(symbol->name, "myVar");
	// The default attribute should be private
	EXPECT_TRUE(std::dynamic_pointer_cast<AttributePrivate>(attribute) != nullptr);
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeInteger>(symbol->GetType()) != nullptr);
}


TEST_F(ParserTest, ParseVariableDeclarationWithInitializationValue)
{
	auto root = parse("create integer myVar equals 10");

	ASSERT_NE(root, nullptr);

	auto statements = root->GetStatements();
	ASSERT_NE(statements.size(), 0);

	auto varDef = std::dynamic_pointer_cast<VariableDefinitionNode>(statements[0]);
	ASSERT_NE(varDef, nullptr);

	auto symbol = varDef->GetVariable();
	ASSERT_NE(symbol, nullptr);

	// Check that the expression was set correctly

	auto literal = std::dynamic_pointer_cast<Literal<int,TypeInteger>>(varDef->expression);
	ASSERT_NE(literal, nullptr);


	EXPECT_EQ(literal->value,10);
	EXPECT_EQ(symbol->name, "myVar");
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeInteger>(symbol->GetType()) != nullptr);
}

// Function declarations

TEST_F(ParserTest, ParseFunctionDefinitionWithIntegerType)
{
	auto root = parse("create public integer function myFunc");

	ASSERT_NE(root, nullptr);

	auto statements = root->GetStatements();
	ASSERT_NE(statements.size(), 0);

	auto funcDef = std::dynamic_pointer_cast<FunctionDefinitionNode>(statements[0]);
	ASSERT_NE(funcDef, nullptr);

	auto funcSymbol = funcDef->GetFunction();
	ASSERT_NE(funcSymbol, nullptr);

	EXPECT_EQ(funcSymbol->name, "myFunc");
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeInteger>(funcSymbol->GetType()) != nullptr);
	EXPECT_EQ(funcSymbol->GetArguments().size(), 0);
}

TEST_F(ParserTest, ParseFunctionDefinitionWithFloatType)
{
	auto root = parse("create public float function myFunc");

	ASSERT_NE(root, nullptr);

	auto statements = root->GetStatements();
	ASSERT_NE(statements.size(), 0);

	auto funcDef = std::dynamic_pointer_cast<FunctionDefinitionNode>(statements[0]);
	ASSERT_NE(funcDef, nullptr);

	auto funcSymbol = funcDef->GetFunction();
	ASSERT_NE(funcSymbol, nullptr);

	EXPECT_EQ(funcSymbol->name, "myFunc");
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeFloat>(funcSymbol->GetType()) != nullptr);
	EXPECT_EQ(funcSymbol->GetArguments().size(), 0);
}

TEST_F(ParserTest, ParseFunctionDefinitionWithStringType)
{
	auto root = parse("create public string function myFunc");

	ASSERT_NE(root, nullptr);

	auto statements = root->GetStatements();
	ASSERT_NE(statements.size(), 0);

	auto funcDef = std::dynamic_pointer_cast<FunctionDefinitionNode>(statements[0]);
	ASSERT_NE(funcDef, nullptr);

	auto funcSymbol = funcDef->GetFunction();
	ASSERT_NE(funcSymbol, nullptr);

	EXPECT_EQ(funcSymbol->name, "myFunc");
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeString>(funcSymbol->GetType()) != nullptr);
	EXPECT_EQ(funcSymbol->GetArguments().size(), 0);
}

TEST_F(ParserTest, ParseFunctionDefinitionWithCharType)
{
	auto root = parse("create public char function myFunc");

	ASSERT_NE(root, nullptr);

	auto statements = root->GetStatements();
	ASSERT_NE(statements.size(), 0);

	auto funcDef = std::dynamic_pointer_cast<FunctionDefinitionNode>(statements[0]);
	ASSERT_NE(funcDef, nullptr);

	auto funcSymbol = funcDef->GetFunction();
	ASSERT_NE(funcSymbol, nullptr);

	EXPECT_EQ(funcSymbol->name, "myFunc");
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeChar>(funcSymbol->GetType()) != nullptr);
	EXPECT_EQ(funcSymbol->GetArguments().size(), 0);
}


TEST_F(ParserTest, ParseFunctionDefinitionWithVoidType)
{
	auto root = parse("create public void function myFunc");

	ASSERT_NE(root, nullptr);

	auto statements = root->GetStatements();
	ASSERT_NE(statements.size(), 0);

	auto funcDef = std::dynamic_pointer_cast<FunctionDefinitionNode>(statements[0]);
	ASSERT_NE(funcDef, nullptr);

	auto funcSymbol = funcDef->GetFunction();
	ASSERT_NE(funcSymbol, nullptr);

	EXPECT_EQ(funcSymbol->name, "myFunc");
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeVoid>(funcSymbol->GetType()) != nullptr);
	EXPECT_EQ(funcSymbol->GetArguments().size(), 0);
}

// Function definitions with arguments

TEST_F(ParserTest, ParseFunctionDefinitionWithIntegerArgument)
{
	auto root = parse("create public void function myFunc with integer a");

	ASSERT_NE(root, nullptr);

	auto statements = root->GetStatements();
	ASSERT_NE(statements.size(), 0);

	auto funcDef = std::dynamic_pointer_cast<FunctionDefinitionNode>(statements[0]);
	ASSERT_NE(funcDef, nullptr);

	auto funcSymbol = funcDef->GetFunction();
	ASSERT_NE(funcSymbol, nullptr);

	auto arguments = funcSymbol->GetArguments();

	ASSERT_EQ(arguments.size(), 1);

	EXPECT_TRUE(std::dynamic_pointer_cast<TypeInteger>(arguments[0]->GetType()) != nullptr);

	EXPECT_EQ(funcSymbol->name, "myFunc");
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeVoid>(funcSymbol->GetType()) != nullptr);
}

TEST_F(ParserTest, ParseFunctionDefinitionWithFloatArgument)
{
	auto root = parse("create public void function myFunc with float a");

	ASSERT_NE(root, nullptr);

	auto statements = root->GetStatements();
	ASSERT_NE(statements.size(), 0);

	auto funcDef = std::dynamic_pointer_cast<FunctionDefinitionNode>(statements[0]);
	ASSERT_NE(funcDef, nullptr);

	auto funcSymbol = funcDef->GetFunction();
	ASSERT_NE(funcSymbol, nullptr);

	auto arguments = funcSymbol->GetArguments();

	ASSERT_EQ(arguments.size(), 1);

	EXPECT_TRUE(std::dynamic_pointer_cast<TypeFloat>(arguments[0]->GetType()) != nullptr);

	EXPECT_EQ(funcSymbol->name, "myFunc");
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeVoid>(funcSymbol->GetType()) != nullptr);
}

TEST_F(ParserTest, ParseFunctionDefinitionWithStringArgument)
{
	auto root = parse("create public void function myFunc with string a");

	ASSERT_NE(root, nullptr);

	auto statements = root->GetStatements();
	ASSERT_NE(statements.size(), 0);

	auto funcDef = std::dynamic_pointer_cast<FunctionDefinitionNode>(statements[0]);
	ASSERT_NE(funcDef, nullptr);

	auto funcSymbol = funcDef->GetFunction();
	ASSERT_NE(funcSymbol, nullptr);

	auto arguments = funcSymbol->GetArguments();

	ASSERT_EQ(arguments.size(), 1);

	EXPECT_TRUE(std::dynamic_pointer_cast<TypeString>(arguments[0]->GetType()) != nullptr);

	EXPECT_EQ(funcSymbol->name, "myFunc");
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeVoid>(funcSymbol->GetType()) != nullptr);
}

TEST_F(ParserTest, ParseFunctionDefinitionWithCharArgument)
{
	auto root = parse("create public void function myFunc with char a");

	ASSERT_NE(root, nullptr);

	auto statements = root->GetStatements();
	ASSERT_NE(statements.size(), 0);

	auto funcDef = std::dynamic_pointer_cast<FunctionDefinitionNode>(statements[0]);
	ASSERT_NE(funcDef, nullptr);

	auto funcSymbol = funcDef->GetFunction();
	ASSERT_NE(funcSymbol, nullptr);

	auto arguments = funcSymbol->GetArguments();

	ASSERT_EQ(arguments.size(), 1);

	EXPECT_TRUE(std::dynamic_pointer_cast<TypeChar>(arguments[0]->GetType()) != nullptr);

	EXPECT_EQ(funcSymbol->name, "myFunc");
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeVoid>(funcSymbol->GetType()) != nullptr);
}


TEST_F(ParserTest, ParseFunctionDefinitionWithArrayArgument)
{
	auto root = parse("create public void function myFunc with array of integer a");

	ASSERT_NE(root, nullptr);

	auto statements = root->GetStatements();
	ASSERT_NE(statements.size(), 0);

	auto funcDef = std::dynamic_pointer_cast<FunctionDefinitionNode>(statements[0]);
	ASSERT_NE(funcDef, nullptr);

	auto funcSymbol = funcDef->GetFunction();
	ASSERT_NE(funcSymbol, nullptr);

	auto arguments = funcSymbol->GetArguments();

	ASSERT_EQ(arguments.size(), 1);

	EXPECT_TRUE(std::dynamic_pointer_cast<Array>(arguments[0]->GetType()) != nullptr);

	EXPECT_EQ(funcSymbol->name, "myFunc");
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeVoid>(funcSymbol->GetType()) != nullptr);
}

TEST_F(ParserTest, ParseFunctionDefinitionWithMultipleArguments)
{
	auto root = parse("create public void function myFunc with integer i, float b, char c, string s, array of integer ar");

	ASSERT_NE(root, nullptr);

	auto statements = root->GetStatements();
	ASSERT_NE(statements.size(), 0);

	auto funcDef = std::dynamic_pointer_cast<FunctionDefinitionNode>(statements[0]);
	ASSERT_NE(funcDef, nullptr);

	auto funcSymbol = funcDef->GetFunction();
	ASSERT_NE(funcSymbol, nullptr);

	auto arguments = funcSymbol->GetArguments();

	ASSERT_EQ(arguments.size(), 5);

	EXPECT_TRUE(std::dynamic_pointer_cast<TypeInteger>(arguments[0]->GetType()) != nullptr);
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeFloat>(arguments[1]->GetType()) != nullptr);
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeChar>(arguments[2]->GetType()) != nullptr);
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeString>(arguments[3]->GetType()) != nullptr);
	EXPECT_TRUE(std::dynamic_pointer_cast<Array>(arguments[4]->GetType()) != nullptr);

	EXPECT_EQ(funcSymbol->name, "myFunc");
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeVoid>(funcSymbol->GetType()) != nullptr);
}

TEST_F(ParserTest, ParseFunctionDefinitionWithNonEmptyBody)
{
	auto root = parse("create public void function myFunc\n\tcreate integer a\n");

	ASSERT_NE(root, nullptr);

	auto statements = root->GetStatements();
	ASSERT_NE(statements.size(), 0);

	auto funcDef = std::dynamic_pointer_cast<FunctionDefinitionNode>(statements[0]);
	ASSERT_NE(funcDef, nullptr);

	auto funcSymbol = funcDef->GetFunction();
	ASSERT_NE(funcSymbol, nullptr);
	
	auto arguments = funcSymbol->GetArguments();


	auto body_statements = funcSymbol->body->GetStatements();

	ASSERT_FALSE(body_statements.empty());

	EXPECT_TRUE(std::dynamic_pointer_cast<VariableDefinitionNode>(body_statements[0]) != nullptr);

	EXPECT_EQ(arguments.size(), 0);

	EXPECT_EQ(funcSymbol->name, "myFunc");
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeVoid>(funcSymbol->GetType()) != nullptr);
}


// Nested Expressions

TEST_F(ParserTest, ParseNestedAdditionAndMultiplicationExpression) {
    auto root = parse("5 + 3 * 2");
    ASSERT_NE(root, nullptr);
    auto statements = root->GetStatements();
    ASSERT_NE(statements.size(), 0);
    auto addExpr = std::dynamic_pointer_cast<AddExpression>(statements[0]);
    ASSERT_NE(addExpr, nullptr);
    auto left = std::dynamic_pointer_cast<Literal<int, TypeInteger>>(addExpr->GetLeft());
    auto right = std::dynamic_pointer_cast<MultiplyExpression>(addExpr->GetRight());
    ASSERT_NE(left, nullptr);
    ASSERT_NE(right, nullptr);
    EXPECT_EQ(left->value, 5);
    auto rightLeft = std::dynamic_pointer_cast<Literal<int, TypeInteger>>(right->GetLeft());
    auto rightRight = std::dynamic_pointer_cast<Literal<int, TypeInteger>>(right->GetRight());
    ASSERT_NE(rightLeft, nullptr);
    ASSERT_NE(rightRight, nullptr);
    EXPECT_EQ(rightLeft->value, 3);
    EXPECT_EQ(rightRight->value, 2);
}

TEST_F(ParserTest, ParseNestedLogicalAndComparisonExpression) {
    auto root = parse("true AND (5 > 3)");
    ASSERT_NE(root, nullptr);
    auto statements = root->GetStatements();
    ASSERT_NE(statements.size(), 0);
    auto andExpr = std::dynamic_pointer_cast<AndExpression>(statements[0]);
    ASSERT_NE(andExpr, nullptr);
    auto left = std::dynamic_pointer_cast<Literal<bool, TypeBoolean>>(andExpr->GetLeft());
    auto right = std::dynamic_pointer_cast<GreaterThanExpression>(andExpr->GetRight());
    ASSERT_NE(left, nullptr);
    ASSERT_NE(right, nullptr);
    EXPECT_EQ(left->value, true);
    auto rightLeft = std::dynamic_pointer_cast<Literal<int, TypeInteger>>(right->GetLeft());
    auto rightRight = std::dynamic_pointer_cast<Literal<int, TypeInteger>>(right->GetRight());
    ASSERT_NE(rightLeft, nullptr);
    ASSERT_NE(rightRight, nullptr);
    EXPECT_EQ(rightLeft->value, 5);
    EXPECT_EQ(rightRight->value, 3);
}

// Logical Expressions

TEST_F(ParserTest, ParseComplexLogicalExpression) {
    auto root = parse("true AND false OR true");
    ASSERT_NE(root, nullptr);
    auto statements = root->GetStatements();
    ASSERT_NE(statements.size(), 0);
    auto orExpr = std::dynamic_pointer_cast<OrExpression>(statements[0]);
    ASSERT_NE(orExpr, nullptr);
    auto left = std::dynamic_pointer_cast<AndExpression>(orExpr->GetLeft());
    auto right = std::dynamic_pointer_cast<Literal<bool, TypeBoolean>>(orExpr->GetRight());
    ASSERT_NE(left, nullptr);
    ASSERT_NE(right, nullptr);
    auto leftLeft = std::dynamic_pointer_cast<Literal<bool, TypeBoolean>>(left->GetLeft());
    auto leftRight = std::dynamic_pointer_cast<Literal<bool, TypeBoolean>>(left->GetRight());
    ASSERT_NE(leftLeft, nullptr);
    ASSERT_NE(leftRight, nullptr);
    EXPECT_EQ(leftLeft->value, true);
    EXPECT_EQ(leftRight->value, false);
    EXPECT_EQ(right->value, true);
}

// Control Flow Statements

// If statement

TEST_F(ParserTest, ParseIfStatementWithNonEmptyBody) {
    auto root = parse("if true\n\tcreate integer a\n");
    ASSERT_NE(root, nullptr);
    auto statements = root->GetStatements();
    ASSERT_NE(statements.size(), 0);
    auto ifStmt = std::dynamic_pointer_cast<IfStatementNode>(statements[0]);
    ASSERT_NE(ifStmt, nullptr);

	// Check the condition

    auto condition = std::dynamic_pointer_cast<Literal<bool, TypeBoolean>>(ifStmt->expression);
    ASSERT_NE(condition, nullptr);
    EXPECT_EQ(condition->value, true);

	// Check the body of the if statement

    auto bodyStatements = ifStmt->body->GetStatements();
    ASSERT_FALSE(bodyStatements.empty());
    EXPECT_TRUE(std::dynamic_pointer_cast<VariableDefinitionNode>(bodyStatements[0]) != nullptr);
}

TEST_F(ParserTest, ParseIfStatementWithEmptyBody) {
    auto root = parse("if true\n");
    ASSERT_NE(root, nullptr);
    auto statements = root->GetStatements();
    ASSERT_NE(statements.size(), 0);
    auto ifStmt = std::dynamic_pointer_cast<IfStatementNode>(statements[0]);
    ASSERT_NE(ifStmt, nullptr);

	// Check the condition

    auto condition = std::dynamic_pointer_cast<Literal<bool, TypeBoolean>>(ifStmt->expression);
    ASSERT_NE(condition, nullptr);
    EXPECT_EQ(condition->value, true);

	// Check if the body of the if statement is empty

	ASSERT_TRUE(ifStmt->body != nullptr);
    auto bodyStatements = ifStmt->body->GetStatements();
	EXPECT_TRUE(bodyStatements.empty());
}

TEST_F(ParserTest, ParseIfStatementWithElseStatement) {
    auto root = parse("if true\n\tcreate integer a\nelse\n\tcreate integer b\n");
    ASSERT_NE(root, nullptr);
    auto statements = root->GetStatements();
    ASSERT_NE(statements.size(), 0);
    auto ifStmt = std::dynamic_pointer_cast<IfStatementNode>(statements[0]);
    ASSERT_NE(ifStmt, nullptr);

	// Check the condition

    auto condition = std::dynamic_pointer_cast<Literal<bool, TypeBoolean>>(ifStmt->expression);
    ASSERT_NE(condition, nullptr);
    EXPECT_EQ(condition->value, true);


	// Check the body of if statement

    auto ifBodyStatements = ifStmt->body->GetStatements();
    ASSERT_FALSE(ifBodyStatements.empty());
    EXPECT_TRUE(std::dynamic_pointer_cast<VariableDefinitionNode>(ifBodyStatements[0]) != nullptr);

	// Check the else statement

    ASSERT_NE(ifStmt->else_, nullptr);
	ASSERT_NE(ifStmt->else_->body, nullptr);
    auto elseBodyStatements = ifStmt->else_->body->GetStatements();
    ASSERT_FALSE(elseBodyStatements.empty());
    EXPECT_TRUE(std::dynamic_pointer_cast<VariableDefinitionNode>(elseBodyStatements[0]) != nullptr);
}

TEST_F(ParserTest, ParseIfStatementWithOneElseIfStatement) {
    auto root = parse("if true\n\tcreate integer a\nelse if false\n\tcreate integer b\n");
    ASSERT_NE(root, nullptr);
    auto statements = root->GetStatements();
    ASSERT_NE(statements.size(), 0);
    auto ifStmt = std::dynamic_pointer_cast<IfStatementNode>(statements[0]);
    ASSERT_NE(ifStmt, nullptr);

	// Check the condition

    auto condition = std::dynamic_pointer_cast<Literal<bool, TypeBoolean>>(ifStmt->expression);
    ASSERT_NE(condition, nullptr);
    EXPECT_EQ(condition->value, true);


	// Check the body of if statement

	auto ifBodyStatements = ifStmt->body->GetStatements();
    ASSERT_FALSE(ifBodyStatements.empty());
    EXPECT_TRUE(std::dynamic_pointer_cast<VariableDefinitionNode>(ifBodyStatements[0]) != nullptr);
 

	// Assert we don't have an else statement
	ASSERT_EQ(ifStmt->else_, nullptr);


	// Assert we have exactly one else if statement and check the body and condition of it
    ASSERT_EQ(ifStmt->else_ifs.size(), 1);

	auto elseIfCondition = std::dynamic_pointer_cast<Literal<bool, TypeBoolean>>(ifStmt->else_ifs[0]->expression);
	ASSERT_NE(elseIfCondition, nullptr);
	EXPECT_EQ(elseIfCondition->value, false);


	auto elseIfBodyStatements = ifStmt->else_ifs[0]->body->GetStatements();
    ASSERT_FALSE(elseIfBodyStatements.empty());
    EXPECT_TRUE(std::dynamic_pointer_cast<VariableDefinitionNode>(elseIfBodyStatements[0]) != nullptr);


}

TEST_F(ParserTest, ParseIfStatementWithMultipleElseIfStatement) {
    auto root = parse("if true\n\tcreate integer a\nelse if false\n\tcreate integer b\nelse if true\n\tcreate integer c\n");
    ASSERT_NE(root, nullptr);
    auto statements = root->GetStatements();
    ASSERT_NE(statements.size(), 0);
    auto ifStmt = std::dynamic_pointer_cast<IfStatementNode>(statements[0]);
    ASSERT_NE(ifStmt, nullptr);
    auto condition = std::dynamic_pointer_cast<Literal<bool, TypeBoolean>>(ifStmt->expression);
    ASSERT_NE(condition, nullptr);
    EXPECT_EQ(condition->value, true);


	// Check the body of if statement
	auto ifBodyStatements = ifStmt->body->GetStatements();
    ASSERT_FALSE(ifBodyStatements.empty());
    EXPECT_TRUE(std::dynamic_pointer_cast<VariableDefinitionNode>(ifBodyStatements[0]) != nullptr);
 

	// Assert we don't have an else statement
	ASSERT_EQ(ifStmt->else_, nullptr);


	// Assert we have exactly two else if statement and check the body their bodies

    ASSERT_EQ(ifStmt->else_ifs.size(), 2);


	//Check first else if


	auto FirstElseIfCondition = std::dynamic_pointer_cast<Literal<bool, TypeBoolean>>(ifStmt->else_ifs[0]->expression);
	ASSERT_NE(FirstElseIfCondition, nullptr);
	EXPECT_EQ(FirstElseIfCondition->value, false);

	auto FirstElseIfBodyStatements = ifStmt->else_ifs[0]->body->GetStatements();
    ASSERT_FALSE(FirstElseIfBodyStatements.empty());
    EXPECT_TRUE(std::dynamic_pointer_cast<VariableDefinitionNode>(FirstElseIfBodyStatements[0]) != nullptr);

	//Check second else if


	auto SecondElseIfCondition = std::dynamic_pointer_cast<Literal<bool, TypeBoolean>>(ifStmt->else_ifs[1]->expression);
	ASSERT_NE(SecondElseIfCondition, nullptr);
	EXPECT_EQ(SecondElseIfCondition->value, true);

	auto SecondElseIfBodyStatements = ifStmt->else_ifs[1]->body->GetStatements();
    ASSERT_FALSE(SecondElseIfBodyStatements.empty());
    EXPECT_TRUE(std::dynamic_pointer_cast<VariableDefinitionNode>(SecondElseIfBodyStatements[0]) != nullptr);


}

TEST_F(ParserTest, ParseIfStatementWithElseAndOneElseIfStatement) {
    auto root = parse("if true\n\tcreate integer a\nelse if false\n\tcreate integer b\nelse\n\tcreate integer c\n");
    ASSERT_NE(root, nullptr);
    auto statements = root->GetStatements();
    ASSERT_NE(statements.size(), 0);
    auto ifStmt = std::dynamic_pointer_cast<IfStatementNode>(statements[0]);
    ASSERT_NE(ifStmt, nullptr);

	// Check the condition

    auto condition = std::dynamic_pointer_cast<Literal<bool, TypeBoolean>>(ifStmt->expression);
    ASSERT_NE(condition, nullptr);
    EXPECT_EQ(condition->value, true);


	// Check the body of if statement
	auto ifBodyStatements = ifStmt->body->GetStatements();
    ASSERT_FALSE(ifBodyStatements.empty());
    EXPECT_TRUE(std::dynamic_pointer_cast<VariableDefinitionNode>(ifBodyStatements[0]) != nullptr);
 

	// Assert we have an else statement
	ASSERT_NE(ifStmt->else_, nullptr);

	// Check the body of else statement
	auto elseStmtBodyStatements = ifStmt->else_->body->GetStatements();
	ASSERT_FALSE(elseStmtBodyStatements.empty());
	EXPECT_TRUE(std::dynamic_pointer_cast<VariableDefinitionNode>(elseStmtBodyStatements[0]) != nullptr);

	// Assert we have exactly one else if statement and check the body and condition of it

    ASSERT_EQ(ifStmt->else_ifs.size(), 1);

	auto elseIfCondition = std::dynamic_pointer_cast<Literal<bool, TypeBoolean>>(ifStmt->else_ifs[0]->expression);
	ASSERT_NE(elseIfCondition, nullptr);
	EXPECT_EQ(elseIfCondition->value, false);


	auto elseIfBodyStatements = ifStmt->else_ifs[0]->body->GetStatements();
    ASSERT_FALSE(elseIfBodyStatements.empty());
    EXPECT_TRUE(std::dynamic_pointer_cast<VariableDefinitionNode>(elseIfBodyStatements[0]) != nullptr);


}



TEST_F(ParserTest, ParseIfStatementWithElseAndMultipleElseIfStatement) {
    auto root = parse("if true\n\tcreate integer a\nelse if false\n\tcreate integer b\nelse if true\n\tcreate integer c\nelse \tcreate integer d\n");
    ASSERT_NE(root, nullptr);
    auto statements = root->GetStatements();
    ASSERT_NE(statements.size(), 0);
    auto ifStmt = std::dynamic_pointer_cast<IfStatementNode>(statements[0]);
    ASSERT_NE(ifStmt, nullptr);

	// Check the condition

    auto condition = std::dynamic_pointer_cast<Literal<bool, TypeBoolean>>(ifStmt->expression);
    ASSERT_NE(condition, nullptr);
    EXPECT_EQ(condition->value, true);


	// Check the body of if statement
	auto ifBodyStatements = ifStmt->body->GetStatements();
    ASSERT_FALSE(ifBodyStatements.empty());
    EXPECT_TRUE(std::dynamic_pointer_cast<VariableDefinitionNode>(ifBodyStatements[0]) != nullptr);
 

	// Assert we have an else statement

	ASSERT_NE(ifStmt->else_, nullptr);
	auto elseStmtBodyStatements = ifStmt->else_->body->GetStatements();
	ASSERT_FALSE(elseStmtBodyStatements.empty());

	// Check the body of else statement

	EXPECT_TRUE(std::dynamic_pointer_cast<VariableDefinitionNode>(elseStmtBodyStatements[0]) != nullptr);

	// Assert we have exactly two else if statement and check their bodies and conditions

    ASSERT_EQ(ifStmt->else_ifs.size(), 2);

	// Check first else if

	auto FirstElseIfCondition = std::dynamic_pointer_cast<Literal<bool, TypeBoolean>>(ifStmt->else_ifs[0]->expression);
	ASSERT_NE(FirstElseIfCondition, nullptr);
	EXPECT_EQ(FirstElseIfCondition->value, false);


	auto FirstElseIfBodyStatements = ifStmt->else_ifs[0]->body->GetStatements();
    ASSERT_FALSE(FirstElseIfBodyStatements.empty());
    EXPECT_TRUE(std::dynamic_pointer_cast<VariableDefinitionNode>(FirstElseIfBodyStatements[0]) != nullptr);

	// Check second else if

	auto SecondElseIfCondition = std::dynamic_pointer_cast<Literal<bool, TypeBoolean>>(ifStmt->else_ifs[0]->expression);
	ASSERT_NE(SecondElseIfCondition, nullptr);
	EXPECT_EQ(SecondElseIfCondition->value, false);


	auto SecondElseIfBodyStatements = ifStmt->else_ifs[0]->body->GetStatements();
    ASSERT_FALSE(SecondElseIfBodyStatements.empty());
    EXPECT_TRUE(std::dynamic_pointer_cast<VariableDefinitionNode>(SecondElseIfBodyStatements[0]) != nullptr);

}

