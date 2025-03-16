#include "ir_block.hh"

IRBlock::IRBlock(std::string nameIn, std::vector<IRStatement> statementsIn,std::shared_ptr<IRBlock> nextIn) : name(nameIn), statements(statementsIn), next(nextIn) {}