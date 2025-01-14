#include "literal.hh"

std::shared_ptr<IRStatement> Literal::TranslateToIR()
{
    throw std::runtime_error("Literal should not be translated to IR as nodes");
}