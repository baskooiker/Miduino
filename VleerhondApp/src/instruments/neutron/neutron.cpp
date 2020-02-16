#include "instruments/neutron/neutron.h"

namespace Vleerhond
{
    Neutron::Neutron(HarmonyStruct & harmony, Modulators & modulators, TimeStruct & time)
        : InstrumentRack(time, 0)
        , const_mono(harmony, modulators, time)
        , closest_mono(harmony, modulators, time)
        , sequence_mono(harmony, modulators, time)
        , arp_mono(harmony, modulators, time)
    {
        this->instruments.push_back(&const_mono);
        this->instruments.push_back(&closest_mono);
        this->instruments.push_back(&sequence_mono);
        this->instruments.push_back(&arp_mono);
    }

    void Neutron::randomize()
    {
        // const
        {
            const_mono.total_randomize();
            const_mono.set_style(MonoStyle::MonoPolyRhythm);
            const_mono.set_pitch_mode(MonoPitchMode::SEQUENCE);
            const_mono.set_const_sequence();
        }
        // closest
        {
            closest_mono.total_randomize();
            closest_mono.set_slow_rhythm();
            closest_mono.set_style(MonoStyle::MonoPolyRhythm);
            closest_mono.set_arp_type(ArpType::CLOSEST_EXC);
            closest_mono.set_pitch_mode(MonoPitchMode::ARP);
            closest_mono.set_arp_range(12);
        }
        // sequence
        {
            sequence_mono.total_randomize();
            sequence_mono.set_style(MonoStyle::MonoPolyRhythm);
            sequence_mono.set_pitch_mode(MonoPitchMode::SEQUENCE);
        }
        // arp
        {
            arp_mono.total_randomize();
            arp_mono.set_style(MonoStyle::MonoSixteenths);
            arp_mono.set_pitch_mode(MonoPitchMode::ARP);
            arp_mono.disableSlides();
        }
    }
}
