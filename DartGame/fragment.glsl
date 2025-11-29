#version 330 core
in vec3 FragPos; 
in vec2 TexCoord;
in mat3 TBN; // [New] 버텍스 쉐이더에서 받은 TBN

out vec4 FragColor;

uniform vec3 lightPos;      
uniform vec3 lightColor;    
uniform vec3 viewPos;       

// [New] 텍스처 샘플러들
uniform sampler2D texture_diffuse; // 기본 색상 (Slot 0)
uniform sampler2D texture_normal;  // 노말 맵 (Slot 1)

void main ()
{
    // 1. 노말 맵에서 법선 벡터 가져오기
    vec3 normal = texture(texture_normal, TexCoord).rgb;
    // [0,1] 범위를 [-1,1]로 변환
    normal = normalize(normal * 2.0 - 1.0);   
    // TBN 행렬을 곱해 월드 공간 법선으로 변환
    normal = normalize(TBN * normal); 

    // 2. 색상 텍스처에서 색상 가져오기
    vec3 color = texture(texture_diffuse, TexCoord).rgb;

    // 3. 조명 계산 (Normal 변수 사용)
    // Ambient
    vec3 ambient = 0.1 * color;
    
    // Diffuse
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * color;
    
    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = spec * lightColor * 0.5; // 반사광 세기 조절
    
    vec3 result = ambient + diffuse + specular;
    
    FragColor = vec4(result, 1.0); 
}