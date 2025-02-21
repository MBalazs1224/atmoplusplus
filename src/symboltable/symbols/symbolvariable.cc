#include "symbolvariable.hh"

VariableSymbol::VariableSymbol(std::shared_ptr<Type> type_in, std::vector<std::shared_ptr<Attribute>> attr_in)
    : SymbolTableElement(type_in, std::move(attr_in)) {}

std::shared_ptr<Type> VariableSymbol::GetType() {
    if (auto id = std::dynamic_pointer_cast<Identifier>(type)) {
        return id->GetType();
    }
    return this->type;
}

bool VariableSymbol::Check()
{


    bool returnValue = true;

    int numberOfAccessModifiers = 0;

    bool hasDuplicates = std::adjacent_find(attributes.begin(),attributes.end()) != attributes.end();
    if(hasDuplicates)
    {
        Error::ShowError("An atribute has been specified multiple times!", this->location);
        returnValue = false;
    }

    for (auto &&attribute : attributes)
    {
        // A variable cannot be virtual
        if(std::dynamic_pointer_cast<AttributeVirtual>(attribute))
        {
            Error::ShowError("A variable cannot be virtual!", this->location);
            returnValue = false;
        }
        // A variable cannot be overriding
        if(std::dynamic_pointer_cast<AttributeOverriding>(attribute))
        {
            Error::ShowError("A variable cannot be overriding!", this->location);
            returnValue = false;
        }
        // A variable cannot have multiple access modifiers like public and private at the same time
        if(std::dynamic_pointer_cast<AttributePublic>(attribute) || std::dynamic_pointer_cast<AttributePrivate>(attribute) ||std::dynamic_pointer_cast<AttributeProtected>(attribute))
        {
            numberOfAccessModifiers++;
            if(numberOfAccessModifiers > 1)
            {
                Error::ShowError("A variable can only have maximum 1 access modifier!",this->location);
                returnValue = false;
            }
        }
    }

    // If the variable doesn't have an access modifier, then default to private
    if(numberOfAccessModifiers == 0)
    {
        attributes.push_back(
            std::make_shared<AttributePrivate>()
        );
    }

    return returnValue;
    
}

DataSize VariableSymbol::GetSize()
{
    return type->GetSize();
}

std::shared_ptr<TranslateExpression> VariableSymbol::TranslateExpressionToIr()
{
    // Use the access object to retreive this variable's location

    auto fp_exp = std::make_shared<IRTemp>(ReservedIrRegisters::StackPointer);
    auto access_exp = access->AsExpression(fp_exp);
    
    return std::make_shared<TranslateValueExpression>(access_exp);
}

std::shared_ptr<IRStatement> VariableSymbol::TranslateToIR()
{
    throw std::runtime_error("VariableSymbol should not be translated to IR");
}
