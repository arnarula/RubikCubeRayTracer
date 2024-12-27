#include <SDL2/SDL.h>
#include <string>
#include "window-manager.h"
#include "window-constants.h"

WindowManager::WindowManager(std::string windowName)
{
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow(windowName.c_str(), 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    pixels = new u_int8_t[X_RES * Y_RES * 3];
    memset(pixels, 0, X_RES * Y_RES * 3);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_RaiseWindow(window);
}

WindowManager::~WindowManager() {
    delete[] pixels;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

WindowManager& WindowManager::GetInstance() {
    static WindowManager instance = WindowManager("Rubik's Cube");
    return instance;
}

SDL_Renderer* WindowManager::GetRenderer() const {
    return renderer;
}
