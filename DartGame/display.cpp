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

        glm::mat4 view;
        if (isCameraMode)
            view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        else
            view = glm::lookAt(cameraPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
        g_Shader->setMat4("view", view);
        g_Shader->setMat4("projection", projection);

        // 방
        glDisable(GL_CULL_FACE);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(30.0f, 20.0f, 40.0f));
        g_Shader->setMat4("model", model);
        glBindTexture(GL_TEXTURE_2D, texRoom);
        g_Room->draw();
        glEnable(GL_CULL_FACE);

        // 다트판
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -10.0f));
        model = glm::rotate(model, glm::radians(boardRotAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(5.0f, 5.0f, 1.0f));
        g_Shader->setMat4("model", model);
        glBindTexture(GL_TEXTURE_2D, texBoard);
        g_Board->draw();

        // 다트
        model = glm::mat4(1.0f);
        model = glm::translate(model, dartWorldPos);
        float pitch = currentDartAim.x;
        float yaw = currentDartAim.y;
        if (isFired) pitch -= dartTime * 15.0f;
        model = glm::rotate(model, glm::radians(-yaw), glm::vec3(0, 1, 0));
        model = glm::rotate(model, glm::radians(pitch), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));
        model = glm::scale(model, glm::vec3(0.2f, 2.0f, 0.2f));
        g_Shader->setMat4("model", model);
        glBindTexture(GL_TEXTURE_2D, texDart);
        g_DartShape->draw();

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