#pragma once

#include "mono_settings.h"

namespace Vleerhond
{
    class MonoDubSettings : public MonoSettings
    {
    protected:

    public:
        MonoDubStyle dub_style;
        MonoSettings& lead_settings;

        MonoDubSettings(
            MonoSettings& lead_settings_ref,
            HarmonyStruct& harmony_ref,
            TimeStruct& time_ref) :
            MonoSettings(harmony_ref, time_ref),
            lead_settings(lead_settings_ref)
        {
            dub_style = MonoDubStyle::MonoDubLead;
        }

        void total_randomize()
        {
            lead_settings.total_randomize();
        }

        void randomize()
        {
            ofLogVerbose("mono_dub", "randomize()");
            lead_settings.randomize();
        }

        bool play()
        {
            this->check_arp_reset();

            if (this->kill)
                return false;

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
                    pitch = this->harmony.scale.get_ascending(pitch);
                    pitch = Utils::clip_pitch(pitch, Utils::rerange(this->variable_pitch_offset, 36, 36));
                    break;
                }


                this->midi_channel.note_on(
                    NoteStruct(pitch, 64, 6, NoteType::Tie),
                    time.get_shuffle_delay()
                );
                return true;
            }
            return false;
        }
    };
}