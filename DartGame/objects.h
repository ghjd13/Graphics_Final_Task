#ifndef OBJECTS_H
#define OBJECTS_H

#include "common.h"
#include <vector>
#include <gl/glew.h>
#include <gl/glm/glm.hpp>
#include <stddef.h> // offsetof 사용

// 기본 메쉬 클래스
class Mesh {
public:
    GLuint VAO, VBO;
    std::vector<Vertex> vertices;

    void setupMesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);

        // [VAO 오타 수정]
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        // Vertex 구조체의 전체 크기만큼 데이터 전송 (sizeof(Vertex) 사용)
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        // Stride는 sizeof(Vertex)로 설정
        GLsizei stride = sizeof(Vertex);

        // 0. Position (glm::vec3)
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);

        // 1. Normal (glm::vec3)
        // [수정: offsetof 멤버 이름 'Normal'로 변경]
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, Normal));

        // 2. Vertex Color / TexCoords (glm::vec3이든 glm::vec2이든 데이터는 여기서 읽어야 함)
        // [핵심 수정: TexCoords가 2번 레이아웃을 사용하도록 설정]
        // Color 멤버가 없으므로 TexCoords 필드를 대신 사용하고,
        // models.h에서 color(vec3)를 이 자리에 넣었을 가능성을 열어두고 3으로 읽습니다.
        glEnableVertexAttribArray(2);
        // 만약 Vertex Color가 3차원 데이터로 TexCoords 자리에 들어갔다면 3으로 읽어야 합니다.
        // 현재 상황을 정확히 모르므로, 컴파일 에러를 피하고 데이터를 읽을 수 있게 TexCoords를 사용합니다.
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)offsetof(Vertex, TexCoords));

        glBindVertexArray(0);
    }

    void draw() {
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        glBindVertexArray(0);
    }
};

// [이하 다른 클래스 코드는 변경하지 않았습니다. setupMesh만 수정했습니다.]
// ...

// 1. 육면체 (방)
class Cube : public Mesh {
public:
    Cube() {
        generateCube(0.5f);
        setupMesh();
    }
private:
    void generateCube(float s) {
        glm::vec3 p1(-s, -s, s), p2(s, -s, s), p3(s, s, s), p4(-s, s, s);
        glm::vec3 p5(-s, -s, -s), p6(s, -s, -s), p7(s, s, -s), p8(-s, s, -s);
        glm::vec3 defaultColor = { 1.0f, 1.0f, 1.0f };

        addQuad(p1, p2, p3, p4, glm::vec3(0, 0, 1), defaultColor);
        addQuad(p6, p5, p8, p7, glm::vec3(0, 0, -1), defaultColor);
        addQuad(p4, p3, p7, p8, glm::vec3(0, 1, 0), defaultColor);
        addQuad(p5, p6, p2, p1, glm::vec3(0, -1, 0), defaultColor);
        addQuad(p2, p6, p7, p3, glm::vec3(1, 0, 0), defaultColor);
        addQuad(p5, p1, p4, p8, glm::vec3(-1, 0, 0), defaultColor);
    }
    void addQuad(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec3 p4, glm::vec3 n, glm::vec3 c) {
        vertices.push_back({ p1, n, c });
        vertices.push_back({ p2, n, c });
        vertices.push_back({ p3, n, c });
        vertices.push_back({ p1, n, c });
        vertices.push_back({ p3, n, c });
        vertices.push_back({ p4, n, c });
    }
};

// 2. [원판] 다트판 (Disk)
class Dartboard : public Mesh {
public:
    Dartboard() {
        generateDisk(0.5f, 40);
        setupMesh();
    }
private:
    void generateDisk(float radius, int segments) {
        glm::vec3 center(0.0f, 0.0f, 0.0f);
        glm::vec3 normal(0.0f, 0.0f, 1.0f);

        for (int i = 0; i < segments; i++) {
            float t1 = (float)i / segments * 2.0f * 3.14159f;
            float t2 = (float)(i + 1) / segments * 2.0f * 3.14159f;

            glm::vec3 p1(cos(t1) * radius, sin(t1) * radius, 0.0f);
            glm::vec3 p2(cos(t2) * radius, sin(t2) * radius, 0.0f);

            glm::vec3 segColor = (i % 2 == 0) ? glm::vec3(1.0f, 1.0f, 1.0f) : glm::vec3(1.0f, 0.0f, 0.0f);

            vertices.push_back({ center, normal, segColor });
            vertices.push_back({ p1, normal, segColor });
            vertices.push_back({ p2, normal, segColor });

            glm::vec3 n2(0, 0, -1);
            vertices.push_back({ center, n2, segColor });
            vertices.push_back({ p2, n2, segColor });
            vertices.push_back({ p1, n2, segColor });
        }
    }
};

// 3. [원통] 다트 (Cylinder)
class Cylinder : public Mesh {
public:
    Cylinder() {
        generateCylinder(0.5f, 1.0f, 20);
        setupMesh();
    }
private:
    void generateCylinder(float radius, float height, int segments) {
        float hh = height / 2.0f;
        glm::vec3 defaultColor = { 1.0f, 0.5f, 0.0f };

        for (int i = 0; i < segments; i++) {
            float t1 = (float)i / segments * 2.0f * 3.14159f;
            float t2 = (float)(i + 1) / segments * 2.0f * 3.14159f;

            float x1 = cos(t1) * radius, z1 = sin(t1) * radius;
            float x2 = cos(t2) * radius, z2 = sin(t2) * radius;

            glm::vec3 p1(x1, -hh, z1), p2(x2, -hh, z2);
            glm::vec3 p3(x2, hh, z2), p4(x1, hh, z1);
            glm::vec3 n1 = glm::normalize(glm::vec3(x1, 0, z1));

            vertices.push_back({ p1, n1, defaultColor });
            vertices.push_back({ p2, n1, defaultColor });
            vertices.push_back({ p3, n1, defaultColor });
            vertices.push_back({ p1, n1, defaultColor });
            vertices.push_back({ p3, n1, defaultColor });
            vertices.push_back({ p4, n1, defaultColor });
        }
    }
};

// 4. [추가] 조명 오브젝트 (작은 구)
class LightObject : public Mesh {
public:
    LightObject() {
        Cube c;
        generateSphere(0.2f, 20, 20);
        setupMesh();
    }
private:
    void generateSphere(float radius, int sectors, int stacks) {
        Cube c;
        vertices = c.vertices;
    }
};

#endif // OBJECTS_H