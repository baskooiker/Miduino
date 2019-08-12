#pragma once

#include "ofMain.h"

#include "gate_patterns.hpp"
#include "instrument_base.hpp"
#include "fugue.hpp"
#include "bass.hpp"

namespace Vleerhond
{
    class BassDubSettings : public TonalInstrumentBase {
        Bass& bass_settings;

    public:
        BassDubStyle style;
        NoteInterval note_interval;
        GatePatternAB hit_probs;
        uint8_t density;
        uint8_t v_pitch;

        BassDubSettings(
            Bass& bass_settings_ref,
            HarmonyStruct& harmony_ref,
            TimeStruct& time_ref) :
            TonalInstrumentBase(harmony_ref, time_ref, false),
            bass_settings(bass_settings_ref)
        {
            style = BassDubStyle::DubOctave;
            note_interval = NoteInterval::IntervalRoot;
            density = 0;
            v_pitch = 0;
        }

        void randomize()
        {
            last_randomized_time = millis();
            ofLogNotice("bass_dub", "randomize()");

            switch (Rand::distribution(0, 30, 10))
            {
            case 0:
                this->style = BassDubStyle::DubUnison;
                break;
            case 1:
                this->style = BassDubStyle::DubOctave;
                break;
            case 2:
                this->style = BassDubStyle::DubOctProbability;
                break;
            }
            switch (Rand::distribution(60, 60))
            {
            case 0:
                this->note_interval = NoteInterval::IntervalThird;
                break;
            case 1:
                this->note_interval = NoteInterval::IntervalFifth;
                break;
            }
            this->hit_probs.randomize(Rand::randf(.25f, .75f));
        }

        bool play()
        {
            if (this->kill)
            {
                return false;
            }

            bool hit = this->bass_settings.get_bass_hit(this->density, time);
            if (hit)
            {
                NoteInterval note_interval = NoteInterval::IntervalRoot;
                switch (this->style)
                {
                case BassDubStyle::DubOctave: note_interval = this->note_interval; break;
                default: break;
                }

                uint8_t pitch = this->bass_settings.get_bass_pitch(
                    this->v_pitch,
                    (uint8_t)note_interval
                );

                switch (this->style)
                {
                default:
                case BassDubStyle::DubUnison:
                case BassDubStyle::DubOctave:
                    pitch = Utils::clip_pitch(pitch, Utils::rerange(this->v_pitch, 36, 48));
                    break;
                case BassDubStyle::DubOctProbability:
                    pitch = Utils::clip_pitch(pitch, 36, Utils::rerange(this->v_pitch, 36, 48));
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