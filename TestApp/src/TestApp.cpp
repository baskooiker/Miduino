#include "TestApp.h"

#include "ofLog.h"
#include "ofxMidiOut.h"
#include "ofxMidiIn.h"

namespace Vleerhond
{
    const char MODULE[] = "TestApp";

    void TestApp::setup()
    {
        ofLogToConsole();

//        receiver.setup(4041);

        midi_out.openVirtualPort("vleerhond_clock");

        start_time = ofGetSystemTimeMicros();
        clock_counter = 0;

    }

    void TestApp::update()
    {
	// check for waiting messages
/*
	while( receiver.hasWaitingMessages() )
	{
            // get the next message
            ofxOscMessage m;
            receiver.getNextMessage( &m );
            ofLogNotice("OSC", "Received from %s", m.getAddress().c_str());
            if ( strcmp( m.getAddress().c_str(), "/mouse/position" ) == 0 )
            {
            }
        }
*/
        double bpm = 150;
        double ppq = 24;
        double elapsed_micros = (double)ofGetSystemTimeMicros() - (double)start_time;
        double pulses_per_minute = ppq * bpm;
        const double micros_per_minute = 60000000;
        double micros_per_pulse = micros_per_minute / pulses_per_minute;
        uint64_t elapsed_pulses = (uint64_t)(elapsed_micros / micros_per_pulse);
        while (elapsed_pulses > clock_counter)
        {
//            ofLogNotice("CLOCK", "PULSE: %d", (int)clock_counter);
//            std::cout << clock_counter << "\n";
            midi_out.sendMidiByte(0xF8);
            clock_counter++;
        }
    }

    void TestApp::draw() {}

    void TestApp::keyPressed(int key) {}

    void TestApp::newMidiMessage(ofxMidiMessage& message)
    {
        switch (message.status)
        {
        case MIDI_TIME_CLOCK:
            break;
        case MIDI_STOP:
            ofLogNotice("Vleerhond", "Stop!");
            break;
        case MIDI_START:
        case MIDI_CONTINUE:
            ofLogNotice("", "Start!");
            break;
        case MIDI_NOTE_ON:
            ofLogNotice("MIDIIN", "NoteOn(d%, %d, %d)", message.channel, message.pitch, message.velocity);
            break;
        case MIDI_NOTE_OFF:
            break;
        case MIDI_CONTROL_CHANGE:
            ofLogVerbose("MIDIIN", "CC in: %d, %d", message.control, message.value);
            break;
        default:
            break;
        }
    }

    void TestApp::keyReleased(int key) {}
    void TestApp::mouseMoved(int x, int y) {}
    void TestApp::mouseDragged(int x, int y, int button) {}
    void TestApp::mousePressed(int x, int y, int button) {}
    void TestApp::mouseReleased(int x, int y, int button) {}
    void TestApp::mouseEntered(int x, int y) {}
    void TestApp::mouseExited(int x, int y) {}
    void TestApp::windowResized(int w, int h) {}
    void TestApp::gotMessage(ofMessage msg) {}
    void TestApp::dragEvent(ofDragInfo dragInfo) { }

    void TestApp::exit()
    {
        ofLogNotice("", "Closing MIDI ports");
    }
}
