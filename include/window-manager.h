#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <cstdint>
#include "linalg.h"
#include "window-constants.h"

class WindowManager
{
    public:
        static WindowManager& GetInstance();
        SDL_Renderer* GetRenderer() const;

        uint8_t* pixels;

    private:
        WindowManager();
        WindowManager(std::string windowName);
        ~WindowManager();

        WindowManager(const WindowManager&) = delete;
        WindowManager& operator=(const WindowManager&) = delete;

        SDL_Window* window;
        SDL_Renderer* renderer;
};
