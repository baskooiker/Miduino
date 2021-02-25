#include "ofMain.h"

#include "VleerhondApp.hpp"

uint32_t millis()
{
    return ofGetSystemTimeMillis();
}

//========================================================================
int main(){
    //ofAppNoWindow headless;
    //ofSetupOpenGL(&headless, 640, 420, OF_WINDOW);
    //ofRunApp(new Vleerhond::VleerhondApp());
    Vleerhond::VleerhondApp app;
    app.setup();
    while(app.stateOk())
    {
        app.update();
    }
    std::cout << "Done...\n";
}
