#version 330 core
layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNormal;
// [추가] 텍스처(그림)의 위치를 맞추기 위한 좌표값을 받아옵니다.
layout (location = 2) in vec2 vTexCoord;

out vec3 FragPos; 
out vec3 Normal;  
// [추가] 프래그먼트 쉐이더로 좌표를 넘겨줍니다.
out vec2 TexCoord;

uniform mat4 model;      
uniform mat4 view;       
uniform mat4 projection; 

void main()
{
    gl_Position = projection * view * model * vec4(vPos, 1.0);
    FragPos = vec3(model * vec4(vPos, 1.0));
    
    // 사용자님이 작성하신 법선 계산식 그대로 유지
    Normal = mat3(transpose(inverse(model))) * vNormal;

    // [추가] 텍스처 좌표 전달
    TexCoord = vTexCoord;
}