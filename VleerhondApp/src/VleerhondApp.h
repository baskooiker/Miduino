#pragma once

#include "ofxMidiMessage.h"

#include "ofMain.h"

#include "application_data.h"

namespace Vleerhond
{
    class VleerhondApp :
        public ofBaseApp,
        public ofxMidiListener {

    protected:
        int stop_counter = 0;
    public:
        ApplicationData data;

        bool initializeMidiPorts();

        void setup();
        //void play();
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