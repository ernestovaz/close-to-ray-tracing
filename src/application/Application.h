#pragma once

#include <SDL2/SDL.h>

#include "../image/Image.h"

class Application {
public:
    Application();
    ~Application();
    bool init();
    bool isRunning();
    void pollEvents();
    void clear();
    void display(Image image);

private:
    bool running = false;

    SDL_Window* window;
    SDL_Renderer* displayRenderer;
    SDL_Texture* texture;

    void handleEvent(SDL_Event* event);
    void initTexture();
};
