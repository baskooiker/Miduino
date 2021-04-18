#include "osc_callbacks.h"

namespace Vleerhond
{
    void handleBassMessage(const ofxOscMessage& m, ApplicationData& data)
    {
        const std::string parameter_name = m.getArgAsString(0);
        const uint8_t value = (uint8_t)m.getArgAsInt32(1);
        if (parameter_name.compare("density") == 0)
        {
            // ofLogNotice("OSC", "Setting density %d", value);
            data.minitaur.setVariableDensity(value);   
        }
        else if (parameter_name.compare("octave") == 0)
        {
            // ofLogNotice("OSC", "Setting octave %d", value);
            data.minitaur.setVariableOctave(value);   
        }
        else if (parameter_name.compare("pitch_offset") == 0)
        {
            ofLogNotice("BASS", "Setting pitch offset %d", value);
            data.minitaur.setVariablePitchOffset(value);   
        }
        else if (parameter_name.compare("pattern") == 0)
        {
            data.minitaur.select(value);   
        }
        else
        {
            ofLogNotice("OSC", "Unexpected Minitaur parameter for name %s", parameter_name.c_str());
        }
    }

    void handleLeadMessage(const ofxOscMessage& m, ApplicationData& data)
    {
        const std::string parameter_name = m.getArgAsString(0);
        const uint8_t value = (uint8_t)m.getArgAsInt32(1);
        if (parameter_name.compare("density") == 0)
        {
            data.neutron.setVariableDensity(value);   
        }
        // else if (parameter_name.compare("octave") == 0)
        // {
        //     data.neutron.setVariableOctave(value);   
        // }
        else if (parameter_name.compare("pitch_offset") == 0)
        {
            data.neutron.setVariablePitchOffset(value);   
        }
        else if (parameter_name.compare("pattern") == 0)
        {
            ofLogNotice("LEAD", "Setting pattern %d", value);
            data.neutron.select(value);   
        }
        else
        {
            ofLogNotice("OSC", "Unexpected Lead parameter for name %s", parameter_name.c_str());
        }
    }

    void handleUnexpectedMessage(const ofxOscMessage& m)
    {
        // unrecognized message: display on the bottom of the screen
        std::string msg_string = m.getAddress();
        msg_string.append(": ");
        for (uint8_t i = 0; i < m.getNumArgs(); i++)
        {
            // get the argument type
            std::string arg_type_name = m.getArgTypeName(i);
            msg_string.append(":");
            // display the argument - make sure we get the right type
            if(m.getArgType(i) == OFXOSC_TYPE_INT32)
                ofLogNotice("OSC", "%s%d ", msg_string.c_str(), m.getArgAsInt32(i));
            else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT)
                ofLogNotice("OSC", "%s%f ", msg_string.c_str(), m.getArgAsFloat(i));
            else if(m.getArgType(i) == OFXOSC_TYPE_STRING)
                ofLogNotice("OSC", "%s\"%s\" ", msg_string, m.getArgAsString(i).c_str());
            else
                ofLogNotice("OSC", "unknown arg type: %d", m.getArgType(i));
        }
    }

    void handleChordPatternMessage(const ofxOscMessage& m, ApplicationData& data)
    {
        std::string pattern_id = m.getArgAsString(0);
        if (pattern_id.compare("STATIC_ROOT" ) == 0 )
        {
            ofLogNotice("OSC", "Setting STATIC_ROOT");
            data.harmony.setTonic();
        }
        else if (pattern_id.compare("STATIC_NON_ROOT" ) == 0 )
        {
            ofLogNotice("OSC", "Setting STATIC_NON_ROOT");
            data.harmony.setType(HarmonyType::Const);
            data.harmony.switchConstChord();
        }
        else if (pattern_id.compare("SIMPLE_PROGRESSION" ) == 0 )
        {
            ofLogNotice("OSC", "Setting SIMPLE_CHORD_PROGRESSION");
            // if (data.ui_state.is_pressed(BSP_PAD_BTM_08))
            {
                data.addEvent(std::make_shared<ChangeHarmonyEvent>(
                    HarmonyType::DominantLow, &data.harmony, &data.time
                    ));
            }
            // else
            // {
            //     data.addEvent(std::make_shared<ChangeHarmonyEvent>(
            //         HarmonyType::TonicLow, &data.harmony, &data.time
            //         ));
            // }
        }
        else if (pattern_id.compare("LONG_PROGRESSION" ) == 0 )
        {
            ofLogNotice("OSC", "Setting LONG_PROGRESSION");
            // if (data.ui_state.is_pressed(BSP_PAD_BTM_08))
            {
                data.addEvent(std::make_shared<ChangeHarmonyEvent>(
                    HarmonyType::DominantHigh, &data.harmony, &data.time
                    ));
            }
            // else
            // {
            //     data.addEvent(std::make_shared<ChangeHarmonyEvent>(
            //         HarmonyType::TonicHigh, &data.harmony, &data.time
            //         ));
            // }
        }
        else
        {
            ofLogNotice("OSC", "Received unexpected pattern ID %s from %s", pattern_id.c_str(), m.getAddress().c_str());
        }
    }

    void receiveOscMessages(ApplicationData& data, ofxOscReceiver& osc_receiver)
    {
        // check for waiting messages
        while( osc_receiver.hasWaitingMessages() )
        {
            // get the next message
            ofxOscMessage m;
            if (!osc_receiver.getNextMessage(m))
            {
                ofLogWarning("OSC", "Unable to retrieve message");
                break;
            }
            const std::string message_address = m.getAddress();
            // ofLogNotice("OSC", "Received from %s", message_address.c_str());
            if (message_address.compare("/chord_pattern") == 0)
            {
                handleChordPatternMessage(m, data);
            }
            else if (message_address.compare("/bass") == 0)
            {
                handleBassMessage(m, data);
            }
            else if (message_address.compare("/lead") == 0)
            {
                handleLeadMessage(m, data);
            }
            else
            {
                handleUnexpectedMessage(m);
            }
        }
    }
}