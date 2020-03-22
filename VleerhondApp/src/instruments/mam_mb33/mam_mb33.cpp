#include "instruments/mam_mb33/mam_mb33.h"

namespace Vleerhond
{
    MamMb33::MamMb33(HarmonyStruct & harmony, Modulators & modulators, TimeStruct & time)
        : InstrumentRack(time)
        , drop_bass(harmony, time)
        , const_bass(harmony, time)
        , acid_bass(harmony, time)
        , arp(harmony, modulators, time)
    {
        instruments.push_back(&drop_bass);
        instruments.push_back(&const_bass);
        instruments.push_back(&acid_bass);
        instruments.push_back(&arp);
    }

    void MamMb33::randomize()
    {
        // drop bass
        {
            drop_bass.totalRandomize();
            drop_bass.randomizeDrop();
        }

        // const bass
        {
            const_bass.totalRandomize();
            const_bass.note_range_value = 0;
        }

        // acid bass
        {
            acid_bass.totalRandomize();
        }

        // Arp
        {
            arp.totalRandomize();
            arp.setStyle(MonoStyle::MonoSixteenths);
            arp.setPitchMode(MonoPitchMode::ARP);
            arp.disableSlides();
        }
    }
}
