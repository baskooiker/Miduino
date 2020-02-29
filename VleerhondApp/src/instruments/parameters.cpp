#include "instruments/parameters.h"

#include "midi_io.h"
#include "rand.h"

namespace Vleerhond
{
namespace Parameters
{
    void randomize_parameters(
        const RandomParam* list, 
        const uint8_t length, 
        const uint8_t channel, 
        const std::string& port_name)
    {
        // TODO: Remove this function
        for (int i = 0; i < length; i++)
        {
            MidiIO::send_cc(
                list[i].note,
                Rand::randui8(list[i].min, list[i].max),
                channel,
                port_name
            );
        }
    }
}
}