#pragma once

#include "ofxMidiMessage.h"

#include "ofxMidiOut.h"

#include "ofMain.h"
#include "ofxOsc.h"

namespace Vleerhond
{
    class TestApp :
        public ofBaseApp,
        public ofxMidiListener {

    protected:
        uint64_t clock_counter = 0;
        uint64_t start_time = 0;

    public:
        ofxOscReceiver receiver;

        ofxMidiOut midi_out;

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
