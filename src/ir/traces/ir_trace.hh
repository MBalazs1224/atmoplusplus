#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include "../blocks/ir_block.hh"
class IRTraceSchedule
{
    private:
        std::shared_ptr<IRBlock> block;

        std::unordered_map<std::shared_ptr<Label>, std::shared_ptr<IRStatementList>> table;

        std::shared_ptr<IRStatementList> GetNext();
        std::shared_ptr<IRStatementList> GetLast(std::shared_ptr<IRStatementList>);
        void Trace(std::shared_ptr<IRStatementList>);

        /// @brief If the key exists returns it's value, otherwise nullptr. (Default behaviour will throw exception)
        /// @param key The wanted key.
        /// @return The key or nullptr.
        std::shared_ptr<IRStatementList> GetElementFromMap(std::shared_ptr<Label> key);

    public:
        std::shared_ptr<IRStatementList> statements;

        IRTraceSchedule(std::shared_ptr<IRBlock>);
};