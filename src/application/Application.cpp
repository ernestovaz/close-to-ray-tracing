#include <iostream>
#include <string>

#include "Application.h"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl.h>
#include "imgui/imgui_impl_sdlrenderer.h"
#include "../renderer/Renderer.h"
#include "imgui/imgui_internal.h"
#include <SDL.h>
#include <SDL_opengl.h>

#include <glm/gtc/type_ptr.hpp>
using glm::value_ptr;
using std::string;
using std::to_string;

Application::Application(int width, int height)
: width(width), height(height) {

    window = nullptr;
    texture = nullptr;
    displayRenderer = nullptr;
}

Application::~Application() {
    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(displayRenderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    window = nullptr;
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

    updateTexture(width, height);
    initImGui();

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
    if(image.width != textureWidth || image.height != textureHeight)
        updateTexture(image.width, image.height);

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
    ImGui_ImplSDL2_ProcessEvent(event);
    switch(event->type){
        case SDL_QUIT:
            running = false;
            break;
        case SDL_WINDOWEVENT:
            if(event->window.event == SDL_WINDOWEVENT_SIZE_CHANGED){
                SDL_GetWindowSize(window, &width, &height);
            }
            break;
    }
}

void Application::clear() {
    SDL_SetRenderDrawColor(displayRenderer, 59, 59, 59, 255);
    SDL_RenderClear(displayRenderer);
}

void Application::updateTexture(float width, float height) {
    textureWidth = width;
    textureHeight = height;

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

void Application::initImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(window, displayRenderer);
    ImGui_ImplSDLRenderer_Init(displayRenderer);

}

static bool firstMenuDraw = true;
void Application::displayMenu(Renderer &renderer) {
    ImGui_ImplSDLRenderer_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
    {
        if(firstMenuDraw) {
            ImGui::SetNextWindowCollapsed(true);
            firstMenuDraw = false;
        }
        ImGui::Begin("Debug");

        if(realTime) setMenuItemsFaded();
        {
            if (ImGui::Button("Draw")) drawSingleFrame = true;
        }
        if(realTime) unsetMenuItemsFaded();

        ImGui::SameLine();
        ImGui::Checkbox("Real Time", &realTime);
        if(!realTime) setMenuItemsFaded();
        {
            ImGui::SameLine();
            float fps;
            if(realTime) fps = ImGui::GetIO().Framerate;
            else fps = 0;
            ImGui::Text("%.3f FPS", fps);
        }
        if(!realTime) unsetMenuItemsFaded();

        ImGui::DragFloat2("Scale", value_ptr(renderer.scale), 0.01f, 0.0f, 1.0f);

        vector<Object*> scene = renderer.getScene();
        for(int i=0; i<scene.size(); i++){
            Object* object = scene[i];
            string label = "Shape " + to_string(i);
            ImGui::DragFloat3(label.c_str(), value_ptr(object->position), 0.01f);
        }
        Camera& camera = renderer.getCamera();
        ImGui::Text("Camera");
        ImGui::DragFloat3("Position", value_ptr(camera.position), 0.01f);
        //camera orientation
        if(ImGui::ArrowButton("left", 0))
            renderer.setCameraDirection(vec3(-1.0f, 0.0f, 0.0f));
        ImGui::SameLine();
        if(ImGui::ArrowButton("right", 1))
            renderer.setCameraDirection(vec3(1.0f, 0.0f, 0.0f));
        ImGui::SameLine();
        if(ImGui::ArrowButton("front", 2))
            renderer.setCameraDirection(vec3(0.0f, 0.0f, -1.0f));
        ImGui::SameLine();
        if(ImGui::ArrowButton("back", 3))
            renderer.setCameraDirection(vec3(0.0f, 0.0f, 1.0f));
        ImGui::SameLine();
        ImGui::Text("Direction");


        ImGui::End();
    }
    ImGui::Render();
    ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
}

void Application::present() {
    SDL_RenderPresent(displayRenderer);
}

void Application::setMenuItemsFaded() {
    ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
    ImGui::PushStyleVar(ImGuiStyleVar_Alpha, ImGui::GetStyle().Alpha * 0.5f);
}

void Application::unsetMenuItemsFaded() {
    ImGui::PopItemFlag();
    ImGui::PopStyleVar();
}

bool Application::shouldDraw() {
    bool response = (drawSingleFrame || realTime);
    if (drawSingleFrame) drawSingleFrame = false;
    return response;
}
