#define _CRT_SECURE_NO_WARNINGS
#include "callbacks.h"
#include "globals.h"

// 램프 쉐이더 소스
static const char* lamp_vs_source = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
)";

static const char* lamp_fs_source = R"(
#version 330 core
out vec4 FragColor;
uniform vec3 lightColor;
void main() {
    FragColor = vec4(lightColor, 1.0); 
}
)";

static GLuint lampProgramID = 0;

static void initLampShader() {
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &lamp_vs_source, NULL);
    glCompileShader(vs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &lamp_fs_source, NULL);
    glCompileShader(fs);
    lampProgramID = glCreateProgram();
    glAttachShader(lampProgramID, vs);
    glAttachShader(lampProgramID, fs);
    glLinkProgram(lampProgramID);
    glDeleteShader(vs);
    glDeleteShader(fs);
}

static void renderText(float x, float y, void* font, const char* string) {
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glUseProgram(0);
    glRasterPos2f(x, y);
    for (const char* c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    g_Shader->use();
}

namespace Game {
    void display() {
        if (lampProgramID == 0) initLampShader();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        g_Shader->use();
        g_Shader->setVec3("lightPos", swingingLightPos);
        g_Shader->setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));
        g_Shader->setVec3("viewPos", cameraPos);

        // 쉐이더 텍스처 슬롯 지정 (0번: 색상, 1번: 노말)
        g_Shader->setInt("texture_diffuse", 0);
        g_Shader->setInt("texture_normal", 1);

        // 뷰, 프로젝션 행렬 설정
        glm::mat4 view;
        if (isCameraMode) view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        else view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
        g_Shader->setMat4("view", view);
        g_Shader->setMat4("projection", projection);

        // ==========================================
        // 1. 바닥 (Floor) 그리기
        // ==========================================
        glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, texFloor);
        glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, texFloorNormal);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -10.0f, 0.0f)); // 아래로 내림
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // 눕히기
        model = glm::scale(model, glm::vec3(40.0f, 40.0f, 1.0f)); // 크기 40x40
        g_Shader->setMat4("model", model);
        g_Plane->draw();

        // ==========================================
        // 2. 벽 (Walls) 그리기 - 타일 텍스처
        // ==========================================
        glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, texWall);
        glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, texWallNormal);

        // [뒷벽]
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 10.0f, -20.0f));
        model = glm::scale(model, glm::vec3(40.0f, 40.0f, 1.0f)); // 가로 40, 높이 20
        g_Shader->setMat4("model", model);
        g_Plane->draw();

        // [왼쪽 벽]
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-20.0f, 10.0f, 0.0f));
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(40.0f, 40.0f, 1.0f));
        g_Shader->setMat4("model", model);
        g_Plane->draw();

        // [오른쪽 벽]
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(20.0f, 10.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(40.0f, 40.0f, 1.0f));
        g_Shader->setMat4("model", model);
        g_Plane->draw();

        // ==========================================
        // 3. 다트판 (기본 텍스처 + 평면 노말)
        // ==========================================
        // 다트판은 노말맵을 따로 안 만들었으므로 'texDefaultNormal'을 씁니다.
        // (만약 다트판 텍스처 변수가 texBoard라면)
        glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, texBoard);
        glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, texDefaultNormal); // 평평함

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
        model = glm::rotate(model, glm::radians(boardRotAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(5.0f, 5.0f, 1.0f));
        g_Shader->setMat4("model", model);
        g_Board->draw();

        // ==========================================
        // 4. 다트 (OBJ 파일)
        // ==========================================
        glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, texDart); // 녹색
        glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, texDefaultNormal); // 평평함

        model = glm::mat4(1.0f);
        model = glm::translate(model, dartWorldPos);
        float pitch = currentDartAim.x;
        float yaw = currentDartAim.y;
        if (isFired) pitch -= dartTime * 15.0f;
        model = glm::rotate(model, glm::radians(-yaw), glm::vec3(0, 1, 0));
        model = glm::rotate(model, glm::radians(pitch), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1, 0, 0)); // OBJ 방향에 따라 조절
        model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f)); // OBJ 크기 조절
        g_Shader->setMat4("model", model);
        g_DartShape->draw(); // Cylinder 대신 OBJ 메쉬 사용

        // ==========================================
        // [추가] 천장 (Ceiling) 그리기
        // ==========================================
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texCeiling); // 천장 색상 맵

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texCeilingNormal); // 천장 노말 맵

        model = glm::mat4(1.0f);
        // 바닥(y=-10)의 반대편인 y=10 위치에 배치
        model = glm::translate(model, glm::vec3(0.0f, 10.0f, 0.0f));

        // 바닥과 마주보게 뒤집기 (X축 기준 90도 회전)
        // 바닥은 -90도였으니 천장은 90도로 하면 서로 안쪽을 보게 됩니다.
        model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        // 크기는 벽과 동일하게
        model = glm::scale(model, glm::vec3(40.0f, 40.0f, 1.0f));

        g_Shader->setMat4("model", model);

        // 바닥용 평면 객체(10x10 반복)를 재활용해서 그립니다.
        // (만약 g_FloorPlane 변수를 쓰신다면 g_FloorPlane->draw()로 하세요)
        if (g_Plane) g_Plane->draw();

        // 램프
        glUseProgram(lampProgramID);
        glUniformMatrix4fv(glGetUniformLocation(lampProgramID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(lampProgramID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3f(glGetUniformLocation(lampProgramID, "lightColor"), 1.0f, 1.0f, 1.0f);
        model = glm::mat4(1.0f);
        model = glm::translate(model, swingingLightPos);
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(lampProgramID, "model"), 1, GL_FALSE, glm::value_ptr(model));
        g_LightObj->draw();

        g_Shader->use();
        glColor3f(1.0f, 1.0f, 1.0f);
        char scoreBuf[50];
        snprintf(scoreBuf, sizeof(scoreBuf), "Score: %d", gameScore);
        renderText(-0.95f, 0.9f, GLUT_BITMAP_TIMES_ROMAN_24, scoreBuf);

        if (isCameraMode)
            renderText(-0.95f, 0.8f, GLUT_BITMAP_HELVETICA_18, "[Camera Mode] WASD: Move, Mouse: Look, C: Game Mode");
        else {
            renderText(-0.3f, -0.8f, GLUT_BITMAP_HELVETICA_18, scoreMessage);
            renderText(-0.95f, 0.8f, GLUT_BITMAP_HELVETICA_12, "[Game Mode] Click: Shoot, C: Camera Mode");
        }
        glutSwapBuffers();
    }
}