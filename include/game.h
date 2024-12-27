#pragma once
#include "camera.h"
#include "rubik-cube.h"

class Game
{
    public:
        Game();
        ~Game();
        void Loop();

    private:
        Camera camera;
        RubikCube rubikCube;
        Block* selectedBlock = nullptr;

        int xMouse = 0, yMouse = 0;

        void Init();
        void Input();
        void Update();
        void Render();

        VEC3 ComputePixelColor(const VEC3& rayDir, bool markSelected = false);
        bool RayIntersectsCube(const VEC3& rayDir, Block& block, double& time, VEC3& color);
};
