#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoord;
in float Brightness;

in vec3 FragPosition;

// Texture Sampler
uniform sampler2D Sampler;

void main()
{
	vec4 TextureColor = texture(Sampler, TexCoord);
    if (TextureColor.a < 0.1) discard;
       
    FragColor = TextureColor * Brightness;
}



