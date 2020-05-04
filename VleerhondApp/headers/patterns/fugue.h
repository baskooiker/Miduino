#pragma once

#include "core/defs.h"
#include "harmony/harmony_struct.h"
#include "midi_channel.h"

namespace Vleerhond
{
    class Fugue 
    {
    public:
        // TODO: privatize
        CvPattern16 pattern;

        void randomizeFugue()
        {
            // Randomize Fugue pattern
            static uint8_t pitch_opts[] = { 0, 4, 2, 3, 6 };
            this->pattern.length = 0;
            uint8_t length = Rand::randui8(64) < 32 ? 4 : 8;
            while (this->pattern.length < length)
            {
                Utils::randomizeOrder(pitch_opts, 5);
                for (int i = 0; i < 5 && this->pattern.length < length; i++)
                {
                    this->pattern.pattern.set(this->pattern.length++, pitch_opts[i]);
                }
            }
        }

    };
}