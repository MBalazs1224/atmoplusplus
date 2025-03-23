#include "assembly_instruction.hh"

std::string AssemblyInstruction::Format(std::shared_ptr<TempMap> map)
{
    auto dst = Def();
    auto src = Use();
    auto j = Jumps();

    std::shared_ptr<LabelList> jump = (j != nullptr) ? j->head : nullptr;  // Jump labels

    std::stringstream result;  // Resulting formatted assembly string

    int len = assemblyInstruction.length();

    for (int i = 0; i < len; i++) {
        if (assemblyInstruction[i] == '`') {
            // Handle placeholder
            i++;  // Move past the backtick
            switch (assemblyInstruction[i]) {
                case 's': {  // Source temp
                    i++;  // Move past 's'
                    int n = assemblyInstruction[i] - '0';  // Extract index
                    result << map->Map(NthTemp(src, n));  // Replace with mapped temp
                    break;
                }
                case 'd': {  // Destination temp
                    i++;  // Move past 'd'
                    int n = assemblyInstruction[i] - '0';  // Extract index
                    result << map->Map(NthTemp(dst, n));  // Replace with mapped temp
                    break;
                }
                case 'j': {  // Jump label
                    i++;  // Move past 'j'
                    int n = assemblyInstruction[i] - '0';  // Extract index
                    result << NthLabel(jump, n)->ToString();  // Replace with label
                    break;
                }
                case '`': {  // Escaped backtick
                    result << '`';
                    break;
                }
                default: {
                    throw std::runtime_error("bad Assem format: invalid placeholder");
                }
            }
        } else {
            // Append regular characters
            result << assemblyInstruction[i];
        }
    }

    return result.str();    

}

std::shared_ptr<Temp> AssemblyInstruction::NthTemp(std::shared_ptr<TempList> list, int counter)
{
    if(counter == 0) return list->head;

    return NthTemp(list->tail, counter - 1);
}

std::shared_ptr<Label> AssemblyInstruction::NthLabel(std::shared_ptr<LabelList> list, int counter)
{
    if(counter == 0) return list->head;

    return NthLabel(list->tail, counter - 1);
}