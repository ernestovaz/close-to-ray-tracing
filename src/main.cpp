#include <iostream>

#include "application/Application.h"

using std::cerr;
using std::endl;

int main() {
    Application application;
    if(!application.init()){
        cerr << "ERROR STARTING SDL WINDOW" << endl;
        return -1;
    }

    while(application.isRunning()){
        application.pollEvents();
        application.clear();
    }

    return 0;
}