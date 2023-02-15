#pragma once

#include "application_data.h"
#include "ofxMidiMessage.h"

namespace Vleerhond {
class NanoKontrol2 : public ofxMidiListener {
    ApplicationData& data;

   public:
    NanoKontrol2(ApplicationData& data);
    void newMidiMessage(ofxMidiMessage& message);
};
}  // namespace Vleerhond
