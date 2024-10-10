#include <gtest/gtest.h>
#include "../../AtmoLexer.hh"
#include "../../parser.tab.hh"
#include <sstream>

class LexerTest : public ::testing::Test {
protected:
    AtmoLexer lexer;
    yy::parser::semantic_type yylval;
    yy::parser::location_type loc;

    void SetUp() override {
        loc.initialize();
    }

    int lex(const std::string& input) {
        std::istringstream iss(input);
        lexer.switch_streams(&iss, nullptr);
        return lexer.yylex(&yylval, &loc);
    }
    LexerTest()
    {
        loc.initialize();
    }
};

TEST_F(LexerTest, BasicTokens) {
    EXPECT_EQ(lex("create"), yy::parser::token::CREATE);
    EXPECT_EQ(lex("class"), yy::parser::token::CLASS);
    EXPECT_EQ(lex("public"), yy::parser::token::PUBLIC);
    EXPECT_EQ(lex("protected"), yy::parser::token::PROTECTED);
    EXPECT_EQ(lex("private"), yy::parser::token::PRIVATE);
    EXPECT_EQ(lex("static"), yy::parser::token::STATIC);
    EXPECT_EQ(lex("function"), yy::parser::token::FUNCTION);
    EXPECT_EQ(lex("return"), yy::parser::token::RETURN);
    EXPECT_EQ(lex("if"), yy::parser::token::IF);
    EXPECT_EQ(lex("else"), yy::parser::token::ELSE);
    EXPECT_EQ(lex("else if"), yy::parser::token::ELSE_IF);
    EXPECT_EQ(lex("until"), yy::parser::token::UNTIL);
    EXPECT_EQ(lex("do"), yy::parser::token::DO);
    EXPECT_EQ(lex("true"), yy::parser::token::TRUE);
    EXPECT_EQ(lex("false"), yy::parser::token::FALSE);
    EXPECT_EQ(lex("call"), yy::parser::token::CALL);
    EXPECT_EQ(lex("class"), yy::parser::token::CLASS);
    EXPECT_EQ(lex("void"), yy::parser::token::VOID);
    EXPECT_EQ(lex("with"), yy::parser::token::WITH);
    EXPECT_EQ(lex("array of"), yy::parser::token::ARRAY_OF);
    EXPECT_EQ(lex("constructor"), yy::parser::token::CONSTRUCTOR);
    EXPECT_EQ(lex("derives from"), yy::parser::token::DERIVES_FROM);
    EXPECT_EQ(lex("derive from"), yy::parser::token::DERIVES_FROM);
}
TEST_F(LexerTest, DataTypes) {
    EXPECT_EQ(lex("integer"),yy::parser::token::INT);
    EXPECT_EQ(lex("string"),yy::parser::token::STRING);
    EXPECT_EQ(lex("char"),yy::parser::token::CHAR);
    EXPECT_EQ(lex("float"),yy::parser::token::FLOAT);
    EXPECT_EQ(lex("boolean"),yy::parser::token::BOOLEAN);
    EXPECT_EQ(lex("void"),yy::parser::token::VOID);
}

TEST_F(LexerTest, IdentifiersAndNumbers) {
    EXPECT_EQ(lex("myVar"), yy::parser::token::IDENTIFIER);
    EXPECT_EQ(yylval.as<std::string>(), "myVar");

    EXPECT_EQ(lex("123"), yy::parser::token::NUMBER);
    EXPECT_EQ(yylval.as<int>(), 123);

    EXPECT_EQ(lex("45.67"), yy::parser::token::NUMBER_FLOAT);
    EXPECT_EQ(yylval.as<double>(), 45.67);
}

TEST_F(LexerTest, StringAndCharLiterals) {
    EXPECT_EQ(lex("\"hello\""), yy::parser::token::STRING_LITERAL);
    EXPECT_EQ(yylval.as<std::string>(), "hello");

    // YYEOF means that there is an error, so it returns the end of file flag

    EXPECT_EQ(lex("'a'"), yy::parser::token::CHAR_LITERAL);
    EXPECT_EQ(yylval.as<char>(), 'a');
}

TEST_F(LexerTest, OperatorsAndPunctuation) {
    EXPECT_EQ(lex("+"), yy::parser::token::PLUS);
    EXPECT_EQ(lex("plus"), yy::parser::token::PLUS);

    EXPECT_EQ(lex("-"), yy::parser::token::MINUS);
    EXPECT_EQ(lex("minus"), yy::parser::token::MINUS);

    EXPECT_EQ(lex("*"), yy::parser::token::MULTIPLY);
    EXPECT_EQ(lex("times"), yy::parser::token::MULTIPLY);

    EXPECT_EQ(lex("/"), yy::parser::token::DIVIDE);
    EXPECT_EQ(lex("divide by"), yy::parser::token::DIVIDE);
    EXPECT_EQ(lex("divided by"), yy::parser::token::DIVIDE);

    EXPECT_EQ(lex(","), yy::parser::token::COMMA);

    EXPECT_EQ(lex("("), yy::parser::token::OPEN_BRACKET);
    EXPECT_EQ(lex(")"), yy::parser::token::CLOSE_BRACKET);
    EXPECT_EQ(lex("or"), yy::parser::token::OR);
    EXPECT_EQ(lex("and"), yy::parser::token::AND);
    EXPECT_EQ(lex("not"), yy::parser::token::NOT);

    EXPECT_EQ(lex("matches"), yy::parser::token::MATCHES);
    EXPECT_EQ(lex("=="), yy::parser::token::MATCHES);

    EXPECT_EQ(lex("not matches"), yy::parser::token::NOT_MATCHES);
    EXPECT_EQ(lex("!="), yy::parser::token::NOT_MATCHES);

    EXPECT_EQ(lex("less than"), yy::parser::token::LESS_THAN);
    EXPECT_EQ(lex("<"), yy::parser::token::LESS_THAN);

    EXPECT_EQ(lex("less than or equal"), yy::parser::token::LESS_THAN_OR_EQUAL);
    EXPECT_EQ(lex("<="), yy::parser::token::LESS_THAN_OR_EQUAL);

    EXPECT_EQ(lex("greater than or equal"), yy::parser::token::GREATER_THAN_OR_EQUAL);
    EXPECT_EQ(lex(">="), yy::parser::token::GREATER_THAN_OR_EQUAL);

    EXPECT_EQ(lex("greater than"), yy::parser::token::GREATER_THAN);
    EXPECT_EQ(lex(">"), yy::parser::token::GREATER_THAN);

    EXPECT_EQ(lex("equals"), yy::parser::token::EQUALS);
    EXPECT_EQ(lex("="), yy::parser::token::EQUALS);
}

TEST_F(LexerTest, IndentationHandling) {
    std::string input = "create class\n\tcreate function\n\t\treturn 5\n";
    std::istringstream iss(input);
    lexer.switch_streams(&iss, nullptr);

    EXPECT_EQ(lexer.yylex(&yylval, &loc), yy::parser::token::CREATE);
    EXPECT_EQ(lexer.yylex(&yylval, &loc), yy::parser::token::CLASS);
    EXPECT_EQ(lexer.yylex(&yylval, &loc), yy::parser::token::INDENT);
    EXPECT_EQ(lexer.yylex(&yylval, &loc), yy::parser::token::CREATE);
    EXPECT_EQ(lexer.yylex(&yylval, &loc), yy::parser::token::FUNCTION);
    EXPECT_EQ(lexer.yylex(&yylval, &loc), yy::parser::token::INDENT);
    EXPECT_EQ(lexer.yylex(&yylval, &loc), yy::parser::token::RETURN);
    EXPECT_EQ(lexer.yylex(&yylval, &loc), yy::parser::token::NUMBER);
    EXPECT_EQ(lexer.yylex(&yylval, &loc), yy::parser::token::DEDENT);
    EXPECT_EQ(lexer.yylex(&yylval, &loc), yy::parser::token::DEDENT);
}