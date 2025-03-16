#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../statements/ir_statement.hh"
class IRBlock
{
    public:
        std::string name;
        std::vector<IRStatement> statements;

        std::shared_ptr<IRBlock> next;

        IRBlock(std::string, std::vector<IRStatement>,std::shared_ptr<IRBlock>);
};