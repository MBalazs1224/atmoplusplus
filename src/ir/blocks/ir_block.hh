#pragma once

#include <memory>
#include <string>
#include <vector>

#include "../statements/ir_statement.hh"
#include "../statements/ir_label.hh"
#include "../statements/ir_jump.hh"
#include "../statements/ir_cjump.hh"
#include "../statements/ir_statement_list.hh"
#include "statement_list_list.hh"
#include "../../frame/label.hh"
#include "../expressions/ir_name.hh"
class IRBlock
{
      private:

        std::shared_ptr<IRStatementListList> lastBlock;
        std::shared_ptr<IRStatementList> lastStatement;


        void MakeBlocks(std::shared_ptr<IRStatementList> statement);
        void DoStatements(std::shared_ptr<IRStatementList> statement);
        void AddStatement(std::shared_ptr<IRStatement> statement);

      public:

        std::shared_ptr<Label> doneLabel;
        std::shared_ptr<IRStatementListList> blocks;

        // Will generate the blocks based on the root statement list node
        IRBlock(std::shared_ptr<IRStatementList>);
};