#pragma once

#include "mono_settings.h"
#include "fugue_settings.h"

class MonoDubSettings : public InstrumentBase
{
protected:
    FugueSettings& fugue_settings;
    HarmonyStruct& harmony;
    TimeStruct& time;

public:
    uint8_t variable_pitch_offset;
    MonoDubStyle style;
    MonoSettings mono_settings;
    MonoSettings lead_settings;

    MonoDubSettings(
        MonoSettings& lead_settings_ref,
        FugueSettings& fugue_settings_ref,
        HarmonyStruct& harmony_ref,
        TimeStruct& time_ref):
        lead_settings(lead_settings_ref),
        mono_settings(fugue_settings_ref, harmony_ref, time_ref),
        fugue_settings(fugue_settings_ref),
        harmony(harmony_ref),
        time(time_ref)
    {
        variable_pitch_offset = 0;
        style = MonoDubStyle::MonoDubLead;
    }

    void randomize_mono_dub()
    {
        this->mono_settings.randomize();
    }

    void play()
    {
        if (mono_settings.kill)
            return;

        bool hit = false;
        switch (this->style)
        {
        case MonoDubStyle::MonoDubLead:
            return this->mono_settings.play();
            break;
        case MonoDubStyle::MonoDubUnison:
            hit = lead_settings.get_mono_hit();
            break;
        }

        if (hit)
        {
            uint8_t pitch = 0;

            switch (this->style)
            {
            case MonoDubStyle::MonoDubLead:
                pitch = this->mono_settings.get_next_mono_pitch();
                break;
            case MonoDubStyle::MonoDubUnison:
                pitch = lead_settings.get_mono_pitch();
                break;
            }

            pitch = clip_pitch(pitch, rerange(this->variable_pitch_offset, 48, 60));

            this->mono_settings.storage.note_on(
                make_note(pitch, 64, 6, NoteType::Tie),
                time.get_shuffle_delay()
            );
        }
    }
};
