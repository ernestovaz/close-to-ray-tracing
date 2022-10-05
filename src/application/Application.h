#pragma once

#include <SDL2/SDL.h>

#include "../image/Image.h"

class Application {
public:
    int width, height;

    Application(int width, int height);
    ~Application();
    bool init();
    bool isRunning();
    void pollEvents();
    void clear();
    void display(Image image);

private:
    bool running = false;
    uint32_t lastFrameTime;

    SDL_Window* window;
    SDL_Renderer* displayRenderer;
    SDL_Texture* texture;

    void handleEvent(SDL_Event* event);
    void deleteTexture();
    void initTexture();
};
