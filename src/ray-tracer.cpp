#include "window-manager.h"
#include "game.h"
#include "linalg.h"
#include <iostream>
#include <algorithm>
 
Game::Game() {}
Game::~Game() {}

void Game::Loop() {
    while (true) {
        static int timer = SDL_GetTicks();
        static int frames = 0;

        Input();
        Update();
        Render();

        std::cout << "Frame " << frames++ << ": ";
        std::cout << SDL_GetTicks() - timer << "ms" << std::endl;
        timer = SDL_GetTicks();
    }
}

void Game::Input() {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            exit(0);
        }

        if (event.type == SDL_MOUSEWHEEL) {
            // pan sideways around the cube
            VEC3 distToCube = camera.getEyeDir() - CUBE_CENTER;
            VEC3 newDistToCube = rotateVector(distToCube, ROTATION_SPEED * event.wheel.x * camera.getUpDir());
            camera.setEyeDir(CUBE_CENTER + newDistToCube);

            // pan below/above the cube
            distToCube = camera.getEyeDir() - CUBE_CENTER;
            newDistToCube = rotateVector(distToCube, ROTATION_SPEED * -event.wheel.y * camera.getScreenXDir().normalized());
            camera.setEyeDir(CUBE_CENTER + newDistToCube);
        }

        // get mouse position if mouse has moved
        if (event.type == SDL_MOUSEMOTION) {
            xMouse = event.motion.x;
            yMouse = event.motion.y;
        }
        else
        {
            xMouse = -1;
            yMouse = -1;
        }

        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    exit(0);
                    break;
                case SDLK_LEFT:
                case SDLK_a:
                    rubikCube.RotateFace(Face::LEFT);
                    break;
                case SDLK_RIGHT:
                case SDLK_d:
                    rubikCube.RotateFace(Face::RIGHT);
                    break;
                case SDLK_UP:
                case SDLK_w:
                    rubikCube.RotateFace(Face::TOP);
                    break;
                case SDLK_DOWN:
                case SDLK_s:
                    rubikCube.RotateFace(Face::BOTTOM);
                    break;
                case SDLK_q:
                    rubikCube.RotateFace(Face::FRONT);
                    break;
                case SDLK_e:
                    rubikCube.RotateFace(Face::BACK);
                    break;
            }
        }
    }
}

void Game::Update() {
    for (int i = 0; i < CUBE_DIMENSIONS_SIZE; i++) {
        for (int j = 0; j < CUBE_DIMENSIONS_SIZE; j++) {
            for (int k = 0; k < CUBE_DIMENSIONS_SIZE; k++) {
                rubikCube.cube[i][j][k].selected = false;
            }
        }
    }

    // mark selected block
    if (xMouse != -1 && yMouse != -1) {
        int x = 1.0 * xMouse / WINDOW_WIDTH * X_RES;
        int y = 1.0 * yMouse / WINDOW_HEIGHT * Y_RES;
        VEC3 screenTap = camera.getScreenTopLeft() + x * camera.getScreenXStep() + y * camera.getScreenYStep();
        VEC3 rayDir = (screenTap - camera.getEyeDir()).normalized();
        ComputePixelColor(rayDir, true);
    }
}

void Game::Render() {
    auto& windowManager = WindowManager::GetInstance();

    // we use ray tracing to render the scene pixel by pixel
    for (int y = 0; y < Y_RES; y++) {
        for (int x = 0; x < X_RES; x++) {
            VEC3 rayDir = camera.getScreenTopLeft() 
                        + (x + 0.5) * camera.getScreenXStep()
                        + (y + 0.5) * camera.getScreenYStep();
            rayDir = (rayDir - camera.getEyeDir()).normalized();
            VEC3 color = ComputePixelColor(rayDir);

            for (int i = 0; i < 3; i++) {
                windowManager.pixels[(y * X_RES + x) * 3 + i] = color[i] * 255;
            }
        }
    }

    auto renderer = windowManager.GetRenderer();
    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGB24,
        SDL_TEXTUREACCESS_STREAMING,
        X_RES,
        Y_RES
    );
    SDL_UpdateTexture(texture, NULL, windowManager.pixels, X_RES * 3);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(texture);
}

VEC3 Game::ComputePixelColor(const VEC3& rayDir, bool markSelected) {
    VEC3 color;
    double minTime = DBL_MAX;

    // speed optimization 1: check intersection with bounding box of the cube before checking individual blocks
    static Block boundingBox(CUBE_CENTER, CUBE_DIMENSIONS_SIZE * (CUBE_BLOCK_SCALE + CUBE_BLOCK_GAP));
    if (!RayIntersectsCube(rayDir, boundingBox, minTime, color)) {
        return VEC3(0, 0, 0);
    }
    else
    {
        color = VEC3(0, 0, 0);
        minTime = DBL_MAX;
    }

    // speed optimization 2: sort the blocks by distance to the camera
    std::vector<std::pair<int, double>> blocksByDistance;
    for (int i = 0; i < CUBE_DIMENSIONS_SIZE; i++) {
        for (int j = 0; j < CUBE_DIMENSIONS_SIZE; j++) {
            for (int k = 0; k < CUBE_DIMENSIONS_SIZE; k++) {
                // skip rendering blocks inside the cube
                if (i > 0 && i < CUBE_DIMENSIONS_SIZE - 1 &&
                    j > 0 && j < CUBE_DIMENSIONS_SIZE - 1 &&
                    k > 0 && k < CUBE_DIMENSIONS_SIZE - 1) {
                    continue;
                }

                blocksByDistance.push_back({
                    i * CUBE_DIMENSIONS_SIZE * CUBE_DIMENSIONS_SIZE + j * CUBE_DIMENSIONS_SIZE + k,
                    (rubikCube.cube[i][j][k].position - camera.getEyeDir()).norm()
                });
            }
        }
    }
    std::sort(blocksByDistance.begin(), blocksByDistance.end(), [](const auto& block1, const auto& block2) {
        return block1.second < block2.second;
    });

    // render closest block that intersects with the ray 
    double time = DBL_MAX;
    VEC3 blockColor = VEC3(0, 0, 0);
    for (int b = 0; b < blocksByDistance.size(); b++) {
        int blockIndex = blocksByDistance[b].first;
        int i = blockIndex / (CUBE_DIMENSIONS_SIZE * CUBE_DIMENSIONS_SIZE);
        int j = (blockIndex % (CUBE_DIMENSIONS_SIZE * CUBE_DIMENSIONS_SIZE)) / CUBE_DIMENSIONS_SIZE;
        int k = blockIndex % CUBE_DIMENSIONS_SIZE;

        Block& block = rubikCube.cube[i][j][k];
        if (RayIntersectsCube(rayDir, block, time, blockColor)) {
            if (markSelected) {
                block.selected = true;
                return blockColor;
            }
            return blockColor * (block.selected ? SELECTED_BLOCK_DAMPENING_PCT : 1.0);
        }
    }
    
    return color;
}

bool Game::RayIntersectsCube(const VEC3& rayDir, Block& block, double& minTime, VEC3& color) {
    const double halfBlockScale = block.scale / 2.0;
    
    // check if ray intersects one of the 6 faces (3 pairs of parallel faces)
    for (int i = 0; i < 3; i++)
    {
        const auto& axis = block.axes.col(i);
        for (int j = 0; j < 2; j++)
        {
            VEC3 faceCenter = block.position + (j == 0 ? 1 : -1) * axis * halfBlockScale;
            double timeToHit = (faceCenter - camera.getEyeDir()).dot(axis) / rayDir.dot(axis);
            VEC3 hitPoint = camera.getEyeDir() + timeToHit * rayDir;
            VEC3 distanceToHit = hitPoint - faceCenter;

            double dist1 = distanceToHit.dot(block.axes.col((i + 1) % 3));
            double dist2 = distanceToHit.dot(block.axes.col((i + 2) % 3));

            if (dist1 >= -halfBlockScale && dist1 <= halfBlockScale &&
                dist2 >= -halfBlockScale && dist2 <= halfBlockScale)
            {
                if (timeToHit < minTime) {
                    minTime = timeToHit;
                    color = CUBE_FACE_COLORS[2 * i + j];
                }
            }
        }
    }

    // if no intersection found
    if (minTime == DBL_MAX) {
        return false;
    }

    return true;
}
