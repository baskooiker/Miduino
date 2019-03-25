#include "ofMain.h"
#include "VleerhondApp.h"

uint32_t millis()
{
    return ofGetSystemTimeMillis();
}

//========================================================================
int main( ){
    if (true)
    {
        ofSetupOpenGL(1024, 768, OF_WINDOW);			// <-------- setup the GL context
        // this kicks off the running of my app
        // can be OF_WINDOW or OF_FULLSCREEN
        // pass in width and height too:
        ofRunApp(new VleerhondApp());
    }
}