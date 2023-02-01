#pragma once

#include "ofxMidiMessage.h"

namespace Vleerhond
{
class NanoKontrol2 : public ofxMidiListener
{
    ApplicationData& data;
    ofxMidiOut& midicloro_out;

    public:
        NanoKontrol2(ApplicationData& data, ofxMidiOut& midicloro_out) 
        : data(data)
        , midicloro_out(midicloro_out)
        {
        }

        void newMidiMessage(ofxMidiMessage& message)
        {
            switch (message.status)
            {
            case MIDI_CONTROL_CHANGE:
                // ofLogNotice("MIDIIN", "CC in: channel %d, control %d, value %d", message.channel, message.control, message.value);
                switch(message.control)
                {
                    case 0: // Slider 1
                        break;
                    case 1: // Slider 2
                        break;
                    case 2: // Slider 3
                        break;
                    case 3: // Slider 4
                        break;
                    case 4: // Slider 5
                        break;
                    case 5: // Slider 6
                        break;
                    case 6: // Slider 7
                        break;
                    case 7: // Slider 8
                        midicloro_out.sendControlChange(0, 10, message.value);
                        data.time.state = PlayState::Playing;
                        break;
                    case 41:  // Play
                        midicloro_out.sendControlChange(0, 13, message.value);
                        data.time.state = PlayState::Playing;
                        break;
                    case 42:  // Stop
                        midicloro_out.sendControlChange(0, 14, message.value);
                        data.time.state = PlayState::Stopped;
                        break;
                    default:
                        break;
                }
                break;
            case MIDI_TIME_CLOCK:
            case MIDI_STOP:
            case MIDI_START:
            case MIDI_CONTINUE:
            case MIDI_NOTE_ON:
            case MIDI_NOTE_OFF:
                break;
            }
        }
};
}

// switch (message.pitch)
// {
//     case 48: // C3
//         data.minitaur.select(0);
//         break;
//     case 64: // E4
//         data.minitaur.select(1);
//         break;
//     case 80: // G#5
//         data.minitaur.select(2);
//         break;
//     case 96: // C7
//         data.minitaur.select(3);
//         break;

//     case 50:  // D3
//         data.neutron.select(0);
//         break;
//     case 66:  // D3
//         data.neutron.select(1);
//         break;
//     case 82:  // F#4
//         data.neutron.select(2);
//         break;
//     case 98:  // A#5
//         data.neutron.select(3);
//         break;

//     case 52:  // E3
//         data.harmony.setTonic();
//         break;
//     case 53:  // F3
//         data.harmony.setType(HarmonyType::Const);
//         data.harmony.switchConstChord();
//         break;
//     case 68:  // G#4
//         break;
//     case 84:  // C6
//         data.addEvent(std::make_shared<ChangeHarmonyEvent>(HarmonyType::TonicLow, &data.harmony, &data.time));
//         break;
//     case 85:  // C#6
//         data.addEvent(std::make_shared<ChangeHarmonyEvent>(HarmonyType::DominantLow, &data.harmony, &data.time));
//         break;
//     case 100:  // E7
//         data.addEvent(std::make_shared<ChangeHarmonyEvent>(HarmonyType::TonicHigh, &data.harmony, &data.time));
//         break;
//     case 101:  // F7
//         data.addEvent(std::make_shared<ChangeHarmonyEvent>(HarmonyType::DominantHigh, &data.harmony, &data.time));
//         break;
//     default:
//         break;
// }