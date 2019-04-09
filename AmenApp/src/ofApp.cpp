#include "ofApp.h"
#include "ofxBpm.h"

#include "rand.h"

std::string MODULE = "ofApp";

typedef struct 
{
    std::string file_name;
    uint8_t length;
    uint8_t swing;
} LoopStruct;

LoopStruct samples[] = {
    {"data/AmenBreak.wav",            64, 0},
    {"data/slowdown.wav",             32, 0},
    {"data/stone_fox_chase.wav",      48, 0},
    {"data/teamo.wav",                32, 0},

    //{"data/lovedrops.wav",            32, 0},
    //{"data/sexy_coffee_pot.wav",     128, 0},
    //{"data/soul_chicken.wav",         32, 0},
    //{"data/speed_kills.wav",          32, 0},
    //{"data/walk_this_way.wav",        32, 0},
    //{"data/long_one.wav",             16, 0},
    //{"data/got_to_get_me_a_job.wav",  32, 0},
};
static const uint8_t nr_of_samples = sizeof(samples) / sizeof(*samples);

LoopStruct loaded_sample;

void ofApp::setup(){
    ofLogToConsole();

    srand(ofGetSystemTime());

    avg_pulse_time = 500;
    last_tick = 0;

    ofAddListener(this->bpm.beatEvent, this, &ofApp::play);
    bpm.setBpm(160 * 24);
    bpm.setBeatPerBar(4);

    sample_buffer.setVerbose(true);
    loaded_sample = samples[Rand::randui8(nr_of_samples)];
    sample_buffer.load(loaded_sample.file_name);

    sampler.addSample(&sample_buffer, 0);

    gate_ctrl.out_trig() >> env;
                            env >> amp.in_mod();

    sampler >> amp * dB(-12.0f) >> engine.audio_out(0);
               amp * dB(-12.0f) >> engine.audio_out(1);

    .0f   >> env.in_attack();
    500.0f >> env.in_hold();
    10.0f  >> env.in_release();

    gate_ctrl.out_trig() >> sampler.in_trig();
    start_ctrl >> sampler.in_start();
    pitch_ctrl >> sampler.in_pitch();

    repitch_sample(125.);

    // set up the audio output device
    engine.listDevices();
    engine.setDeviceID(0); // REMEMBER TO SET THIS AT THE RIGHT INDEX!!!!
    // start your audio engine !
    engine.setup(44100, 512, 3);
}

void ofApp::repitch_sample(float pulse_time)
{
    // Repitch
    int samples_per_step = sample_buffer.length / loaded_sample.length;
    int orig_samples_per_step = (sample_buffer.fileSampleRate * pulse_time) / 1000;
    double cents = 12. * std::log2((double)samples_per_step / (double)orig_samples_per_step) / log2(2);
    pitch_ctrl.set(cents);
    ofLogNotice(MODULE, "samples_per_step      : %d", samples_per_step);
    ofLogNotice(MODULE, "orig_samples_per_step : %d", orig_samples_per_step);
    ofLogNotice(MODULE, "cents                 : %f", (double)cents);
}

void ofApp::playSample(int index)
{
    repitch_sample(avg_pulse_time / 4.);
    start_ctrl.set((index % loaded_sample.length) / (double)loaded_sample.length);
    gate_ctrl.trigger(1);
}

void ofApp::play()
{
    const int NR_STEPS = 4;
    if (count % (6 * NR_STEPS) == 0)
    {
        uint64_t t = ofGetSystemTime();
        if (count <= (6 * NR_STEPS) + 1)
        {
            avg_pulse_time = t - last_tick;
        }
        if (last_tick > 0)
        {
            avg_pulse_time = avg_pulse_time * .5 + (t - last_tick) * .5;
        }
        last_tick = t;

        float gate_time = (NR_STEPS * avg_pulse_time);
        gate_time * .75 >> env.in_hold();
        gate_time * 1.0 >> env.in_release();
        playSample(count / 6);

        ofLogVerbose(MODULE, "pulse time : %f", avg_pulse_time);
    }
    count++;
}

//--------------------------------------------------------------
void ofApp::update()
{
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    switch (key)
    {
    case 'p':
        count = 0;
        bpm.start();
        ofLogNotice(MODULE, "play");
        break;
    case 's':
        bpm.stop();
        count = 0;
        last_tick = 0;
        ofLogNotice(MODULE, "stop");
        break;
    case 'a':
        break;
    case 'z':
        playSample(0);
        break;
    case 'x':
        playSample(1);
        break;
    case 'c':
        playSample(2);
        break;
    case 'v':
        playSample(3);
        break;
    case 'b':
        playSample(4);
        break;
    case 'n':
        playSample(5);
        break;
    case 'm':
        playSample(6);
        break;
    case ',':
        playSample(7);
        break;
    case 'q':
        ofExit();
        break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void ofApp::exit()
{
    bpm.stop();
}
