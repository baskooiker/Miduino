#pragma once

#include "ofxOsc.h"

#include "application_data.h"

namespace Vleerhond
{
    void receiveOscMessages(ApplicationData& data, ofxOscReceiver& osc_receiver);
}
