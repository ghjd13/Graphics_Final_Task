#ifndef TEXTURE_UTIL_H
#define TEXTURE_UTIL_H

#include "common.h"

// 간단한 체크무늬 텍스처 생성기 (이미지 파일 없이 테스트 가능하도록)
GLuint createCheckerTexture() {
    const int width = 64;
    const int height = 64;
    GLubyte data[height][width][3]; // RGB

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // x, y 좌표에 따라 검은색/흰색 교차
            int c = ((((y & 8) == 0) ^ ((x & 8)) == 0)) * 255;
            data[y][x][0] = (GLubyte)c; // R
            data[y][x][1] = (GLubyte)c; // G
            data[y][x][2] = (GLubyte)c; // B
        }
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // 텍스처 파라미터 설정 (반복, 선형 필터링)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 이미지 데이터 전송
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    return textureID;
}

// 색상 텍스처 생성기 (단색 텍스처가 필요할 때)
GLuint createColorTexture(GLubyte r, GLubyte g, GLubyte b) {
    GLubyte data[1][1][3];
    data[0][0][0] = r;
    data[0][0][1] = g;
    data[0][0][2] = b;

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    return textureID;
}

#endif