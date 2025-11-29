#ifndef GLOBALS_H
#define GLOBALS_H

#include "common.h"
#include "shader_util.h"
#include "objects.h" 

extern unsigned int shaderProgram;
extern int windowWidth;
extern int windowHeight;

// 카메라
extern glm::vec3 cameraPos;
extern glm::vec3 cameraFront;
extern glm::vec3 cameraUp;
extern float cameraYaw;
extern float cameraPitch;
extern bool isCameraMode;

// 다트 상태
extern bool isFired;
extern float dartTime;
extern glm::vec3 dartStartPos;
extern glm::vec3 dartWorldPos;
extern glm::vec3 dartLaunchPos;
extern glm::vec3 dartInitialVel;
extern glm::vec3 currentDartAim;

extern bool isResetting;
extern float resetTimer;

// 조명
extern float lightSwingAngle;
extern glm::vec3 swingingLightPos;

// 객체 포인터
extern Shader* g_Shader;
extern Plane* g_Plane;
extern ObjMesh* g_DartShape;
extern Dartboard* g_Board;
extern Cube* g_LightObj;

// 텍스처 ID
extern GLuint texFloor, texFloorNormal;
extern GLuint texWall, texWallNormal;
extern GLuint texDart;
extern GLuint texBoard;         // [복구] 다트판 텍스처 변수
extern GLuint texDefaultNormal; // 기본 노말 맵

extern float lastFrameTime;
extern float deltaTime;
extern float boardRotAngle;
extern int gameScore;
extern char scoreMessage[100];

void initGlobalObjects();

#endif