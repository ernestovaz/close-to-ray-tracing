#include "Application.h"
#include <cstdio>

#define WIDTH 480
#define HEIGHT 480

Application::Application() {
    window = nullptr;
    texture = nullptr;
    displayRenderer = nullptr;
}

Application::~Application() {
    SDL_DestroyRenderer(displayRenderer);
    SDL_DestroyWindow(window);
    window = nullptr;
    SDL_Quit();
}

bool Application::init() {
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) return false;

    window = SDL_CreateWindow("Render",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            480, 480, SDL_WINDOW_SHOWN);

    if(window == nullptr) return false;

    displayRenderer = SDL_CreateRenderer(window, -1, 0);
    running = true;

    initTexture();
    return true;
}

void Application::display(Image image) {
    auto pixels = new uint32_t[WIDTH * HEIGHT];
    for(int x=0; x<WIDTH; x++) {
        for(int y=0; y<HEIGHT; y++){
            pixels[(y*WIDTH)+x] = image[x][y].toInt();
        }
    }
    SDL_UpdateTexture(texture, nullptr, pixels, WIDTH * sizeof(uint32_t));
    delete[] pixels;

    SDL_Rect screen, bounds;
    screen.x = 0;
    screen.y = 0;
    screen.w = WIDTH;
    screen.h = HEIGHT;
    bounds = screen;

    SDL_RenderCopy(displayRenderer, texture,
                   &screen, &bounds);
    SDL_RenderPresent(displayRenderer);
}

bool Application::isRunning() {
    return running;
}

void Application::pollEvents() {
    SDL_Event event;
    while(SDL_PollEvent(&event) != 0){
        handleEvent(&event);
    }
}

void Application::handleEvent(SDL_Event *event) {
    if(event->type == SDL_QUIT) running = false;
}

void Application::clear() {
    SDL_SetRenderDrawColor(displayRenderer, 59, 59, 59, 255);
    SDL_RenderClear(displayRenderer);
}

void Application::initTexture() {
    uint32_t RMask, GMask, BMask, AMask;
    RMask = 0x000000ff;
    GMask = 0x0000ff00;
    BMask = 0x00ff0000;
    AMask = 0xff000000;

    SDL_Surface* surface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32,
        RMask, GMask, BMask, AMask);

    texture = SDL_CreateTextureFromSurface(displayRenderer, surface);
    SDL_FreeSurface(surface);
}
