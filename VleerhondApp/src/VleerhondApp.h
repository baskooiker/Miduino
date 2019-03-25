#pragma once

#include "ofxMidiMessage.h"

#include "ofMain.h"
#include "print_midi.h"

#include "application_data.h"

class VleerhondApp : 
    public ofBaseApp,
    public ofxMidiListener {
	public:
        ApplicationData data;

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
        void newMidiMessage(ofxMidiMessage& message);
};