#pragma once

#include "gate_patterns.h"
#include "instrument_base.h"
#include "fugue.h"

class BassDubSettings : public TonalInstrumentBase {
    BassSettings& bass_settings;
    FugueSettings& fugue_settings;

public:
    BassDubStyle style;
    NoteInterval note_interval;
    GatePatternAB hit_probs;
    uint8_t density;
    uint8_t v_pitch;
    uint8_t fugue_id;

    BassDubSettings(
        BassSettings& bass_settings_ref, 
        FugueSettings& fugue_settings_ref,
        HarmonyStruct& harmony_ref, 
        TimeStruct& time_ref):
        TonalInstrumentBase(harmony_ref, time_ref),
        bass_settings(bass_settings_ref),
        fugue_settings(fugue_settings_ref)
    {
        style = BassDubStyle::DubOctave;
        note_interval = NoteInterval::IntervalRoot;
        density = 0;
        v_pitch = 0;
    }

    void randomize()
    {
        last_randomized_time = millis();
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

    void play()
    {
        if (this->kill)
        {
            return;
        }

        if (this->style == BassDubStyle::DubFugue)
        {
            return this->fugue_settings.play_fugue(
                this->fugue_id,
                harmony,
                time,
                this->storage);
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
                note_interval
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

            this->storage.note_on(
                NoteStruct(pitch, 64, 6, NoteType::Tie),
                time.get_shuffle_delay()
            );
        }
    }
};
