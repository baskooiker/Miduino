#pragma once

#include "defs.h"

namespace Vleerhond
{
    class Tanzbar
    {
    public:
        static void randomize_parameters(const RandomParam* list, const uint8_t length)
        {
            for (int i = 0; i < length; i++)
            {
                MidiIO::send_cc(list[i].note,
                    Rand::randui8(list[i].min, list[i].max),
                    MIDI_CC_CHANNEL_TANZBAR);
            }
        }

    };
}