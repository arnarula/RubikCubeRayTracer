#include <iostream>

#include "window-manager.h"
#include "game.h"

#include <SDL2/SDL.h>

int main() {
    Game rubikCube;
    rubikCube.Loop();
    return 0;
}
