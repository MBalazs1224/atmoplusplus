#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../statements/ir_statement.hh"
class IRBlock
{
    public:
        std::string name;
        std::vector<std::shared_ptr<IRStatement>> statements;

        std::vector<std::shared_ptr<IRBlock>> nextBlocks;

        IRBlock(std::string, std::vector<std::shared_ptr<IRStatement>>,std::vector<std::shared_ptr<IRBlock>>);

        IRBlock(std::string);
};