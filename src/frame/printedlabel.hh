#pragma once
#include <memory>
#include "access.hh"
#include "label.hh"
#include "../ir/expressions/ir_name.hh"
// Represents that the needed resource can be found at a certain label
class PrintedLabel : public Access
{
    private:
        std::shared_ptr<Label> label;
    
    public:
        PrintedLabel(std::shared_ptr<Label>);

        /// @param startOffset The offset will be ignored.
        /// @return Will return an expression that points to the needed label.
        std::shared_ptr<IRExpression> AsExpression(std::shared_ptr<IRExpression> startOffset) override;
};