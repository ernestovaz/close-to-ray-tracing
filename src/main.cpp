#include <iostream>

#include "application/Application.h"
#include "renderer/Renderer.h"

using std::cerr;
using std::endl;

int main() {
    Application application;
    Renderer renderer;

    if(!application.init()){
        cerr << "ERROR STARTING SDL WINDOW" << endl;
        return -1;
    }

    while(application.isRunning()){
        application.pollEvents();
        application.clear();

        Image image = renderer.render();
        application.display(image);
    }

    return 0;
}