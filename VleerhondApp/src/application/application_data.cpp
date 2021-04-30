#include "application_data.h"

#include <algorithm>

#include "core/defs.h"
#include "utils/utils.h"

namespace Vleerhond
{
    ApplicationData::ApplicationData() :
        tanzbar(harmony, modulators, time),
        minitaur(modulators, harmony, time),
        neutron(harmony, modulators, time),
        mam_mb33(harmony, modulators, time),
        vermona(harmony, time),
        eurorack_triggers(time)
    {
        this->randomizeAll();
    }

    void ApplicationData::probabilityRandomize()
    {
        if (Utils::intervalHit(TimeDivision::Four, time.add(TICKS_PER_STEP / 2)))
        {
            for (InstrumentBase* ptr : getInstrumentPtrs())
            {
                ptr->autoRandomize();
            }
        }
    }

    void ApplicationData::playAll()
    {
        for (auto instrument : getInstrumentPtrs())
        {
            instrument->play();
        }

        probabilityRandomize();
        this->handleUserEvents();
    }

    void ApplicationData::processActiveNotes()
    {
        for (InstrumentBase* instrument : getInstrumentPtrs())
        {
            instrument->getChannel()->processActiveNotes();
        }
    }

    void ApplicationData::randomizeAll()
    {
        for (InstrumentBase* value : getInstrumentPtrs())
        {
            value->randomize();
        }

        this->harmony.randomize();
    }

    void ApplicationData::processNoteEvents()
    {
        for (auto instrument : getInstrumentPtrs())
        {
            instrument->processNoteEvents();
        }
    }

    void ApplicationData::handleUserEvents()
    {
        for (auto event : this->user_events)
        {
            if (!event->isHandled())
            {
                event->handle();
            }
        }

        this->user_events.erase(std::remove_if(
            this->user_events.begin(), 
            this->user_events.end(), 
            [](std::shared_ptr<UserEvent>& user_event) { return user_event->isHandled(); }
        ), this->user_events.end()
        );
    }

    void ApplicationData::updatePedalState()
    {
        for (auto instrument : getInstrumentPtrs())
        {
            instrument->updatePedalState();
        }
    }

    std::vector<InstrumentBase*> ApplicationData::getInstrumentPtrs()
    {
        std::vector<InstrumentBase*> ptrs;

        ptrs.push_back(&this->tanzbar);

        ptrs.push_back(&this->mam_mb33);
        ptrs.push_back(&this->minitaur);
        ptrs.push_back(&this->neutron);
        ptrs.push_back(&this->vermona);
        ptrs.push_back(&eurorack_triggers);

        return ptrs;
    }

    void ApplicationData::stopAll()
    {
        for (auto instrument : getInstrumentPtrs())
        {
            instrument->stopNotes();
        }
    }

    void ApplicationData::addEvent(std::shared_ptr<UserEvent> user_event)
    {
        this->user_events.push_back(user_event);
    }

    void ApplicationData::connect()
    {
        this->tanzbar.setChannel(std::make_shared<MidiChannel>(MIDI_CHANNEL_TANZBAR, MIDI_CC_CHANNEL_TANZBAR, MIDI_C_NAME));
         
        this->neutron.setChannel(std::make_shared<MidiChannel>(MIDI_CHANNEL_NEUTRON, MIDI_A_NAME));

        //this->mam_mb33.setChannel(std::make_shared<MidiChannel>(MIDI_CHANNEL_TB303, MIDI_A_NAME));
        //this->minitaur.setChannel(std::make_shared<MidiChannel>(12, MIDI_A_NAME));
        this->mam_mb33.setChannel(std::make_shared<MidiChannel>(MIDI_CHANNEL_TB303, MIDI_BSP_OUT));
        this->minitaur.setChannel(std::make_shared<MidiChannel>(12, MIDI_BSP_OUT));
        this->eurorack_triggers.setChannel(std::make_shared<MidiChannel>(11, MIDI_BSP_OUT));

        this->vermona.setChannel(std::make_shared<MidiChannel>(7, VERMONA_CONTROL_CHANNEL, MIDI_B_NAME));
    }
}