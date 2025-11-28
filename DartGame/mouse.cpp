#define _CRT_SECURE_NO_WARNINGS
#include "globals.h"

namespace Game {
    void mouse(int button, int state, int x, int y) {
        if (isCameraMode) return;
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            if (!isFired && !isResetting) {
                isFired = true;
                dartTime = 0.0f;
                dartLaunchPos = dartWorldPos;
                float pitch = glm::radians(currentDartAim.x);
                float yaw = glm::radians(currentDartAim.y);
                glm::vec3 dir;
                dir.x = sin(yaw) * cos(pitch);
                dir.y = sin(pitch);
                dir.z = -cos(yaw) * cos(pitch);
                float power = 25.0f;
                dartInitialVel = glm::normalize(dir) * power;
                snprintf(scoreMessage, sizeof(scoreMessage), "Flying...");
            }
        }
    }
}