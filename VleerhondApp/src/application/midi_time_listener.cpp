#include "midi_time_listener.h"

#include "callbacks.h"

namespace Vleerhond {

MidiTimeListener::MidiTimeListener(ApplicationData& data) : data(data) {}

void MidiTimeListener::newMidiMessage(ofxMidiMessage& message) {
    switch (message.status) {
        case MIDI_TIME_CLOCK:
            MidiIO::sendTimeClock();
            handleClock(this->data);
            break;
        case MIDI_STOP:
            ofLogNotice("Vleerhond", "Stop!");
            MidiIO::sendStop();
            handleStop(this->data);
            stop_counter++;
            if (stop_counter > 8) {
                ::exit(0);
            } else {
                for (InstrumentBase* inst : data.getInstrumentPtrs()) {
                    inst->getChannel()->allNotesOff();
                }
            }
            break;
        case MIDI_START:
        case MIDI_CONTINUE:
            stop_counter = 0;
            MidiIO::sendContinue();
            MidiIO::sendStart();
            ofLogNotice("Vleerhond", "Start!");
            data.time.state = PlayState::Playing;
            break;
        case MIDI_NOTE_ON:
        case MIDI_NOTE_OFF:
        case MIDI_CONTROL_CHANGE:
        default:
            break;
    }
}
}  // namespace Vleerhond
