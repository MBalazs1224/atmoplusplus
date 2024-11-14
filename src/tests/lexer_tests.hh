#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../lexer/AtmoLexer.hh"
#include "../parser/parser.tab.hh"
#include <sstream>
#include <memory>

using ::testing::HasSubstr;

class LexerTest : public ::testing::Test {
protected:
	std::unique_ptr<AtmoLexer> lexer;
	yy::parser::semantic_type yylval;
	yy::parser::location_type loc;

	// The error_buffer that holds all the last error message written to cerr
	std::stringstream error_buffer;

	void SetUp() override {
		lexer = std::make_unique<AtmoLexer>();
		loc.initialize();
		Error::InTest = true;
		// Redirect cerr to the error_buffer
		std::cerr.rdbuf(error_buffer.rdbuf());
	}
	~LexerTest() = default;

	int lex(const std::string& input) {
		std::istringstream iss(input);
		lexer->switch_streams(&iss, nullptr);
		return static_cast<yy::parser::token::yytokentype>(lexer->yylex(&yylval, &loc));
	}
};

// Basic Tokens
TEST_F(LexerTest, TokenCreate) {
	EXPECT_EQ(lex("create"), yy::parser::token::CREATE);
	EXPECT_EQ(lex("CREATE"), yy::parser::token::CREATE);
}

TEST_F(LexerTest, TokenClass) {
	EXPECT_EQ(lex("class"), yy::parser::token::CLASS);
	EXPECT_EQ(lex("CLASS"), yy::parser::token::CLASS);
}

TEST_F(LexerTest, TokenPublic) {
	EXPECT_EQ(lex("public"), yy::parser::token::PUBLIC);
	EXPECT_EQ(lex("PUBLIC"), yy::parser::token::PUBLIC);
}

TEST_F(LexerTest, TokenProtected) {
	EXPECT_EQ(lex("protected"), yy::parser::token::PROTECTED);
	EXPECT_EQ(lex("PROTECTED"), yy::parser::token::PROTECTED);
}

TEST_F(LexerTest, TokenPrivate) {
	EXPECT_EQ(lex("private"), yy::parser::token::PRIVATE);
	EXPECT_EQ(lex("PRIVATE"), yy::parser::token::PRIVATE);
}

TEST_F(LexerTest, TokenStatic) {
	EXPECT_EQ(lex("static"), yy::parser::token::STATIC);
	EXPECT_EQ(lex("STATIC"), yy::parser::token::STATIC);
}

TEST_F(LexerTest, TokenFunction) {
	EXPECT_EQ(lex("function"), yy::parser::token::FUNCTION);
	EXPECT_EQ(lex("FUNCTION"), yy::parser::token::FUNCTION);
}

TEST_F(LexerTest, TokenReturn) {
	EXPECT_EQ(lex("return"), yy::parser::token::RETURN);
	EXPECT_EQ(lex("RETURN"), yy::parser::token::RETURN);
}

TEST_F(LexerTest, TokenIf) {
	EXPECT_EQ(lex("if"), yy::parser::token::IF);
	EXPECT_EQ(lex("IF"), yy::parser::token::IF);
}

TEST_F(LexerTest, TokenElse) {
	EXPECT_EQ(lex("else"), yy::parser::token::ELSE);
	EXPECT_EQ(lex("ELSE"), yy::parser::token::ELSE);
}

TEST_F(LexerTest, TokenElseIf) {
	EXPECT_EQ(lex("else if"), yy::parser::token::ELSE_IF);
	EXPECT_EQ(lex("ELSE IF"), yy::parser::token::ELSE_IF);
}

TEST_F(LexerTest, TokenUntil) {
	EXPECT_EQ(lex("until"), yy::parser::token::UNTIL);
	EXPECT_EQ(lex("UNTIL"), yy::parser::token::UNTIL);
}

TEST_F(LexerTest, TokenDo) {
	EXPECT_EQ(lex("do"), yy::parser::token::DO);
	EXPECT_EQ(lex("DO"), yy::parser::token::DO);
}

TEST_F(LexerTest, TokenTrue) {
	EXPECT_EQ(lex("true"), yy::parser::token::TRUE);
	EXPECT_EQ(lex("TRUE"), yy::parser::token::TRUE);
}

TEST_F(LexerTest, TokenFalse) {
	EXPECT_EQ(lex("false"), yy::parser::token::FALSE);
	EXPECT_EQ(lex("FALSE"), yy::parser::token::FALSE);
}

TEST_F(LexerTest, TokenCall) {
	EXPECT_EQ(lex("call"), yy::parser::token::CALL);
	EXPECT_EQ(lex("CALL"), yy::parser::token::CALL);
}

TEST_F(LexerTest, TokenVoid) {
	EXPECT_EQ(lex("void"), yy::parser::token::VOID);
	EXPECT_EQ(lex("VOID"), yy::parser::token::VOID);
}

TEST_F(LexerTest, TokenWith) {
	EXPECT_EQ(lex("with"), yy::parser::token::WITH);
	EXPECT_EQ(lex("WITH"), yy::parser::token::WITH);
}

TEST_F(LexerTest, TokenArrayOf) {
	EXPECT_EQ(lex("array of"), yy::parser::token::ARRAY_OF);
	EXPECT_EQ(lex("ARRAY OF"), yy::parser::token::ARRAY_OF);
}

TEST_F(LexerTest, TokenConstructor) {
	EXPECT_EQ(lex("constructor"), yy::parser::token::CONSTRUCTOR);
	EXPECT_EQ(lex("CONSTRUCTOR"), yy::parser::token::CONSTRUCTOR);
}

TEST_F(LexerTest, TokenDerivesFrom) {
	EXPECT_EQ(lex("derives from"), yy::parser::token::DERIVES_FROM);
	EXPECT_EQ(lex("derive from"), yy::parser::token::DERIVES_FROM);
	EXPECT_EQ(lex("DERIVES FROM"), yy::parser::token::DERIVES_FROM);
	EXPECT_EQ(lex("DERIVE FROM"), yy::parser::token::DERIVES_FROM);
}

// Data Types
TEST_F(LexerTest, DataTypeInteger) {
	EXPECT_EQ(lex("integer"), yy::parser::token::INT);
	EXPECT_EQ(lex("INTEGER"), yy::parser::token::INT);
}

TEST_F(LexerTest, DataTypeString) {
	EXPECT_EQ(lex("string"), yy::parser::token::STRING);
	EXPECT_EQ(lex("STRING"), yy::parser::token::STRING);
}

TEST_F(LexerTest, DataTypeChar) {
	EXPECT_EQ(lex("char"), yy::parser::token::CHAR);
	EXPECT_EQ(lex("CHAR"), yy::parser::token::CHAR);
}

TEST_F(LexerTest, DataTypeFloat) {
	EXPECT_EQ(lex("float"), yy::parser::token::FLOAT);
	EXPECT_EQ(lex("FLOAT"), yy::parser::token::FLOAT);
}

TEST_F(LexerTest, DataTypeBoolean) {
	EXPECT_EQ(lex("boolean"), yy::parser::token::BOOLEAN);
	EXPECT_EQ(lex("BOOLEAN"), yy::parser::token::BOOLEAN);
}

TEST_F(LexerTest, DataTypeVoid) {
	EXPECT_EQ(lex("void"), yy::parser::token::VOID);
	EXPECT_EQ(lex("VOID"), yy::parser::token::VOID);
}



TEST_F(LexerTest,NormalIdentifier)
{
	EXPECT_EQ(lex("myVar"), yy::parser::token::IDENTIFIER);
	EXPECT_EQ(yylval.as<std::string>(), "myVar");
}

TEST_F(LexerTest,ShowsErrorForInvalidIdentifier)
{
	lex("3myVar");
	EXPECT_THAT(error_buffer.str(),HasSubstr("Invalid identifier"));
}



TEST_F(LexerTest,IntegerLiteral)
{
	EXPECT_EQ(lex("5"), yy::parser::token::NUMBER);
	EXPECT_EQ(yylval.as<int>(), 5);
}

TEST_F(LexerTest,NegativeIntegerLiteral)
{
	// Negative integers will be processed by the parser so it will return a MINUS and a 5

	std::string input = "-5";
	std::istringstream iss(input);
	lexer->switch_streams(&iss, nullptr);

	EXPECT_EQ(lexer->yylex(&yylval, &loc), yy::parser::token::MINUS);

	EXPECT_EQ(lexer->yylex(&yylval, &loc), yy::parser::token::NUMBER);
	EXPECT_EQ(yylval.as<int>(), 5);
}

// TODO: We need to check for incorrect float literals (like 3.23a, a.5 etc.)

TEST_F(LexerTest,FloatLiteral)
{
	EXPECT_EQ(lex("3.2"), yy::parser::token::NUMBER_FLOAT);
	EXPECT_EQ(yylval.as<double>(), 3.2);
}

TEST_F(LexerTest,ShowsErrorOnFloatLiteralStartingWithALetter)
{
	lex("a.323");
	EXPECT_THAT(error_buffer.str(),HasSubstr("Invalid float literal"));
}

TEST_F(LexerTest,ShowsErrorOnFloatLiteralEndingWithALetter)
{
	lex("3.2a");
	EXPECT_THAT(error_buffer.str(),HasSubstr("Invalid float literal"));
}

TEST_F(LexerTest,ShowsErrorOnFloatLiteralWithALetterInTheMiddle)
{
	lex("3.2a3");
	EXPECT_THAT(error_buffer.str(),HasSubstr("Invalid float literal"));
}

TEST_F(LexerTest,NegativeFloatIntegerLiteral)
{
	// Negative floats will be processed by the parser so it will return a MINUS and a 5
	
	std::string input = "-5.2";
	std::istringstream iss(input);
	lexer->switch_streams(&iss, nullptr);

	EXPECT_EQ(lexer->yylex(&yylval, &loc), yy::parser::token::MINUS);

	EXPECT_EQ(lexer->yylex(&yylval, &loc), yy::parser::token::NUMBER_FLOAT);
	EXPECT_EQ(yylval.as<double>(), 5.2);
}

TEST_F(LexerTest,AbbreviatedFloatLiteral)
{
	EXPECT_EQ(lex(".2"), yy::parser::token::NUMBER_FLOAT);
	EXPECT_EQ(yylval.as<double>(), 0.2);
}

// String Literals

TEST_F(LexerTest, NormalStringLiteral) {
	EXPECT_EQ(lex("\"Hello World!\""), yy::parser::token::STRING_LITERAL);
	EXPECT_EQ(yylval.as<std::string>(), "Hello World!");
}

TEST_F(LexerTest, IntegerInsideStringLiteral) {
	EXPECT_EQ(lex("\"3\""), yy::parser::token::STRING_LITERAL);
	EXPECT_EQ(yylval.as<std::string>(), "3");
}

TEST_F(LexerTest, NegativeIntegerInsideStringLiteral) {
	EXPECT_EQ(lex("\"-3\""), yy::parser::token::STRING_LITERAL);
	EXPECT_EQ(yylval.as<std::string>(), "-3");
}

TEST_F(LexerTest, FloatInsideStringLiteral) {
	EXPECT_EQ(lex("\"3.2\""), yy::parser::token::STRING_LITERAL);
	EXPECT_EQ(yylval.as<std::string>(), "3.2");
}

TEST_F(LexerTest, NegativeFloatInsideStringLiteral) {
	EXPECT_EQ(lex("\"-3.2\""), yy::parser::token::STRING_LITERAL);
	EXPECT_EQ(yylval.as<std::string>(), "-3.2");
}

TEST_F(LexerTest, AbbreviatedFloatInsideStringLiteral) {
	EXPECT_EQ(lex("\".2\""), yy::parser::token::STRING_LITERAL);
	EXPECT_EQ(yylval.as<std::string>(), ".2");
}

TEST_F(LexerTest, NegativeAbbreviatedFloatInsideStringLiteral) {
	EXPECT_EQ(lex("\"-.2\""), yy::parser::token::STRING_LITERAL);
	EXPECT_EQ(yylval.as<std::string>(), "-.2");
}

// String literal errors

TEST_F(LexerTest, ShowsErrorOnUnclosedStringLiteral) {
	lex("\"Hello World");

	EXPECT_THAT(error_buffer.str(), HasSubstr("Unclosed string literal"));

}

TEST_F(LexerTest, ShowsErrorOnInvalidTabulatorInsideStringLiteral) {
	lex("\"\t\"");
	EXPECT_THAT(error_buffer.str(), HasSubstr("Invalid tabulator inside string literal"));
}

TEST_F(LexerTest, ShowsErrorOnInvalidNewLineInsideStringLiteral) {
	lex("\"\n\"");
	EXPECT_THAT(error_buffer.str(), HasSubstr("Invalid new line inside string literal"));
}

// Char literals

TEST_F(LexerTest,NormalCharLiteral)
{
	EXPECT_EQ(lex("'a'"), yy::parser::token::CHAR_LITERAL);
	EXPECT_EQ(yylval.as<char>(), 'a');
}


TEST_F(LexerTest,IntegerInsideCharLiteral)
{
	EXPECT_EQ(lex("'5'"), yy::parser::token::CHAR_LITERAL);
	EXPECT_EQ(yylval.as<char>(), '5');
}

TEST_F(LexerTest,TabulatorInsideCharLiteral)
{
	EXPECT_EQ(lex("'\t'"), yy::parser::token::CHAR_LITERAL);
	EXPECT_EQ(yylval.as<char>(), '\t');
}

TEST_F(LexerTest,NewLineInsideCharLiteral)
{
	EXPECT_EQ(lex("'\\n'"), yy::parser::token::CHAR_LITERAL);
	EXPECT_EQ(yylval.as<char>(), '\n');
}

TEST_F(LexerTest,ShowsErrorOnTooManyCharactersInsideCharLiteral)
{

	lex("'ab'");
	EXPECT_THAT(error_buffer.str(), HasSubstr("Too many characters"));
}



// Operators and Punctuation

// Operator Tokens
TEST_F(LexerTest, OperatorPlusWithWord) {
	EXPECT_EQ(lex("plus"), yy::parser::token::PLUS);
	EXPECT_EQ(lex("PLUS"), yy::parser::token::PLUS);
}

TEST_F(LexerTest, OperatorPlusWithOperator) {
	EXPECT_EQ(lex("+"), yy::parser::token::PLUS);
}

TEST_F(LexerTest, OperatorMinusWithWord) {
	EXPECT_EQ(lex("minus"), yy::parser::token::MINUS);
	EXPECT_EQ(lex("MINUS"), yy::parser::token::MINUS);
}

TEST_F(LexerTest, OperatorMinusWithOperator) {
	EXPECT_EQ(lex("-"), yy::parser::token::MINUS);
}

TEST_F(LexerTest, OperatorMultiplyWithWord) {
	EXPECT_EQ(lex("times"), yy::parser::token::MULTIPLY);
	EXPECT_EQ(lex("TIMES"), yy::parser::token::MULTIPLY);
}

TEST_F(LexerTest, OperatorMultiplyWithOperator) {
	EXPECT_EQ(lex("*"), yy::parser::token::MULTIPLY);
}

TEST_F(LexerTest, OperatorDivideWithWords) {
	EXPECT_EQ(lex("divide by"), yy::parser::token::DIVIDE);
	EXPECT_EQ(lex("divided by"), yy::parser::token::DIVIDE);
	EXPECT_EQ(lex("DIVIDE BY"), yy::parser::token::DIVIDE);
	EXPECT_EQ(lex("DIVIDED BY"), yy::parser::token::DIVIDE);
}

TEST_F(LexerTest, OperatorDivideWithOperator) {
	EXPECT_EQ(lex("/"), yy::parser::token::DIVIDE);
}

// Punctuation Tokens
TEST_F(LexerTest, PunctuationComma) {
	EXPECT_EQ(lex(","), yy::parser::token::COMMA);
}

TEST_F(LexerTest, PunctuationOpenBracket) {
	EXPECT_EQ(lex("("), yy::parser::token::OPEN_BRACKET);
}

TEST_F(LexerTest, PunctuationCloseBracket) {
	EXPECT_EQ(lex(")"), yy::parser::token::CLOSE_BRACKET);
}

// Logical Operators
TEST_F(LexerTest, OperatorOr) {
	EXPECT_EQ(lex("or"), yy::parser::token::OR);
	EXPECT_EQ(lex("OR"), yy::parser::token::OR);
}

TEST_F(LexerTest, OperatorAnd) {
	EXPECT_EQ(lex("and"), yy::parser::token::AND);
	EXPECT_EQ(lex("AND"), yy::parser::token::AND);
}

TEST_F(LexerTest, OperatorNot) {
	EXPECT_EQ(lex("not"), yy::parser::token::NOT);
	EXPECT_EQ(lex("NOT"), yy::parser::token::NOT);
}

// Comparison Operators
TEST_F(LexerTest, OperatorMatchesWithWord) {
	EXPECT_EQ(lex("matches"), yy::parser::token::MATCHES);
	EXPECT_EQ(lex("MATCHES"), yy::parser::token::MATCHES);
}

TEST_F(LexerTest, OperatorMatchesWithOperator) {
	EXPECT_EQ(lex("=="), yy::parser::token::MATCHES);
}

TEST_F(LexerTest, OperatorNotMatchesWithWord) {
	EXPECT_EQ(lex("not matches"), yy::parser::token::NOT_MATCHES);
	EXPECT_EQ(lex("NOT MATCHES"), yy::parser::token::NOT_MATCHES);
}

TEST_F(LexerTest, OperatorNotMatchesWithOperator) {
	EXPECT_EQ(lex("!="), yy::parser::token::NOT_MATCHES);
}

TEST_F(LexerTest, OperatorLessThanWithWord) {
	EXPECT_EQ(lex("less than"), yy::parser::token::LESS_THAN);
	EXPECT_EQ(lex("LESS THAN"), yy::parser::token::LESS_THAN);
}

TEST_F(LexerTest, OperatorLessThanWithOperator) {
	EXPECT_EQ(lex("<"), yy::parser::token::LESS_THAN);
}

TEST_F(LexerTest, OperatorLessThanOrEqualWithWord) {
	EXPECT_EQ(lex("less than or equal"), yy::parser::token::LESS_THAN_OR_EQUAL);
	EXPECT_EQ(lex("LESS THAN OR EQUAL"), yy::parser::token::LESS_THAN_OR_EQUAL);
}

TEST_F(LexerTest, OperatorLessThanOrEqualWithOperator) {
	EXPECT_EQ(lex("<="), yy::parser::token::LESS_THAN_OR_EQUAL);
}

TEST_F(LexerTest, OperatorGreaterThanOrEqualWithWord) {
	EXPECT_EQ(lex("greater than or equal"), yy::parser::token::GREATER_THAN_OR_EQUAL);
	EXPECT_EQ(lex("GREATER THAN OR EQUAL"), yy::parser::token::GREATER_THAN_OR_EQUAL);
}

TEST_F(LexerTest, OperatorGreaterThanOrEqualWithOperator) {
	EXPECT_EQ(lex(">="), yy::parser::token::GREATER_THAN_OR_EQUAL);
}

TEST_F(LexerTest, OperatorGreaterThanWithWord) {
	EXPECT_EQ(lex("greater than"), yy::parser::token::GREATER_THAN);
	EXPECT_EQ(lex("GREATER THAN"), yy::parser::token::GREATER_THAN);
}

TEST_F(LexerTest, OperatorGreaterThanWithOperator) {
	EXPECT_EQ(lex(">"), yy::parser::token::GREATER_THAN);
}

TEST_F(LexerTest, OperatorEqualsWithWord) {
	EXPECT_EQ(lex("equals"), yy::parser::token::EQUALS);
	EXPECT_EQ(lex("EQUALS"), yy::parser::token::EQUALS);
}


TEST_F(LexerTest,OperatorEqualsWithWOperator) {
	EXPECT_EQ(lex("="), yy::parser::token::EQUALS);
}

// Indentation Handling
TEST_F(LexerTest,IndentationHandling) {
	std::string input = "create class\n\tcreate function\n\t\treturn 5\n";
	std::istringstream iss(input);
	lexer->switch_streams(&iss, nullptr);

	EXPECT_EQ(lexer->yylex(&yylval, &loc), yy::parser::token::CREATE);
	EXPECT_EQ(lexer->yylex(&yylval, &loc), yy::parser::token::CLASS);
	EXPECT_EQ(lexer->yylex(&yylval, &loc), yy::parser::token::INDENT);
	EXPECT_EQ(lexer->yylex(&yylval, &loc), yy::parser::token::CREATE);
	EXPECT_EQ(lexer->yylex(&yylval, &loc), yy::parser::token::FUNCTION);
	EXPECT_EQ(lexer->yylex(&yylval, &loc), yy::parser::token::INDENT);
	EXPECT_EQ(lexer->yylex(&yylval, &loc), yy::parser::token::RETURN);
	EXPECT_EQ(lexer->yylex(&yylval, &loc), yy::parser::token::NUMBER);
	EXPECT_EQ(lexer->yylex(&yylval, &loc), yy::parser::token::DEDENT);
	EXPECT_EQ(lexer->yylex(&yylval, &loc), yy::parser::token::DEDENT);
}