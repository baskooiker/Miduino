#pragma once

#include "instrument_group.h"

#include "tanzbar_lo.h"
#include "tanzbar_sd.h"
#include "tanzbar_cp.h"
#include "tanzbar_tom.h"
#include "tanzbar_cb.h"
#include "tanzbar_cl.h"
#include "tanzbar_ma.h"
#include "tanzbar_hats.h"
#include "tanzbar_cy.h"

namespace Vleerhond
{
class Tanzbar : public InstrumentGroup
{
public:

    TanzbarLo tanzbar_lo;
    TanzbarMid tanzbar_mid;
    TanzbarCp tanzbar_cp;
    TanzbarTom tanzbar_tom;
    TanzbarCb tanzbar_cb;
    TanzbarCl tanzbar_cl;
    TanzbarHats tanzbar_hats;
    TanzbarCy tanzbar_cy;
    TanzbarMa tanzbar_ma;

    Tanzbar(HarmonyStruct& harmony, Modulators& modulators, TimeStruct& time) :
        InstrumentGroup(harmony, time),
        tanzbar_lo(modulators, time),
        tanzbar_mid(modulators, time),
        tanzbar_cp(modulators, time),
        tanzbar_tom(modulators, time),
        tanzbar_cb(modulators, time),
        tanzbar_cl(modulators, time),
        tanzbar_ma(modulators, time),
        tanzbar_cy(modulators, time),
        tanzbar_hats(modulators, time)
    {
        instruments.push_back(&tanzbar_lo);
        instruments.push_back(&tanzbar_mid);
        instruments.push_back(&tanzbar_cp);
        instruments.push_back(&tanzbar_tom);
        instruments.push_back(&tanzbar_cb);
        instruments.push_back(&tanzbar_cl);
        instruments.push_back(&tanzbar_hats);
        instruments.push_back(&tanzbar_cy);
        instruments.push_back(&tanzbar_ma);
    }
};
}