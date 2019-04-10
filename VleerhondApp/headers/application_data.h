#pragma once

#include <algorithm>

#include "bass_settings.h"
#include "defs.h"
#include "harmony_struct.h"
#include "modulators.h"
#include "tanzbar_settings.h"
#include "tanzbar_lo.h"
#include "tanzbar_mid.h"
#include "tanzbar_perc.h"
#include "tanzbar_hi.h"
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

    TanzbarModulators tanzbar_modulators;
    TanzbarTimeSettings tanzbar_time;

    TanzbarLo tanzbar_lo;
    TanzbarMid tanzbar_mid;
    TanzbarPerc tanzbar_perc;
    TanzbarHi tanzbar_hi;

    BassSettings bass_settings;
    BassDubSettings bass_dub_settings;
    MonoSettings mono_settings;
    MonoDubSettings mono_dub_settings;
    Drone drone;

    PolySettings poly_settings;
    LeadSettings lead_settings;

    UiState ui_state;

    ApplicationData():
        tanzbar_modulators(modulators),
        tanzbar_lo(tanzbar_modulators, tanzbar_time, time),
        tanzbar_mid(tanzbar_modulators, tanzbar_time, time),
        tanzbar_perc(tanzbar_modulators, tanzbar_time, time),
        tanzbar_hi(tanzbar_modulators, tanzbar_time, time),
        bass_settings(fugue_settings, harmony, time),
        bass_dub_settings(bass_settings, fugue_settings, harmony, time),
        mono_settings(fugue_settings, harmony, time),
        mono_dub_settings(mono_settings, fugue_settings, harmony, time),
        poly_settings(harmony, time),
        lead_settings(harmony, time),
        drone(harmony, time)
    {
        tanzbar_lo.storage.set_channel(MIDI_CHANNEL_TANZBAR);
        tanzbar_mid.storage.set_channel(MIDI_CHANNEL_TANZBAR);
        tanzbar_perc.storage.set_channel(MIDI_CHANNEL_TANZBAR);
        tanzbar_hi.storage.set_channel(MIDI_CHANNEL_TANZBAR);

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
        if (!Utils::interval_hit(TimeDivision::Four, time.add(TICKS_PER_STEP)))
        {
            return;
        }

        InstrumentBase* instruments[16];
        uint8_t length = 0;
        get_instrument_ptrs(instruments, length);

        InstrumentBase* latest_randomize_ptr = instruments[0];
        for (int i = 0; i < length; i++)
        {
            if (instruments[i]->randomized_time() < latest_randomize_ptr->randomized_time())
            {
                latest_randomize_ptr = instruments[i];
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
        InstrumentBase* instruments[16];
        uint8_t length = 0;
        get_instrument_ptrs(instruments, length);
        for (int i = 0; i < length; i++)
        {
            instruments[i]->play();
        }

        probability_randomize();
    }

    void process_active_notes()
    {
        InstrumentBase* instruments[16];
        uint8_t length = 0;
        get_instrument_ptrs(instruments, length);
        for (int i = 0; i < length; i++)
        {
            instruments[i]->storage.process_active_notes();
        }
    }

    void randomize_all()
    {
        InstrumentBase* instruments[16];
        uint8_t length = 0;
        get_instrument_ptrs(instruments, length);
        std::vector<InstrumentBase*> inst_ptrs(instruments, instruments + length);
        std::random_shuffle(inst_ptrs.begin(), inst_ptrs.end());
        ofLogNotice("application_data", "%d == %d", length, inst_ptrs.size());
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
        this->tanzbar_mid.kill = true;
        this->tanzbar_perc.kill = true;
        this->tanzbar_hi.kill = true;

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
        InstrumentBase* instruments[16];
        uint8_t length = 0;
        get_instrument_ptrs(instruments, length);
        for (int i = 0; i < length; i++)
        {
            instruments[i]->process_events();
        }
    }

    void get_instrument_ptrs(InstrumentBase** ptrs, uint8_t& length)
    {
        // TODO: Add all instruments
        length = 0;
        ptrs[length++] = (InstrumentBase*)&this->tanzbar_lo;
        ptrs[length++] = (InstrumentBase*)&this->tanzbar_mid;
        ptrs[length++] = (InstrumentBase*)&this->tanzbar_perc;
        ptrs[length++] = (InstrumentBase*)&this->tanzbar_hi;
        ptrs[length++] = (InstrumentBase*)&this->bass_settings;
        ptrs[length++] = (InstrumentBase*)&this->bass_dub_settings;
        ptrs[length++] = (InstrumentBase*)&this->mono_settings;
        ptrs[length++] = (InstrumentBase*)&this->mono_dub_settings;
        ptrs[length++] = (InstrumentBase*)&this->poly_settings;
        ptrs[length++] = (InstrumentBase*)&this->lead_settings;
        ptrs[length++] = (InstrumentBase*)&this->drone;
    }

    void stop_all()
    {

        InstrumentBase* instruments[16];
        uint8_t length = 0;
        get_instrument_ptrs(instruments, length);
        for (int i = 0; i < length; i++)
        {
            instruments[i]->stop_notes();
        }
    }
};
