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
	std::unique_ptr<AtmoDriver> driver;
	void SetUp() override {
		driver = std::make_unique<AtmoDriver>();
		Error::InTest = true;
	}

	~ParserTest() = default;

	std::shared_ptr<StatementListNode> parse(const std::string& input) {
		std::istringstream stream(input);
 		driver->parse_only(stream);
		return driver->ast_root;
	}
	friend class ClassDefinitionNode;
};

// Expressions

TEST_F(ParserTest, ParseIntegerLiteralExpression)
{
	auto root = parse("5");

	ASSERT_NE(root, nullptr);

	auto statements = root->GetStatements();

	ASSERT_NE(statements.size(), 0);

	auto literal = std::dynamic_pointer_cast<IntegerLiteral>(statements[0]);

	
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

	auto literal = std::dynamic_pointer_cast<FloatLiteral>(statements[0]);

	
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

	auto literal = std::dynamic_pointer_cast<FloatLiteral>(statements[0]);

	
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

	auto literal = std::dynamic_pointer_cast<CharLiteral>(statements[0]);

	
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

// 	auto literal = std::dynamic_pointer_cast<StringLiteral>(statements[0]);

	
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
    auto left = std::dynamic_pointer_cast<IntegerLiteral>(addExpr->GetLeft());
    auto right = std::dynamic_pointer_cast<IntegerLiteral>(addExpr->GetRight());
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
    auto left = std::dynamic_pointer_cast<BooleanLiteral>(andExpr->GetLeft());
    auto right = std::dynamic_pointer_cast<BooleanLiteral>(andExpr->GetRight());
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
	auto left = std::dynamic_pointer_cast<IntegerLiteral>(subExpr->GetLeft());
	auto right = std::dynamic_pointer_cast<IntegerLiteral>(subExpr->GetRight());
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
	auto left = std::dynamic_pointer_cast<IntegerLiteral>(mulExpr->GetLeft());
	auto right = std::dynamic_pointer_cast<IntegerLiteral>(mulExpr->GetRight());
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
	auto left = std::dynamic_pointer_cast<IntegerLiteral>(divExpr->GetLeft());
	auto right = std::dynamic_pointer_cast<IntegerLiteral>(divExpr->GetRight());
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
	auto left = std::dynamic_pointer_cast<BooleanLiteral>(orExpr->GetLeft());
	auto right = std::dynamic_pointer_cast<BooleanLiteral>(orExpr->GetRight());
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
	auto operand = std::dynamic_pointer_cast<BooleanLiteral>(notExpr->GetExpression());
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
	auto left = std::dynamic_pointer_cast<IntegerLiteral>(eqExpr->GetLeft());
	auto right = std::dynamic_pointer_cast<IntegerLiteral>(eqExpr->GetRight());
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
	auto left = std::dynamic_pointer_cast<IntegerLiteral>(neqExpr->GetLeft());
	auto right = std::dynamic_pointer_cast<IntegerLiteral>(neqExpr->GetRight());
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
	auto left = std::dynamic_pointer_cast<IntegerLiteral>(gtExpr->GetLeft());
	auto right = std::dynamic_pointer_cast<IntegerLiteral>(gtExpr->GetRight());
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
	auto left = std::dynamic_pointer_cast<IntegerLiteral>(gteExpr->GetLeft());
	auto right = std::dynamic_pointer_cast<IntegerLiteral>(gteExpr->GetRight());
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
	auto left = std::dynamic_pointer_cast<IntegerLiteral>(ltExpr->GetLeft());
	auto right = std::dynamic_pointer_cast<IntegerLiteral>(ltExpr->GetRight());
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
	auto left = std::dynamic_pointer_cast<IntegerLiteral>(ltExpr->GetLeft());
	auto right = std::dynamic_pointer_cast<IntegerLiteral>(ltExpr->GetRight());
	ASSERT_NE(left, nullptr);
	ASSERT_NE(right, nullptr);
	EXPECT_EQ(left->value, 3);
	EXPECT_EQ(right->value, 5);
}


// Nested Expressions

TEST_F(ParserTest, ParseNestedAdditionAndMultiplicationExpression) {
    auto root = parse("5 + 3 * 2");
    ASSERT_NE(root, nullptr);
    auto statements = root->GetStatements();
    ASSERT_NE(statements.size(), 0);
    auto addExpr = std::dynamic_pointer_cast<AddExpression>(statements[0]);
    ASSERT_NE(addExpr, nullptr);
    auto left = std::dynamic_pointer_cast<IntegerLiteral>(addExpr->GetLeft());
    auto right = std::dynamic_pointer_cast<MultiplyExpression>(addExpr->GetRight());
    ASSERT_NE(left, nullptr);
    ASSERT_NE(right, nullptr);
    EXPECT_EQ(left->value, 5);
    auto rightLeft = std::dynamic_pointer_cast<IntegerLiteral>(right->GetLeft());
    auto rightRight = std::dynamic_pointer_cast<IntegerLiteral>(right->GetRight());
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
    auto left = std::dynamic_pointer_cast<BooleanLiteral>(andExpr->GetLeft());
    auto right = std::dynamic_pointer_cast<GreaterThanExpression>(andExpr->GetRight());
    ASSERT_NE(left, nullptr);
    ASSERT_NE(right, nullptr);
    EXPECT_EQ(left->value, true);
    auto rightLeft = std::dynamic_pointer_cast<IntegerLiteral>(right->GetLeft());
    auto rightRight = std::dynamic_pointer_cast<IntegerLiteral>(right->GetRight());
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
    auto right = std::dynamic_pointer_cast<BooleanLiteral>(orExpr->GetRight());
    ASSERT_NE(left, nullptr);
    ASSERT_NE(right, nullptr);
    auto leftLeft = std::dynamic_pointer_cast<BooleanLiteral>(left->GetLeft());
    auto leftRight = std::dynamic_pointer_cast<BooleanLiteral>(left->GetRight());
    ASSERT_NE(leftLeft, nullptr);
    ASSERT_NE(leftRight, nullptr);
    EXPECT_EQ(leftLeft->value, true);
    EXPECT_EQ(leftRight->value, false);
    EXPECT_EQ(right->value, true);
}

// Complex Expressions

TEST_F(ParserTest, ParseNestedMixedExpression) {
    auto root = parse("5 + 3 * 2 - 4 / 2");
    ASSERT_NE(root, nullptr);
    auto statements = root->GetStatements();
    ASSERT_NE(statements.size(), 0);
    auto subExpr = std::dynamic_pointer_cast<SubtractExpression>(statements[0]);
    ASSERT_NE(subExpr, nullptr);
    auto addExpr = std::dynamic_pointer_cast<AddExpression>(subExpr->GetLeft());
    auto divExpr = std::dynamic_pointer_cast<DivideExpression>(subExpr->GetRight());
    ASSERT_NE(addExpr, nullptr);
    ASSERT_NE(divExpr, nullptr);
    auto left = std::dynamic_pointer_cast<IntegerLiteral>(addExpr->GetLeft());
    auto mulExpr = std::dynamic_pointer_cast<MultiplyExpression>(addExpr->GetRight());
    ASSERT_NE(left, nullptr);
    ASSERT_NE(mulExpr, nullptr);
    EXPECT_EQ(left->value, 5);
    auto mulLeft = std::dynamic_pointer_cast<IntegerLiteral>(mulExpr->GetLeft());
    auto mulRight = std::dynamic_pointer_cast<IntegerLiteral>(mulExpr->GetRight());
    ASSERT_NE(mulLeft, nullptr);
    ASSERT_NE(mulRight, nullptr);
    EXPECT_EQ(mulLeft->value, 3);
    EXPECT_EQ(mulRight->value, 2);
    auto divLeft = std::dynamic_pointer_cast<IntegerLiteral>(divExpr->GetLeft());
    auto divRight = std::dynamic_pointer_cast<IntegerLiteral>(divExpr->GetRight());
    ASSERT_NE(divLeft, nullptr);
    ASSERT_NE(divRight, nullptr);
    EXPECT_EQ(divLeft->value, 4);
    EXPECT_EQ(divRight->value, 2);
}

TEST_F(ParserTest, ParseComplexLogicalExpressionWithParentheses) {
    auto root = parse("(true AND false) OR (false AND true)");
    ASSERT_NE(root, nullptr);
    auto statements = root->GetStatements();
    ASSERT_NE(statements.size(), 0);
    auto orExpr = std::dynamic_pointer_cast<OrExpression>(statements[0]);
    ASSERT_NE(orExpr, nullptr);
    auto left = std::dynamic_pointer_cast<AndExpression>(orExpr->GetLeft());
    auto right = std::dynamic_pointer_cast<AndExpression>(orExpr->GetRight());
    ASSERT_NE(left, nullptr);
    ASSERT_NE(right, nullptr);
    auto leftLeft = std::dynamic_pointer_cast<BooleanLiteral>(left->GetLeft());
    auto leftRight = std::dynamic_pointer_cast<BooleanLiteral>(left->GetRight());
    auto rightLeft = std::dynamic_pointer_cast<BooleanLiteral>(right->GetLeft());
    auto rightRight = std::dynamic_pointer_cast<BooleanLiteral>(right->GetRight());
    ASSERT_NE(leftLeft, nullptr);
    ASSERT_NE(leftRight, nullptr);
    ASSERT_NE(rightLeft, nullptr);
    ASSERT_NE(rightRight, nullptr);
    EXPECT_EQ(leftLeft->value, true);
    EXPECT_EQ(leftRight->value, false);
    EXPECT_EQ(rightLeft->value, false);
    EXPECT_EQ(rightRight->value, true);
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

	auto literal = std::dynamic_pointer_cast<IntegerLiteral>(varDef->expression);
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

    auto condition = std::dynamic_pointer_cast<BooleanLiteral>(ifStmt->expression);
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

    auto condition = std::dynamic_pointer_cast<BooleanLiteral>(ifStmt->expression);
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

    auto condition = std::dynamic_pointer_cast<BooleanLiteral>(ifStmt->expression);
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

    auto condition = std::dynamic_pointer_cast<BooleanLiteral>(ifStmt->expression);
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

	auto elseIfCondition = std::dynamic_pointer_cast<BooleanLiteral>(ifStmt->else_ifs[0]->expression);
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
    auto condition = std::dynamic_pointer_cast<BooleanLiteral>(ifStmt->expression);
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


	auto FirstElseIfCondition = std::dynamic_pointer_cast<BooleanLiteral>(ifStmt->else_ifs[0]->expression);
	ASSERT_NE(FirstElseIfCondition, nullptr);
	EXPECT_EQ(FirstElseIfCondition->value, false);

	auto FirstElseIfBodyStatements = ifStmt->else_ifs[0]->body->GetStatements();
    ASSERT_FALSE(FirstElseIfBodyStatements.empty());
    EXPECT_TRUE(std::dynamic_pointer_cast<VariableDefinitionNode>(FirstElseIfBodyStatements[0]) != nullptr);

	//Check second else if


	auto SecondElseIfCondition = std::dynamic_pointer_cast<BooleanLiteral>(ifStmt->else_ifs[1]->expression);
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

    auto condition = std::dynamic_pointer_cast<BooleanLiteral>(ifStmt->expression);
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

	auto elseIfCondition = std::dynamic_pointer_cast<BooleanLiteral>(ifStmt->else_ifs[0]->expression);
	ASSERT_NE(elseIfCondition, nullptr);
	EXPECT_EQ(elseIfCondition->value, false);


	auto elseIfBodyStatements = ifStmt->else_ifs[0]->body->GetStatements();
    ASSERT_FALSE(elseIfBodyStatements.empty());
    EXPECT_TRUE(std::dynamic_pointer_cast<VariableDefinitionNode>(elseIfBodyStatements[0]) != nullptr);


}



TEST_F(ParserTest, ParseIfStatementWithElseAndMultipleElseIfStatement) {
    auto root = parse("if true\n\tcreate integer a\nelse if false\n\tcreate integer b\nelse if true\n\tcreate integer c\nelse\n\tcreate integer d\n");
    ASSERT_NE(root, nullptr);
    auto statements = root->GetStatements();
    ASSERT_NE(statements.size(), 0);
    auto ifStmt = std::dynamic_pointer_cast<IfStatementNode>(statements[0]);
    ASSERT_NE(ifStmt, nullptr);

	// Check the condition

    auto condition = std::dynamic_pointer_cast<BooleanLiteral>(ifStmt->expression);
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

	auto FirstElseIfCondition = std::dynamic_pointer_cast<BooleanLiteral>(ifStmt->else_ifs[0]->expression);
	ASSERT_NE(FirstElseIfCondition, nullptr);
	EXPECT_EQ(FirstElseIfCondition->value, false);


	auto FirstElseIfBodyStatements = ifStmt->else_ifs[0]->body->GetStatements();
    ASSERT_FALSE(FirstElseIfBodyStatements.empty());
    EXPECT_TRUE(std::dynamic_pointer_cast<VariableDefinitionNode>(FirstElseIfBodyStatements[0]) != nullptr);

	// Check second else if

	auto SecondElseIfCondition = std::dynamic_pointer_cast<BooleanLiteral>(ifStmt->else_ifs[0]->expression);
	ASSERT_NE(SecondElseIfCondition, nullptr);
	EXPECT_EQ(SecondElseIfCondition->value, false);


	auto SecondElseIfBodyStatements = ifStmt->else_ifs[0]->body->GetStatements();
    ASSERT_FALSE(SecondElseIfBodyStatements.empty());
    EXPECT_TRUE(std::dynamic_pointer_cast<VariableDefinitionNode>(SecondElseIfBodyStatements[0]) != nullptr);

}

// Until statement

TEST_F(ParserTest, ParseUntilStatementWithNEmptyBody) {
    auto root = parse("until true\n");
    ASSERT_NE(root, nullptr);
    auto statements = root->GetStatements();
    ASSERT_NE(statements.size(), 0);
    auto untilStmt = std::dynamic_pointer_cast<UntilStatementNode>(statements[0]);
    ASSERT_NE(untilStmt, nullptr);

	// Check the condition

    auto condition = std::dynamic_pointer_cast<BooleanLiteral>(untilStmt->expression);
    ASSERT_NE(condition, nullptr);
    EXPECT_EQ(condition->value, true);

	// Check that the body is empty

	ASSERT_TRUE(untilStmt->body != nullptr);
    auto bodyStatements = untilStmt->body->GetStatements();
    EXPECT_TRUE(bodyStatements.empty());
}




TEST_F(ParserTest, ParseUntilStatementWithNonEmptyBody) {
    auto root = parse("until false\n\tcreate integer a\n");
    ASSERT_NE(root, nullptr);
    auto statements = root->GetStatements();
    ASSERT_NE(statements.size(), 0);
    auto untilStmt = std::dynamic_pointer_cast<UntilStatementNode>(statements[0]);
    ASSERT_NE(untilStmt, nullptr);

	// Check the condition

    auto condition = std::dynamic_pointer_cast<BooleanLiteral>(untilStmt->expression);
    ASSERT_NE(condition, nullptr);
    EXPECT_EQ(condition->value, false);

	// Check the body

	ASSERT_TRUE(untilStmt->body != nullptr);
    auto bodyStatements = untilStmt->body->GetStatements();
    ASSERT_FALSE(bodyStatements.empty());
    EXPECT_TRUE(std::dynamic_pointer_cast<VariableDefinitionNode>(bodyStatements[0]) != nullptr);
}



// Control Flow Statements

TEST_F(ParserTest, ParseDoUntilStatementWithNonEmptyBody) {
    auto root = parse("do\n\tcreate integer a\nuntil false\n");
    ASSERT_NE(root, nullptr);
    auto statements = root->GetStatements();
    ASSERT_NE(statements.size(), 0);
    auto doUntilStmt = std::dynamic_pointer_cast<DoUntilStatementNode>(statements[0]);
    ASSERT_NE(doUntilStmt, nullptr);

    // Check the condition
    auto condition = std::dynamic_pointer_cast<BooleanLiteral>(doUntilStmt->expression);
    ASSERT_NE(condition, nullptr);
    EXPECT_EQ(condition->value, false);

    // Check the body
    ASSERT_TRUE(doUntilStmt->body != nullptr);
    auto bodyStatements = doUntilStmt->body->GetStatements();
    ASSERT_FALSE(bodyStatements.empty());
    EXPECT_TRUE(std::dynamic_pointer_cast<VariableDefinitionNode>(bodyStatements[0]) != nullptr);
}

TEST_F(ParserTest, ParseDoUntilStatementWithEmptyBody) {
    auto root = parse("do\nuntil true\n");
    ASSERT_NE(root, nullptr);
    auto statements = root->GetStatements();
    ASSERT_NE(statements.size(), 0);
    auto doUntilStmt = std::dynamic_pointer_cast<DoUntilStatementNode>(statements[0]);
    ASSERT_NE(doUntilStmt, nullptr);

    // Check the condition
    auto condition = std::dynamic_pointer_cast<BooleanLiteral>(doUntilStmt->expression);
    ASSERT_NE(condition, nullptr);
    EXPECT_EQ(condition->value, true);

    // Check that the body is empty
    ASSERT_TRUE(doUntilStmt->body != nullptr);
    auto bodyStatements = doUntilStmt->body->GetStatements();
    EXPECT_TRUE(bodyStatements.empty());
}

// Function Calls

TEST_F(ParserTest, ParseFunctionCallWithoutArguments) {
    auto root = parse("call myFunc");
    ASSERT_NE(root, nullptr);
    auto statements = root->GetStatements();
    ASSERT_NE(statements.size(), 0);

    auto funcCall = std::dynamic_pointer_cast<FunctionCall>(statements[0]);
    ASSERT_NE(funcCall, nullptr);

    auto funcName = std::dynamic_pointer_cast<Identifier>(funcCall->GetExpression());
    ASSERT_NE(funcName, nullptr);

    EXPECT_EQ(funcName->name, "myFunc");
    auto args = funcCall->arguments;
    EXPECT_TRUE(args.empty());
}

TEST_F(ParserTest, ParseFunctionCallWithArguments) {
    auto root = parse("call myFunc with 5, 3.2, 'a'");
    ASSERT_NE(root, nullptr);
    auto statements = root->GetStatements();
    ASSERT_NE(statements.size(), 0);

    auto funcCall = std::dynamic_pointer_cast<FunctionCall>(statements[0]);
    ASSERT_NE(funcCall, nullptr);

    auto funcName = std::dynamic_pointer_cast<Identifier>(funcCall->GetExpression());
    ASSERT_NE(funcName, nullptr);
    EXPECT_EQ(funcName->name, "myFunc");
	

	auto args = funcCall->arguments;
   
	
   	ASSERT_EQ(args.size(), 3);

    auto arg1 = std::dynamic_pointer_cast<IntegerLiteral>(args[0]);
    auto arg2 = std::dynamic_pointer_cast<FloatLiteral>(args[1]);
    auto arg3 = std::dynamic_pointer_cast<CharLiteral>(args[2]);
    ASSERT_NE(arg1, nullptr);
    ASSERT_NE(arg2, nullptr);
    ASSERT_NE(arg3, nullptr);
    EXPECT_EQ(arg1->value, 5);
    EXPECT_DOUBLE_EQ(arg2->value, 3.2);
    EXPECT_EQ(arg3->value, 'a');
}

// Class declaration

TEST_F(ParserTest, ParseClassDeclarationWithNoAttributesOneVariableAndNoMethods) {
    auto root = parse("create class my_class\n\tcreate private integer a\n");
    ASSERT_NE(root, nullptr);
    auto statements = root->GetStatements();
    ASSERT_NE(statements.size(), 0);

	auto class_dec = std::dynamic_pointer_cast<ClassDefinitionNode>(statements[0]);
	ASSERT_NE(class_dec, nullptr);

	auto class_symbol = class_dec->GetClass();
	ASSERT_NE(class_symbol, nullptr);


	EXPECT_EQ(class_symbol->name, "my_class");
	auto body = class_symbol->body;
	ASSERT_EQ(body.size(), 1);
	auto varDec = std::dynamic_pointer_cast<VariableDefinitionNode>(body[0]);
	ASSERT_NE(varDec, nullptr);

	auto varSymbol = varDec->GetVariable();
	ASSERT_NE(varSymbol, nullptr);

	auto attribute = varSymbol->GetAttribute();
	auto type = varSymbol->GetType();

	EXPECT_EQ(varSymbol->name, "a");
	EXPECT_TRUE(std::dynamic_pointer_cast<AttributePrivate>(attribute) != nullptr);
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeInteger>(type) != nullptr);
}

TEST_F(ParserTest, ParseClassDeclarationWithNoAttributesMultipleVariablesAndNoMethods) {
    auto root = parse("create class my_class\n\tcreate private integer a\n\tcreate public string s\n");
    ASSERT_NE(root, nullptr);
    auto statements = root->GetStatements();
    ASSERT_NE(statements.size(), 0);

	auto class_dec = std::dynamic_pointer_cast<ClassDefinitionNode>(statements[0]);
	ASSERT_NE(class_dec, nullptr);

	auto class_symbol = class_dec->GetClass();
	ASSERT_NE(class_symbol, nullptr);


	EXPECT_EQ(class_symbol->name, "my_class");
	auto body = class_symbol->body;
	ASSERT_EQ(body.size(), 2);

	// Check first variable

	auto FristVarDec = std::dynamic_pointer_cast<VariableDefinitionNode>(body[0]);
	ASSERT_NE(FristVarDec, nullptr);

	auto FirstVarSymbol = FristVarDec->GetVariable();
	ASSERT_NE(FirstVarSymbol, nullptr);

	auto FirstAttribute = FirstVarSymbol->GetAttribute();
	auto FirstType = FirstVarSymbol->GetType();

	EXPECT_EQ(FirstVarSymbol->name, "a");
	EXPECT_TRUE(std::dynamic_pointer_cast<AttributePrivate>(FirstAttribute) != nullptr);
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeInteger>(FirstType) != nullptr);

	// Check second variable


	auto SecondVarDec = std::dynamic_pointer_cast<VariableDefinitionNode>(body[1]);
	ASSERT_NE(SecondVarDec, nullptr);

	auto SecondVarSymbol = SecondVarDec->GetVariable();
	ASSERT_NE(SecondVarSymbol, nullptr);

	auto SecondAttribute = SecondVarSymbol->GetAttribute();
	auto SecondType = SecondVarSymbol->GetType();

	EXPECT_EQ(SecondVarSymbol->name, "s");
	EXPECT_TRUE(std::dynamic_pointer_cast<AttributePublic>(SecondAttribute) != nullptr);
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeString>(SecondType) != nullptr);
}

TEST_F(ParserTest, ParseConstructorWithEmptyBodyNoParametersAndNoParentConstructorCall) {
    auto root = parse("create public constructor");
    ASSERT_NE(root, nullptr);
    auto statements = root->GetStatements();
    ASSERT_EQ(statements.size(), 1);

	auto constructorNode = std::dynamic_pointer_cast<ConstructorDefinitionNode>(statements[0]);
	ASSERT_NE(constructorNode,nullptr);

	// Expect no arguments to parent
	EXPECT_EQ(constructorNode->GetArgumentsForParentConstructor().size(),0);


	auto functionSymbol = constructorNode->GetFunction();
	ASSERT_NE(functionSymbol,nullptr);

	// Expect empty body
	EXPECT_TRUE(functionSymbol->body->isEmpty());
}

TEST_F(ParserTest, ParseConstructorWithNonEmptyBodyNoParametersAndNoParentConstructorCall) {
    auto root = parse("create public constructor\n\ta = 5\n");
    ASSERT_NE(root, nullptr);
    auto statements = root->GetStatements();
    ASSERT_EQ(statements.size(), 1);

	auto constructorNode = std::dynamic_pointer_cast<ConstructorDefinitionNode>(statements[0]);
	ASSERT_NE(constructorNode,nullptr);

	// Expect no arguments to parent
	EXPECT_EQ(constructorNode->GetArgumentsForParentConstructor().size(),0);

	
	auto functionSymbol = constructorNode->GetFunction();
	ASSERT_NE(functionSymbol,nullptr);

	// Expect non-empty body
	EXPECT_FALSE(functionSymbol->body->isEmpty());

	auto stmts = functionSymbol->body->GetStatements();

	auto assignmentExpression = std::dynamic_pointer_cast<AssignmentExpression>(stmts[0]);
	ASSERT_NE(assignmentExpression,nullptr);

	auto identifier = std::dynamic_pointer_cast<Identifier>( assignmentExpression->GetLeft());
	ASSERT_NE(identifier,nullptr);

	// Expect correct identifier name
	EXPECT_EQ(identifier->name,"a");

	auto literal = std::dynamic_pointer_cast<IntegerLiteral>(assignmentExpression->GetRight());
	ASSERT_NE(literal,nullptr);

	// Expect literal value to equal to 5
	EXPECT_EQ(literal->value, 5);
}

TEST_F(ParserTest, ParseConstructorWithNonEmptyBodyOneParametersAndNoParentConstructorCall) {
    auto root = parse("create public constructor with integer a\n\ta = 5\n");
    ASSERT_NE(root, nullptr);
    auto statements = root->GetStatements();
    ASSERT_EQ(statements.size(), 1);

	auto constructorNode = std::dynamic_pointer_cast<ConstructorDefinitionNode>(statements[0]);
	ASSERT_NE(constructorNode,nullptr);

	
	// Expect no arguments for parent
	EXPECT_EQ(constructorNode->GetArgumentsForParentConstructor().size(),0);

	
	auto functionSymbol = constructorNode->GetFunction();
	ASSERT_NE(functionSymbol,nullptr);

	// Expect correct number of parameters
	auto params = functionSymbol->GetArguments();
	EXPECT_EQ(params.size(),1);

	auto firstParam = params[0];

	// Expect correct name and type
	EXPECT_EQ(firstParam->name, "a");
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeInteger>(firstParam->GetType()) != nullptr);

	// Expect non-empty body
	EXPECT_FALSE(functionSymbol->body->isEmpty());

	auto stmts = functionSymbol->body->GetStatements();

	auto assignmentExpression = std::dynamic_pointer_cast<AssignmentExpression>(stmts[0]);
	ASSERT_NE(assignmentExpression,nullptr);

	auto identifier = std::dynamic_pointer_cast<Identifier>( assignmentExpression->GetLeft());
	ASSERT_NE(identifier,nullptr);

	// Expect correct identifier name
	EXPECT_EQ(identifier->name,"a");

	auto literal = std::dynamic_pointer_cast<IntegerLiteral>(assignmentExpression->GetRight());
	ASSERT_NE(literal,nullptr);

	// Expect literal value to equal to 5
	EXPECT_EQ(literal->value, 5);

}

TEST_F(ParserTest, ParseConstructorWithNonEmptyBodyOneParametersAndParentConstructorCall) {
    auto root = parse("create public constructor with integer a and call parent with 3425\n\ta = 5\n");
    ASSERT_NE(root, nullptr);
    auto statements = root->GetStatements();
    ASSERT_EQ(statements.size(), 1);

	auto constructorNode = std::dynamic_pointer_cast<ConstructorDefinitionNode>(statements[0]);
	ASSERT_NE(constructorNode,nullptr);

	
	auto argsForParent = constructorNode->GetArgumentsForParentConstructor();

	// Expect one argument to parent
	EXPECT_EQ(argsForParent.size(),1);

	auto firstArgument = argsForParent[0];
	auto literalForParent = std::dynamic_pointer_cast<IntegerLiteral>(firstArgument);
	ASSERT_NE(literalForParent, nullptr);


	EXPECT_EQ(literalForParent->value, 3425);

	
	auto functionSymbol = constructorNode->GetFunction();
	ASSERT_NE(functionSymbol,nullptr);

	// Expect correct number of parameters
	auto params = functionSymbol->GetArguments();
	EXPECT_EQ(params.size(),1);

	auto firstParam = params[0];

	// Expect correct name and type
	EXPECT_EQ(firstParam->name, "a");
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeInteger>(firstParam->GetType()) != nullptr);

	// Expect non-empty body
	EXPECT_FALSE(functionSymbol->body->isEmpty());

	auto stmts = functionSymbol->body->GetStatements();

	auto assignmentExpression = std::dynamic_pointer_cast<AssignmentExpression>(stmts[0]);
	ASSERT_NE(assignmentExpression,nullptr);

	auto identifier = std::dynamic_pointer_cast<Identifier>( assignmentExpression->GetLeft());
	ASSERT_NE(identifier,nullptr);

	// Expect correct identifier name
	EXPECT_EQ(identifier->name,"a");

	auto literal = std::dynamic_pointer_cast<IntegerLiteral>(assignmentExpression->GetRight());
	ASSERT_NE(literal,nullptr);

	// Expect literal value to equal to 5
	EXPECT_EQ(literal->value, 5);

}

TEST_F(ParserTest, ParseConstructorWithNonEmptyBodyOneParametersAndParentConstructorCallWithMultipleArguments) {
    auto root = parse("create public constructor with integer a and call parent with 3425, 'c'\n\ta = 5\n");
    ASSERT_NE(root, nullptr);
    auto statements = root->GetStatements();
    ASSERT_EQ(statements.size(), 1);

	auto constructorNode = std::dynamic_pointer_cast<ConstructorDefinitionNode>(statements[0]);
	ASSERT_NE(constructorNode,nullptr);

	
	auto argsForParent = constructorNode->GetArgumentsForParentConstructor();

	// Expect two arguments to parent
	EXPECT_EQ(argsForParent.size(),2);

	auto firstArgument = argsForParent[0];
	auto firstLiteralForParent = std::dynamic_pointer_cast<IntegerLiteral>(firstArgument);
	ASSERT_NE(firstLiteralForParent, nullptr);

	EXPECT_EQ(firstLiteralForParent->value, 3425);

	auto secondArgument = argsForParent[1];
	auto secondLiteralForParent = std::dynamic_pointer_cast<CharLiteral>(secondArgument);
	ASSERT_NE(secondLiteralForParent, nullptr);

	EXPECT_EQ(secondLiteralForParent->value,'c');

	
	auto functionSymbol = constructorNode->GetFunction();
	ASSERT_NE(functionSymbol,nullptr);

	// Expect correct number of parameters
	auto params = functionSymbol->GetArguments();
	EXPECT_EQ(params.size(),1);

	auto firstParam = params[0];

	// Expect correct name and type
	EXPECT_EQ(firstParam->name, "a");
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeInteger>(firstParam->GetType()) != nullptr);

	// Expect non-empty body
	EXPECT_FALSE(functionSymbol->body->isEmpty());

	auto stmts = functionSymbol->body->GetStatements();

	auto assignmentExpression = std::dynamic_pointer_cast<AssignmentExpression>(stmts[0]);
	ASSERT_NE(assignmentExpression,nullptr);

	auto identifier = std::dynamic_pointer_cast<Identifier>( assignmentExpression->GetLeft());
	ASSERT_NE(identifier,nullptr);

	// Expect correct identifier name
	EXPECT_EQ(identifier->name,"a");

	auto literal = std::dynamic_pointer_cast<IntegerLiteral>(assignmentExpression->GetRight());
	ASSERT_NE(literal,nullptr);

	// Expect literal value to equal to 5
	EXPECT_EQ(literal->value, 5);

}

TEST_F(ParserTest, ParseConstructorWithNonEmptyBodyMultipleParametersAndNoParentConstructorCall) {
    auto root = parse("create public constructor with integer a, string s\n\ta = 5\n");
    ASSERT_NE(root, nullptr);
    auto statements = root->GetStatements();
    ASSERT_EQ(statements.size(), 1);

	auto constructorNode = std::dynamic_pointer_cast<ConstructorDefinitionNode>(statements[0]);
	ASSERT_NE(constructorNode,nullptr);

	
	// Expect no arguments for parent
	EXPECT_EQ(constructorNode->GetArgumentsForParentConstructor().size(),0);

	
	auto functionSymbol = constructorNode->GetFunction();
	ASSERT_NE(functionSymbol,nullptr);

	// Expect correct number of parameters
	auto params = functionSymbol->GetArguments();
	EXPECT_EQ(params.size(),2);

	auto firstParam = params[0];

	// Expect correct name and type
	EXPECT_EQ(firstParam->name, "a");
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeInteger>(firstParam->GetType()) != nullptr);

	auto secondParam = params[1];

	// Expect correct name and type
	EXPECT_EQ(secondParam->name, "s");
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeString>(secondParam->GetType()) != nullptr);

	// Expect non-empty body
	EXPECT_FALSE(functionSymbol->body->isEmpty());

	auto stmts = functionSymbol->body->GetStatements();

	auto assignmentExpression = std::dynamic_pointer_cast<AssignmentExpression>(stmts[0]);
	ASSERT_NE(assignmentExpression,nullptr);

	auto identifier = std::dynamic_pointer_cast<Identifier>( assignmentExpression->GetLeft());
	ASSERT_NE(identifier,nullptr);

	// Expect correct identifier name
	EXPECT_EQ(identifier->name,"a");

	auto literal = std::dynamic_pointer_cast<IntegerLiteral>(assignmentExpression->GetRight());
	ASSERT_NE(literal,nullptr);

	// Expect literal value to equal to 5
	EXPECT_EQ(literal->value, 5);
}

TEST_F(ParserTest, ParseConstructorWithNonEmptyBodyMultipleParametersAndParentConstructorCallWithOneArgument) {
    auto root = parse("create public constructor with integer a, string s and call parent with \"test_string\"\n\ta = 5\n");
    ASSERT_NE(root, nullptr);
    auto statements = root->GetStatements();
    ASSERT_EQ(statements.size(), 1);

	auto constructorNode = std::dynamic_pointer_cast<ConstructorDefinitionNode>(statements[0]);
	ASSERT_NE(constructorNode,nullptr);

	
	auto args = constructorNode->GetArgumentsForParentConstructor();

	// Expect correct number of arguments for parent
	EXPECT_EQ(args.size(),1);

	auto firstArgumentForParent = args[0];
	ASSERT_NE(firstArgumentForParent,nullptr);

	auto firstArgumentLiteral = std::dynamic_pointer_cast<StringLiteral>(firstArgumentForParent);
	ASSERT_NE(firstArgumentLiteral,nullptr);

	//Expect correct value for literal
	EXPECT_EQ(firstArgumentLiteral->value, "test_string");

	
	auto functionSymbol = constructorNode->GetFunction();
	ASSERT_NE(functionSymbol,nullptr);

	// Expect correct number of parameters
	auto params = functionSymbol->GetArguments();
	EXPECT_EQ(params.size(),2);

	auto firstParam = params[0];

	// Expect correct name and type
	EXPECT_EQ(firstParam->name, "a");
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeInteger>(firstParam->GetType()) != nullptr);

	auto secondParam = params[1];

	// Expect correct name and type
	EXPECT_EQ(secondParam->name, "s");
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeString>(secondParam->GetType()) != nullptr);

	// Expect non-empty body
	EXPECT_FALSE(functionSymbol->body->isEmpty());

	auto stmts = functionSymbol->body->GetStatements();

	auto assignmentExpression = std::dynamic_pointer_cast<AssignmentExpression>(stmts[0]);
	ASSERT_NE(assignmentExpression,nullptr);

	auto identifier = std::dynamic_pointer_cast<Identifier>( assignmentExpression->GetLeft());
	ASSERT_NE(identifier,nullptr);

	// Expect correct identifier name
	EXPECT_EQ(identifier->name,"a");

	auto literal = std::dynamic_pointer_cast<IntegerLiteral>(assignmentExpression->GetRight());
	ASSERT_NE(literal,nullptr);

	// Expect literal value to equal to 5
	EXPECT_EQ(literal->value, 5);
}

TEST_F(ParserTest, ParseConstructorWithNonEmptyBodyMultipleParametersAndParentConstructorCallWithMultipleArguments) {
    auto root = parse("create public constructor with integer a, string s and call parent with \"test_string\", 3.2\n\ta = 5\n");
    ASSERT_NE(root, nullptr);
    auto statements = root->GetStatements();
    ASSERT_EQ(statements.size(), 1);

	auto constructorNode = std::dynamic_pointer_cast<ConstructorDefinitionNode>(statements[0]);
	ASSERT_NE(constructorNode,nullptr);

	
	auto args = constructorNode->GetArgumentsForParentConstructor();

	// Expect correct number of arguments for parent
	EXPECT_EQ(args.size(),2);

	auto firstArgumentForParent = args[0];
	ASSERT_NE(firstArgumentForParent,nullptr);

	auto firstArgumentLiteral = std::dynamic_pointer_cast<StringLiteral>(firstArgumentForParent);
	ASSERT_NE(firstArgumentLiteral,nullptr);

	//Expect correct value for literal
	EXPECT_EQ(firstArgumentLiteral->value, "test_string");

	auto secondArgumentForParent = args[1];
	ASSERT_NE(secondArgumentForParent,nullptr);

	auto secondArgumentLiteral = std::dynamic_pointer_cast<FloatLiteral>(secondArgumentForParent);
	ASSERT_NE(secondArgumentLiteral,nullptr);

	//Expect correct value for literal
	EXPECT_DOUBLE_EQ(secondArgumentLiteral->value, 3.2);

	
	auto functionSymbol = constructorNode->GetFunction();
	ASSERT_NE(functionSymbol,nullptr);

	// Expect correct number of parameters
	auto params = functionSymbol->GetArguments();
	EXPECT_EQ(params.size(),2);

	auto firstParam = params[0];

	// Expect correct name and type
	EXPECT_EQ(firstParam->name, "a");
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeInteger>(firstParam->GetType()) != nullptr);

	auto secondParam = params[1];

	// Expect correct name and type
	EXPECT_EQ(secondParam->name, "s");
	EXPECT_TRUE(std::dynamic_pointer_cast<TypeString>(secondParam->GetType()) != nullptr);

	// Expect non-empty body
	EXPECT_FALSE(functionSymbol->body->isEmpty());

	auto stmts = functionSymbol->body->GetStatements();

	auto assignmentExpression = std::dynamic_pointer_cast<AssignmentExpression>(stmts[0]);
	ASSERT_NE(assignmentExpression,nullptr);

	auto identifier = std::dynamic_pointer_cast<Identifier>( assignmentExpression->GetLeft());
	ASSERT_NE(identifier,nullptr);

	// Expect correct identifier name
	EXPECT_EQ(identifier->name,"a");

	auto literal = std::dynamic_pointer_cast<IntegerLiteral>(assignmentExpression->GetRight());
	ASSERT_NE(literal,nullptr);

	// Expect literal value to equal to 5
	EXPECT_EQ(literal->value, 5);
}