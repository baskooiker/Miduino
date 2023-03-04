#pragma once

#include "application_data.h"
#include "ofxMidiMessage.h"

namespace Vleerhond {
class MidiTimeListener : public ofxMidiListener {
    ApplicationData& data;

    int stop_counter = 0;

   public:
    MidiTimeListener(ApplicationData& data);
    void newMidiMessage(ofxMidiMessage& message);
};
}  // namespace Vleerhond