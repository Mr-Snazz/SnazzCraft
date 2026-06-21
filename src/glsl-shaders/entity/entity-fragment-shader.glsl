#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoord;

in vec3 FragPosition;

// Texture Sampler
uniform sampler2D Sampler;

void main()
{
    vec2 TextureCoordinate = TexCoord;

	vec4 TextureColor = texture(Sampler, TextureCoordinate);
    if (TextureColor.a < 0.1) discard;
       
    FragColor = TextureColor;
}



