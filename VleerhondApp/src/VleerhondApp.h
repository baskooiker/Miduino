#pragma once

#include "application_data.h"
#include "ofxMidiMessage.h"

namespace Vleerhond {
class VleerhondApp {
   public:
    ApplicationData data;

    std::vector<std::shared_ptr<ofxMidiListener>> midi_listeners;

    bool initializeMidiPorts();

    void setup();
    void update();

    void gotMessage(ofMessage msg);
    void newMidiMessage(ofxMidiMessage& message);
};
}  // namespace Vleerhond
