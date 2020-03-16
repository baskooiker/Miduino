#pragma once

#include "application/ui.h"
#include "application/user_event.h"
#include "core/defs.h"
#include "harmony/harmony_struct.h"
#include "patterns/modulators.h"

#include "tanzbar.h"
#include "instruments/drumstation/drumstation.h"

#include "instruments/minitaur/minitaur.h"
#include "instruments/neutron/neutron.h"
#include "instruments/mam_mb33/mam_mb33.h"

namespace Vleerhond
{
    class ApplicationData
    {
    private:
        std::vector<std::shared_ptr<UserEvent>> user_events;

    public:
        TimeStruct time;
        HarmonyStruct harmony;

        Modulators modulators;

        Minitaur minitaur;
        Neutron neutron;
        MamMb33 mam_mb33;

        Tanzbar tanzbar;
        DrumStation drumstation;

        UiState ui_state;

        ApplicationData();
        void probability_randomize();
        void play_all();
        void process_active_notes();
        void randomize_all();
        void processNoteEvents();
        void handleUserEvents();
        void updatePedalState();
        std::vector<InstrumentBase*> get_instrument_ptrs();
        void stop_all();
        void addEvent(std::shared_ptr<UserEvent> user_event);
        void connect();
    };
}