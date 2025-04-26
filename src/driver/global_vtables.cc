#include "global_vtables.hh"

std::vector<std::shared_ptr<VtableInfo>> GlobalVtables::vtables = std::vector<std::shared_ptr<VtableInfo>>();


bool GlobalVtables::IsEmpty()
{
    return vtables.empty();
}