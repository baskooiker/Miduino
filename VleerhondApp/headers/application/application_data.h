#pragma once

#include <algorithm>

#include "core/defs.h"
#include "harmony/harmony_struct.h"
#include "patterns/modulators.h"
#include "parameters.h"

#include "tanzbar.h"
#include "mfb_522.h"

#include "303_acid_bass.h"
#include "moog_bass.h"
#include "acid_bass.h"
#include "rocket_mono.h"
#include "neutron_mono.h"

#include "vermona_mono.h"
#include "vermona_mono_dub.h"
#include "drone.h"
#include "fugue_player.h"

namespace Vleerhond
{
    class ApplicationData
    {
    public:
        TimeStruct time;
        HarmonyStruct harmony;

        Fugue fugue;
        Modulators modulators;

        TB303Bass tb303_bass;
        MoogBass moog_bass;
        AcidBass acid_bass;
        VermonaMono mono;
        VermonaMonoDub mono_dub;
        NeutronMono neutron_mono;
        Drone drone;

        FuguePlayer fugue_vermona_2;
        FuguePlayer fugue_vermona_3;
        FuguePlayer fugue_vermona_4;

        Mfb522 mfb_522;
        Tanzbar tanzbar;

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
        void set_regular();
        void set_fugue();
    };
}