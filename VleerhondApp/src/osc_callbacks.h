#pragma once

#include "application_data.h"
#include "ofxOsc.h"

namespace Vleerhond {
void receiveOscMessages(ApplicationData& data, ofxOscReceiver& osc_receiver);
}
