#include <iostream>

#include "application/Application.h"
#include "renderer/Renderer.h"

#define INITIAL_WIDTH   480
#define INITIAL_HEIGHT  480

using std::cerr;
using std::endl;

int main() {
    Application application(INITIAL_WIDTH, INITIAL_HEIGHT);
    Renderer renderer;

    if(!application.init()){
        cerr << "ERROR STARTING APPLICATION" << endl;
        return -1;
    }

    while(application.isRunning()){
        application.pollEvents();
        Image image = renderer.render(
                application.width,
                application.height
        );
        application.clear();
        application.display(image);
    }

    return 0;
}