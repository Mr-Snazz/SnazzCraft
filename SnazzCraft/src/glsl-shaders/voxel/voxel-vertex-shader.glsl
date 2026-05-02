#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoord;
layout(location = 3) in float aBrightness;

out vec3 Normal;
out vec2 TexCoord;
out float Brightness;

out vec3 FragPosition;
  
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
    gl_Position = Projection * View * Model * vec4(aPos, 1.0);

    // Transform normal by normal matrix (inverse transpose of Model matrix)
    Normal = mat3(transpose(inverse(Model))) * aNormal;
    TexCoord = aTexCoord;
    Brightness = aBrightness;

    FragPosition = vec3(Model * vec4(aPos, 1.0));
} 



