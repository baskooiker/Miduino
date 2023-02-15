#include "VleerhondApp.h"
#include "ofAppNoWindow.h"
#include "ofMain.h"

//========================================================================
int main() {
    ofAppNoWindow headless;
    ofSetupOpenGL(&headless, 640, 420, OF_WINDOW);

    ofRunApp(new Vleerhond::VleerhondApp());
}
