#include "linear_scan.hh"

void LinearScanMap::ExpireOldIntervals(std::vector<LiveInterval>& activeIntervals, int currentStart, std::unordered_set<std::string>& freeRegs, const std::unordered_map<std::shared_ptr<Temp>, std::string, TempPtrHash, TempPtrEqual>& tempToReg)
{
    activeIntervals.erase(
        std::remove_if(activeIntervals.begin(),activeIntervals.end(),[&](const LiveInterval& interval){
            if (interval.end < currentStart)
            {
                freeRegs.insert(tempToReg.at(interval.temp)); // Insert the expired temp to the free regs list, so it can be reused
                return true;
            }
            return false;
        }),
        activeIntervals.end()
    );
}

bool LinearScanMap::IsRegisterReserved(std::shared_ptr<Temp> temp)
{
    return ReservedIrRegisters::ReservedTempToReg.count(temp) > 0;
}

std::vector<AllocatedReg> LinearScanMap::RegisterAllocation(std::vector<LiveInterval>& intervals,const std::vector<std::string>& regPool) 
{
    std::vector<LiveInterval> active;
    std::unordered_map<std::shared_ptr<Temp>, std::string, TempPtrHash, TempPtrEqual> tempToReg;
    std::unordered_set<std::string> freeRegs(regPool.begin(), regPool.end());

    for (const auto& interval : intervals) {
        ExpireOldIntervals(active, interval.start, freeRegs, tempToReg);

        if (IsRegisterReserved(interval.temp))
            continue;

        if (freeRegs.empty()) {
            throw std::logic_error("Register spilling is not implemented yet!");
        } else {
            // Assign the first available free register
            auto reg = *freeRegs.begin();
            tempToReg[interval.temp] = reg;
            freeRegs.erase(reg);
            active.push_back(interval);
        }
    }

    // Restore freed registers when intervals expire

    for (const auto& interval : active) {
        freeRegs.insert(tempToReg[interval.temp]);
    }

    std::vector<AllocatedReg> allocations;
    for (auto&& interval : intervals) {
        allocations.emplace_back(interval.temp, tempToReg[interval.temp]);
    }
    return allocations;
    
}

std::vector<LiveInterval> LinearScanMap::ComputeLiveIntervals(std::shared_ptr<AssemblyInstructionList> instructions)
{

    std::unordered_map<std::shared_ptr<Temp>, std::pair<int, int>, TempPtrHash, TempPtrEqual> intervalMap;


    // Used for giving an physical index number to instructions, so we know at witch instruction this temp goes out of scope (is not used again)
    int instructionIndex = 0;

    auto temp = instructions;
    while (temp)
    {
        auto currentInstruction = temp->head;


        auto defs = currentInstruction->Def();
        auto uses = currentInstruction->Use();



        for(auto tempList = defs; tempList != nullptr; tempList = tempList->tail)
        {
            auto temp = tempList->head;
            if(!temp) // It can happen that it doesn't have a source or destination (at immediate values etc.)
              	continue;

            // If the map doesn't contain a interval for this temp, add it to it with the current index as it's start and end

            if(intervalMap.count(temp) == 0)
            {
                intervalMap[temp] = {instructionIndex,instructionIndex};
            }

            // Set the start or end of the liveness based on if the current index is lower then the one already set

            intervalMap[temp].first = std::min(intervalMap[temp].first,instructionIndex);
            intervalMap[temp].second = std::max(intervalMap[temp].second,instructionIndex);

        }

        for(auto tempList = uses; tempList != nullptr; tempList = tempList->tail)
        {
            auto temp = tempList->head;
            if(!temp) // It can happen that it doesn't have a source or destination (at immediate values etc.)
              	continue;


            // If the map doesn't contain a interval for this temp, add it to it with the current index as it's start and end

            if(intervalMap.count(temp) == 0)
            {
                intervalMap[temp] = {instructionIndex,instructionIndex};
            }

            // Set the start or end of the liveness based on if the current index is lower then the one already set
            intervalMap[temp].first = std::min(intervalMap[temp].first,instructionIndex);
            intervalMap[temp].second = std::max(intervalMap[temp].second,instructionIndex);

        }
        
        instructionIndex++;

        temp = temp->tail;
    }

    // Add the intervals from the map to a vector

    std::vector<LiveInterval> intervals;

    for (auto& [temp, range] : intervalMap)
    {
        intervals.emplace_back(temp, range.first, range.second);
    }
    
    // Sort the intervals descending

    std::sort(intervals.begin(), intervals.end(), [](const LiveInterval& a, const LiveInterval& b){
        return a.start < b.start;
    });

    return intervals;
    
}

LinearScanMap::LinearScanMap(std::shared_ptr<AssemblyInstructionList> instructions)
{
    auto intervals = ComputeLiveIntervals(instructions);

    // Only caller saved regs are available for now

    std::vector<std::string> availableRegisters = {"r10","r11","r12","r13","r14","r15"};

    auto allocations = RegisterAllocation(intervals,availableRegisters);

    for (auto &&alloc : allocations)
    {
        this->tempToPhysical[alloc.temp] = alloc.physicalReg;
    }
    


}

std::string LinearScanMap::TruncateRegToSize(std::string reg, int size)
{
    if (size == 8)
        return reg;

    if (reg == "rax") {
        switch(size) { case 1: return "al"; case 2: return "ax"; case 4: return "eax"; }
    } else if (reg == "rbx") {
        switch(size) { case 1: return "bl"; case 2: return "bx"; case 4: return "ebx"; }
    } else if (reg == "rcx") {
        switch(size) { case 1: return "cl"; case 2: return "cx"; case 4: return "ecx"; }
    } else if (reg == "rdx") {
        switch(size) { case 1: return "dl"; case 2: return "dx"; case 4: return "edx"; }
    } else if (reg == "rsi") {
        switch(size) { case 1: return "sil"; case 2: return "si"; case 4: return "esi"; }
    } else if (reg == "rdi") {
        switch(size) { case 1: return "dil"; case 2: return "di"; case 4: return "edi"; }
    } else if (reg == "rbp") {
        switch(size) { case 1: return "bpl"; case 2: return "bp"; case 4: return "ebp"; }
    } else if (reg == "rsp") {
        switch(size) { case 1: return "spl"; case 2: return "sp"; case 4: return "esp"; }
    } else if (reg == "r8") {
        switch(size) { case 1: return "r8b"; case 2: return "r8w"; case 4: return "r8d"; }
    } else if (reg == "r9") {
        switch(size) { case 1: return "r9b"; case 2: return "r9w"; case 4: return "r9d"; }
    } else if (reg == "r10") {
        switch(size) { case 1: return "r10b"; case 2: return "r10w"; case 4: return "r10d"; }
    } else if (reg == "r11") {
        switch(size) { case 1: return "r11b"; case 2: return "r11w"; case 4: return "r11d"; }
    } else if (reg == "r12") {
        switch(size) { case 1: return "r12b"; case 2: return "r12w"; case 4: return "r12d"; }
    } else if (reg == "r13") {
        switch(size) { case 1: return "r13b"; case 2: return "r13w"; case 4: return "r13d"; }
    } else if (reg == "r14") {
        switch(size) { case 1: return "r14b"; case 2: return "r14w"; case 4: return "r14d"; }
    } else if (reg == "r15") {
        switch(size) { case 1: return "r15b"; case 2: return "r15w"; case 4: return "r15d"; }
    }

    // Default fallback (in case of unknown register or size)
    return reg;
}

std::string LinearScanMap::Map(std::shared_ptr<Temp> temp, int sizeNeeded)
{

    // If the register is a reserved registers, just return it's pre calculated name
    if(ReservedIrRegisters::ReservedTempToReg.find(temp) != ReservedIrRegisters::ReservedTempToReg.end())
    {
        auto correctReg = ReservedIrRegisters::ReservedTempToReg[temp];
        return TruncateRegToSize(correctReg, sizeNeeded);
    }


    if(tempToPhysical.count(temp))
    {
        auto correctReg = tempToPhysical[temp];




        return TruncateRegToSize(correctReg, sizeNeeded);
    }

    throw std::logic_error("Temp wasn't allocated in time!");
}