#pragma once

#include "mono_settings.h"

class MonoDubSettings : public MonoSettings
{
protected:
    NoteInterval note_interval;

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
        switch (Rand::distribution(16, 16, 16))
        {
        case 0: this->note_interval = NoteInterval::IntervalRoot; break;
        case 1: this->note_interval = NoteInterval::IntervalThird; break;
        case 2: this->note_interval = NoteInterval::IntervalFifth; break;
        }
    }

    void play()
    {
        this->check_arp_reset();
        if (this->kill)
            return;

        bool hit = false;
        switch (this->dub_style)
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

            switch (this->dub_style)
            {
            case MonoDubStyle::MonoDubLead:
                pitch = this->get_next_mono_pitch();
                break;
            case MonoDubStyle::MonoDubUnison:
                pitch = lead_settings.get_mono_pitch();
                pitch = this->harmony.scale.get_ascending(pitch, note_interval);
                pitch = Utils::clip_pitch(pitch, Utils::rerange(this->variable_pitch_offset, 36, 36));
                break;
            }


            this->storage.note_on(
                NoteStruct(pitch, 64, 6, NoteType::Tie),
                time.get_shuffle_delay()
            );
        }
    }
};
