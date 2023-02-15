#pragma once

#include "instrument_group.h"
#include "tanzbar_cb.h"
#include "tanzbar_cl.h"
#include "tanzbar_cp.h"
#include "tanzbar_cy.h"
#include "tanzbar_hats.h"
#include "tanzbar_lo.h"
#include "tanzbar_ma.h"
#include "tanzbar_rs.h"
#include "tanzbar_sd.h"
#include "tanzbar_tom.h"

namespace Vleerhond {
class Tanzbar : public InstrumentGroup {
   public:
    TanzbarLo tanzbar_lo;
    TanzbarMid tanzbar_mid;
    TanzbarRs tanzbar_rs;
    TanzbarCp tanzbar_cp;
    TanzbarTom tanzbar_tom;
    TanzbarCb tanzbar_cb;
    TanzbarCl tanzbar_cl;
    TanzbarHats tanzbar_hats;
    TanzbarCy tanzbar_cy;
    TanzbarMa tanzbar_ma;

    Tanzbar(HarmonyStruct& harmony, Modulators& modulators, TimeStruct& time);

    std::vector<InstrumentBase*> getLow() { return {&tanzbar_lo}; }

    std::vector<InstrumentBase*> getMid() {
        return {&tanzbar_mid, &tanzbar_rs, &tanzbar_cp,
                &tanzbar_tom, &tanzbar_cb, &tanzbar_cl};
    }

    std::vector<InstrumentBase*> getHigh() {
        return {&tanzbar_hats, &tanzbar_cy, &tanzbar_ma};
    }

    void killLow(const bool kill) {
        for (auto p : getLow()) {
            p->kill(kill);
        }
    }

    void killMid(const bool kill) {
        for (auto p : getMid()) {
            p->kill(kill);
        }
    }

    void killHigh(const bool kill) {
        for (auto p : getHigh()) {
            p->kill(kill);
        }
    }
};
}  // namespace Vleerhond