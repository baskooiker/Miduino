#pragma once

#include <algorithm>

#include "application/ui.h"
#include "core/defs.h"
#include "harmony/harmony_struct.h"
#include "patterns/modulators.h"
#include "parameters.h"

#include "tanzbar.h"
#include "mfb_522.h"
#include "instruments/drumstation/drumstation.h"

#include "303_acid_bass.h"
#include "moog_bass.h"
#include "neutron_mono.h"

namespace Vleerhond
{
    class ApplicationData
    {
    public:
        TimeStruct time;
        HarmonyStruct harmony;

        Modulators modulators;

        TB303Bass tb303_bass;
        MoogBass moog_bass;
        NeutronMono neutron_mono;

        Mfb522 mfb_522;
        Tanzbar tanzbar;
        DrumStation drumstation;

        UiState ui_state;

        ApplicationData();
        void probability_randomize();
        void play_all();
        void process_active_notes();
        void randomize_all();
        void process_events();
        std::vector<InstrumentBase*> get_instrument_ptrs();
        std::vector<InstrumentBase*> get_active_instrument();
        std::vector<InstrumentBase*> get_randomizable_instruments();
        void stop_all();
    };
}