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
    public:
        GatePatternAB gates_low;
        GatePatternAB gates;
        GatePatternAB tie_pattern;
        uint8_t pitch_offset;
        PolyType type;

        PolySettings(HarmonyStruct& harmony_ref, TimeStruct& time_ref) :
            TonalInstrumentBase(harmony_ref, time_ref, true)
        {
            pitch_offset = 48;
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
            this->pitch_offset = Rand::randui8(42, 54);

            switch (Rand::distribution(16, 16))
            {
            case 0: this->type = PolyType::PolyHigh; break;
            case 1: this->type = PolyType::PolyLow; break;
            }
        }

        void play()
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

                // TODO: get poly pitch offset into settings 
                uint8_t poly_pitch_offset = 64;

                uint8_t pitch_offset = this->pitch_offset
                    + (((uint16_t)poly_pitch_offset * 24) / 128)
                    - 12;
                ChordUtils::get_chord(chord_nr, harmony.scale, pitch_offset, chord_notes, size);

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

                this->storage.note_on(note_structs, size);
            }
        }
    };
}