#pragma once

#include "mono_settings.h"
#include "fugue_settings.h"

class MonoDubSettings : public MonoSettings
{
public:
    MonoDubStyle dub_style;
    MonoSettings& lead_settings;

    MonoDubSettings(
        MonoSettings& lead_settings_ref,
        FugueSettings& fugue_settings_ref,
        HarmonyStruct& harmony_ref,
        TimeStruct& time_ref):
        MonoSettings(fugue_settings_ref, harmony_ref, time_ref),
        lead_settings(lead_settings_ref)
    {
        dub_style = MonoDubStyle::MonoDubLead;
    }

    void randomize_mono_dub()
    {
        this->randomize();
    }

    void play()
    {
        if (this->kill)
            return;

        bool hit = false;
        switch (this->style)
        {
        case MonoDubStyle::MonoDubLead:
            hit = get_mono_hit();
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
                pitch = this->get_next_mono_pitch();
                break;
            case MonoDubStyle::MonoDubUnison:
                pitch = lead_settings.get_mono_pitch();
                break;
            }

            pitch = Utils::clip_pitch(pitch, Utils::rerange(this->variable_pitch_offset, 48, 60));

            this->storage.note_on(
                NoteStruct(pitch, 64, 6, NoteType::Tie),
                time.get_shuffle_delay()
            );
        }
    }
};
