#pragma once

#include "ofxMidiMessage.h"

#include "application_data.h"

namespace Vleerhond
{
class NanoKontrol2 : public ofxMidiListener
{
    ApplicationData& data;

    public:
        NanoKontrol2(ApplicationData& data);
        void newMidiMessage(ofxMidiMessage& message);
};
}
