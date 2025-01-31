#pragma once
#include "../symboltableelement.hh"
#include "../../ast/expressions/identifier.hh"
#include "../../frame/access.hh"
class VariableSymbol : public SymbolTableElement
{
    public:
    VariableSymbol(std::shared_ptr<Type> type_in, std::shared_ptr<Attribute> attr_in);
    std::shared_ptr<Type> GetType() override;
    bool Check() override;

    DataSize GetSize() override;

    // Will contain the Access object to this variable (wether it's in reg or frame)
    std::shared_ptr<Access> access;


    std::shared_ptr<TranslateExpression> TranslateExpressionToIr() override;
    std::shared_ptr<IRStatement> TranslateToIR() override;
};
