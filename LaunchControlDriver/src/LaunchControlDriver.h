#pragma once

#include "ofxMidiMessage.h"

#include "ofxMidi.h"

#include "ofMain.h"
#include "ofxOsc.h"

#include "lcd_consts.h"

namespace Vleerhond
{
    class LaunchControlDriver :
        public ofBaseApp,
        public ofxMidiListener {

    protected:
        uint64_t clock_counter = 0;
        uint64_t last_processed_counter = 0xFFFFFFFF;
        ControlMode control_mode = ControlMode::CHORD_MODE;
        ControlMode last_processed_mode = ControlMode::CHORD_MODE;

        uint64_t t_last_messages_sent_micros = 0;
        std::queue<ofxOscMessage> pending_messages;


        void receiveOscMessages();
        // void handleNoteOnMessage(const uint8_t channel, const uint8_t pitch, const uint8_t velocity);
        // void handleChordModeMessage(const uint8_t channel, const uint8_t pitch);
        // void handlePatternModeMessage(const uint8_t channel, const uint8_t pitch);
        // void handleNoteOffMessage(const uint8_t channel, const uint8_t pitch);
        void handleCcMessage(std::queue<ofxOscMessage>& message_queue, const uint8_t channel, const uint8_t control, const uint8_t value);

    public:
        LaunchControlDriver();

        ofxOscSender osc_sender;
        ofxOscReceiver osc_receiver;

        ofxMidiOut midi_out;
        ofxMidiIn midi_in;

        bool initializeMidiPorts();

        void setup();
        void update();
        void draw();

        void keyPressed(int key);
        void keyReleased(int key);
        void mouseMoved(int x, int y);
        void mouseDragged(int x, int y, int button);
        void mousePressed(int x, int y, int button);
        void mouseReleased(int x, int y, int button);
        void mouseEntered(int x, int y);
        void mouseExited(int x, int y);
        void windowResized(int w, int h);
        void dragEvent(ofDragInfo dragInfo);
        void exit();
        void gotMessage(ofMessage msg);
        void newMidiMessage(ofxMidiMessage& message);
    };
}
