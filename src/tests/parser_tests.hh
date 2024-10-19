#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <sstream>
#include "../driver/atmo_driver.hh"
#include "../ast/nodes/statement_list_node.hh"
#include "../ast/literals/literal.hh"
#include "../ast/types/all_types.hh"

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

TEST_F(ParserTest, IntegerLiteralExpression)
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

TEST_F(ParserTest, FloatLiteralExpression)
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

TEST_F(ParserTest, AbbreviatedFloatLiteralExpression)
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

TEST_F(ParserTest, CharLiteralExpression)
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

TEST_F(ParserTest, StringLiteralExpression)
{
	auto root = parse("\"Hello World\"");

	ASSERT_NE(root, nullptr);

	auto statements = root->GetStatements();

	ASSERT_NE(statements.size(), 0);

	auto literal = std::dynamic_pointer_cast<Literal<std::string,TypeString>>(statements[0]);

	
	//If the cast didn't return a nullptr means that the class was csted successfully

	ASSERT_NE(literal, nullptr);


	// Expect that the parser set the correct value for the literal

	EXPECT_EQ(literal->value, "Hello World");

}