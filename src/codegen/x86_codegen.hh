#pragma once
#include "codegen.hh"
#include "../asm/assembly_label.hh"
#include "../asm/assembly_move.hh"
#include "../asm/assembly_oper.hh"

class x86CodeGenerator : public CodeGenerator
{
        std::string RelationalOperatorToString(RelationalOperator op) override;
        
        // Statements

        void MunchLabel(std::shared_ptr<IRLabel>) override;
        void MunchCjump(std::shared_ptr<IRCJump>) override;
        void MunchEnter(std::shared_ptr<IREnter>) override;
        void MunchEvaluateExpression(std::shared_ptr<IREvaluateExpression>) override;
        void MunchJump(std::shared_ptr<IRJump>) override;
        void MunchLeave(std::shared_ptr<IRLeave>) override;
        void MunchMove(std::shared_ptr<IRMove>) override;
        void MunchPop(std::shared_ptr<IRPop>) override;
        void MunchPush(std::shared_ptr<IRPush>) override;
        void MunchReturn(std::shared_ptr<IRReturn>) override;
        void MunchSysCall(std::shared_ptr<IRSysCall>) override;

        /// @brief Converts the numerical size into assembly size instruction.
        /// @param size The size in bytes.
        /// @return The assembly size instruction.
        std::string SizeToString(int size);

        // Expressions

        std::shared_ptr<Temp> MunchBinaryOperator(std::shared_ptr<IRBinaryOperator>) override;
        std::shared_ptr<Temp> MunchFunctionCall(std::shared_ptr<IRCall>) override;
        std::shared_ptr<Temp> MunchConstFloat(std::shared_ptr<IRConstFloat>) override;
        std::shared_ptr<Temp> MunchConstInteger(std::shared_ptr<IRConst>) override;
        std::shared_ptr<Temp> MunchMem(std::shared_ptr<IRMem>) override;
        std::shared_ptr<Temp> MunchName(std::shared_ptr<IRName>) override;
        std::shared_ptr<Temp> MunchTemp(std::shared_ptr<IRTemp>) override;
        std::shared_ptr<Temp> MunchLoadEffectiveAddress(std::shared_ptr<IRLoadEffectiveAddress>) override;
};