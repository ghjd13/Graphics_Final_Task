#ifndef TEXTURE_UTIL_H
#define TEXTURE_UTIL_H

#include "common.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// [New] 파일 경로에서 텍스처 로드하는 함수
GLuint loadTexture(const char* path) {
    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    // OpenGL은 Y축 좌표가 반대이므로 이미지를 뒤집어서 로드
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);

    if (data) {
        GLenum format;
        if (nrComponents == 1) format = GL_RED;
        else if (nrComponents == 3) format = GL_RGB;
        else if (nrComponents == 4) format = GL_RGBA;
        else format = GL_RGB;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // 텍스처 반복 설정 (타일링을 위해 필수)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // 필터링 설정 (축소시 선형 보간 + 밉맵, 확대시 선형 보간)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
        std::cout << "Texture loaded: " << path << std::endl;
    }
    else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

// [New] 노말 맵이 없는 물체를 위한 '평평한 노말 맵' (Flat Blue) 생성
// 다트(OBJ)에는 노말 맵이 없지만 쉐이더는 노말 맵을 요구하므로 이걸 사용합니다.
GLuint createFlatNormalTexture() {
    GLubyte data[1][1][3];
    data[0][0][0] = 128; // R = 0.5 (X축 기울기 0)
    data[0][0][1] = 128; // G = 0.5 (Y축 기울기 0)
    data[0][0][2] = 255; // B = 1.0 (Z축 수직)

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