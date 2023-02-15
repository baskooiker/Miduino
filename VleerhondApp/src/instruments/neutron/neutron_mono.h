#pragma once

#include "midi/midi_io.h"
#include "mono.h"
#include "patterns/modulators.h"

namespace Vleerhond {
class NeutronMono : public Mono {
   private:
    const std::vector<uint8_t> sysex_header = {0xF0, 0x00, 0x20,
                                               0x32, 0x28, 0x7F};

   public:
    NeutronMono(
        HarmonyStruct& harmony, Modulators& modulators, TimeStruct& time);

    void sendSysexMessage(const std::vector<uint8_t> msg);

    /* 0-24 is autoglide range -12/12. 12 is no autoglide
     */
    virtual void setAutoglide(const uint8_t value);

    virtual void setShapeBlend(const bool value);

    /*
     * 0 - disabled
     * 1 - aftertouch
     * 2 - modwheel
     * 3 - velocity
     **/
    virtual void setVcfModSource(const uint8_t value);

    /*
     * 0-3F where 0 is the minimum & 3F(63dec) is the maximum (100%)
     **/
    virtual void setVcfModDepth(const uint8_t value);

    virtual void randomize();
    virtual void autoRandomize();
};
}  // namespace Vleerhond
