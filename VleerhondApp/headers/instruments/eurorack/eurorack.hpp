#include "instruments/instrument_base.h"

namespace Vleerhond
{
    class EurorackTriggers : public InstrumentBase
    {
    private:
        const std::vector<int> drum_channels = {};

    public:
        EurorackTriggers(TimeStruct& time);
        bool play();
    };
}
