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

// Arithmetic expressions

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

TEST_F(SemanticAnalyzerTest, CheckAddExpressionReturnsCorrectIntegerType) {
    auto left = std::make_shared<IntegerLiteral>(5);
    auto right = std::make_shared<IntegerLiteral>(2);
    AddExpression addExpr(left, right, yy::location());

    auto type = addExpr.GetType();

    auto casted = std::dynamic_pointer_cast<TypeInteger>(type);

    // Expect that the type could be casted, meaning the type was correct
    EXPECT_NE(casted, nullptr);

}

TEST_F(SemanticAnalyzerTest, CheckAddExpressionReturnsCorrectFloatType) {
    auto left = std::make_shared<FloatLiteral>(.5);
    auto right = std::make_shared<FloatLiteral>(.2);
    AddExpression addExpr(left, right, yy::location());

    auto type = addExpr.GetType();

    auto casted = std::dynamic_pointer_cast<TypeFloat>(type);

    // Expect that the type could be casted, meaning the type was correct
    EXPECT_NE(casted, nullptr);

}

TEST_F(SemanticAnalyzerTest, CheckAddExpressionReturnsFalseForCheckDuringCharAddition) {
    auto left = std::make_shared<CharLiteral>('a');
    auto right = std::make_shared<CharLiteral>('b');

    AddExpression expr(left, right, yy::location());

    EXPECT_FALSE(expr.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Only numerical values or strings can be added"));

}

// Strings could be added together



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


TEST_F(SemanticAnalyzerTest, CheckSubtractExpressionReturnsCorrectIntegerType) {
    auto left = std::make_shared<IntegerLiteral>(.5);
    auto right = std::make_shared<IntegerLiteral>(.2);

    SubtractExpression expr(left, right, yy::location());

    auto type = expr.GetType();

    auto casted = std::dynamic_pointer_cast<TypeInteger>(type);

    // Expect that the type could be casted, meaning the type was correct
    EXPECT_NE(casted, nullptr);

}

TEST_F(SemanticAnalyzerTest, CheckSubtractExpressionReturnsCorrectFloatType) {
    auto left = std::make_shared<FloatLiteral>(.5);
    auto right = std::make_shared<FloatLiteral>(.2);

    SubtractExpression expr(left, right, yy::location());

    auto type = expr.GetType();

    auto casted = std::dynamic_pointer_cast<TypeFloat>(type);

    // Expect that the type could be casted, meaning the type was correct
    EXPECT_NE(casted, nullptr);

}


TEST_F(SemanticAnalyzerTest, CheckSubtractExpressionReturnsFalseForCheckDuringCharSubtraction) {
    auto left = std::make_shared<CharLiteral>('a');
    auto right = std::make_shared<CharLiteral>('b');

    SubtractExpression expr(left, right, yy::location());

    EXPECT_FALSE(expr.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Only numerical values can be subtracted"));

}

TEST_F(SemanticAnalyzerTest, CheckSubtractExpressionReturnsFalseForCheckDuringStringSubtraction) {
    auto left = std::make_shared<StringLiteral>("asd");
    auto right = std::make_shared<StringLiteral>("asd");

    SubtractExpression expr(left, right, yy::location());

    EXPECT_FALSE(expr.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Only numerical values can be subtracted"));

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


TEST_F(SemanticAnalyzerTest, CheckMultiplyExpressionReturnsCorrectIntegerType) {
    auto left = std::make_shared<IntegerLiteral>(.5);
    auto right = std::make_shared<IntegerLiteral>(.2);

    MultiplyExpression expr(left, right, yy::location());

    auto type = expr.GetType();

    auto casted = std::dynamic_pointer_cast<TypeInteger>(type);

    // Expect that the type could be casted, meaning the type was correct
    EXPECT_NE(casted, nullptr);

}

TEST_F(SemanticAnalyzerTest, CheckMultiplyExpressionReturnsCorrectFloatType) {
    auto left = std::make_shared<FloatLiteral>(.5);
    auto right = std::make_shared<FloatLiteral>(.2);

    MultiplyExpression expr(left, right, yy::location());

    auto type = expr.GetType();

    auto casted = std::dynamic_pointer_cast<TypeFloat>(type);

    // Expect that the type could be casted, meaning the type was correct
    EXPECT_NE(casted, nullptr);

}

TEST_F(SemanticAnalyzerTest, CheckMultiplyExpressionReturnsFalseForCheckDuringCharMultiplication) {
    auto left = std::make_shared<CharLiteral>('a');
    auto right = std::make_shared<CharLiteral>('b');

    MultiplyExpression expr(left, right, yy::location());

    EXPECT_FALSE(expr.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Only numerical values can be multiplied"));

}

TEST_F(SemanticAnalyzerTest, CheckMultiplyExpressionReturnsFalseForCheckDuringStringMultiplication) {
    auto left = std::make_shared<StringLiteral>("asd");
    auto right = std::make_shared<StringLiteral>("asd");

    MultiplyExpression expr(left, right, yy::location());

    EXPECT_FALSE(expr.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Only numerical values can be multiplied"));

}

TEST_F(SemanticAnalyzerTest, CheckDivideExpressionWithValidOperands) {
    auto left = std::make_shared<IntegerLiteral>(5);
    auto right = std::make_shared<IntegerLiteral>(5);
    DivideExpression divExpr(left, right, yy::location());
    EXPECT_TRUE(divExpr.Check());
    EXPECT_TRUE(error_buffer.str().empty());
}

TEST_F(SemanticAnalyzerTest, CheckDivideExpressionWithInvalidOperands) {
    auto left = std::make_shared<IntegerLiteral>(5);
    auto right = std::make_shared<FloatLiteral>(3.2);
    DivideExpression divExpr(left, right, yy::location());
    EXPECT_FALSE(divExpr.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("The two operands of DIVIDE (/) expression must be of same type!"));
}

TEST_F(SemanticAnalyzerTest, CheckDivideExpressionReturnsCorrectFloatTypeAtFloatDivision) {
    auto left = std::make_shared<FloatLiteral>(.5);
    auto right = std::make_shared<FloatLiteral>(.2);

    DivideExpression expr(left, right, yy::location());

    auto type = expr.GetType();

    auto casted = std::dynamic_pointer_cast<TypeFloat>(type);

    // Expect that the type could be casted, meaning the type was correct
    EXPECT_NE(casted, nullptr);

}

TEST_F(SemanticAnalyzerTest, CheckDivideExpressionReturnsCorrectFloatTypeAtIntegerDivision) {
    auto left = std::make_shared<IntegerLiteral>(5);
    auto right = std::make_shared<IntegerLiteral>(2);

    DivideExpression expr(left, right, yy::location());

    auto type = expr.GetType();

    auto casted = std::dynamic_pointer_cast<TypeFloat>(type);

    // Expect that the type could be casted, meaning the type was correct
    EXPECT_NE(casted, nullptr);

}

TEST_F(SemanticAnalyzerTest, CheckDivideExpressionReturnsFalseForCheckDuringCharDivision) {
    auto left = std::make_shared<CharLiteral>('a');
    auto right = std::make_shared<CharLiteral>('b');

    DivideExpression expr(left, right, yy::location());

   EXPECT_FALSE(expr.Check());
   EXPECT_THAT(error_buffer.str(), HasSubstr("Only numerical values can be divided"));

}

TEST_F(SemanticAnalyzerTest, CheckDivideExpressionReturnsFalseForCheckDuringStringDivision) {
    auto left = std::make_shared<StringLiteral>("asd");
    auto right = std::make_shared<StringLiteral>("asd");

    DivideExpression expr(left, right, yy::location());

   EXPECT_FALSE(expr.Check());
   EXPECT_THAT(error_buffer.str(), HasSubstr("Only numerical values can be divided"));

}

// Logical expressions

TEST_F(SemanticAnalyzerTest, CheckAndExpressionWithValidOperands) {
    auto left = std::make_shared<BooleanLiteral>(true);
    auto right = std::make_shared<BooleanLiteral>(false);
    AndExpression andExpr(left, right, yy::location());
    EXPECT_TRUE(andExpr.Check());
    EXPECT_TRUE(error_buffer.str().empty());
}

TEST_F(SemanticAnalyzerTest, CheckAndExpressionWithInvalidOperands) {
    auto left = std::make_shared<IntegerLiteral>(5);
    auto right = std::make_shared<BooleanLiteral>(false);
    AndExpression andExpr(left, right, yy::location());
    EXPECT_FALSE(andExpr.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Both of the two operands of AND (&&) expression must be of type boolean!"));
}

TEST_F(SemanticAnalyzerTest, CheckAndExpressionReturnsBooleanAsItsType) {
    auto left = std::make_shared<BooleanLiteral>(true);
    auto right = std::make_shared<BooleanLiteral>(false);

    AndExpression andExpr(left, right, yy::location());

    auto type = andExpr.GetType();

    auto casted = std::dynamic_pointer_cast<TypeBoolean>(type);

    EXPECT_NE(casted,nullptr);
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

TEST_F(SemanticAnalyzerTest, CheckLessThanExpressionReturnsBooleanAsItsType) {
    auto left = std::make_shared<BooleanLiteral>(true);
    auto right = std::make_shared<BooleanLiteral>(false);

    LessThanExpression expr(left, right, yy::location());

    auto type = expr.GetType();

    auto casted = std::dynamic_pointer_cast<TypeBoolean>(type);

    EXPECT_NE(casted,nullptr);
}

TEST_F(SemanticAnalyzerTest, CheckLessThanOrEqualExpressionWithValidOperands) {
    auto left = std::make_shared<IntegerLiteral>(5);
    auto right = std::make_shared<IntegerLiteral>(5);
    LessThanOrEqualExpression lteExpr(left, right, yy::location());
    EXPECT_TRUE(lteExpr.Check());
    EXPECT_TRUE(error_buffer.str().empty());
}

TEST_F(SemanticAnalyzerTest, CheckLessThanOrEqualExpressionWithInvalidOperands) {
    auto left = std::make_shared<IntegerLiteral>(5);
    auto right = std::make_shared<FloatLiteral>(3.2);
    LessThanOrEqualExpression lteExpr(left, right, yy::location());
    EXPECT_FALSE(lteExpr.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("The two operands of LESS THAN OR EQUAL (<=) expression must be of same type!"));
}

TEST_F(SemanticAnalyzerTest, CheckLessThanOrEqualExpressionReturnsBooleanAsItsType) {
    auto left = std::make_shared<BooleanLiteral>(true);
    auto right = std::make_shared<BooleanLiteral>(false);

    LessThanOrEqualExpression expr(left, right, yy::location());

    auto type = expr.GetType();

    auto casted = std::dynamic_pointer_cast<TypeBoolean>(type);

    EXPECT_NE(casted,nullptr);
}

TEST_F(SemanticAnalyzerTest, CheckGreaterThanExpressionWithValidOperands) {
    auto left = std::make_shared<IntegerLiteral>(5);
    auto right = std::make_shared<IntegerLiteral>(5);
    GreaterThanExpression gtExpr(left, right, yy::location());
    EXPECT_TRUE(gtExpr.Check());
    EXPECT_TRUE(error_buffer.str().empty());
}

TEST_F(SemanticAnalyzerTest, CheckGreaterThanExpressionWithInvalidOperands) {
    auto left = std::make_shared<IntegerLiteral>(5);
    auto right = std::make_shared<FloatLiteral>(3.2);
    GreaterThanExpression gtExpr(left, right, yy::location());
    EXPECT_FALSE(gtExpr.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("The two operands of GREATER THAN (>) expression must be of same type!"));
}

TEST_F(SemanticAnalyzerTest, CheckGreaterThanExpressionReturnsBooleanAsItsType) {
    auto left = std::make_shared<BooleanLiteral>(true);
    auto right = std::make_shared<BooleanLiteral>(false);

    GreaterThanExpression expr(left, right, yy::location());

    auto type = expr.GetType();

    auto casted = std::dynamic_pointer_cast<TypeBoolean>(type);

    EXPECT_NE(casted,nullptr);
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

TEST_F(SemanticAnalyzerTest, CheckGreaterThanOrEqualExpressionReturnsBooleanAsItsType) {
    auto left = std::make_shared<BooleanLiteral>(true);
    auto right = std::make_shared<BooleanLiteral>(false);

    GreaterThanOrEqualExpression expr(left, right, yy::location());

    auto type = expr.GetType();

    auto casted = std::dynamic_pointer_cast<TypeBoolean>(type);

    EXPECT_NE(casted,nullptr);
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

TEST_F(SemanticAnalyzerTest, CheckMatchesExpressionReturnsBooleanAsItsType) {
    auto left = std::make_shared<BooleanLiteral>(true);
    auto right = std::make_shared<BooleanLiteral>(false);

    MatchesExpression expr(left, right, yy::location());

    auto type = expr.GetType();

    auto casted = std::dynamic_pointer_cast<TypeBoolean>(type);

    EXPECT_NE(casted,nullptr);
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

TEST_F(SemanticAnalyzerTest, CheckNotMachesExpressionReturnsBooleanAsItsType) {
    auto left = std::make_shared<BooleanLiteral>(true);
    auto right = std::make_shared<BooleanLiteral>(false);

    NotMatchesExpression expr(left, right, yy::location());

    auto type = expr.GetType();

    auto casted = std::dynamic_pointer_cast<TypeBoolean>(type);

    EXPECT_NE(casted,nullptr);
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

TEST_F(SemanticAnalyzerTest, CheckOrExpressionReturnsBooleanAsItsType) {
    auto left = std::make_shared<BooleanLiteral>(true);
    auto right = std::make_shared<BooleanLiteral>(false);

    OrExpression expr(left, right, yy::location());

    auto type = expr.GetType();

    auto casted = std::dynamic_pointer_cast<TypeBoolean>(type);

    EXPECT_NE(casted,nullptr);
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

TEST_F(SemanticAnalyzerTest, CheckNotExpressionReturnsBooleanAsItsType) {
    auto left = std::make_shared<BooleanLiteral>(true);

    NotExpression expr(left, yy::location());

    auto type = expr.GetType();

    auto casted = std::dynamic_pointer_cast<TypeBoolean>(type);

    EXPECT_NE(casted,nullptr);
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

TEST_F(SemanticAnalyzerTest, CheckMemberAccessExpressionWithChainedAccess) {

    Error::ShouldShowWarnings = false;

    // Create a dummy body for the class
    auto dummyBody = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    // Define the main class symbol
    auto outerClassSymbol = std::make_shared<ClassSymbol>(
        std::vector<std::shared_ptr<Identifier>>(), 
        std::move(dummyBody)
        );
    outerClassSymbol->name = "OuterClass";

    auto dummyBody2 = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    // Define an inner class symbol
    auto innerClassSymbol = std::make_shared<ClassSymbol>(
        std::vector<std::shared_ptr<Identifier>>(), 
        std::move(dummyBody2)
    );
    innerClassSymbol->name = "InnerClass";

    // Define a variable in the outer class pointing to the inner class
    auto innerVar = std::make_shared<VariableSymbol>(
        innerClassSymbol, 
        std::make_shared<AttributePublic>()
        );
    innerVar->name = "inner";

    auto innerDefinition = std::make_shared<VariableDefinitionNode>(
        innerVar, 
        nullptr, 
        yy::location(), std::vector<std::shared_ptr<IExpressionable>>()
        );
    outerClassSymbol->InsertVariable(innerDefinition);

    // Define a variable in the inner class
    auto innerVar2 = std::make_shared<VariableSymbol>(
        std::make_shared<TypeInteger>(), std::make_shared<AttributePublic>()
        );
    innerVar2->name = "value";

    auto innerDefinition2 = std::make_shared<VariableDefinitionNode>(
        innerVar2, 
        std::make_shared<IntegerLiteral>(10), 
        yy::location(), std::vector<std::shared_ptr<IExpressionable>>()
        );

    innerClassSymbol->InsertVariable(innerDefinition2);

    // Create a chained member access expression: obj.inner.value
    auto innerAccess = std::make_shared<Identifier>(
        innerVar, 
        "inner", 
        yy::location());

    auto outerAccess = std::make_shared<Identifier>(
        outerClassSymbol, 
        "OuterClass", 
        yy::location());

    auto chainedRight = std::make_shared<MemberAccessExpression>(
        innerAccess, 
        outerAccess, 
        yy::location());

    auto valueAccess = std::make_shared<Identifier>(
        innerVar2, 
        "value", 
        yy::location()
        );

    
    MemberAccessExpression memberAccessExpr(
        valueAccess, 
        chainedRight, 
        yy::location()
        );

    
    EXPECT_TRUE(memberAccessExpr.Check());
    EXPECT_TRUE(error_buffer.str().empty());
}

TEST_F(SemanticAnalyzerTest, CheckMemberAccessExpressionWithInheritedVariable) {
    Error::ShouldShowWarnings = false;

    // Create base and derived class symbols
    auto dummyBody = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    auto baseClass = std::make_shared<ClassSymbol>(
        std::vector<std::shared_ptr<Identifier>>(), 
        std::move(dummyBody)
        );

    baseClass->name = "BaseClass";

    auto identifierToParent = std::make_shared<Identifier>(
        baseClass,
        baseClass->name,
        yy::location()
    );


    // Set the base class a parent to the derived class

    auto dummyBody2 = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    auto derivedClass = std::make_shared<ClassSymbol>(
        std::vector<std::shared_ptr<Identifier>>{
            identifierToParent
        }, 
        std::move(dummyBody2)
    );
    derivedClass->name = "DerivedClass";

    // Define a public variable in the base class
    auto baseVar = std::make_shared<VariableSymbol>(std::make_shared<TypeInteger>(), std::make_shared<AttributePublic>());
    baseVar->name = "baseVar";

    auto baseVarDefinition = std::make_shared<VariableDefinitionNode>(baseVar, nullptr, yy::location(), std::vector<std::shared_ptr<IExpressionable>>());
    baseClass->InsertVariable(baseVarDefinition);


    // Create identifiers
    auto right = std::make_shared<Identifier>(derivedClass, "DerivedClass", yy::location());
    auto left = std::make_shared<Identifier>(baseVar, "baseVar", yy::location());
    MemberAccessExpression memberAccessExpr(left, right, yy::location());

    // Check the function and buffer
    EXPECT_TRUE(memberAccessExpr.Check());
    EXPECT_TRUE(error_buffer.str().empty());
}




TEST_F(SemanticAnalyzerTest,CheckFunctionCallWithIdentifierPointingToVariable) {

    // Create the variable symbol

    auto varSymbol = std::make_shared<VariableSymbol>(std::make_shared<TypeInteger>(), std::make_shared<AttributePrivate>());

    // Create the identifier

    auto id = std::make_shared<Identifier>(varSymbol, "a", yy::location());

    // Create the function call

    FunctionCall funcCall(id, std::vector<std::shared_ptr<IExpressionable>>(), yy::location());

    EXPECT_FALSE(funcCall.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Only functions can be called!"));
}

TEST_F(SemanticAnalyzerTest,CheckFunctionCallWithIdentifierPointingToClass) {

    // Create the class symbol

    auto body = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    auto classSymbol = std::make_shared<ClassSymbol>(std::vector<std::shared_ptr<Identifier>>(),std::move(body));

    // Create the identifier

    auto id = std::make_shared<Identifier>(classSymbol, "a", yy::location());

    // Create the function call

    FunctionCall funcCall(id, std::vector<std::shared_ptr<IExpressionable>>(), yy::location());

    EXPECT_FALSE(funcCall.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Only functions can be called!"));
}

TEST_F(SemanticAnalyzerTest,CheckFunctionCallWithIdentifierPointingToInvalidFunction) {


    // Create the identifier

    auto id = std::make_shared<Identifier>(nullptr, "a", yy::location());

    // Create the function call

    FunctionCall funcCall(id, std::vector<std::shared_ptr<IExpressionable>>(), yy::location());

    EXPECT_FALSE(funcCall.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Unknown identifier 'a'"));
}


TEST_F(SemanticAnalyzerTest,CheckFunctionCallWithIdentifierPointingToValidFunctionWithZeroArguments) {

    // Create the function symbol

    auto body = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    auto funcSymbol = std::make_shared<FunctionSymbol>(std::make_shared<TypeInteger>(), std::make_shared<AttributePublic>(), std::vector<std::shared_ptr<VariableSymbol>>(),std::move(body));

    // Create the identifier

    auto id = std::make_shared<Identifier>(funcSymbol, "a", yy::location());

    // Create the function call

    FunctionCall funcCall(id, std::vector<std::shared_ptr<IExpressionable>>(), yy::location());

    EXPECT_TRUE(funcCall.Check());
    EXPECT_TRUE(error_buffer.str().empty());
}

TEST_F(SemanticAnalyzerTest,CheckFunctionCallWithIdentifierPointingToValidFunctionExpectingOneParameterAndReceivedOneValidArgument) {

    // Create argument symbol

    auto arg1 = std::make_shared<VariableSymbol>(std::make_shared<TypeInteger>(), std::make_shared<AttributePublic>());

    // Create arguments vector

    std::vector<std::shared_ptr<VariableSymbol>> args{arg1};

    // Create the function symbol

    auto body = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    auto funcSymbol = std::make_shared<FunctionSymbol>(std::make_shared<TypeInteger>(), std::make_shared<AttributePublic>(),
    args,
    std::move(body));

    // Create the identifier

    auto id = std::make_shared<Identifier>(funcSymbol, "a", yy::location());

    // Create the function call


    auto argExpr = std::make_shared<IntegerLiteral>(5);

    FunctionCall funcCall(id, std::vector<std::shared_ptr<IExpressionable>>{argExpr}, yy::location());

    EXPECT_TRUE(funcCall.Check());
    EXPECT_TRUE(error_buffer.str().empty());
}

TEST_F(SemanticAnalyzerTest,CheckFunctionCallWithIdentifierPointingToValidFunctionExpectingOneParameterAndReceivedOneInvalidArgument) {

    // Create argument symbol

    auto arg1 = std::make_shared<VariableSymbol>(std::make_shared<TypeInteger>(), std::make_shared<AttributePublic>());

    // Create arguments vector

    std::vector<std::shared_ptr<VariableSymbol>> args{arg1};

    // Create the function symbol

    auto body = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    auto funcSymbol = std::make_shared<FunctionSymbol>(std::make_shared<TypeInteger>(), std::make_shared<AttributePublic>(),
    args,
    std::move(body));

    funcSymbol->name = "a";

    // Create the identifier

    auto id = std::make_shared<Identifier>(funcSymbol, "a", yy::location());

    // Create the function call


    auto argExpr = std::make_shared<CharLiteral>('c');

    FunctionCall funcCall(id, std::vector<std::shared_ptr<IExpressionable>>{argExpr}, yy::location());

    EXPECT_FALSE(funcCall.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Type mismatch between the 1. parameter to call 'a'!"));
}

TEST_F(SemanticAnalyzerTest,CheckFunctionCallWithIdentifierPointingToValidFunctionExpectingOneParameterAndReceivedMultipleArguments) {

    // Create argument symbol

    auto arg1 = std::make_shared<VariableSymbol>(std::make_shared<TypeInteger>(), std::make_shared<AttributePublic>());

    // Create arguments vector

    std::vector<std::shared_ptr<VariableSymbol>> args{arg1};

    // Create the function symbol

    auto body = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    auto funcSymbol = std::make_shared<FunctionSymbol>(std::make_shared<TypeInteger>(), std::make_shared<AttributePublic>(),
    args,
    std::move(body));

    // Create the identifier

    auto id = std::make_shared<Identifier>(funcSymbol, "a", yy::location());

    // Create the function call


    auto argExpr1 = std::make_shared<CharLiteral>('c');
    auto argExpr2 = std::make_shared<IntegerLiteral>(10);

    FunctionCall funcCall(id, std::vector<std::shared_ptr<IExpressionable>>{argExpr1,argExpr2},
    yy::location()
    );

    EXPECT_FALSE(funcCall.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Wrong number of parameters"));
}

TEST_F(SemanticAnalyzerTest,CheckFunctionCallWithIdentifierPointingToValidFunctionExpectingMultipleParametersAndReceivedZeroArgument) {

    // Create argument symbol

    auto arg1 = std::make_shared<VariableSymbol>(std::make_shared<TypeInteger>(), std::make_shared<AttributePublic>());

    auto arg2 = std::make_shared<VariableSymbol>(std::make_shared<TypeFloat>(), std::make_shared<AttributePublic>());

    // Create arguments vector

    std::vector<std::shared_ptr<VariableSymbol>> args{arg1,arg2};

    // Create the function symbol

    auto body = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    auto funcSymbol = std::make_shared<FunctionSymbol>(std::make_shared<TypeInteger>(), std::make_shared<AttributePublic>(),
    args,
    std::move(body));

    // Create the identifier

    auto id = std::make_shared<Identifier>(funcSymbol, "a", yy::location());

    // Create the function call



    FunctionCall funcCall(id, std::vector<std::shared_ptr<IExpressionable>>(),
    yy::location()
    );

    EXPECT_FALSE(funcCall.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Wrong number of parameters"));
}

TEST_F(SemanticAnalyzerTest,CheckFunctionCallWithIdentifierPointingToValidFunctionExpectingMultipleParametersAndReceivedOneArgument) {

    // Create argument symbol

    auto arg1 = std::make_shared<VariableSymbol>(std::make_shared<TypeInteger>(), std::make_shared<AttributePublic>());

    auto arg2 = std::make_shared<VariableSymbol>(std::make_shared<TypeFloat>(), std::make_shared<AttributePublic>());

    // Create arguments vector

    std::vector<std::shared_ptr<VariableSymbol>> args{arg1,arg2};

    // Create the function symbol

    auto body = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    auto funcSymbol = std::make_shared<FunctionSymbol>(std::make_shared<TypeInteger>(), std::make_shared<AttributePublic>(),
    args,
    std::move(body));

    // Create the identifier

    auto id = std::make_shared<Identifier>(funcSymbol, "a", yy::location());

    // Create the function call


    auto argExpr1 = std::make_shared<IntegerLiteral>(10);

    FunctionCall funcCall(id, std::vector<std::shared_ptr<IExpressionable>>{argExpr1},
    yy::location()
    );

    EXPECT_FALSE(funcCall.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Wrong number of parameters"));
}

TEST_F(SemanticAnalyzerTest,CheckFunctionCallWithIdentifierPointingToValidFunctionExpectingMultipleParametersAndReceivedCorrectAmountOfArgumentButFirstIsInvalid) {

    // Create argument symbol

    auto arg1 = std::make_shared<VariableSymbol>(std::make_shared<TypeInteger>(), std::make_shared<AttributePublic>());

    auto arg2 = std::make_shared<VariableSymbol>(std::make_shared<TypeFloat>(), std::make_shared<AttributePublic>());

    // Create arguments vector

    std::vector<std::shared_ptr<VariableSymbol>> args{arg1,arg2};

    // Create the function symbol

    auto body = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    auto funcSymbol = std::make_shared<FunctionSymbol>(std::make_shared<TypeInteger>(), std::make_shared<AttributePublic>(),
    args,
    std::move(body));

    // Create the identifier

    auto id = std::make_shared<Identifier>(funcSymbol, "a", yy::location());

    // Create the function call


    auto argExpr1 = std::make_shared<FloatLiteral>(10.6);
    auto argExpr2 = std::make_shared<FloatLiteral>(.10);

    FunctionCall funcCall(id, std::vector<std::shared_ptr<IExpressionable>>{argExpr1,argExpr2},
    yy::location()
    );

    EXPECT_FALSE(funcCall.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Type mismatch between the 1. parameter"));
}

TEST_F(SemanticAnalyzerTest,CheckFunctionCallWithIdentifierPointingToValidFunctionExpectingMultipleParametersAndReceivedCorrectAmountOfArgumentButSecondIsInvalid) {

    // Create argument symbol

    auto arg1 = std::make_shared<VariableSymbol>(std::make_shared<TypeInteger>(), std::make_shared<AttributePublic>());

    auto arg2 = std::make_shared<VariableSymbol>(std::make_shared<TypeFloat>(), std::make_shared<AttributePublic>());

    // Create arguments vector

    std::vector<std::shared_ptr<VariableSymbol>> args{arg1,arg2};

    // Create the function symbol

    auto body = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    auto funcSymbol = std::make_shared<FunctionSymbol>(std::make_shared<TypeInteger>(), std::make_shared<AttributePublic>(),
    args,
    std::move(body));

    // Create the identifier

    auto id = std::make_shared<Identifier>(funcSymbol, "a", yy::location());

    // Create the function call


    auto argExpr1 = std::make_shared<IntegerLiteral>(10);
    auto argExpr2 = std::make_shared<IntegerLiteral>(10);

    FunctionCall funcCall(id, std::vector<std::shared_ptr<IExpressionable>>{argExpr1,argExpr2},
    yy::location()
    );

    EXPECT_FALSE(funcCall.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Type mismatch between the 2. parameter"));
}

TEST_F(SemanticAnalyzerTest,CheckFunctionCallWithIdentifierPointingToValidFunctionExpectingMultipleParametersAndReceivedCorrectAmountOfArgumentWithCorrectTypes) {

    // Create argument symbol

    auto arg1 = std::make_shared<VariableSymbol>(std::make_shared<TypeInteger>(), std::make_shared<AttributePublic>());

    auto arg2 = std::make_shared<VariableSymbol>(std::make_shared<TypeFloat>(), std::make_shared<AttributePublic>());

    // Create arguments vector

    std::vector<std::shared_ptr<VariableSymbol>> args{arg1,arg2};

    // Create the function symbol

    auto body = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    auto funcSymbol = std::make_shared<FunctionSymbol>(std::make_shared<TypeInteger>(), std::make_shared<AttributePublic>(),
    args,
    std::move(body));

    // Create the identifier

    auto id = std::make_shared<Identifier>(funcSymbol, "a", yy::location());

    // Create the function call


    auto argExpr1 = std::make_shared<IntegerLiteral>(10);
    auto argExpr2 = std::make_shared<FloatLiteral>(10.5);

    FunctionCall funcCall(id, std::vector<std::shared_ptr<IExpressionable>>{argExpr1,argExpr2},
    yy::location()
    );

    EXPECT_TRUE(funcCall.Check());
    EXPECT_TRUE(error_buffer.str().empty());
}


TEST_F(SemanticAnalyzerTest, CheckAssignmentExpressionWithLeftOperandIsInvalidIdentifier) {

    auto left = std::make_shared<Identifier>(nullptr, "a", yy::location());
    auto right = std::make_shared<IntegerLiteral>(5);
    AssignmentExpression assignExpr(left, right, yy::location());
    EXPECT_FALSE(assignExpr.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Unknown identifier 'a'"));
}

TEST_F(SemanticAnalyzerTest, CheckAssignmentExpressionWithInvalidRightOperandIsInvalidIdentifier) {

    auto varSymbol = std::make_shared<VariableSymbol>(std::make_shared<TypeInteger>(), std::make_shared<AttributePrivate>());
    auto right = std::make_shared<Identifier>(nullptr, "a", yy::location());
    auto left = std::make_shared<IntegerLiteral>(5);
    AssignmentExpression assignExpr(left, right, yy::location());
    EXPECT_FALSE(assignExpr.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Unknown identifier 'a'"));
}

TEST_F(SemanticAnalyzerTest, CheckAssignmentExpressionWithLeftOperandIsNotMemberContainerAndNotArraySubscript) {

    auto varSymbol = std::make_shared<VariableSymbol>(std::make_shared<TypeInteger>(), std::make_shared<AttributePrivate>());

    auto left = std::make_shared<IntegerLiteral>(10);

    auto right = std::make_shared<IntegerLiteral>(5);
    AssignmentExpression assignExpr(left, right, yy::location());
    EXPECT_FALSE(assignExpr.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Only member containers and array subscripts can appear on the left side of an assignment (=) expression!"));
}

TEST_F(SemanticAnalyzerTest, CheckAssignmentExpressionWithLeftOperandIsMemberContainerButIsAFunction) {

    auto funcSymbol = std::make_shared<FunctionSymbol>(std::make_shared<TypeInteger>(), std::make_shared<AttributePublic>(), std::vector<std::shared_ptr<VariableSymbol>>(), std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>()));


    auto identifier = std::make_shared<Identifier>(funcSymbol, "a", yy::location());

    auto right = std::make_shared<IntegerLiteral>(5);
    AssignmentExpression assignExpr(identifier, right, yy::location());

    EXPECT_FALSE(assignExpr.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Only variables can appear on the left side of an assignment (=) expression!"));
}

TEST_F(SemanticAnalyzerTest, CheckAssignmentExpressionWithLeftOperandIsMemberContainerButIsAClass) {

    auto classSymbol = std::make_shared<ClassSymbol>(std::vector<std::shared_ptr<Identifier>>(), std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>()));


    auto identifier = std::make_shared<Identifier>(classSymbol, "a", yy::location());

    auto right = std::make_shared<IntegerLiteral>(5);
    AssignmentExpression assignExpr(identifier, right, yy::location());

    EXPECT_FALSE(assignExpr.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Only variables can appear on the left side of an assignment (=) expression!"));
}

TEST_F(SemanticAnalyzerTest, CheckAssignmentExpressionWithLeftOperandIsMemberContainerButIsAVariable) {

    auto varSymbol = std::make_shared<VariableSymbol>(std::make_shared<TypeInteger>(), std::make_shared<AttributePrivate>());


    auto identifier = std::make_shared<Identifier>(varSymbol, "a", yy::location());

    auto right = std::make_shared<IntegerLiteral>(5);
    AssignmentExpression assignExpr(identifier, right, yy::location());

    EXPECT_TRUE(assignExpr.Check());
    EXPECT_TRUE(error_buffer.str().empty());
}

TEST_F(SemanticAnalyzerTest, CheckAssignmentExpressionWithDifferentTypeOperands) {

    auto varSymbol = std::make_shared<VariableSymbol>(std::make_shared<TypeFloat>(), std::make_shared<AttributePrivate>());


    auto identifier = std::make_shared<Identifier>(varSymbol, "a", yy::location());

    auto right = std::make_shared<IntegerLiteral>(5);

    AssignmentExpression assignExpr(identifier, right, yy::location());

    EXPECT_FALSE(assignExpr.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Invalid type of expression on the right side of assignment (=) expression!"));
}

TEST_F(SemanticAnalyzerTest, CheckAssignmentExpressionWithValidTypeOperands) {

    auto varSymbol = std::make_shared<VariableSymbol>(std::make_shared<TypeInteger>(), std::make_shared<AttributePrivate>());


    auto identifier = std::make_shared<Identifier>(varSymbol, "a", yy::location());

    auto right = std::make_shared<IntegerLiteral>(5);

    AssignmentExpression assignExpr(identifier, right, yy::location());

    EXPECT_TRUE(assignExpr.Check());
    EXPECT_TRUE(error_buffer.str().empty());
}

TEST_F(SemanticAnalyzerTest, CheckAssignmentExpressionWithArraySubscriptOnLeftOperand) {
    
    auto arrayType = std::make_shared<Array>(
        std::make_shared<TypeInteger>(), 
        std::make_shared<IntegerLiteral>(5)
        );

    auto varSymbol = std::make_shared<VariableSymbol>(
        arrayType, 
        std::make_shared<AttributePrivate>()
        );

    auto arraySubscript = std::make_shared<ArraySubscriptExpression>(
        std::make_shared<Identifier>(varSymbol, "arr", yy::location()),
        std::make_shared<IntegerLiteral>(0),
        yy::location()
    );

    auto right = std::make_shared<IntegerLiteral>(5);
    AssignmentExpression assignExpr(arraySubscript, right, yy::location());

    EXPECT_TRUE(assignExpr.Check());
    EXPECT_TRUE(error_buffer.str().empty());
}

// Statements

TEST_F(SemanticAnalyzerTest, CheckDoUntilStatementNodeWithValidExpressionAndNonEmptyBody) {
    auto expression = std::make_shared<BooleanLiteral>(true);

    // Create non-empty body



    auto ret = std::make_shared<ReturnStatementNode>(
        std::make_shared<IntegerLiteral>(5), 
        yy::location()
    );


    auto body = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>{ret});

    DoUntilStatementNode doUntilStmt(expression, std::move(body), yy::location());

    EXPECT_TRUE(doUntilStmt.Check());
    EXPECT_TRUE(error_buffer.str().empty());
}

TEST_F(SemanticAnalyzerTest, CheckDoUntilStatementNodeWithInvalidExpression) {
    auto expression = std::make_shared<IntegerLiteral>(5);
    auto body = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    DoUntilStatementNode doUntilStmt(expression, std::move(body), yy::location());

    EXPECT_FALSE(doUntilStmt.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("The expression of a do-until statement must be of type boolean!"));
}

TEST_F(SemanticAnalyzerTest, CheckDoUntilStatementNodeWithAssignmentExpressionAsCondition) {
    auto left = std::make_shared<Identifier>(std::make_shared<VariableSymbol>(
        std::make_shared<TypeBoolean>(), std::make_shared<AttributePrivate>()),
    "a",
    yy::location()
    );

    auto right = std::make_shared<BooleanLiteral>(true);

    auto expression = std::make_shared<AssignmentExpression>(left, right, yy::location());

    auto body = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    DoUntilStatementNode doUntilStmt(expression, std::move(body), yy::location());

    EXPECT_FALSE(doUntilStmt.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Cannot use assignment expression as a condition to a do-until statement!"));
}

TEST_F(SemanticAnalyzerTest, CheckDoUntilStatementNodeWithEmptyBody) {

    Error::ShouldShowWarnings = true;

    auto expression = std::make_shared<BooleanLiteral>(true);

    auto body = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    DoUntilStatementNode doUntilStmt(expression, std::move(body), yy::location());

    EXPECT_TRUE(doUntilStmt.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Empty body of do-until statement!"));
}

TEST_F(SemanticAnalyzerTest, CheckUntilStatementNodeWithValidExpressionAndNonEmptyBody) {
    
    Error::ShouldShowWarnings = false;

    auto expression = std::make_shared<BooleanLiteral>(true);

    auto ret = std::make_shared<ReturnStatementNode>(
        std::make_shared<IntegerLiteral>(5), 
        yy::location()
    );

    auto body = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>{ret});

    UntilStatementNode untilStmt(expression, std::move(body), yy::location());

    EXPECT_TRUE(untilStmt.Check());
    EXPECT_TRUE(error_buffer.str().empty());
}

TEST_F(SemanticAnalyzerTest, CheckUntilStatementNodeWithInvalidExpression) {
    auto expression = std::make_shared<IntegerLiteral>(5);
    auto body = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    UntilStatementNode untilStmt(expression, std::move(body), yy::location());

    EXPECT_FALSE(untilStmt.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("The expression of an until statement must be of type boolean!"));
}

TEST_F(SemanticAnalyzerTest, CheckUntilStatementNodeWithAssignmentExpressionAsCondition) {
    auto left = std::make_shared<Identifier>(std::make_shared<VariableSymbol>(std::make_shared<TypeBoolean>(), std::make_shared<AttributePrivate>()), "a", yy::location());

    auto right = std::make_shared<BooleanLiteral>(false);
    auto expression = std::make_shared<AssignmentExpression>(left, right, yy::location());

    auto body = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    UntilStatementNode untilStmt(expression, std::move(body), yy::location());
    EXPECT_FALSE(untilStmt.Check());

    EXPECT_THAT(error_buffer.str(), HasSubstr("Cannot use assignment expression as a condition to an until statement!"));
}

TEST_F(SemanticAnalyzerTest, CheckUntilStatementNodeWithEmptyBody) {

    Error::ShouldShowWarnings = true;

    auto expression = std::make_shared<BooleanLiteral>(true);

    auto body = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    UntilStatementNode untilStmt(expression, std::move(body), yy::location());

    EXPECT_TRUE(untilStmt.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Empty body of until statement!"));
}

TEST_F(SemanticAnalyzerTest, CheckIfStatementNodeWithValidExpressionAndNonEmptyBody) {
    auto expression = std::make_shared<BooleanLiteral>(true);

    auto ret = std::make_shared<ReturnStatementNode>(
        std::make_shared<IntegerLiteral>(5), 
        yy::location()
    );

    auto body = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>{ret});

    IfStatementNode ifStmt(expression, std::move(body), std::vector<std::unique_ptr<ElseIfStatementNode>>(), nullptr, yy::location());

    EXPECT_TRUE(ifStmt.Check());
    EXPECT_TRUE(error_buffer.str().empty());
}

TEST_F(SemanticAnalyzerTest, CheckIfStatementNodeWithInvalidExpression) {
    auto expression = std::make_shared<IntegerLiteral>(5);

    auto body = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    IfStatementNode ifStmt(expression, std::move(body), std::vector<std::unique_ptr<ElseIfStatementNode>>(), nullptr, yy::location());

    EXPECT_FALSE(ifStmt.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("The expression of an if statement must be of type boolean!"));
}

TEST_F(SemanticAnalyzerTest, CheckIfStatementNodeWithAssignmentExpressionAsCondition) {
    auto left = std::make_shared<Identifier>(std::make_shared<VariableSymbol>(std::make_shared<TypeBoolean>(), std::make_shared<AttributePrivate>()), "a", yy::location());

    auto right = std::make_shared<BooleanLiteral>(false);

    auto expression = std::make_shared<AssignmentExpression>(left, right, yy::location());

    auto body = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    IfStatementNode ifStmt(expression, std::move(body), std::vector<std::unique_ptr<ElseIfStatementNode>>(), nullptr, yy::location());

    EXPECT_FALSE(ifStmt.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Cannot use assignment expression as a condition to an if statement!"));
}

TEST_F(SemanticAnalyzerTest, CheckIfStatementNodeWithEmptyBody) {

    Error::ShouldShowWarnings = true;

    auto expression = std::make_shared<BooleanLiteral>(true);

    auto body = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    IfStatementNode ifStmt(expression, std::move(body), std::vector<std::unique_ptr<ElseIfStatementNode>>(), nullptr, yy::location());
    EXPECT_TRUE(ifStmt.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Empty body of if statement!"));
}

TEST_F(SemanticAnalyzerTest, CheckIfStatementNodeWithOneElseIfStatement) {

    Error::ShouldShowWarnings = false;

    auto expression = std::make_shared<BooleanLiteral>(true);

    auto ret = std::make_shared<ReturnStatementNode>(
        std::make_shared<IntegerLiteral>(5), 
        yy::location()
    );

    auto body = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>{ret});

    auto elseIfExpression = std::make_shared<BooleanLiteral>(false);

    auto elseIfBody = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    auto elseIfStmt = std::make_unique<ElseIfStatementNode>(elseIfExpression, std::move(elseIfBody), yy::location());

    std::vector<std::unique_ptr<ElseIfStatementNode>> elseIfs;

    elseIfs.push_back(std::move(elseIfStmt));
    IfStatementNode ifStmt(expression, std::move(body), std::move(elseIfs), nullptr, yy::location());

    EXPECT_TRUE(ifStmt.Check());
    EXPECT_TRUE(error_buffer.str().empty());
}

TEST_F(SemanticAnalyzerTest, CheckIfStatementNodeWithMultipleElseIfStatements) {
    
    Error::ShouldShowWarnings = false;

    auto expression = std::make_shared<BooleanLiteral>(true);

    auto body = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    auto elseIfExpression = std::make_shared<BooleanLiteral>(false);

    auto elseIfBody = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    auto elseIfStmt = std::make_unique<ElseIfStatementNode>(elseIfExpression, std::move(elseIfBody), yy::location());

    auto elseIfExpression2 = std::make_shared<BooleanLiteral>(false);

    auto elseIfBody2 = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    auto elseIfStmt2 = std::make_unique<ElseIfStatementNode>(elseIfExpression2, std::move(elseIfBody2), yy::location());

    std::vector<std::unique_ptr<ElseIfStatementNode>> elseIfs;

    elseIfs.push_back(std::move(elseIfStmt));
    elseIfs.push_back(std::move(elseIfStmt2));

    IfStatementNode ifStmt(expression, std::move(body), std::move(elseIfs), nullptr, yy::location());

    EXPECT_TRUE(ifStmt.Check());
    EXPECT_TRUE(error_buffer.str().empty());
}

TEST_F(SemanticAnalyzerTest, CheckIfStatementNodeWithElseStatement) {
    auto expression = std::make_shared<BooleanLiteral>(true);

    auto body = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    auto elseBody = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    auto elseStmt = std::make_unique<ElseStatementNode>(std::move(elseBody), yy::location());

    IfStatementNode ifStmt(expression, std::move(body), std::vector<std::unique_ptr<ElseIfStatementNode>>(), std::move(elseStmt), yy::location());

    EXPECT_TRUE(ifStmt.Check());
    EXPECT_TRUE(error_buffer.str().empty());
}

// TODO: Implement rest of the nodes

// Symbols

TEST_F(SemanticAnalyzerTest, CheckFunctionSymbolWithLessThanSixParameters) {

    auto type = std::make_shared<TypeVoid>();
    auto attribute = std::make_shared<AttributePublic>();

    std::vector<std::shared_ptr<VariableSymbol>> args_in;

    auto body = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    // Create the function symbol

    auto func = FunctionSymbol(
        type,
        attribute,
        args_in,
        std::move(body)
    );


    EXPECT_TRUE(func.Check());
    EXPECT_TRUE(error_buffer.str().empty());
}

TEST_F(SemanticAnalyzerTest, CheckFunctionSymbolWithMoreThanSixParameters) {

    auto type = std::make_shared<TypeVoid>();
    auto attribute = std::make_shared<AttributePublic>();

    auto var1 = std::make_shared<VariableSymbol>(
        type,
        attribute
    );

    // Create more than six parameters

    std::vector<std::shared_ptr<VariableSymbol>> args_in
    {
        var1,
        var1,
        var1,
        var1,
        var1,
        var1,
        var1
    };


    auto body = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());

    // Create the function symbol

    auto func = FunctionSymbol(
        type,
        attribute,
        args_in,
        std::move(body)
    );


    EXPECT_FALSE(func.Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Functions cannot have  more than 6 arguments!"));
}

TEST_F(SemanticAnalyzerTest, CheckClassWithoutParentAndEmptyBody) {

    auto classSymbol = std::make_shared<ClassSymbol>(
        std::vector<std::shared_ptr<Identifier>>(), 
        std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>())
        );

    EXPECT_TRUE(classSymbol->Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Empty class definition!"));
}

TEST_F(SemanticAnalyzerTest, CheckClassWithInvalidParent) {

    auto invalidParent = std::make_shared<Identifier>(
        nullptr,
        "InvalidParent",
        yy::location()
        );

    std::vector<std::shared_ptr<Identifier>> parents = {
        invalidParent
        };

    auto classSymbol = std::make_shared<ClassSymbol>(
        parents, 
        std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>())
        );

    EXPECT_FALSE(classSymbol->Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Unknown identifier 'InvalidParent'"));
}

TEST_F(SemanticAnalyzerTest, CheckClassWithMultipleParents) {

    auto classParent = std::make_shared<ClassSymbol>(
        std::vector<std::shared_ptr<Identifier>>(),
        std::make_unique<BodyNode>(
            std::vector<std::shared_ptr<Node>>()
        )
    );

    auto classParent2 = std::make_shared<ClassSymbol>(
        std::vector<std::shared_ptr<Identifier>>(),
        std::make_unique<BodyNode>(
            std::vector<std::shared_ptr<Node>>()
        )
    );

    auto parentId1 = std::make_shared<Identifier>(
        classParent,
        "ClassParent",
        yy::location()
    );

    auto parentId2 = std::make_shared<Identifier>(
        classParent2,
        "ClassParent",
        yy::location()
    );
    std::vector<std::shared_ptr<Identifier>> parents = {
        parentId1,
        parentId2
        };

    auto classSymbol = std::make_shared<ClassSymbol>(
        parents, 
        std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>())
        );

    EXPECT_FALSE(classSymbol->Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("A class can only have at most one parent!"));
}

TEST_F(SemanticAnalyzerTest, CheckClassWithParentButNotAClass) {

    auto parentVariable = std::make_shared<VariableSymbol>(
        std::make_shared<TypeInteger>(), 
        std::make_shared<AttributePublic>()
        );

    auto invalidParent = std::make_shared<Identifier>(
        parentVariable, 
        "ParentVar", 
        yy::location()
        );

    std::vector<std::shared_ptr<Identifier>> parents = {
        invalidParent
        };

    auto classSymbol = std::make_shared<ClassSymbol>(
        parents, 
        std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>())
        );

    EXPECT_FALSE(classSymbol->Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Only classes can be used as a parent to another class!"));
}

TEST_F(SemanticAnalyzerTest, CheckClassWithValidParentClass) {

    auto parentClass = std::make_shared<ClassSymbol>(
        std::vector<std::shared_ptr<Identifier>>(), 
        std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>())
        );

    auto parentIdentifier = std::make_shared<Identifier>(
        parentClass,
        "ParentClass",
        yy::location()
    );

    std::vector<std::shared_ptr<Identifier>> parents = {
        parentIdentifier
        };

    auto classSymbol = std::make_shared<ClassSymbol>(
        parents, 
        std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>())
        );

    EXPECT_TRUE(classSymbol->Check());
    EXPECT_TRUE(error_buffer.str().empty());
}
TEST_F(SemanticAnalyzerTest, CheckClassInsertVariableOnce) {

    auto var = std::make_shared<VariableSymbol>(
        std::make_shared<TypeInteger>(),
        std::make_shared<AttributePublic>()
    );

    auto varDef = std::make_shared<VariableDefinitionNode>(
        var,
        nullptr,
        yy::location(),
        std::vector<std::shared_ptr<IExpressionable>>()
    );

    auto classSymbol = std::make_shared<ClassSymbol>(
        std::vector<std::shared_ptr<Identifier>>(), 
        std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>{
            varDef
        })
    );

    EXPECT_TRUE(classSymbol->Check());

    EXPECT_TRUE(classSymbol->GetVariable("parentVar") != nullptr);

    EXPECT_TRUE(error_buffer.str().empty());
}

TEST_F(SemanticAnalyzerTest, CheckClassInsertVariableMultipleTimes) {

    auto var = std::make_shared<VariableSymbol>(
        std::make_shared<TypeInteger>(),
        std::make_shared<AttributePublic>()
    );
    var->name = "TestVar";

    auto varDef = std::make_shared<VariableDefinitionNode>(
        var,
        nullptr,
        yy::location(),
        std::vector<std::shared_ptr<IExpressionable>>()
    );

    auto classSymbol = std::make_shared<ClassSymbol>(
        std::vector<std::shared_ptr<Identifier>>(), 
        std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>{
            varDef,
            varDef
        })
    );

    EXPECT_FALSE(classSymbol->Check());
    
    EXPECT_THAT(error_buffer.str(), HasSubstr("Identifier 'TestVar' has already been declared in this scope!"));
}

TEST_F(SemanticAnalyzerTest, CheckClassInsertFunctionOnce) {

    auto func = std::make_shared<FunctionSymbol>(
        std::make_shared<TypeInteger>(),
        std::make_shared<AttributePublic>(),
        std::vector<std::shared_ptr<VariableSymbol>>(),
        std::make_unique<BodyNode>(
            std::vector<std::shared_ptr<Node>>()
        )
    );
    func->name = "TestFunc";

    auto funcDef = std::make_shared<FunctionDefinitionNode>(
        func
    );

    auto classSymbol = std::make_shared<ClassSymbol>(
        std::vector<std::shared_ptr<Identifier>>(), 
        std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>{
            funcDef
        })
    );

    EXPECT_TRUE(classSymbol->Check());

    EXPECT_TRUE(classSymbol->GetFunction("TestFunc") != nullptr);

    EXPECT_TRUE(error_buffer.str().empty());
}

TEST_F(SemanticAnalyzerTest, CheckClassInsertFunctionMultipleTimes) {

    auto func = std::make_shared<FunctionSymbol>(
        std::make_shared<TypeInteger>(),
        std::make_shared<AttributePublic>(),
        std::vector<std::shared_ptr<VariableSymbol>>(),
        std::make_unique<BodyNode>(
            std::vector<std::shared_ptr<Node>>()
        )
    );
    func->name = "TestFunc";

    auto funcDef = std::make_shared<FunctionDefinitionNode>(
        func
    );

    auto classSymbol = std::make_shared<ClassSymbol>(
        std::vector<std::shared_ptr<Identifier>>(), 
        std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>{
            funcDef,
            funcDef
        })
    );

    EXPECT_FALSE(classSymbol->Check());
    
    EXPECT_THAT(error_buffer.str(), HasSubstr("Identifier 'TestFunc' has already been declared in this scope!"));
}

TEST_F(SemanticAnalyzerTest, CheckClassWithDuplicateVariableAndFunctionName) {

    auto var = std::make_shared<VariableSymbol>(
        std::make_shared<TypeInteger>(),
        std::make_shared<AttributePublic>()
    );
    var->name = "TestVar";

    auto variableNode = std::make_shared<VariableDefinitionNode>(
        var,
        nullptr,
        yy::location(),
        std::vector<std::shared_ptr<IExpressionable>>()
    );

    auto func = std::make_shared<FunctionSymbol>(
        std::make_unique<BodyNode>(
            std::vector<std::shared_ptr<Node>>()
        )
    );

    auto functionNode = std::make_shared<FunctionDefinitionNode>(func);

    auto bodyNodes = std::vector<std::shared_ptr<Node>>{variableNode, functionNode};
    auto classSymbol = std::make_shared<ClassSymbol>(std::vector<std::shared_ptr<Identifier>>(), std::make_unique<BodyNode>(bodyNodes));

    EXPECT_FALSE(classSymbol->Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Identifier 'duplicateName' has already been declared in this scope!"));
}

TEST_F(SemanticAnalyzerTest, CheckClassWithMultipleConstructorsButNoParent) {

    auto func = std::make_shared<FunctionSymbol>(
        std::make_unique<BodyNode>(
            std::vector<std::shared_ptr<Node>>()
        )
    );

    auto constructorNode1 = std::make_shared<ConstructorDefinitionNode>(
        func,
        std::vector<std::shared_ptr<IExpressionable>>(),
        yy::location()
        );

    auto constructorNode2 = std::make_shared<ConstructorDefinitionNode>(
        func,
        std::vector<std::shared_ptr<IExpressionable>>(),
        yy::location()
        );
        

    auto bodyNodes = std::vector<std::shared_ptr<Node>>{constructorNode1, constructorNode2};
    auto classSymbol = std::make_shared<ClassSymbol>(std::vector<std::shared_ptr<Identifier>>(), std::make_unique<BodyNode>(bodyNodes));

    EXPECT_TRUE(classSymbol->Check());
    EXPECT_TRUE(error_buffer.str().empty());
}

TEST_F(SemanticAnalyzerTest, CheckClassWithValidParentConstructorCall) {

    auto func = std::make_shared<FunctionSymbol>(
        std::make_unique<BodyNode>(
            std::vector<std::shared_ptr<Node>>()
        )
    );


    auto parentConstructor1 = std::make_shared<ConstructorDefinitionNode>(
        func,
        std::vector<std::shared_ptr<IExpressionable>>(),
        yy::location()
        );

    auto parentClass = std::make_shared<ClassSymbol>(
        std::vector<std::shared_ptr<Identifier>>(), 
        std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>{
            parentConstructor1
            })
        );

    auto parentIdentifier = std::make_shared<Identifier>(parentClass, "ParentClass", yy::location());

    std::vector<std::shared_ptr<Identifier>> parents = {parentIdentifier};


    auto constructorNode = std::make_shared<ConstructorDefinitionNode>(
        func,
        std::vector<std::shared_ptr<IExpressionable>>(),
        yy::location()
        );

    auto bodyNodes = std::vector<std::shared_ptr<Node>>{constructorNode};
    auto classSymbol = std::make_shared<ClassSymbol>(parents, std::make_unique<BodyNode>(bodyNodes));

    EXPECT_TRUE(classSymbol->Check());
    EXPECT_TRUE(error_buffer.str().empty());
}

TEST_F(SemanticAnalyzerTest, CheckClassWithParentConstructorCallNotFound) {
    auto parentClass = std::make_shared<ClassSymbol>(
        std::vector<std::shared_ptr<Identifier>>(), 
        std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>())
        );

    auto parentIdentifier = std::make_shared<Identifier>(parentClass, "ParentClass", yy::location());

    std::vector<std::shared_ptr<Identifier>> parents = {parentIdentifier};

    auto func = std::make_shared<FunctionSymbol>(
        std::make_unique<BodyNode>(
            std::vector<std::shared_ptr<Node>>()
        )
    );

    auto constructorNode = std::make_shared<ConstructorDefinitionNode>(
        func,
        std::vector<std::shared_ptr<IExpressionable>>(),
        yy::location()
        );

    auto bodyNodes = std::vector<std::shared_ptr<Node>>{constructorNode};
    auto classSymbol = std::make_shared<ClassSymbol>(parents, std::make_unique<BodyNode>(bodyNodes));

    EXPECT_TRUE(classSymbol->Check());
    EXPECT_TRUE(error_buffer.str().empty());
}

TEST_F(SemanticAnalyzerTest, CheckClassWithAmbiguousParentConstructorCall) {

    auto func = std::make_shared<FunctionSymbol>(
        std::make_unique<BodyNode>(
            std::vector<std::shared_ptr<Node>>()
        )
    );

    auto parentConstructor1 = std::make_shared<ConstructorDefinitionNode>(
        func,
        std::vector<std::shared_ptr<IExpressionable>>(),
        yy::location()
        );

    auto parentConstructor2= std::make_shared<ConstructorDefinitionNode>(
        func,
        std::vector<std::shared_ptr<IExpressionable>>(),
        yy::location()
        );

    auto parentClass = std::make_shared<ClassSymbol>(
        std::vector<std::shared_ptr<Identifier>>(), 
        std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>{
            parentConstructor1,
            parentConstructor2
            })
        );

    auto parentIdentifier = std::make_shared<Identifier>(parentClass, "ParentClass", yy::location());

    std::vector<std::shared_ptr<Identifier>> parents = {parentIdentifier};


    auto constructorNode = std::make_shared<ConstructorDefinitionNode>(
        func,
        std::vector<std::shared_ptr<IExpressionable>>(),
        yy::location()
        );

    auto bodyNodes = std::vector<std::shared_ptr<Node>>{constructorNode};
    auto classSymbol = std::make_shared<ClassSymbol>(parents, std::make_unique<BodyNode>(bodyNodes));

    EXPECT_FALSE(classSymbol->Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Ambiguous parent constructor call for the given parameters!"));
}

TEST_F(SemanticAnalyzerTest, CheckClassWithMissingConstructorAndDestructor) {
    auto classSymbol = std::make_shared<ClassSymbol>(std::vector<std::shared_ptr<Identifier>>(), std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>()));

    EXPECT_TRUE(classSymbol->Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("No destructor defined for class, generating default one!"));
    EXPECT_THAT(error_buffer.str(), HasSubstr("No constructor defined for class, generating default one!"));
}

TEST_F(SemanticAnalyzerTest, CheckClassWithOneDestructor) {
    auto destructorNode1 = std::make_shared<DestructorDefinitionNode>(yy::location());
    auto bodyNodes = std::vector<std::shared_ptr<Node>>{destructorNode1};
    auto classSymbol = std::make_shared<ClassSymbol>(std::vector<std::shared_ptr<Identifier>>(), std::make_unique<BodyNode>(bodyNodes));

    EXPECT_TRUE(classSymbol->Check());
    EXPECT_TRUE(error_buffer.str().empty());
}

TEST_F(SemanticAnalyzerTest, CheckClassWithMultipleDestructors) {
    auto destructorNode1 = std::make_shared<DestructorDefinitionNode>(yy::location());
    auto destructorNode2 = std::make_shared<DestructorDefinitionNode>(yy::location());

    auto bodyNodes = std::vector<std::shared_ptr<Node>>{destructorNode1, destructorNode2};
    auto classSymbol = std::make_shared<ClassSymbol>(std::vector<std::shared_ptr<Identifier>>(), std::make_unique<BodyNode>(bodyNodes));

    EXPECT_FALSE(classSymbol->Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("A class can only have one destructor!"));
}

TEST_F(SemanticAnalyzerTest, CheckClassWithInvalidTopLevelStatement) {

    auto ifStatement = std::make_shared<IfStatementNode>(
        nullptr,
        nullptr,
        std::vector<std::unique_ptr<ElseIfStatementNode>>(),
        nullptr,
        yy::location()
    );

    auto bodyNodes = std::vector<std::shared_ptr<Node>>{
        ifStatement
        };

    auto classSymbol = std::make_shared<ClassSymbol>(std::vector<std::shared_ptr<Identifier>>(), std::make_unique<BodyNode>(bodyNodes));

    EXPECT_FALSE(classSymbol->Check());
    EXPECT_THAT(error_buffer.str(), HasSubstr("Invalid top-level statement inside class!"));
}

TEST_F(SemanticAnalyzerTest, CheckClassWithParentAndInheritedVariables) {

    auto var = std::make_shared<VariableSymbol>(
        std::make_shared<TypeInteger>(),
        std::make_shared<AttributePrivate>()

    );

    auto parentVariable = std::make_shared<VariableDefinitionNode>(
        var,
        nullptr,
        yy::location(),
        std::vector<std::shared_ptr<IExpressionable>>()

    );
    auto parentClass = std::make_shared<ClassSymbol>(std::vector<std::shared_ptr<Identifier>>(), std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>{parentVariable}));
    auto parentIdentifier = std::make_shared<Identifier>(parentClass, "ParentClass", yy::location());

    std::vector<std::shared_ptr<Identifier>> parents = {parentIdentifier};
    auto classSymbol = std::make_shared<ClassSymbol>(parents, std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>()));

    EXPECT_TRUE(classSymbol->Check());
    EXPECT_TRUE(classSymbol->GetVariable("parentVar") != nullptr);
    EXPECT_TRUE(error_buffer.str().empty());
}

TEST_F(SemanticAnalyzerTest, CheckClassCompatibleWithSelf) {
	// Create a class with no parents and an empty body
	auto body = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());
	auto classA = std::make_shared<ClassSymbol>(std::vector<std::shared_ptr<Identifier>>(), std::move(body));
	classA->name = "ClassA";

	// A class should be compatible with itself
	EXPECT_TRUE(classA->Compatible(classA));
}

TEST_F(SemanticAnalyzerTest, CheckClassWithParentClass) {
	// Create the parent class
	yy::location loc;
	auto parentBody = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());
	auto parentClass = std::make_shared<ClassSymbol>(std::vector<std::shared_ptr<Identifier>>(), std::move(parentBody));
	parentClass->name = "ParentClass";

	// Create an identifier for the parent class
	auto parentIdentifier = std::make_shared<Identifier>(parentClass, "ParentClass", loc);

	// Create the child class with the parent as its parent
	auto childBody = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());
	auto childClass = std::make_shared<ClassSymbol>(std::vector<std::shared_ptr<Identifier>>{parentIdentifier}, std::move(childBody));
	childClass->name = "ChildClass";

	// A child class should be compatible with its parent
	EXPECT_TRUE(childClass->Compatible(parentClass));
}

TEST_F(SemanticAnalyzerTest, CheckClassNotCompatibleWithUnrelatedClass) {
	// Create the first class
	auto bodyA = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());
	auto classA = std::make_shared<ClassSymbol>(std::vector<std::shared_ptr<Identifier>>(), std::move(bodyA));
	classA->name = "ClassA";

	// Create the second unrelated class
	auto bodyB = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());
	auto classB = std::make_shared<ClassSymbol>(std::vector<std::shared_ptr<Identifier>>(), std::move(bodyB));
	classB->name = "ClassB";

	// Unrelated classes should not be compatible
	EXPECT_FALSE(classA->Compatible(classB));
}

TEST_F(SemanticAnalyzerTest, CheckClassNotCompatibleWithNullType) {
	// Create a class
	auto body = std::make_unique<BodyNode>(std::vector<std::shared_ptr<Node>>());
	auto classA = std::make_shared<ClassSymbol>(std::vector<std::shared_ptr<Identifier>>(), std::move(body));
	classA->name = "ClassA";

	// A class should not be compatible with a null type
	std::shared_ptr<Type> nullType = nullptr;
	EXPECT_FALSE(classA->Compatible(nullType));
}



//TODO Implement literals and symbols


// TODO: Check class class as a whole (variable, fucntion insertion (returns false if already isnerted with same name), parent cosntructor chaining, valid parents, ettc)