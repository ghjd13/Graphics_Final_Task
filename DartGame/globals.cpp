#include "globals.h"
#include "texture_util.h" 

unsigned int shaderProgram = 0;
int windowWidth = 800;
int windowHeight = 600;

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 15.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float cameraYaw = -90.0f;
float cameraPitch = 0.0f;
bool isCameraMode = false;

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
Plane* g_Plane = nullptr;
ObjMesh* g_DartShape = nullptr;
Dartboard* g_Board = nullptr;
Cube* g_LightObj = nullptr;

// 텍스처 ID 초기화
GLuint texFloor = 0, texFloorNormal = 0;
GLuint texWall = 0, texWallNormal = 0;
GLuint texDart = 0;
GLuint texBoard = 0; // [복구] 초기화
GLuint texCeiling = 0;
GLuint texCeilingNormal = 0;
GLuint texDefaultNormal = 0;

float lastFrameTime = 0.0f;
float deltaTime = 0.0f;
float boardRotAngle = 0.0f;

int gameScore = 0;
char scoreMessage[100] = "Ready";

void initGlobalObjects() {
    glEnable(GL_DEPTH_TEST);

    g_Shader = new Shader("vertex.glsl", "fragment.glsl");
    shaderProgram = g_Shader->ID;

    // 객체 생성
    g_Plane = new Plane(10.0f);

    // [경로 확인] resource 폴더 안의 파일명으로 설정 (한글->영어 변경 필수)
    g_DartShape = new ObjMesh("resource/dart.obj");

    g_Board = new Dartboard();
    g_LightObj = new Cube();

    // 텍스처 로드
    texFloor = loadTexture("resource/uvMap.png");
    texFloorNormal = loadTexture("resource/normalMap.png");

    texWall = loadTexture("resource/wallUV.png");
    texWallNormal = loadTexture("resource/wallNormal.png");

    texCeiling = loadTexture("resource/roofUV.png");
    texCeilingNormal = loadTexture("resource/roofNormal.png");

    texDart = loadTexture("resource/dart.png"); // 다트 텍스처 (다트.png -> dart.png)

    // [복구] 다트판 텍스처 생성
    // 만약 다트판 이미지도 있다면: loadTexture("resource/board.png"); 로 바꾸세요.
    // 지금은 이미지가 없는 것 같아 기존의 빨간색 생성 코드를 넣습니다.
    texBoard = createColorTexture(120, 0, 0);

    // 노말 맵 없는 물체용 (다트판, 다트 등)
    texDefaultNormal = createFlatNormalTexture();
}