#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoord;
in float Brightness;

in vec3 FragPosition;

// Texture Sampler
uniform sampler2D texture1;
uniform vec3 LightPosition;
uniform vec3 ViewPosition;
uniform float Ambient;

vec2 AdjustVoxelTextureCoordinate(inout vec2 TextureCoordinate);

void main()
{
    vec2 TextureCoordinate = TexCoord;
    TextureCoordinate = AdjustVoxelTextureCoordinate(TextureCoordinate);

	vec4 TextureColor = texture(texture1, TextureCoordinate);
    if (TextureColor.a < 0.1) discard;

    // Apply diffuse lighting
    vec3 NormalizedNormal = normalize(Normal);
    vec3 LightDirection = normalize(LightPosition - FragPosition);
    vec3 ViewDirection = normalize(ViewPosition - FragPosition);
    vec3 ReflectDirection = reflect(-LightDirection, NormalizedNormal);

    float Diffuse = max(dot(NormalizedNormal, LightDirection), 0.0);

    float FinalBrightness = (Ambient + Diffuse) * Brightness;
    TextureColor.rgb *= FinalBrightness;
       
    FragColor = TextureColor;
}

vec2 AdjustVoxelTextureCoordinate(inout vec2 TextureCoordinate)
{
    TextureCoordinate.x -= TextureCoordinate.x > 1.0 ? int(TextureCoordinate.x) : 0.0;
    TextureCoordinate.y -= TextureCoordinate.y > 1.0 ? int(TextureCoordinate.y) : 0.0;

    return TextureCoordinate;
}

