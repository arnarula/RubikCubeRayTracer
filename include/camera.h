#pragma once
#include "linalg.h"
#include "camera-constants.h"

class Camera
{
    private:
        VEC3 eyeDir = EYE;
        VEC3 lookAtDir = LOOKAT;
        VEC3 upDir = UP;

        // screen properties
        VEC3 screenTopLeft; // top left corner of the screen
        VEC3 xScreenDir, yScreenDir; // screen direction vectors in 3D space
        VEC3 xStep, yStep; // step size based on screen resolution (X_RES, Y_RES)

        const double fov = FOV;
        const double aspect = ASPECT;

        void updateState()
        {
            double distToNearPlane = (lookAtDir - eyeDir).norm(); 
            VEC3 eyeToScreen = (lookAtDir - eyeDir).normalized();

            VEC3 xDir = eyeToScreen.cross(upDir).normalized();
            upDir = xDir.cross(eyeToScreen).normalized();

            // keep camera orientation consistent
            static VEC3 lastUpDir = upDir;
            if ((upDir - lastUpDir).norm() > upDir.norm()) upDir *= -1;
            lastUpDir = upDir;

            yScreenDir = -tan(fov / 2.0f) * distToNearPlane * upDir * 2.0;
            xScreenDir = yScreenDir.cross(eyeToScreen) * aspect;
            xStep = xScreenDir / X_RES;
            yStep = yScreenDir / Y_RES;

            screenTopLeft = lookAtDir - xScreenDir / 2.0 - yScreenDir / 2.0;
        }
    
    public:
        Camera()
        {
            updateState();
        }

        const VEC3& getEyeDir() const
        {
            return eyeDir;
        }

        void setEyeDir(const VEC3& eye)
        {
            eyeDir = eye;
            updateState();
        }

        const VEC3& getLookAtDir() const
        {
            return lookAtDir;
        }

        void setLookAtDir(const VEC3& lookAt)
        {
            lookAtDir = lookAt;
            updateState();
        }

        const VEC3& getUpDir() const
        {
            return upDir;
        }

        void setUpDir(const VEC3& up)
        {
            upDir = up;
            updateState();
        }

        const VEC3& getScreenTopLeft() const
        {
            return screenTopLeft;
        }

        const VEC3& getScreenXDir() const
        {
            return xScreenDir;
        }

        const VEC3& getScreenYDir() const
        {
            return yScreenDir;
        }

        const VEC3& getScreenXStep() const
        {
            return xStep;
        }

        const VEC3& getScreenYStep() const
        {
            return yStep;
        }
};
