#pragma once

#include "core/defs.h"
#include "midi_io.h"
#include "rand.h"

namespace Vleerhond
{
    class Parameters
    {
    public:
        static void randomize_parameters(const RandomParam* list, const uint8_t length, const uint8_t channel)
        {
            for (int i = 0; i < length; i++)
            {
                MidiIO::send_cc(
                    list[i].note,
                    Rand::randui8(list[i].min, list[i].max),
                    channel
                );
            }
        }

    };
}