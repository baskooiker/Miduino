#include "ofMain.h"

#include "LaunchControlDriver.h"

#include "ofAppNoWindow.h"

//========================================================================
int main( )
{
    ofAppNoWindow headless;
    ofSetupOpenGL(&headless, 640, 420, OF_WINDOW);

    ofRunApp(new Vleerhond::LaunchControlDriver());
}
