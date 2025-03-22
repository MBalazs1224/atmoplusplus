#pragma once
#include <memory>
#include "../asm/assembly_instruction_list.hh"
#include "../ir/statements/ir_statement.hh"
#include "../ir/statements/ir_statement_list.hh"
#include "../ir/expressions/all_ir_expressions.hh"
#include "../ir/statements/all_ir_statements.hh"
#include "../frame/temp.hh"
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
        void MunchStatement(std::shared_ptr<IRStatement> stmt);

        // Abstract functions for all statement nodes
        
        virtual void MunchLabel(std::shared_ptr<IRLabel>) = 0;
        virtual void MunchCjump(std::shared_ptr<IRCJump>) = 0;
        virtual void MunchEnter(std::shared_ptr<IREnter>) = 0;
        virtual void MunchEvaluateExpression(std::shared_ptr<IREvaluateExpression>) = 0;
        virtual void MunchJump(std::shared_ptr<IRJump>) = 0;
        virtual void MunchLeave(std::shared_ptr<IRLeave>) = 0;
        virtual void MunchMove(std::shared_ptr<IRMove>) = 0;
        virtual void MunchPop(std::shared_ptr<IRPop>) = 0;
        virtual void MunchPush(std::shared_ptr<IRPush>) = 0;
        virtual void MunchReturn(std::shared_ptr<IRReturn>) = 0;
        virtual void MunchSysCall(std::shared_ptr<IRSysCall>) = 0;
        


        /// @brief Munches the expression and returns the location of it's value.
        /// @param exp The given IR expression.
        /// @return The register where the value can be found.
        virtual std::shared_ptr<Temp> MunchExpression(std::shared_ptr<IRExpression> exp);

        // Abstract functions for all expression nodes

        virtual std::shared_ptr<Temp> MunchBinaryOperator(std::shared_ptr<IRBinaryOperator>) = 0;
        virtual std::shared_ptr<Temp> MunchFunctionCall(std::shared_ptr<IRCall>) = 0;
        virtual std::shared_ptr<Temp> MunchConstFloat(std::shared_ptr<IRConstFloat>) = 0;
        virtual std::shared_ptr<Temp> MunchConstInteger(std::shared_ptr<IRConst>) = 0;
        virtual std::shared_ptr<Temp> MunchMem(std::shared_ptr<IRMem>) = 0;
        virtual std::shared_ptr<Temp> MunchName(std::shared_ptr<IRName>) = 0;
        virtual std::shared_ptr<Temp> MunchTemp(std::shared_ptr<IRTemp>) = 0;
        
    public:

        std::shared_ptr<AssemblyInstructionList> CodeGen(std::shared_ptr<IRStatementList> statementList);
};