#pragma once

#include <vector>
#include <algorithm>
#include <unordered_map>

#include "../tempmap.hh"
#include "allocated_reg.hh"
#include "live_interval.hh"
#include "../../asm/assembly_instruction_list.hh"
#include "../../ir/reserved_ir_registers.hh"
class LinearScanMap : public TempMap
{
    private:


        std::unordered_map<std::shared_ptr<Temp>, std::string> tempToPhysical;

        /// @brief Will truncate the register to the correct size if needed.
        /// @param reg The register's name.
        /// @param size The needed size in bytes.
        /// @return The correct register.
        std::string TruncateRegToSize(std::string reg, int size);

        /// @brief Computes the liveness intervals for the received assembly instructions.
        /// @param instructions The asm instructions.
        /// @return A vector conatining all liveness intervals.
        std::vector<LiveInterval> ComputeLiveIntervals(std::shared_ptr<AssemblyInstructionList> instructions);

        /// @brief Allocates physical regs to temps based on the live intervals.
        /// @param intervals The liveness intervals.
        /// @param regPool The available registers.
        /// @return The allocated temp and register pairs.
        std::vector<AllocatedReg> RegisterAllocation(std::vector<LiveInterval>& intervals, const std::vector<std::string>& regPool);

        /// @brief Removes expired intervals.
        /// @param activeIntervals A vector containing all active intervals..
        /// @param start The start interval.
        void ExpireOldIntervals(std::vector<LiveInterval>& activeIntervals,int start);


    public:
        std::string Map(std::shared_ptr<Temp>, int size) override;

        LinearScanMap(std::shared_ptr<AssemblyInstructionList>);
};