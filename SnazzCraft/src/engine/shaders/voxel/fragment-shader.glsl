#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// Texture Sampler
uniform sampler2D texture1;

vec2 AdjustVoxelTextureCoordinate(inout vec2 TextureCoordinate);

void main()
{
    vec2 TextureCoordinate = TexCoord;
    TextureCoordinate = AdjustVoxelTextureCoordinate(TextureCoordinate);

	vec4 texColor = texture(texture1, TextureCoordinate);

    if (texColor.a < 0.1) {
        discard;
    }
       
    FragColor = texColor;
}

vec2 AdjustVoxelTextureCoordinate(inout vec2 TextureCoordinate)
{
    TextureCoordinate.x -= TextureCoordinate.x > 1.0 ? int(TextureCoordinate.x) : 0.0;
    TextureCoordinate.y -= TextureCoordinate.y > 1.0 ? int(TextureCoordinate.y) : 0.0;

    return TextureCoordinate;
}