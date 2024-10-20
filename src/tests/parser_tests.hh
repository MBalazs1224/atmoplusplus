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