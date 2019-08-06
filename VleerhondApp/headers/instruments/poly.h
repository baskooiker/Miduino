#pragma once

#include "arp.h"
#include "chords.h"
#include "defs.h"
#include "euclid.h"
#include "midi_io.h"

#include "gate_patterns.h"

namespace Vleerhond
{
    class PolySettings : public TonalInstrumentBase
    {
    protected:
        GatePatternAB gates_low;
        GatePatternAB gates;
        GatePatternAB tie_pattern;
        uint8_t pitch_offset;
        PolyType type;

    public:
        uint8_t variable_pitch_offset;

        PolySettings(HarmonyStruct& harmony_ref, TimeStruct& time_ref) :
            TonalInstrumentBase(harmony_ref, time_ref, true)
        {
            pitch_offset = 36;
            type = PolyType::PolyLow;
        }

        void randomize()
        {
            ofLogNotice("poly", "randomize()");
            last_randomized_time = millis();

            // Set pattern low
            this->gates_low.set_gates_low();

            // Set pattern high
            uint8_t steps = Rand::randui8(5, 11);
            this->gates.set_euclid(16, steps);
            this->gates.abPattern.set_ab_pattern();

            // Set Tie Pattern
            this->tie_pattern.randomize(Rand::randf(.1f, .4f));

            // Randomize pitch range
            this->pitch_offset = Rand::randui8(30, 44);

            switch (Rand::distribution(16, 16))
            {
            case 0: this->type = PolyType::PolyHigh; break;
            case 1: this->type = PolyType::PolyLow; break;
            }
        }

        bool play()
        {
            uint8_t velocity = 64;

            bool hit = false;
            switch (this->type)
            {
            case PolyType::PolyLow: hit = this->gates_low.gate(time); break;
            case PolyType::PolyHigh: hit = this->gates.gate(time); break;
            }

            if (hit)
            {
                const uint8_t MAX_CHORD_NOTES = 8;

                uint8_t chord_nr = harmony.get_chord_step(time);
                uint8_t size = 0;
                uint8_t chord_notes[MAX_CHORD_NOTES];

                uint8_t total_pitch_offset = Utils::rerange(this->variable_pitch_offset, 36, this->pitch_offset);
                ofLogVerbose("poly", "variable pitch offset: %d, pitch_offset: %d", variable_pitch_offset, total_pitch_offset);
                ChordUtils::get_chord(chord_nr, harmony.scale, total_pitch_offset, chord_notes, size);

                uint8_t length = 6;
                if (this->tie_pattern.gate(time)
                    || this->type == PolyType::PolyLow)
                {
                    length = time.ticks_left_in_bar();
                }

                NoteStruct note_structs[MAX_CHORD_NOTES];
                for (int i = 0; i < size; i++)
                {
                    note_structs[i].pitch = chord_notes[i];
                    note_structs[i].velocity = 64;
                    note_structs[i].type = NoteType::Tie;
                    note_structs[i].length = length;
                }

                this->midi_channel.note_on(note_structs, size);
                return true;
            }
            return false;
        }
    };
}
