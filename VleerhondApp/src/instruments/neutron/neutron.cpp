#include "instruments/neutron/neutron.h"

namespace Vleerhond {
Neutron::Neutron(
    HarmonyStruct& harmony, Modulators& modulators, TimeStruct& time)
    : InstrumentRack(time),
      sequence_mono(harmony, modulators, time),
      arp_mono(harmony, modulators, time),
      const_mono(harmony, modulators, time),
      closest_mono(harmony, modulators, time) {
    this->instruments.push_back(&const_mono);
    this->instruments.push_back(&closest_mono);
    this->instruments.push_back(&sequence_mono);
    this->instruments.push_back(&arp_mono);

    for (NeutronMono* neutron :
         {&const_mono, &closest_mono, &sequence_mono, &arp_mono}) {
        neutron->settings.min_pitch = 40;
    }
}

void Neutron::randomize() {
    // const
    {
        const_mono.totalRandomize();
        const_mono.setStyle(MonoStyle::MonoPolyRhythm);
        const_mono.setPitchMode(MonoPitchMode::SEQUENCE);
        const_mono.setConstSequence();
    }
    // closest
    {
        closest_mono.totalRandomize();
        closest_mono.setSlowRhythm();
        closest_mono.setStyle(MonoStyle::MonoPolyRhythm);
        closest_mono.setArpType(ArpType::CLOSEST_EXC);
        closest_mono.setPitchMode(MonoPitchMode::ARP);
        closest_mono.setArpRange(12);
    }
    // sequence
    {
        sequence_mono.totalRandomize();
        sequence_mono.setStyle(MonoStyle::MonoPolyRhythm);
        sequence_mono.setPitchMode(MonoPitchMode::SEQUENCE);
    }
    // arp
    {
        arp_mono.totalRandomize();
        arp_mono.setStyle(MonoStyle::MonoSixteenths);
        arp_mono.setPitchMode(MonoPitchMode::ARP);
        arp_mono.disableSlides();
    }
}
void Neutron::setArpType(ArpType type) { arp_mono.setArpType(type); }
}  // namespace Vleerhond
