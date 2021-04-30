#pragma once

#include "application/ui.h"
#include "application/user_event.h"
#include "core/defs.h"
#include "harmony/harmony_struct.h"
#include "patterns/modulators.h"

#include "tanzbar.h"

#include "instruments/minitaur/minitaur.h"
#include "instruments/neutron/neutron.h"
#include "instruments/mam_mb33/mam_mb33.h"
#include "instruments/vermona/vermona.h"
#include "instruments/eurorack/eurorack.hpp"

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
        Vermona vermona;

        Tanzbar tanzbar;
        EurorackTriggers eurorack_triggers;

        UiState ui_state;

        ApplicationData();
        void probabilityRandomize();
        void playAll();
        void processActiveNotes();
        void randomizeAll();
        void processNoteEvents();
        void handleUserEvents();
        void updatePedalState();
        std::vector<InstrumentBase*> getInstrumentPtrs();
        void stopAll();
        void addEvent(std::shared_ptr<UserEvent> user_event);
        void connect();
    };
}