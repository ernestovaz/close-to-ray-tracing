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
    Image* image = nullptr;

    if(!application.init()){
        cerr << "ERROR STARTING APPLICATION" << endl;
        return -1;
    }

    while(application.isRunning()){
        application.pollEvents();
        application.clear();

        if(!image || application.shouldDraw()){
            delete image;
            image = new Image(renderer.produceImage(
                    application.width,
                    application.height
            ));
        }

        application.display(*image);
        application.displayMenu(renderer);

        application.present();
    }
    delete image;
    return 0;
}