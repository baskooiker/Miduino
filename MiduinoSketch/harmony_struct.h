#pragma once

#include "defs.h"
#include "scale.h"

class HarmonyStruct {
public:
    CvPatternAB const_pattern;
    CvPatternAB high_pattern;
    HarmonyType type;
    Scale scale;

    HarmonyStruct()
    {
        type = HarmonyType::HarmonyConst;
    }

    ~HarmonyStruct() {};
};
