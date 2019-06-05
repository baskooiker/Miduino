#pragma once

#include "instrument_base.h"
#include "fugue.h"

namespace Vleerhond
{
    static const std::vector<uint8_t> time_intervals{ 1, 2, 3, 4, 5, 6, 7, 8, 10, 12, 14, 16 };

    class FuguePlayer : public TonalInstrumentBase
    {
        Fugue& fugue;

    public:
        uint8_t pitch_offset;
        uint8_t manual_pitch_offset;
        uint8_t length;
        FuguePlayerType type;
        uint32_t counter;
        uint8_t rhythm;
        uint8_t density;
        NoteInterval note_interval;
        uint8_t note_repeat;

        FuguePlayer(HarmonyStruct& harmony, TimeStruct& time, Fugue& fugue) :
            TonalInstrumentBase(harmony, time, true),
            fugue(fugue)
        {
            pitch_offset = 36;
            length = 4;
            type = FuguePlayerType::FugueForward;
            note_interval = NoteInterval::IntervalRoot;
            note_repeat = 1;
        }

        void play()
        {
            if (Utils::interval_hit(TimeDivision::Sixteenth, time))
            {
                bool hit = false;

                uint8_t player_length_idx = Utils::rerange(this->density, 5, this->length - 2);
                uint32_t player_length = time_intervals.at(MAX(MIN(player_length_idx, time_intervals.size()), 0));

                if (this->counter % player_length == 0)
                {
                    hit = Utils::gate(this->rhythm, this->counter / player_length, 4);
                }
                uint8_t pat_length = this->fugue.pattern.length * this->note_repeat;
                uint32_t hit_count = (this->counter / player_length) / this->note_repeat;

                if (hit)
                {
                    uint32_t c = hit_count;
                    switch (this->type)
                    {
                    case FuguePlayerType::FugueForward:
                        c = hit_count % pat_length;
                        break;
                    case FuguePlayerType::FugueBackward:
                        c = pat_length - (c % pat_length) - 1;
                        break;
                    case FuguePlayerType::FugueBackAndForth:
                        c = c % (pat_length * 2);
                        if (c < pat_length)
                        {
                            c = c % pat_length;
                        }
                        else
                        {
                            c = pat_length - (c % pat_length) - 1;
                        }
                        break;
                    }

                    uint8_t note_step = this->fugue.pattern.value(c);

                    uint8_t pitch_offset = Utils::rerange(this->manual_pitch_offset, 36, this->pitch_offset);
                    uint8_t pitch = harmony.scale.apply_scale_offset(
                        note_step,
                        pitch_offset,
                        (uint8_t)this->note_interval
                    );
                    storage.note_on(
                        NoteStruct(pitch, 64, player_length * TICKS_PER_STEP, NoteType::Tie),
                        time.get_shuffle_delay()
                    );
                }

                this->counter++;
            }
        }

        void randomize()
        {
            last_randomized_time = millis();

            uint8_t pitch_offsets[] = { 36, 40, 44, 48, 52 };
            uint8_t nr_pitch_offsets = sizeof(pitch_offsets) / sizeof(*pitch_offsets);
            Utils::randomize_order(pitch_offsets, nr_pitch_offsets);

            NoteInterval intervals[4] = {};
            intervals[0] = NoteInterval::IntervalRoot;
            intervals[1] = NoteInterval::IntervalFifth;
            intervals[2] = Utils::random_note_interval();
            intervals[3] = Utils::random_note_interval();
            Utils::randomize_order(intervals, 4);

            //for (int i = 0; i < this->fugue.players.size(); i++)
            //{
            //    if (i != id)
            //    {
            //        Utils::remove(this->fugue.players.at(i)->pitch_offset, pitch_offsets, nr_pitch_offsets);
            //    }
            //}

            this->pitch_offset = pitch_offsets[0];
            this->length = Rand::randui8(2, time_intervals.size() - 1);
            //player_settings.note_interval = intervals[0];
            this->note_interval = NoteInterval::IntervalRoot;

            switch (Rand::randui8(3))
            {
            default:
            case 0: this->type = FuguePlayerType::FugueForward; break;
            case 1: this->type = FuguePlayerType::FugueBackward; break;
            case 2: this->type = FuguePlayerType::FugueBackAndForth; break;
            }

            switch (Rand::distribution(32, 4, 4, 4, 4))
            {
            case 0: this->rhythm = BXXXX; break;
            case 1: this->rhythm = BXXX0; break;
            case 2: this->rhythm = BXX0X; break;
            case 3: this->rhythm = BX0XX; break;
            case 4: this->rhythm = B0XXX; break;
            }

            switch (Rand::distribution(32, 4, 4, 4))
            {
            case 0: this->note_repeat = 1; break;
            case 1: this->note_repeat = 2; break;
            case 2: this->note_repeat = 3; break;
            case 3: this->note_repeat = 4; break;
            }
        }

        void reset()
        {
            counter = 0;
        }

    };
}