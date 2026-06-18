#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoord;
in float Brightness;

in vec3 FragPosition;

// Texture Sampler
uniform sampler2D texture1;
uniform vec3 LightPosition = vec3(0.0f);
uniform vec3 ViewPosition  = vec3(0.0f);
uniform float AmbientStrength = 0.1f;
uniform bool ComplexLightingEnabled = false;

vec2 AdjustVoxelTextureCoordinate(in vec2 TextureCoordinate)
{
    TextureCoordinate.x -= TextureCoordinate.x > 1.0 ? int(TextureCoordinate.x) : 0.0;
    TextureCoordinate.y -= TextureCoordinate.y > 1.0 ? int(TextureCoordinate.y) : 0.0;

    return TextureCoordinate;
}

void main()
{
    vec2 TextureCoordinate = TexCoord;
    TextureCoordinate = AdjustVoxelTextureCoordinate(TextureCoordinate);

	vec4 TextureColor = texture(texture1, TextureCoordinate);
    if (TextureColor.a < 0.1) discard;
    
    if (ComplexLightingEnabled) {
        // Ambient
        vec3 Ambient = AmbientStrength * TextureColor.rgb;

        // Diffuse
        vec3 NormalizedNormal = normalize(Normal);
        vec3 LightDirection = normalize(LightPosition - FragPosition);
        vec3 Diffuse = max(dot(NormalizedNormal, LightDirection), 0.0) * TextureColor.rgb;

        // Specular
        vec3 ViewDirection = normalize(ViewPosition - FragPosition);
        vec3 ReflectDirection = reflect(-LightDirection, NormalizedNormal);
        vec3 specular = pow(max(dot(ViewDirection, ReflectDirection), 0.0), 32) * TextureColor.rgb;

        TextureColor.rgb = Ambient + Diffuse + specular;
    }
       
    FragColor = TextureColor * Brightness;
}



