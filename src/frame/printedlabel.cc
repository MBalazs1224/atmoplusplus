#include "printedlabel.hh"

PrintedLabel::PrintedLabel(std::shared_ptr<Label> labelIn) : label(labelIn) {}

std::shared_ptr<IRExpression> PrintedLabel::AsExpression(std::shared_ptr<IRExpression> startOffset)
{
    return std::make_shared<IRName>(
        label
    );
}