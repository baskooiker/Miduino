#pragma once

#include "ofMain.h"

#include "ofxBpm.h"

#include "ofxPDSP.h"

class ofApp : public ofBaseApp
{
    uint32_t count;
    float avg_pulse_time;
    uint64_t last_tick;

    ofxBpm bpm;

    pdsp::Engine engine;
    pdsp::SampleBuffer sample_buffer;
    pdsp::Sampler sampler;
    pdsp::TriggerControl gate_ctrl;
    pdsp::ValueControl start_ctrl;
    pdsp::ValueControl pitch_ctrl;
    pdsp::Amp amp;
    pdsp::AHR env;

	public:
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
        void exit();

        void play();
        void repitch_sample(float pulse_time);
        void playSample(int index);
};
