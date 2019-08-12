#include "ofMain.h"

#include "ofAppNoWindow.h"

#include "VleerhondApp.hpp"

uint32_t millis()
{
    return ofGetSystemTimeMillis();
}

//========================================================================
int main( ){
    if (true)
    {
        ofSetupOpenGL(512, 384, OF_WINDOW);			// <-------- setup the GL context
        // this kicks off the running of my app
        // can be OF_WINDOW or OF_FULLSCREEN
        // pass in width and height too:
        ofRunApp(new Vleerhond::VleerhondApp());
    }
}
