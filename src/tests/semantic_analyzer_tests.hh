#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include <sstream>
#include <vector>

#include "../ast/nodes/statement_list_node.hh"
#include "../ast/literals/all_literals.hh"
#include "../ast/types/all_types.hh"
#include "../ast/nodes/all_nodes.hh"
#include "../ast/expressions/all_expressions.hh"
#include "../symboltable/all_symbols.hh"
class AtmoDriver;

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
    auto left = std::make_shared<IntegerLiteral>(5);
    auto right = std::make_shared<IntegerLiteral>(5);
    AddExpression addExpr(left, right, yy::location());
    EXPECT_TRUE(addExpr.Check());
    EXPECT_TRUE(error_buffer.str().empty());
}

TEST_F(SemanticAnalyzerTest, CheckAddExpressionWithInvalidOperands) {
    auto left = std::make_shared<IntegerLiteral>(5);
    auto right = std::make_shared<FloatLiteral>(3.2);
    AddExpression addExpr(left, right, yy::location());
    EXPECT_FALSE(addExpr.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("The two operands of ADD (+) expression must be of same type!"));
}


TEST_F(SemanticAnalyzerTest, CheckSubtractExpressionWithValidOperands) {
    auto left = std::make_shared<IntegerLiteral>(5);
    auto right = std::make_shared<IntegerLiteral>(5);
    SubtractExpression subExpr(left, right, yy::location());
    EXPECT_TRUE(subExpr.Check());
    EXPECT_TRUE(error_buffer.str().empty());
}

TEST_F(SemanticAnalyzerTest, CheckSubtractExpressionWithInvalidOperands) {
    auto left = std::make_shared<IntegerLiteral>(5);
    auto right = std::make_shared<FloatLiteral>(3.2);
    SubtractExpression subExpr(left, right, yy::location());
    EXPECT_FALSE(subExpr.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("The two operands of SUBTRACT (-) expression must be of same type!"));
}

TEST_F(SemanticAnalyzerTest, CheckMultiplyExpressionWithValidOperands) {
    auto left = std::make_shared<IntegerLiteral>(5);
    auto right = std::make_shared<IntegerLiteral>(5);
    MultiplyExpression mulExpr(left, right, yy::location());
    EXPECT_TRUE(mulExpr.Check());
    EXPECT_TRUE(error_buffer.str().empty());
}

TEST_F(SemanticAnalyzerTest, CheckMultiplyExpressionWithInvalidOperands) {
    auto left = std::make_shared<IntegerLiteral>(5);
    auto right = std::make_shared<FloatLiteral>(3.2);
    MultiplyExpression mulExpr(left, right, yy::location());
    EXPECT_FALSE(mulExpr.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("The two operands of MULTIPLY (*) expression must be of same type!"));
}

TEST_F(SemanticAnalyzerTest, CheckLessThanExpressionWithValidOperands) {
    auto left = std::make_shared<IntegerLiteral>(5);
    auto right = std::make_shared<IntegerLiteral>(5);
    LessThanExpression ltExpr(left, right, yy::location());
    EXPECT_TRUE(ltExpr.Check());
    EXPECT_TRUE(error_buffer.str().empty());
}

TEST_F(SemanticAnalyzerTest, CheckLessThanExpressionWithInvalidOperands) {
    auto left = std::make_shared<IntegerLiteral>(5);
    auto right = std::make_shared<FloatLiteral>(3.2);
    LessThanExpression ltExpr(left, right, yy::location());
    EXPECT_FALSE(ltExpr.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("The two operands of LESS THAN (<) expression must be of same type!"));
}

TEST_F(SemanticAnalyzerTest, CheckGreaterThanOrEqualExpressionWithValidOperands) {
    auto left = std::make_shared<IntegerLiteral>(5);
    auto right = std::make_shared<IntegerLiteral>(5);
    GreaterThanOrEqualExpression gteExpr(left, right, yy::location());
    EXPECT_TRUE(gteExpr.Check());
    EXPECT_TRUE(error_buffer.str().empty());
}

TEST_F(SemanticAnalyzerTest, CheckGreaterThanOrEqualExpressionWithInvalidOperands) {
    auto left = std::make_shared<IntegerLiteral>(5);
    auto right = std::make_shared<FloatLiteral>(3.2);
    GreaterThanOrEqualExpression gteExpr(left, right, yy::location());
    EXPECT_FALSE(gteExpr.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("The two operands of GREATER THAN OR EQUAL (>=) expression must be of same type!"));
}

TEST_F(SemanticAnalyzerTest, CheckMatchesExpressionWithValidOperands) {
    auto left = std::make_shared<IntegerLiteral>(5);
    auto right = std::make_shared<IntegerLiteral>(5);
    MatchesExpression matchesExpr(left, right, yy::location());
    EXPECT_TRUE(matchesExpr.Check());
    EXPECT_TRUE(error_buffer.str().empty());
}

TEST_F(SemanticAnalyzerTest, CheckMatchesExpressionWithInvalidOperands) {
    auto left = std::make_shared<IntegerLiteral>(5);
    auto right = std::make_shared<FloatLiteral>(3.2);
    MatchesExpression matchesExpr(left, right, yy::location());
    EXPECT_FALSE(matchesExpr.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("The two operands of matches (==) expression must be of same type!"));
}

TEST_F(SemanticAnalyzerTest, CheckNotMatchesExpressionWithValidOperands) {
    auto left = std::make_shared<IntegerLiteral>(5);
    auto right = std::make_shared<IntegerLiteral>(5);
    NotMatchesExpression notMatchesExpr(left, right, yy::location());
    EXPECT_TRUE(notMatchesExpr.Check());
    EXPECT_TRUE(error_buffer.str().empty());
}

TEST_F(SemanticAnalyzerTest, CheckNotMatchesExpressionWithInvalidOperands) {
    auto left = std::make_shared<IntegerLiteral>(5);
    auto right = std::make_shared<FloatLiteral>(3.2);
    NotMatchesExpression notMatchesExpr(left, right, yy::location());
    EXPECT_FALSE(notMatchesExpr.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("The two operands of NOT MATCHES (!=) expression must be of same type!"));
}

TEST_F(SemanticAnalyzerTest, CheckOrExpressionWithValidOperands) {
    auto left = std::make_shared<BooleanLiteral>(true);
    auto right = std::make_shared<BooleanLiteral>(false);
    OrExpression orExpr(left, right, yy::location());
    EXPECT_TRUE(orExpr.Check());
    EXPECT_TRUE(error_buffer.str().empty());
}

TEST_F(SemanticAnalyzerTest, CheckOrExpressionWithInvalidOperands) {
    auto left = std::make_shared<IntegerLiteral>(5);
    auto right = std::make_shared<BooleanLiteral>(false);
    OrExpression orExpr(left, right, yy::location());
    EXPECT_FALSE(orExpr.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Both of the two operands of OR (||) expression must be of type boolean!"));
}

TEST_F(SemanticAnalyzerTest, CheckNotExpressionWithValidOperand) {
    auto exp = std::make_shared<BooleanLiteral>(true);
    NotExpression notExpr(exp, yy::location());
    EXPECT_TRUE(notExpr.Check());
    EXPECT_TRUE(error_buffer.str().empty());
}

TEST_F(SemanticAnalyzerTest, CheckNotExpressionWithInvalidOperand) {
    auto exp = std::make_shared<IntegerLiteral>(5);
    NotExpression notExpr(exp, yy::location());
    EXPECT_FALSE(notExpr.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("The expression of not expression must be of type boolean!"));
}

TEST_F(SemanticAnalyzerTest, CheckMemberAccessExpressionWithInvalidRightExpressionType) {
    // Create empty body
    auto dummyBody = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    auto classSymbol = std::make_shared<ClassSymbol>(std::vector<std::shared_ptr<Identifier>>(),std::move(dummyBody));
    classSymbol->name = "TestClass";


    auto left = std::make_shared<Identifier>(classSymbol, "TestClass", yy::location());
    auto right = std::make_shared<IntegerLiteral>(5);
    MemberAccessExpression memberAccessExpr(left, right, yy::location());


    EXPECT_FALSE(memberAccessExpr.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Only class types can appear on the right side of member access (inside) expression!"));
}

TEST_F(SemanticAnalyzerTest, CheckMemberAccessExpressionWithValidExpressionTypesAndExistingAndAccessibleVariable) {


    Error::ShouldShowWarnings = false;

    // Create class and variable symbols

    // Create empty body
    auto dummyBody = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    auto classSymbol = std::make_shared<ClassSymbol>(std::vector<std::shared_ptr<Identifier>>(),std::move(dummyBody));
    classSymbol->name = "TestClass";
    auto symbolVar = std::make_shared<VariableSymbol>(std::make_shared<TypeInteger>(), std::make_shared<AttributePublic>());
    symbolVar->name = "a";

    // Create the definition of the variable

    auto varDefinition = std::make_shared<VariableDefinitionNode>(symbolVar, nullptr,yy::location(),std::vector<std::shared_ptr<IExpressionable>>());

    //Insert the variable into the class symbol

    // Assert that the variable can be inserted
    ASSERT_TRUE(classSymbol->InsertVariable(varDefinition));

    // Create the member access expression

    auto right = std::make_shared<Identifier>(classSymbol, "TestClass", yy::location());
    auto left = std::make_shared<Identifier>(symbolVar, "a", yy::location());
    MemberAccessExpression memberAccessExpr(left, right, yy::location());

    // Check the function and buffer

    EXPECT_TRUE(memberAccessExpr.Check());
    EXPECT_TRUE(error_buffer.str().empty());
}

TEST_F(SemanticAnalyzerTest, CheckMemberAccessExpressionWithValidExpressionTypesAndNonExistingVariable) {
    Error::ShouldShowWarnings = false;

    // Create class and variable symbols

    // Create empty body
    auto dummyBody = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    auto classSymbol = std::make_shared<ClassSymbol>(std::vector<std::shared_ptr<Identifier>>(),std::move(dummyBody));
    classSymbol->name = "TestClass";
    auto symbolVar = std::make_shared<VariableSymbol>(std::make_shared<TypeInteger>(), std::make_shared<AttributePublic>());
    symbolVar->name = "a";

    // Create the definition of the variable

    auto varDefinition = std::make_shared<VariableDefinitionNode>(symbolVar, nullptr,yy::location(),std::vector<std::shared_ptr<IExpressionable>>());

    //Not insert the variable so it should show error


    // Create the member access expression

    auto right = std::make_shared<Identifier>(classSymbol, "TestClass", yy::location());
    auto left = std::make_shared<Identifier>(symbolVar, "a", yy::location());
    MemberAccessExpression memberAccessExpr(left, right, yy::location());

    // Check the function and buffer

    EXPECT_FALSE(memberAccessExpr.Check());
    EXPECT_THAT(error_buffer.str(),HasSubstr("Cannot find 'a' inside 'type class (TestClass)'"));
}

TEST_F(SemanticAnalyzerTest, CheckMemberAccessExpressionWithValidExpressionTypesAndExistingButNonAccessiblePrivateVariable) {

    Error::ShouldShowWarnings = false;
    
    // Create class and variable symbols

    // Create empty body
    auto dummyBody = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    auto classSymbol = std::make_shared<ClassSymbol>(std::vector<std::shared_ptr<Identifier>>(),std::move(dummyBody));
    classSymbol->name = "TestClass";
    auto symbolVar = std::make_shared<VariableSymbol>(std::make_shared<TypeInteger>(), std::make_shared<AttributePrivate>());
    symbolVar->name = "a";

    // Create the definition of the variable

    auto varDefinition = std::make_shared<VariableDefinitionNode>(symbolVar, nullptr,yy::location(),std::vector<std::shared_ptr<IExpressionable>>());

    //Insert the variable into the class symbol

    classSymbol->InsertVariable(varDefinition);

    // Create the member access expression

    auto right = std::make_shared<Identifier>(classSymbol, "TestClass", yy::location());
    auto left = std::make_shared<Identifier>(symbolVar, "a", yy::location());
    MemberAccessExpression memberAccessExpr(left, right, yy::location());

    // Check the function and buffer

    EXPECT_FALSE(memberAccessExpr.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("'a' is inaccessible due to it's protection level!"));
}

TEST_F(SemanticAnalyzerTest, CheckMemberAccessExpressionWithValidExpressionTypesAndExistingButNonAccessibleProtectedVariable) {

    Error::ShouldShowWarnings = false;
    
    // Create class and variable symbols

    // Create empty body
    auto dummyBody = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    auto classSymbol = std::make_shared<ClassSymbol>(std::vector<std::shared_ptr<Identifier>>(),std::move(dummyBody));
    classSymbol->name = "TestClass";
    auto symbolVar = std::make_shared<VariableSymbol>(std::make_shared<TypeInteger>(), std::make_shared<AttributeProtected>());
    symbolVar->name = "a";

    // Create the definition of the variable

    auto varDefinition = std::make_shared<VariableDefinitionNode>(symbolVar, nullptr,yy::location(),std::vector<std::shared_ptr<IExpressionable>>());

    //Insert the variable into the class symbol

    classSymbol->InsertVariable(varDefinition);

    // Create the member access expression

    auto right = std::make_shared<Identifier>(classSymbol, "TestClass", yy::location());
    auto left = std::make_shared<Identifier>(symbolVar, "a", yy::location());
    MemberAccessExpression memberAccessExpr(left, right, yy::location());

    // Check the function and buffer

    EXPECT_FALSE(memberAccessExpr.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("'a' is inaccessible due to it's protection level!"));
}

TEST_F(SemanticAnalyzerTest, CheckMemberAccessExpressionWithInvalidLeftExpressionType) {
    
    Error::ShouldShowWarnings = false;
    
    // Create empty body
    auto dummyBody = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    auto classSymbol = std::make_shared<ClassSymbol>(std::vector<std::shared_ptr<Identifier>>(),std::move(dummyBody));
    classSymbol->name = "TestClass";
    auto right = std::make_shared<Identifier>(classSymbol, "TestClass", yy::location());
    auto left = std::make_shared<IntegerLiteral>(5);
    MemberAccessExpression memberAccessExpr(left, right, yy::location());
    EXPECT_FALSE(memberAccessExpr.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Only identifiers can appear on the left side of the member access (inside) expression!"));
}



// TODO: Implement chained member access expressions

// TODO: Check class class as a whole (variable, fucntion insertion (returns false if already isnerted with same name), parent cosntructor chaining, valid parents, ettc)