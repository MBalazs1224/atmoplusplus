#include "ir_block.hh"

IRBlock::IRBlock(std::string nameIn, std::vector<std::shared_ptr<IRStatement>> statementsIn,std::vector<std::shared_ptr<IRBlock>> nextIn) : name(nameIn), statements(statementsIn), nextBlocks(nextIn) {}

IRBlock::IRBlock(std::string nameIn) : IRBlock(nameIn, std::vector<std::shared_ptr<IRStatement>>{},std::vector<std::shared_ptr<IRBlock>>{}) {}