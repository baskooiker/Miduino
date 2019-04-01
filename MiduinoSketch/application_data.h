#pragma once

#include "bass_settings.h"
#include "defs.h"
#include "harmony_struct.h"
#include "modulators.h"
#include "tanzbar_settings.h"
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

    Tanzbar tanzbar;

    BassSettings bass_settings;
    BassDubSettings bass_dub_settings;
    MonoSettings mono_settings;
    MonoDubSettings mono_dub_settings;
    Drone drone;

    PolySettings poly_settings;
    LeadSettings lead_settings;

    UiState ui_state;

    ApplicationData():
        tanzbar(modulators, harmony, time),
        bass_settings(fugue_settings, harmony, time),
        bass_dub_settings(bass_settings, fugue_settings, harmony, time),
        mono_settings(fugue_settings, harmony, time),
        mono_dub_settings(mono_settings, fugue_settings, harmony, time),
        poly_settings(harmony, time),
        lead_settings(harmony, time),
        drone(harmony, time)
    {
        tanzbar.storage.set_channel(MIDI_CHANNEL_TANZBAR);
        mono_settings.storage.set_channel(MIDI_CHANNEL_MONO);
        mono_dub_settings.storage.set_channel(MIDI_CHANNEL_MONO_2);

        bass_settings.storage.set_channel(MIDI_CHANNEL_BASS);
        bass_settings.storage.set_channel(MIDI_CHANNEL_ROCKET, -24);

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

    void play_all()
    {
        InstrumentBase* instruments[16];
        uint8_t length = 0;
        get_instrument_ptrs(instruments, length);

        for (int i = 0; i < length; i++)
        {
            instruments[i]->play();
        }
    }

    void process_active_notes()
    {
        this->tanzbar.storage.process_active_notes();
        this->bass_settings.storage.process_active_notes();
        this->bass_dub_settings.storage.process_active_notes();
        this->drone.storage.process_active_notes();
        this->mono_settings.storage.process_active_notes();
        this->mono_dub_settings.storage.process_active_notes();
        poly_settings.storage.process_active_notes();
        lead_settings.storage.process_active_notes();
    }

    void randomize_all()
    {
        this->harmony.randomize();

        this->fugue_settings.randomize_fugue();

        this->tanzbar.randomize_tanzbar();
        this->bass_settings.randomize();
        this->drone.randomize();
        this->bass_dub_settings.randomize();

        this->mono_settings.randomize();
        this->mono_dub_settings.randomize_mono_dub();

        this->tanzbar.randomize_tanzbar_sound();

        poly_settings.randomize();
        lead_settings.randomize();
    }

    void set_fugue()
    {
        this->fugue_settings.randomize_fugue();
        this->tanzbar.kill_low = true;
        this->tanzbar.kill_mid = true;
        this->tanzbar.kill_perc = true;
        this->tanzbar.kill_hats = true;

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
        ptrs[length++] = (InstrumentBase*)&this->tanzbar;
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
