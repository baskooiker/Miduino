#pragma once

#include "instruments/instrument_rack.h"
#include "instruments/neutron/neutron_mono.h"

namespace Vleerhond {
class Neutron : public InstrumentRack {
   private:
    NeutronMono sequence_mono;
    NeutronMono arp_mono;
    NeutronMono const_mono;
    NeutronMono closest_mono;

   public:
    Neutron(HarmonyStruct& harmony, Modulators& modulators, TimeStruct& time);
    void randomize();
    void setArpType(ArpType type);
};
}  // namespace Vleerhond
