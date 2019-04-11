#ifndef COMMON_H
#define COMMON_H

#include "componentmodel/r_componentbase.h"

constexpr auto GROUP_TYPE_GROUPA = "groupA";
constexpr auto GROUP_TYPE_GROUPB = "groupB";

static riaps::groups::GroupId groupIdA{GROUP_TYPE_GROUPA, "A"};
static riaps::groups::GroupId groupIdB{GROUP_TYPE_GROUPB, "B"};

#endif