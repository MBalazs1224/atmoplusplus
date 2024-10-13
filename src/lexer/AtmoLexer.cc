#include "AtmoLexer.hh"

AtmoLexer::AtmoLexer() : yyFlexLexer(nullptr)
{
    loc = std::make_unique<yy::parser::location_type>();
}


AtmoLexer::AtmoLexer(std::istream* in) : yyFlexLexer(in)
{
    loc = std::make_unique<yy::parser::location_type>();
}