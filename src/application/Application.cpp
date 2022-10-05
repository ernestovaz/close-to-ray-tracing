#include <iostream>
#include "Application.h"

Application::Application(int width, int height)
: width(width), height(height) {

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
            480, 480, SDL_WINDOW_RESIZABLE);

    if(window == nullptr) return false;

    displayRenderer = SDL_CreateRenderer(
            window, -1,SDL_RENDERER_PRESENTVSYNC |
            SDL_RENDERER_ACCELERATED);
    running = true;

    initTexture();
    lastFrameTime = SDL_GetTicks();

    return true;
}

void Application::display(Image image) {
    auto pixels = new uint32_t[image.width * image.height];
    for(int y=0; y < image.height; y++) {
        for(int x=0; x < image.width; x++){
            //flip texture vertically to correct UV coordinates
            Color color = image[x][image.height - y - 1];
            pixels[(y*image.width) + x] = color.toInt();
        }
    }
    SDL_UpdateTexture(texture, nullptr, pixels, image.width * sizeof(uint32_t));
    delete[] pixels;

    SDL_Rect source, destination;
    source.x = 0;
    source.y = 0;
    source.w = image.width;
    source.h = image.height;

    destination.x = 0;
    destination.y = 0;
    destination.w = this->width;
    destination.h = this->height;

    SDL_RenderCopy(displayRenderer, texture,&source, &destination);
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
    switch(event->type){
        case SDL_QUIT:
            running = false;
            break;
        case SDL_WINDOWEVENT:
            if(event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED){
                SDL_GetWindowSize(window, &width, &height);
                deleteTexture();
                initTexture();
            }

            break;
    }
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

    SDL_Surface* surface = SDL_CreateRGBSurface(0, width, height,
        32,RMask, GMask, BMask, AMask);

    texture = SDL_CreateTextureFromSurface(displayRenderer, surface);
    SDL_FreeSurface(surface);
}

void Application::deleteTexture() {
    if(texture != nullptr)
        SDL_DestroyTexture(texture);
}
