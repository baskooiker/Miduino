#pragma once
#include "application_data.h"

#include <algorithm>

#include "core/defs.h"
#include "harmony/harmony_struct.h"
#include "patterns/modulators.h"
#include "utils/utils.h"
#include "parameters.h"

#include "tanzbar.h"
#include "mfb_522.h"

#include "moog_bass.h"

namespace Vleerhond
{
    ApplicationData::ApplicationData() :
        tanzbar(harmony, modulators, time),
        mfb_522(harmony, modulators, time),
        drumstation(harmony, modulators, time),
        moog_bass(modulators, harmony, time),
        neutron(harmony, modulators, time),
        tb303_bass(harmony, time)
    {
        this->randomize_all();
    }

    void ApplicationData::probability_randomize()
    {
        if (Utils::interval_hit(TimeDivision::Four, time.add(TICKS_PER_STEP / 2)))
        {
            for (InstrumentBase* ptr : get_active_instrument())
            {
                ptr->auto_randomize();
            }
        }
    }

    void ApplicationData::play_all()
    {
        for (auto instrument : get_active_instrument())
        {
            instrument->play();
        }

        probability_randomize();
        this->handleUserEvents();
    }

    void ApplicationData::process_active_notes()
    {
        for (auto instrument : get_instrument_ptrs())
        {
            instrument->midi_channel.process_active_notes();
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

    std::vector<InstrumentBase*> ApplicationData::get_instrument_ptrs()
    {
        std::vector<InstrumentBase*> ptrs;

        ptrs.push_back(&this->drumstation);
        ptrs.push_back(&this->tanzbar);

        ptrs.push_back(&this->tb303_bass);
        ptrs.push_back(&this->moog_bass);
        ptrs.push_back(&this->neutron);

        return ptrs;
    }

    std::vector<InstrumentBase*> ApplicationData::get_active_instrument()
    {
        std::vector<InstrumentBase*> instruments = get_instrument_ptrs();
        std::vector<InstrumentBase*> ptrs;
        std::copy_if(instruments.begin(), instruments.end(), std::back_inserter(ptrs),
            [](const InstrumentBase* i) {return i->is_active(); });
        return ptrs;
    }

    std::vector<InstrumentBase*> ApplicationData::get_randomizable_instruments()
    {
        std::vector<InstrumentBase*> ptrs;
        for (auto ptr : get_active_instrument())
        {
            if (ptr->is_randomizable())
            {
                std::vector<InstrumentBase*> v = ptr->get_ptrs();
                ptrs.insert(ptrs.end(), v.begin(), v.end());
            }
        }
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
}