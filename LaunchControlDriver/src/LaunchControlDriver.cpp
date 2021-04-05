#include "LaunchControlDriver.h"

#include "ofLog.h"
#include "ofxMidiOut.h"
#include "ofxMidiIn.h"

namespace Vleerhond
{
    static const uint8_t OFF_OFF = 12;
    static const uint8_t RED_LOW = 13;
    static const uint8_t RED_FULL = 15;
    static const uint8_t AMBER_LOW = 29;
    static const uint8_t AMBER_FULL = 63;
    static const uint8_t YELLOW_FULL = 62;
    static const uint8_t GREEN_LOW = 28;
    static const uint8_t GREEN_FULL = 60;

    const char MODULE[] = "LaunchControlDriver";


    static const uint8_t CHANNEL = 1;
    static const uint8_t BUTTON_0 = 9;
    static const uint8_t BUTTON_1 = 10;
    static const uint8_t BUTTON_2 = 11;
    static const uint8_t BUTTON_3 = 12;
    static const uint8_t BUTTON_4 = 25;
    static const uint8_t BUTTON_5 = 26;
    static const uint8_t BUTTON_6 = 27;
    static const uint8_t BUTTON_7 = 28;
    static const uint8_t BUTTONS[] = {BUTTON_0, BUTTON_1, BUTTON_2, BUTTON_3, BUTTON_4, BUTTON_5, BUTTON_6, BUTTON_7};
    static const uint8_t ENCODERS_A[] = {21, 22, 23, 24, 25, 26, 27, 28};
    static const uint8_t ENCODERS_B[] = {41, 42, 43, 44, 45, 46, 47, 48};
    static const uint8_t UP = 114;
    static const uint8_t DOWN = 115;
    static const uint8_t LEFT = 116;
    static const uint8_t RIGHT = 117;

    LaunchControlDriver::LaunchControlDriver()
        : midi_out("ofxMidiOutClient", MIDI_API_JACK)
        , midi_in("ofxMidiInClient", MIDI_API_JACK)
    {
    }

    void LaunchControlDriver::setup()
    {
        ofLogToConsole();

        osc_sender.setup("127.0.0.1", 4042);
        osc_receiver.setup(4041);

        std::string port_out_name = "Launch Control";
        uint8_t port_index = 0xFF;
        uint8_t i = 0;
        for (const std::string name : midi_out.getOutPortList())
        {
            ofLogNotice("MIDI", "Out port %d: %s", i, name.c_str());
            if(port_index == 0xFF)
            {
                if (name.find(port_out_name) != std::string::npos)
                {
                    port_index = i;
                    ofLogNotice("MIDI", "That's it!!!");
                }
            }
            i++;
        }
        if (port_index == 0xFF)
        {
            ofExit(-1);
        }
        midi_out.openPort(port_index);

        std::string port_in_name = "Launch Control";
        port_index = 0xFF;
        i = 0;
        for (const std::string name : midi_in.getInPortList())
        {
            ofLogNotice("MIDI", "In port %d: %s", i, name.c_str());
            if(port_index == 0xFF)
            {
                if (name.find(port_in_name) != std::string::npos)
                {
                    port_index = i;
                    ofLogNotice("MIDI", "That's it!!!");
                }
            }
            i++;
        }
        if (port_index == 0xFF)
        {
            ofExit(-1);
        }
        midi_in.openPort(port_index);
        midi_in.ignoreTypes(false, false, false);
        midi_in.setVerbose(true);
        midi_in.addListener(this);

        start_time = ofGetSystemTimeMicros();
        clock_counter = 0;


        // Reset button colors
        std::vector<uint8_t> bytes = {0xB0, 0x00, 0x00};
        midi_out.sendMidiBytes(bytes);
        }

    void LaunchControlDriver::update()
    {
        // check for waiting messages
        static uint8_t counter = 0;

        uint64_t micros_since_start = ofGetSystemTimeMicros() - start_time;

        if (micros_since_start / 100000 > clock_counter)
        {
            clock_counter++;
            counter = (counter + 1) % 128;
            // ofLogNotice("", "Sending NoteOne(%d, %d, %d)", buttons[counter % 8], AMBER_LOW, i);
            // midi_out.sendNoteOn(buttons[counter % 8], AMBER_LOW, i);
            uint8_t color_for_mode = this->control_mode == ControlMode::CHORD_MODE ? AMBER_LOW : GREEN_LOW;

            // std::vector<uint8_t> bytes = {0xB0, 0x00, 0x7F};
            for (auto note : BUTTONS)
            {
                uint8_t channel = 144 + CHANNEL;
                std::vector<uint8_t> bytes = {channel, note, color_for_mode};
                // midi_out.sendMidiBytes(bytes);
                midi_out.sendNoteOn(CHANNEL, note, color_for_mode);
            }
        }
    }

    void LaunchControlDriver::draw() {}

    void LaunchControlDriver::keyPressed(int key) {}

    void LaunchControlDriver::newMidiMessage(ofxMidiMessage& message)
    {
        switch (message.status)
        {
        case MIDI_TIME_CLOCK:
            break;
        case MIDI_STOP:
            // ofLogNotice("Vleerhond", "Stop!");
            break;
        case MIDI_START:
        case MIDI_CONTINUE:
            // ofLogNotice("", "Start!");
            break;
        case MIDI_NOTE_ON:
            switch (message.pitch)
            {
                case BUTTON_0:
                    ofLogNotice("MIDIIN", "BUTTON[0] on");
                    break;
                case BUTTON_1:
                    ofLogNotice("MIDIIN", "BUTTON[1] on");
                    break;
                case BUTTON_2:
                    ofLogNotice("MIDIIN", "BUTTON[2] on");
                    break;
                case BUTTON_3:
                    ofLogNotice("MIDIIN", "BUTTON[3] on");
                    break;
                case BUTTON_4:
                    ofLogNotice("MIDIIN", "BUTTON[4] on");
                    break;
                case BUTTON_5:
                    ofLogNotice("MIDIIN", "BUTTON[5] on");
                    break;
                case BUTTON_6:
                    ofLogNotice("MIDIIN", "BUTTON[6] on");
                    break;
                case BUTTON_7:
                    ofLogNotice("MIDIIN", "BUTTON[7] on");
                    break;
                default:
                    ofLogNotice("MIDIIN", "NoteOn(channel: %d, pitch: %d, velocity: %d)", message.channel, message.pitch, message.velocity);
                    break;
            }
            break;
        case MIDI_NOTE_OFF:

            switch (message.pitch)
            {
                case BUTTON_0:
                    if (this->control_mode == ControlMode::CHORD_MODE)
                    {
                        if (message.value == 0)
                        {
                            ofxOscMessage m;
                            m.setAddress( "/chord_pattern" );
                            m.addStringArg( "STATIC_ROOT" );
                            osc_sender.sendMessage( m );
                            ofLogNotice("MIDIIN", "BUTTON[0]: OSC STATIC_ROOT sent");
                        }
                    }
                    break;
                case BUTTON_1:
                    if (this->control_mode == ControlMode::CHORD_MODE)
                    {
                        if (message.value == 0)
                        {
                            ofxOscMessage m;
                            m.setAddress( "/chord_pattern" );
                            m.addStringArg( "STATIC_NON_ROOT" );
                            osc_sender.sendMessage( m );
                            ofLogNotice("MIDIIN", "BUTTON[1]: OSC STATIC_NON_ROOT sent");
                        }
                    }
                    break;
                case BUTTON_2:
                    if (this->control_mode == ControlMode::CHORD_MODE)
                    {
                        if (message.value == 0)
                        {
                            ofxOscMessage m;
                            m.setAddress( "/chord_pattern" );
                            m.addStringArg( "SIMPLE_PROGRESSION" );
                            osc_sender.sendMessage( m );
                            ofLogNotice("MIDIIN", "BUTTON[2]: OSC SIMPLE_PROGRESSION sent");
                        }
                    }
                    break;
                case BUTTON_3:
                    if (this->control_mode == ControlMode::CHORD_MODE)
                    {
                        if (message.value == 0)
                        {
                            ofxOscMessage m;
                            m.setAddress( "/chord_pattern" );
                            m.addStringArg( "LONG_PROGRESSION" );
                            osc_sender.sendMessage( m );
                            ofLogNotice("MIDIIN", "BUTTON[3]: OSC LONG_PROGRESSION sent");
                        }
                    }
                    break;
                default:
                    ofLogNotice("MIDIIN", "NoteOff(channel: %d, pitch: %d, velocity: %d)", message.channel, message.pitch, message.velocity);
                    break;
            }
            break;
        case MIDI_CONTROL_CHANGE:
            
            switch (message.control)
            {
                case UP:
                    if (message.value > 0)
                    {
                        ofLogNotice("MIDIIN", "UP");
                    }
                    break;
                case DOWN:
                    if (message.value > 0)
                    {
                        ofLogNotice("MIDIIN", "DOWN");
                    }
                    break;
                case LEFT:
                    if (message.value > 0)
                    {
                        this->control_mode = ControlMode::CHORD_MODE;
                        ofLogNotice("MIDIIN", "Switch to Chord Mode");
                    }
                    break;
                case RIGHT:
                    if (message.value > 0)
                    {
                        this->control_mode = ControlMode::PATTERN_MODE;
                        ofLogNotice("MIDIIN", "Switch to Pattern Mode");
                    }
                    break;
                default:
                    ofLogNotice("MIDIIN", "CC(channel: %d, control: %d, value: %d)", message.channel, message.control, message.value);
                    break;
            }
            break;
        default:
            ofLogNotice("MIDIIN", "Unexpected status %d: channel %d, cc %d, value %d", message.status, message.channel, message.control, message.value);
            break;
        }
    }

    void LaunchControlDriver::keyReleased(int key) {}
    void LaunchControlDriver::mouseMoved(int x, int y) {}
    void LaunchControlDriver::mouseDragged(int x, int y, int button) {}
    void LaunchControlDriver::mousePressed(int x, int y, int button) {}
    void LaunchControlDriver::mouseReleased(int x, int y, int button) {}
    void LaunchControlDriver::mouseEntered(int x, int y) {}
    void LaunchControlDriver::mouseExited(int x, int y) {}
    void LaunchControlDriver::windowResized(int w, int h) {}
    void LaunchControlDriver::gotMessage(ofMessage msg) {}
    void LaunchControlDriver::dragEvent(ofDragInfo dragInfo) { }

    void LaunchControlDriver::exit()
    {
        ofLogNotice("", "Closing MIDI ports");
    }
}
