#include "tanzbar.h"

#include "instrument_group.h"
#include "tanzbar_cb.h"
#include "tanzbar_cl.h"
#include "tanzbar_cp.h"
#include "tanzbar_cy.h"
#include "tanzbar_hats.h"
#include "tanzbar_lo.h"
#include "tanzbar_ma.h"
#include "tanzbar_sd.h"
#include "tanzbar_tom.h"

namespace Vleerhond {
Tanzbar::Tanzbar(
    HarmonyStruct& harmony, Modulators& modulators, TimeStruct& time)
    : InstrumentGroup(harmony, time),
      tanzbar_lo(modulators, time),
      tanzbar_mid(modulators, time),
      tanzbar_rs(modulators, time),
      tanzbar_cp(modulators, time),
      tanzbar_tom(modulators, time),
      tanzbar_cb(modulators, time),
      tanzbar_cl(modulators, time),
      tanzbar_hats(modulators, time),
      tanzbar_cy(modulators, time),
      tanzbar_ma(modulators, time) {
    instruments.push_back(&tanzbar_lo);
    instruments.push_back(&tanzbar_mid);
    instruments.push_back(&tanzbar_rs);
    instruments.push_back(&tanzbar_cp);
    instruments.push_back(&tanzbar_tom);
    instruments.push_back(&tanzbar_cb);
    instruments.push_back(&tanzbar_cl);
    instruments.push_back(&tanzbar_hats);
    instruments.push_back(&tanzbar_cy);
    instruments.push_back(&tanzbar_ma);
}
}  // namespace Vleerhond