#pragma once

#include <SDL2/SDL.h>

#include "../image/Image.h"
#include "../renderer/Renderer.h"

class Application {
public:
    int width, height;

    Application(int width, int height);
    ~Application();

    bool init();
    bool isRunning();
    bool shouldDraw();
    void pollEvents();
    void clear();
    void display(Image image);

    void displayMenu(Renderer& renderer);
    void setMenuItemsFaded();
    void unsetMenuItemsFaded();

    void present();

private:
    bool running = false;
    bool realTime = false;
    bool drawSingleFrame = false;

    SDL_Window* window;
    SDL_Renderer* displayRenderer;
    SDL_Texture* texture;
    float textureWidth, textureHeight;

    void handleEvent(SDL_Event* event);
    void initImGui();
    void updateTexture(float width, float height);
    void deleteTexture();
};
