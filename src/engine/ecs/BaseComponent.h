#pragma once

#include <string>
#include <vector>

typedef size_t ComponentID;
typedef size_t ComponentTypeId;

struct BaseComponent
{
    std::string              Name;
    ComponentID              Id;
    ComponentTypeId          TypeId;
    std::vector<std::string> Tags;
};