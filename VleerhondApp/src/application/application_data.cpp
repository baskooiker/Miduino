#pragma once

#include <algorithm>

#include "application_data.h"

#include <algorithm>

#include "utils/utils.h"

namespace Vleerhond
{
    ApplicationData::ApplicationData() :
        tanzbar(harmony, modulators, time),
        drumstation(harmony, modulators, time),
        moog_bass(modulators, harmony, time),
        neutron(harmony, modulators, time),
        mam_mb33(harmony, modulators, time)
    {
        this->randomize_all();
    }

    void ApplicationData::probability_randomize()
    {
        if (Utils::interval_hit(TimeDivision::Four, time.add(TICKS_PER_STEP / 2)))
        {
            for (InstrumentBase* ptr : get_instrument_ptrs())
            {
                ptr->auto_randomize();
            }
        }
    }

    void ApplicationData::play_all()
    {
        for (auto instrument : get_instrument_ptrs())
        {
            instrument->play();
        }

        probability_randomize();
        this->handleUserEvents();
    }

    void ApplicationData::process_active_notes()
    {
        for (InstrumentBase* instrument : get_instrument_ptrs())
        {
            instrument->getChannel()->process_active_notes();
        }
    }

    void ApplicationData::randomize_all()
    {
        auto inst_ptrs = get_instrument_ptrs();
        std::random_shuffle(inst_ptrs.begin(), inst_ptrs.end());
        for (auto& value : inst_ptrs)
        {
            value->randomize();
        }

        this->harmony.randomize();
    }

    void ApplicationData::processNoteEvents()
    {
        for (auto instrument : get_instrument_ptrs())
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
        for (auto instrument : get_instrument_ptrs())
        {
            instrument->updatePedalState();
        }
    }

    std::vector<InstrumentBase*> ApplicationData::get_instrument_ptrs()
    {
        std::vector<InstrumentBase*> ptrs;

        ptrs.push_back(&this->drumstation);
        ptrs.push_back(&this->tanzbar);

        //ptrs.push_back(&this->tb303_bass);
        ptrs.push_back(&this->mam_mb33);
        ptrs.push_back(&this->moog_bass);
        ptrs.push_back(&this->neutron);

        return ptrs;
    }

    void ApplicationData::stop_all()
    {
        for (auto instrument : get_instrument_ptrs())
        {
            instrument->stop_notes();
        }
    }

    void ApplicationData::addEvent(std::shared_ptr<UserEvent> user_event)
    {
        // TODO: filter on event type
        this->user_events.push_back(user_event);
    }

    void ApplicationData::connect()
    {
        this->tanzbar.setChannel(std::make_shared<MidiChannel>(MIDI_CHANNEL_TANZBAR, MIDI_CC_CHANNEL_TANZBAR, MIDI_A_NAME));
        this->drumstation.setChannel(std::make_shared<MidiChannel>(MIDI_CHANNEL_DS, MIDI_B_NAME));

        this->neutron.setChannel(std::make_shared<MidiChannel>(MIDI_CHANNEL_NEUTRON, MIDI_A_NAME));
        this->mam_mb33.setChannel(std::make_shared<MidiChannel>(MIDI_CHANNEL_TB303, MIDI_A_NAME));
        this->moog_bass.setChannel(std::make_shared<MidiChannel>(MIDI_CHANNEL_MINITAUR, MIDI_B_NAME));
    }
}