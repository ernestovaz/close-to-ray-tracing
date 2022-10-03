#include "Application.h"

Application::Application() {
    window = nullptr;
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

    return true;
}

void Application::render(Image image) {

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
    SDL_RenderPresent(displayRenderer);
}
