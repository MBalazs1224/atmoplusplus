#pragma once
#include <memory>
#include "vtable_info.hh"
class GlobalVtables
{
    public:
        static std::vector<std::shared_ptr<VtableInfo>> vtables;

        static bool IsEmpty();
};