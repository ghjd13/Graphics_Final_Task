#include "globals.h"
#include "texture_util.h" 

unsigned int shaderProgram = 0;
int windowWidth = 800;
int windowHeight = 600;

// [카메라 초기화]
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 15.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float cameraYaw = -90.0f; // -Z 방향을 보려면 -90도 시작
float cameraPitch = 0.0f;
bool isCameraMode = false; // 기본은 다트 조준 모드

bool isFired = false;
float dartTime = 0.0f;
glm::vec3 dartStartPos = glm::vec3(0.0f, -1.0f, 12.0f);
glm::vec3 dartWorldPos = dartStartPos;
glm::vec3 dartLaunchPos = dartStartPos;
glm::vec3 dartInitialVel = glm::vec3(0.0f);
glm::vec3 currentDartAim = glm::vec3(0.0f);

bool isResetting = false;
float resetTimer = 0.0f;

float lightSwingAngle = 0.0f;
glm::vec3 swingingLightPos = glm::vec3(0.0f, 4.0f, 5.0f);

Shader* g_Shader = nullptr;
Cube* g_Room = nullptr;
Cylinder* g_DartShape = nullptr;
Dartboard* g_Board = nullptr;
Cube* g_LightObj = nullptr;

GLuint texRoom = 0, texBoard = 0, texDart = 0, texLight = 0;

float lastFrameTime = 0.0f;
float deltaTime = 0.0f;
float boardRotAngle = 0.0f;

int gameScore = 0;
char scoreMessage[100] = "Ready";

void initGlobalObjects() {
    glEnable(GL_DEPTH_TEST);

    g_Shader = new Shader("vertex.glsl", "fragment.glsl");
    shaderProgram = g_Shader->ID;

    g_Room = new Cube();
    g_DartShape = new Cylinder();
    g_Board = new Dartboard();
    g_LightObj = new Cube();

    texRoom = createColorTexture(200, 200, 200);
    texBoard = createColorTexture(120, 0, 0);
    texDart = createColorTexture(0, 255, 0);
}