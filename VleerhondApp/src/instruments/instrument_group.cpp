#include "instruments/instrument_group.h"

namespace Vleerhond {
InstrumentGroup::InstrumentGroup(HarmonyStruct& harmony, TimeStruct& time)
    : TonalInstrumentBase(harmony, time) {}
bool InstrumentGroup::play() {
    if (this->_kill) {
        return false;
    }

    bool rv = false;
    for (InstrumentBase* instrument : instruments) {
        rv |= instrument->play();
    }
    return rv;
}
void InstrumentGroup::randomize() {
    for (InstrumentBase* instrument : instruments) {
        instrument->randomize();
    }
}
std::vector<InstrumentBase*> InstrumentGroup::getPtrs() { return instruments; }
void InstrumentGroup::autoRandomize() {
    for (InstrumentBase* instrument : instruments) {
        instrument->autoRandomize();
    }
}
void InstrumentGroup::setChannel(std::shared_ptr<MidiChannel> channel) {
    this->midi_channel = channel;
    for (InstrumentBase* inst_ptr : instruments) {
        inst_ptr->setChannel(channel);
    }
}
std::shared_ptr<MidiChannel> InstrumentGroup::getChannel() {
    // Uggly assumption that all instruments use the same channel
    return instruments[0]->getChannel();
}
}  // namespace Vleerhond
