#include "linear_scan.hh"

void LinearScanMap::ExpireOldIntervals(std::vector<LiveInterval>& activeIntervals, int currentStart)
{
    activeIntervals.erase(
        std::remove_if(activeIntervals.begin(),activeIntervals.end(),[&](const LiveInterval& interval){
            return interval.end < currentStart; // Remove the interval if it's already ended
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

    std::unordered_map<std::shared_ptr<Temp>, std::string> tempToReg;

    for (const auto& interval : intervals)
    {
        // Remove expired intervals
        ExpireOldIntervals(active, interval.start);

        // If the register is already reserved, just skip it
        if(IsRegisterReserved(interval.temp))
            continue;

        if(active.size() == regPool.size())
        {
            // There are no more registers available, spillingis required
            throw std::logic_error("Register spilling is not implemented yet!");
        }
        else
        {
            int indexForReg = active.size();
            tempToReg[interval.temp] = regPool[indexForReg];
            active.push_back(interval);
        }
        
    }

    std::vector<AllocatedReg> allocations;

    for (auto &&interval : intervals)
    {
        auto temp = interval.temp;
        auto allocatedRegister = tempToReg[temp];


        allocations.push_back(
            AllocatedReg(temp,allocatedRegister)
        );
    }
    
    return allocations;
    
}

std::vector<LiveInterval> LinearScanMap::ComputeLiveIntervals(std::shared_ptr<AssemblyInstructionList> instructions)
{

    std::unordered_map<std::shared_ptr<Temp>, std::pair<int, int>> intervalMap;


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

            // If the map doesn't contain a interval for this temp, add it to it with the current index as it's start and end

            if(intervalMap.count(temp) == 0)
            {
                intervalMap[temp] = {instructionIndex,instructionIndex};
            }

            // Set the start or end of the liveness based on if the current index is lower then the one already set

            intervalMap[temp].first = std::min(intervalMap[temp].first,instructionIndex);
            intervalMap[temp].second = std::max(intervalMap[temp].second,instructionIndex);

        }

        for(auto tempList = defs; tempList != nullptr; tempList = tempList->tail)
        {
            auto temp = tempList->head;

            // If the map doesn't contain a interval for this temp, add it to it with the current index as it's start and end

            if(intervalMap.count(temp) == 0)
            {
                intervalMap[temp] = {instructionIndex,instructionIndex};
            }

            // Set the start or end of the liveness based on if the current index is lower then the one already set

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
    // If it's a 64 bit register, the register is already in the correct form
    if(size == 8)
        return reg;

    if (reg == "r10")
    {
        switch(size) {
            case 1: return "r10b";
            case 2: return "r10w";
            case 4: return "r10d";
        }
    }
    else if (reg == "r11")
    {
        switch(size) {
            case 1: return "r11b";
            case 2: return "r11w";
            case 4: return "r11d";
        }
    }
    else if (reg == "r12")
    {
        switch(size) {
            case 1: return "r12b";
            case 2: return "r12w";
            case 4: return "r12d";
        }
    }
    else if (reg == "r13")
    {
        switch(size) {
            case 1: return "r13b";
            case 2: return "r13w";
            case 4: return "r13d";
        }
    }
    else if (reg == "r14")
    {
        switch(size) {
            case 1: return "r14b";
            case 2: return "r14w";
            case 4: return "r14d";
        }
    }
    else if (reg == "r15")
    {
        switch(size) {
            case 1: return "r15b";
            case 2: return "r15w";
            case 4: return "r15d";
        }
    }

    // If not of it matched, just return the reg as it is
    return reg;
}

std::string LinearScanMap::Map(std::shared_ptr<Temp> temp, int sizeNeeded)
{

    // If the register is a reserved registers, just return it's pre calculated name
    if(ReservedIrRegisters::ReservedTempToReg.find(temp) != ReservedIrRegisters::ReservedTempToReg.end())
    {
        return ReservedIrRegisters::ReservedTempToReg[temp];
    }


    if(tempToPhysical.count(temp))
    {
        auto correctReg = tempToPhysical[temp];




        return TruncateRegToSize(correctReg, sizeNeeded);
    }

    throw std::logic_error("Temp wasn't allocated in time!");
}