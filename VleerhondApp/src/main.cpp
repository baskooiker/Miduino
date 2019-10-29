#include "ofMain.h"
#include "VleerhondApp.h"

#include "ofAppNoWindow.h"

uint32_t millis()
{
    return ofGetSystemTimeMillis();
}

//========================================================================
int main( )
{
    //ofSetupOpenGL(512, 384, OF_WINDOW);

    ofAppNoWindow headless;
    ofSetupOpenGL(&headless, 640, 420, OF_WINDOW);

    ofRunApp(new Vleerhond::VleerhondApp());
}
