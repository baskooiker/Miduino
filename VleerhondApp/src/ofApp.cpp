#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    data.randomize_all();
    start_time = ofGetSystemTimeMillis();
}

//--------------------------------------------------------------
void ofApp::update(){
    uint32_t t = ofGetSystemTimeMillis() - start_time;
    // TODO: Variable tempo.
    // 20 ms per tick is about 120 bpm
    uint32_t tick = t / 20;
    for (;data.time.tick < tick; data.time.tick++)
    {
        data.stop_notes_all_instruments();
        data.play_all();
    }
    data.process_events();
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
