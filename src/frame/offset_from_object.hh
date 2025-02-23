#pragma once
#include "access.hh"
#include "../ir/expressions/all_ir_expressions.hh"
#include "../ir/reserved_ir_registers.hh"
// Represents an offseting from a specific object in memory (like accessing class members etc.)
class OffsetFromObject : public Access
{
    
    public:
        int offset;

        bool shouldUseRDI;
        /*
        * @param objectLocation The expression that yields the location of the object in memory.
        * @returns The expression that correctly offsets from that object.
        */
        std::shared_ptr<IRExpression> AsExpression(std::shared_ptr<IRExpression> objectLocation) override;

        OffsetFromObject(int);
};