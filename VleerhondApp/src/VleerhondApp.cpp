#include "VleerhondApp.h"

#include <stdlib.h>

#include "application/midi_time_listener.h"
#include "application/nanokontrol2.h"
#include "callbacks.h"
#include "core/defs.h"
#include "midi/console_midi_channel.h"
#include "midi_io.h"
#include "ofLog.h"
#include "ofxMidiIn.h"
#include "ofxMidiOut.h"

namespace Vleerhond {
const char MODULE[] = "VleerhondApp";

bool openOutputPort(const std::string& target_port_name, ofxMidiOut& out) {
    for (int16_t i = 0; i < out.getNumOutPorts(); i++) {
        if (out.getOutPortName(i).find(target_port_name) != std::string::npos) {
            if (out.openPort(i)) {
                return true;
            }
        }
    }
    return false;
}

bool openFirstInput(
    const std::vector<std::string> midi_in_names, ofxMidiListener* listener) {
    bool in_found = false;
    for (std::string in_name : midi_in_names) {
        ofLogNotice("", "Trying to open port %s", in_name.c_str());
        if (MidiIO::portAvailable(in_name)) {
            if (MidiIO::setMainInput(in_name, listener)) {
                in_found = true;
                break;
            } else {
                return false;
            }
        }
    }
    if (!in_found) {
        return false;
    }

    return true;
}

bool VleerhondApp::initializeMidiPorts() {
    auto midi_time_listener = std::make_shared<MidiTimeListener>(data);
    midi_listeners.push_back(midi_time_listener);
    if (!openFirstInput({"Midi Through Port-1"}, midi_time_listener.get())) {
        return false;
    }

    if (!MidiIO::addOutput("ttymidi", 1)) {
        ofLogNotice("", "Failed to open output!");
        return false;
    }
    ofLogNotice("", "Succesfully opened output!");

    return true;
}

void VleerhondApp::setup() {
    ofLogToConsole();

    if (!initializeMidiPorts()) {
        ::exit(-1);
    }

    auto nanokontrol = std::make_shared<NanoKontrol2>(data);
    midi_listeners.push_back(nanokontrol);
    if (!openFirstInput({"nanoKONTROL2"}, nanokontrol.get())) {
        ofLogError("", "Failed to open input 'nanoKONTROL2'!");
        ::exit(-1);
    }

    data.connect();

    // Init app
    data.randomizeAll();

    // TODO: Send All note offs to all instruments

    data.vermona.select(0);

    // Set default key to G
    data.harmony.scale.setRoot(Root::ROOT_G);

    // Set default MS20 to ARP
    data.neutron.select(3);
}

void VleerhondApp::update() {
    if (!MidiIO::portsOpen()) {
        ::exit(-1);
    }
    data.processNoteEvents();
}
}  // namespace Vleerhond
