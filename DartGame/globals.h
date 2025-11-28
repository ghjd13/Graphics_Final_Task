#ifndef GLOBALS_H
#define GLOBALS_H

#include "common.h"
#include "shader_util.h"
#include "objects.h" 

extern unsigned int shaderProgram;
extern int windowWidth;
extern int windowHeight;

// [카메라 관련 변수 추가]
extern glm::vec3 cameraPos;
extern glm::vec3 cameraFront;
extern glm::vec3 cameraUp;
extern float cameraYaw;
extern float cameraPitch;
extern bool isCameraMode; // 카메라 모드 토글 (조준 vs 이동)

extern bool isFired;
extern float dartTime;
extern glm::vec3 dartStartPos;
extern glm::vec3 dartWorldPos;
extern glm::vec3 dartLaunchPos;
extern glm::vec3 dartInitialVel;
extern glm::vec3 currentDartAim;

extern bool isResetting;
extern float resetTimer;

extern float lightSwingAngle;
extern glm::vec3 swingingLightPos;

// 객체 포인터
extern Shader* g_Shader;
extern Cube* g_Room;
extern Cylinder* g_DartShape;
extern Dartboard* g_Board;
extern Cube* g_LightObj;

extern GLuint texRoom, texBoard, texDart, texLight;

extern float lastFrameTime;
extern float deltaTime;
extern float boardRotAngle;
extern int gameScore;
extern char scoreMessage[100];

void initGlobalObjects();

#endif