#pragma once

#include "defs.h"
#include "scale.h"

class HarmonyStruct {
public:
    CvPattern16 const_pattern;
    CvPattern16 high_pattern;
    HarmonyType type;
    Scale scale;

    HarmonyStruct()
    {
        type = HarmonyType::HarmonyConst;
    }

    ~HarmonyStruct() {};
};
