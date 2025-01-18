#include "lexer_tests.hh"
#include "parser_tests.hh"
#include "semantic_analyzer_tests.hh"
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}