#pragma once
#include "application_data.h"

#include <algorithm>

#include "defs.h"
#include "harmony_struct.h"
#include "modulators.h"
#include "parameters.h"

#include "tanzbar.h"
#include "mfb_522.h"

#include "moog_bass.h"
#include "acid_bass.h"

#include "vermona_mono.h"
#include "vermona_mono_dub.h"
#include "drone.h"
#include "fugue_player.h"

namespace Vleerhond
{
    ApplicationData::ApplicationData() :
        tanzbar(harmony, modulators, time),
        mfb_522(harmony, modulators, time),
        moog_bass(modulators, harmony, time),
        acid_bass(harmony, time),
        mono(harmony, modulators, time),
        mono_dub(mono, harmony, time, modulators),
        drone(harmony, time),
        fugue_vermona_2(harmony, time, fugue, MIDI_CHANNEL_BASS_DUB),
        fugue_vermona_3(harmony, time, fugue, MIDI_CHANNEL_MONO),
        fugue_vermona_4(harmony, time, fugue, MIDI_CHANNEL_MONO_2),
        neutron_mono(harmony, modulators, time, MIDI_CHANNEL_ROCKET),
        tb303_bass(harmony, time)
    {
        this->randomize_all();
        set_regular();
    }

    void ApplicationData::probability_randomize()
    {
        if (!Utils::interval_hit(TimeDivision::Four, time.add(TICKS_PER_STEP / 2)))
        {
            return;
        }

        std::vector<InstrumentBase*> ptrs = get_randomizable_instruments();
        InstrumentBase* latest_randomize_ptr = ptrs.at(0);
        for (auto instrument : ptrs)
        {
            if (instrument->randomized_time() < latest_randomize_ptr->randomized_time())
            {
                latest_randomize_ptr = instrument;
            }
        }

        uint64_t diff = millis() - latest_randomize_ptr->randomized_time();
        if (diff > 60000)
        {
            if (Rand::distribution(diff / 1000, 240) == 0)
            {
                latest_randomize_ptr->randomize();
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
        this->fugue.randomize_fugue();
    }

    void ApplicationData::process_events()
    {
        for (auto instrument : get_instrument_ptrs())
        {
            instrument->process_events();
        }
    }

    std::vector<InstrumentBase*> ApplicationData::get_instrument_ptrs()
    {
        std::vector<InstrumentBase*> ptrs;

        ptrs.push_back(&this->tanzbar);
        ptrs.push_back(&this->mfb_522);

        ptrs.push_back(&this->tb303_bass);
        ptrs.push_back(&this->moog_bass);
        ptrs.push_back(&neutron_mono);

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

    void ApplicationData::set_regular()
    {
        acid_bass.set_active(true);
        drone.set_active(true);
        mono.set_active(true);
        mono_dub.set_active(true);

        fugue_vermona_2.set_active(false);
        fugue_vermona_3.set_active(false);
        fugue_vermona_4.set_active(false);
    }

    void ApplicationData::set_fugue()
    {
        //fugue_vermona_1.set_active(true);
        fugue_vermona_2.set_active(true);
        fugue_vermona_3.set_active(true);
        fugue_vermona_4.set_active(true);

        //acid_bass.set_active(false);
        drone.set_active(false);
        mono.set_active(false);
        mono_dub.set_active(false);
    }
}