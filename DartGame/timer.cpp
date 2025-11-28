#define _CRT_SECURE_NO_WARNINGS
#include "globals.h"
#include "callbacks.h"

namespace Game {
    void timer(int value) {
        float currentTime = glutGet(GLUT_ELAPSED_TIME) * 0.001f;
        deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        boardRotAngle += 100.0f * deltaTime;
        if (boardRotAngle > 360.0f) boardRotAngle -= 360.0f;

        lightSwingAngle = sin(currentTime * 2.0f);
        swingingLightPos.x = sin(lightSwingAngle) * 8.0f;

        if (isResetting) {
            resetTimer += deltaTime;
            if (resetTimer >= 1.0f) {
                isResetting = false;
                resetTimer = 0.0f;
                dartWorldPos = dartStartPos;
                currentDartAim = glm::vec3(0.0f);
                dartTime = 0.0f;
                snprintf(scoreMessage, sizeof(scoreMessage), "Ready");
            }
        }
        else if (isFired) {
            dartTime += deltaTime;
            glm::vec3 gravity(0.0f, -3.0f, 0.0f);
            dartWorldPos = dartLaunchPos + (dartInitialVel * dartTime) + (0.5f * gravity * dartTime * dartTime);

            if (dartWorldPos.z < -9.5f) {
                isFired = false;
                dartWorldPos.z = -9.5f;
                isResetting = true;

                float dist = sqrt(dartWorldPos.x * dartWorldPos.x + dartWorldPos.y * dartWorldPos.y);
                float angle = atan2(dartWorldPos.y, dartWorldPos.x);
                float angleDeg = glm::degrees(angle);
                if (angleDeg < 0) angleDeg += 360.0f;

                float relativeAngle = angleDeg - boardRotAngle;
                while (relativeAngle < 0) relativeAngle += 360.0f;
                while (relativeAngle >= 360.0f) relativeAngle -= 360.0f;

                if (dist > 2.5f) {
                    snprintf(scoreMessage, sizeof(scoreMessage), "Miss...");
                }
                else {
                    int section = (int)(relativeAngle / 30.0f) + 1;
                    int points = section * 10;
                    if (dist < 0.2f) { points = 200; snprintf(scoreMessage, sizeof(scoreMessage), "BULLSEYE! (+200)"); }
                    else { snprintf(scoreMessage, sizeof(scoreMessage), "Hit Section %d (+%d pts)", section, points); }
                    gameScore += points;
                }
            }
            if (dartWorldPos.y < -8.0f) {
                isFired = false;
                dartWorldPos.y = -8.0f;
                isResetting = true;
                snprintf(scoreMessage, sizeof(scoreMessage), "Floor...");
            }
        }
        glutPostRedisplay();
        // 주의: 여기서 ::timer는 callbacks.h의 전역 콜백 함수입니다.
        glutTimerFunc(16, ::timer, 0);
    }
}