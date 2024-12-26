#include "ir_label.hh"

IRLabel::IRLabel(std::shared_ptr<Label> l) : label(std::move(l))
{

}