#pragma once

#include <algorithm>

#include "bass_settings.h"
#include "defs.h"
#include "harmony_struct.h"
#include "modulators.h"
#include "tanzbar_settings.h"
#include "tanzbar_lo.h"

#include "tanzbar_mid.h"
#include "tanzbar_cp.h"

#include "tanzbar_tom.h"
#include "tanzbar_cb.h"
#include "tanzbar_cl.h"

#include "tanzbar_ma.h"
#include "tanzbar_hats.h"
#include "tanzbar_cy.h"
#include "bass_dub_settings.h"
#include "mono_settings.h"
#include "mono_dub_settings.h"
#include "poly.h"
#include "lead.h"
#include "drone.h"

class ApplicationData 
{
public:
    TimeStruct time;
    HarmonyStruct harmony;

    FugueSettings fugue_settings;
    Modulators modulators;

    TanzbarLo tanzbar_lo;

    TanzbarMid tanzbar_mid;
    TanzbarCp tanzbar_cp;

    TanzbarTom tanzbar_tom;
    TanzbarCb tanzbar_cb;
    TanzbarCl tanzbar_cl;

    TanzbarHats tanzbar_hats;
    TanzbarCy tanzbar_cy;
    TanzbarMa tanzbar_ma;

    BassSettings bass_settings;
    BassDubSettings bass_dub_settings;
    MonoSettings mono_settings;
    MonoDubSettings mono_dub_settings;
    Drone drone;

    PolySettings poly_settings;
    LeadSettings lead_settings;

    UiState ui_state;

    ApplicationData():
        tanzbar_lo(modulators, time),

        tanzbar_mid(modulators, time),
        tanzbar_cp(modulators, time),

        tanzbar_tom(modulators, time),
        tanzbar_cb(modulators, time),
        tanzbar_cl(modulators, time),

        tanzbar_ma(modulators, time),
        tanzbar_cy(modulators, time),
        tanzbar_hats(modulators, time),
        bass_settings(fugue_settings, harmony, time),
        bass_dub_settings(bass_settings, fugue_settings, harmony, time),
        mono_settings(fugue_settings, harmony, time),
        mono_dub_settings(mono_settings, fugue_settings, harmony, time),
        poly_settings(harmony, time),
        lead_settings(harmony, time),
        drone(harmony, time)
    {
        mono_settings.storage.set_channel(MIDI_CHANNEL_MONO);
        mono_dub_settings.storage.set_channel(MIDI_CHANNEL_MONO_2);

        bass_settings.storage.set_channel(MIDI_CHANNEL_BASS, 24);
        bass_settings.storage.set_channel(MIDI_CHANNEL_ROCKET);

        drone.storage.set_channel(MIDI_CHANNEL_BASS_DUB);

        //bass_dub_settings.storage.set_channel(MIDI_CHANNEL_BASS_DUB);

        this->poly_settings.storage.set_channel(MIDI_CHANNEL_POLY);
        this->lead_settings.storage.set_channel(MIDI_CHANNEL_LEAD);

        bass_settings.fugue_id = 0;
        bass_dub_settings.fugue_id = 1;
        mono_settings.fugue_id = 2;
        mono_dub_settings.fugue_id = 3;

        this->randomize_all();
    }

    void probability_randomize()
    {
        if (!Utils::interval_hit(TimeDivision::Four, time.add(TICKS_PER_STEP / 2)))
        {
            return;
        }

        std::vector<InstrumentBase*> ptrs = get_instrument_ptrs();
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
            if(Rand::distribution(diff / 1000, 240) == 0)
            {
                latest_randomize_ptr->randomize();
            }
        }

    }

    void play_all()
    {
        for (auto instrument : get_instrument_ptrs())
        {
            instrument->play();
        }

        probability_randomize();
    }

    void process_active_notes()
    {
        for (auto instrument : get_instrument_ptrs())
        {
            instrument->storage.process_active_notes();
        }
    }

    void randomize_all()
    {
        auto inst_ptrs = get_instrument_ptrs();
        std::random_shuffle(inst_ptrs.begin(), inst_ptrs.end());
        for (auto& value : inst_ptrs)
        {
            value->randomize();
        }

        this->harmony.randomize();
        this->fugue_settings.randomize_fugue();
    }

    void set_fugue()
    {
        this->fugue_settings.randomize_fugue();

        this->tanzbar_lo.kill = true;
        this->tanzbar_hats.kill = true;

        // Set bass
        this->bass_settings.style = BassStyle::BassFugue;
        // Set bass dub
        this->bass_dub_settings.style = BassDubStyle::DubFugue;
        // Set mono
        this->mono_settings.style = MonoStyle::MonoFugue;
        // Set mono dub
        this->mono_dub_settings.dub_style = MonoDubStyle::MonoDubLead;
        this->mono_dub_settings.style = MonoStyle::MonoFugue;
    }

    void process_events()
    {
        for (auto instrument : get_instrument_ptrs())
        {
            instrument->process_events();
        }
    }

    std::vector<InstrumentBase*> get_instrument_ptrs()
    {
        std::vector<InstrumentBase*> ptrs;

        ptrs.push_back(&this->tanzbar_lo);

        ptrs.push_back(&this->tanzbar_mid);
        ptrs.push_back(&this->tanzbar_cp);

        ptrs.push_back(&this->tanzbar_tom);
        ptrs.push_back(&this->tanzbar_cb);
        ptrs.push_back(&this->tanzbar_cl);

        ptrs.push_back(&this->tanzbar_ma);
        ptrs.push_back(&this->tanzbar_hats);
        ptrs.push_back(&this->tanzbar_cy);

        ptrs.push_back(&this->bass_settings);
        ptrs.push_back(&this->bass_dub_settings);
        ptrs.push_back(&this->mono_settings);
        ptrs.push_back(&this->mono_dub_settings);
        ptrs.push_back(&this->poly_settings);
        ptrs.push_back(&this->lead_settings);
        ptrs.push_back(&this->drone);

        return ptrs;
    }

    std::vector<InstrumentBase*> get_randomizable_instruments()
    {
        std::vector<InstrumentBase*> ptrs;
        std::vector<InstrumentBase*> all_ptrs = get_instrument_ptrs();
        for (auto ptr : all_ptrs)
        {
            if (ptr->is_randomizable())
            {
                ptrs.push_back(ptr);
            }
        }
        return ptrs;
    }

    void stop_all()
    {
        for (auto instrument: get_instrument_ptrs())
        {
            instrument->stop_notes();
        }
    }
};
