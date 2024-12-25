#pragma once
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

using ::testing::HasSubstr;

class SemanticAnalyzerTest : public ::testing::Test {
protected:
	std::unique_ptr<AtmoDriver> driver;
    
    // The error_buffer that holds all the last error message written to cerr
	std::stringstream error_buffer;

	void SetUp() override {
		driver = std::make_unique<AtmoDriver>();
		Error::InTest = true;

        // Redirect cerr to the error_buffer
        std::cerr.rdbuf(error_buffer.rdbuf());
	}

	~SemanticAnalyzerTest() = default;

	void parseAndAnalyze(const std::string& input) {
        std::istringstream stream(input);
		driver->parseAndAnalyze(stream);
	}
};

TEST_F(SemanticAnalyzerTest, CheckIdentifierWithValidElement) {
    // Create valid dummy symbol
    auto symbolVar = std::make_shared<VariableSymbol>(std::make_shared<TypeInteger>(), std::make_shared<AttributePrivate>());

    // Create identifier with the valid symbol
    Identifier identifier(symbolVar, "a", yy::location());

    EXPECT_TRUE(identifier.Check());

    // Expect no errors
    EXPECT_TRUE(error_buffer.str().empty());
}

TEST_F(SemanticAnalyzerTest, CheckIdentifierWithInvalidElement) {
    Identifier identifier(nullptr, "invalidIdentifier", yy::location());

    EXPECT_FALSE(identifier.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Unknown identifier 'invalidIdentifier'"));
}


TEST_F(SemanticAnalyzerTest, CheckAddExpressionWithValidOperands) {
    auto left = std::make_shared<Literal<int, TypeInteger>>();
    auto right = std::make_shared<Literal<int, TypeInteger>>();
    AddExpression addExpr(left, right, yy::location());
    EXPECT_TRUE(addExpr.Check());
    EXPECT_TRUE(error_buffer.str().empty());
}

TEST_F(SemanticAnalyzerTest, CheckAddExpressionWithInvalidOperands) {
    auto left = std::make_shared<Literal>(std::make_shared<TypeInteger>(), 5, yy::location());
    auto right = std::make_shared<Literal>(std::make_shared<TypeFloat>(), 10.5, yy::location());
    AddExpression addExpr(left, right, yy::location());
    EXPECT_FALSE(addExpr.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("The two operands of ADD (+) expression must be of same type!"));
}