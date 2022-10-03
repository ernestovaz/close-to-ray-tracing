#pragma once

#include <SDL2/SDL.h>

#include "../Image.h"

class Application {
public:
    Application();
    ~Application();
    bool init();
    bool isRunning();
    void pollEvents();
    void clear();
    void render(Image image);

private:
    bool running = false;

    SDL_Window* window;
    SDL_Renderer* displayRenderer;

    void handleEvent(SDL_Event* event);
};
