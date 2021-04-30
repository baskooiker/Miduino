#include "instruments/eurorack/eurorack.hpp"

#include "utils/rand.h"

namespace Vleerhond
{
    EurorackTriggers::EurorackTriggers(TimeStruct & time)
        : InstrumentBase(time)
    {
    }

    bool EurorackTriggers::play()
    {
        // Generate random triggers
        for (int pitch : {36, 37})
        {
            if (Rand::distribution(32, 32))
            {
                this->midi_channel->noteOn(
                    NoteStruct(
                        pitch,
                        127,
                        1,
                        NoteType::Normal
                    ),
                    time.getShuffleDelay()
                );
            }
        }

        // Generate random gates
        for (int pitch : {38, 39})
        {
            if (Rand::distribution(32, 32))
            {
                this->midi_channel->noteOn(
                    NoteStruct(
                        pitch,
                        127,
                        6,
                        NoteType::Normal
                    ),
                    time.getShuffleDelay()
                );
            }
        }

        // Generate clock
        for (int pitch : {40, 41, 42, 43})
        {
            if (Utils::intervalHit(TimeDivision::Sixteenth, time))
            {
                this->midi_channel->noteOn(
                    NoteStruct(
                        pitch,
                        127,
                        1,
                        NoteType::Normal
                    ),
                    time.getShuffleDelay()
                );
            }
        }

        return true;
    }
}
