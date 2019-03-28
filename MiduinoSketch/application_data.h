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

class ApplicationData 
{
public:
    TimeStruct time;
    HarmonyStruct harmony;

    FugueSettings fugue_settings;
    Modulators modulators;

    TanzbarSettings tanzbar_settings;

    BassSettings bass_settings;
    BassDubSettings bass_dub_settings;
    MonoSettings mono_settings;
    MonoDubSettings mono_dub_settings;

    PolySettings poly_settings;
    LeadSettings lead_settings;

    UiState ui_state;

    ApplicationData():
        tanzbar_settings(modulators, time),
        bass_settings(fugue_settings, harmony, time),
        bass_dub_settings(bass_settings, fugue_settings, harmony, time),
        mono_settings(fugue_settings, harmony, time),
        mono_dub_settings(mono_settings, fugue_settings, harmony, time),
        poly_settings(harmony, time),
        lead_settings(harmony, time)
    {
        tanzbar_settings.storage.set_channel(MIDI_CHANNEL_TANZBAR);
        mono_settings.storage.set_channel(MIDI_CHANNEL_MONO);
        mono_dub_settings.storage.set_channel(MIDI_CHANNEL_MONO_2);

        bass_settings.storage.set_channel(MIDI_CHANNEL_BASS);
        bass_settings.storage.set_channel(MIDI_CHANNEL_ROCKET, -24);

        bass_dub_settings.storage.set_channel(MIDI_CHANNEL_BASS_DUB);

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
        this->tanzbar_settings.play();
        this->bass_settings.play();
        this->bass_dub_settings.play();
        this->mono_settings.play();
        this->mono_dub_settings.play();

        poly_settings.play();
        lead_settings.play();
    }

    void stop_notes_all_instruments()
    {
        this->tanzbar_settings.storage.stop_notes();
        this->bass_settings.storage.stop_notes();
        this->bass_dub_settings.storage.stop_notes();
        this->mono_settings.storage.stop_notes();
        this->mono_dub_settings.storage.stop_notes();
        poly_settings.storage.stop_notes();
        lead_settings.storage.stop_notes();
    }

    void randomize_all()
    {
        this->harmony.randomize();

        this->fugue_settings.randomize_fugue();

        this->tanzbar_settings.randomize_tanzbar();
        this->bass_settings.randomize();
        this->bass_dub_settings.randomize();

        this->mono_settings.randomize();
        this->mono_dub_settings.randomize_mono_dub();

        this->tanzbar_settings.randomize_tanzbar_sound();

        poly_settings.randomize();
        lead_settings.randomize();
    }

    void set_fugue()
    {
        this->fugue_settings.randomize_fugue();
        this->tanzbar_settings.kill_low = true;
        this->tanzbar_settings.kill_mid = true;
        this->tanzbar_settings.kill_perc = true;
        this->tanzbar_settings.kill_hats = true;

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
        this->tanzbar_settings.storage.process_events();
        this->bass_settings.storage.process_events();
        this->bass_dub_settings.storage.process_events();
        this->mono_settings.storage.process_events();
        this->mono_dub_settings.storage.process_events();
        poly_settings.storage.process_events();
        lead_settings.storage.process_events();
    }

    void get_instrument_ptrs(InstrumentBase** ptrs, uint8_t& length)
    {
        // TODO: Add all instruments
        length = 0;
        ptrs[length++] = &this->tanzbar_settings;
    }
};
