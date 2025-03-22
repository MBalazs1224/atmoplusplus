#pragma once
#include <memory>
#include "../asm/assembly_instruction_list.hh"
#include "../ir/statements/ir_statement.hh"
#include "../ir/statements/ir_statement_list.hh"
#include "../ir/expressions/ir_temp.hh"
class CodeGenerator
{
    protected:
        std::shared_ptr<AssemblyInstructionList> ilist;
        std::shared_ptr<AssemblyInstructionList> last;
        
        /// @brief Correctly adds the instruction to the final list.
        /// @param instruction The newly generated instruction.
        void EmitInstruction(std::shared_ptr<AssemblyInstruction> instruction); 

        /// @brief Munches the statement.
        /// @param stmt The given IR statement.
        virtual void MunchStatement(std::shared_ptr<IRStatement> stmt) = 0;

        /// @brief Munches the expression and returns the location of it's value.
        /// @param exp The given IR expression.
        /// @return The register where the value can be found.
        virtual std::shared_ptr<IRTemp> MunchExpression(std::shared_ptr<IRExpression> exp) = 0;

        std::shared_ptr<AssemblyInstructionList> CodeGen(std::shared_ptr<IRStatementList> statementList);
};