#version 330 core
in vec3 FragPos; 
in vec3 Normal;
// [추가] 버텍스 쉐이더에서 넘겨준 텍스처 좌표 받기
in vec2 TexCoord; 

out vec4 FragColor;

uniform vec3 lightPos;      
uniform vec3 lightColor;    
uniform vec3 viewPos;       
// [변경] objectColor(단색) 대신 그림파일(texture1)을 사용합니다.
uniform sampler2D texture1; 

void main ()
{
	// --- 1. 주변광 (Ambient) ---
    // (사용자님 수식 100% 그대로)
	vec3 ambientLight = vec3(0.3); 
	vec3 ambient = ambientLight * lightColor;
	
	// --- 2. 산란 반사광 (Diffuse) ---
    // (사용자님 수식 100% 그대로)
	vec3 normVector = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos); 
	float diffuseLight = max(dot(normVector, lightDir), 0.0); 
	vec3 diffuse = diffuseLight * lightColor; 
	
	// --- 3. 거울 반사광 (Specular) ---
    // (사용자님 수식 100% 그대로)
	int shininess = 128; 
	vec3 viewDir = normalize(viewPos - FragPos); 
	vec3 reflectDir = reflect(-lightDir, normVector); 
	float specularLight = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	vec3 specular = specularLight * lightColor;
	
	// --- 4. 최종 색상 ---
    // [수정] 텍스처에서 색상을 뽑아옵니다.
    vec4 texColor = texture(texture1, TexCoord);

    // 사용자님의 수식 (ambient + diffuse + specular) 전체에
    // 단색(objectColor) 대신 그림(texColor)을 입혔습니다.
	vec3 result = (ambient + diffuse + specular) * texColor.rgb; 
	
	FragColor = vec4(result, 1.0); 
}