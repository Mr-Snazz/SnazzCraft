#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoord;
in float Brightness;

in vec3 FragPosition;

// Texture Sampler
uniform sampler2D texture1;
//uniform vec3 LightPosition;
uniform vec3 ViewPosition;

struct DirectionalLightStruct
{
    // vec3 Position; // Not necessary with directional light
    vec3 Direction;
    vec3 Ambient;
    vec3 Diffuse;
    vec3 Specular;
};
uniform DirectionalLightStruct DirectionalLight;

vec2 AdjustVoxelTextureCoordinate(inout vec2 TextureCoordinate);

const float SpecularStrength = 0.5f;

void main()
{
    // 1. Texture Sampling
    // fract() handles the tiling. Use a small epsilon if you see border flickering.
    vec2 TextureCoordinate = fract(TexCoord);
    vec4 TextureColor = texture(texture1, TextureCoordinate);

    // Alpha Clipping (Essential for voxels like leaves/tall grass)
    if (TextureColor.a < 0.1) discard;

    // 2. Vectors
    vec3 NormalizedNormal = normalize(Normal);
    vec3 LightDirection = normalize(-DirectionalLight.Direction);
    vec3 ViewDirection = normalize(ViewPosition - FragPosition);
    vec3 HalfwayDir = normalize(LightDirection + ViewDirection);

    // 3. Lighting Components
    // Ambient - Use Brightness here for AO
    vec3 AmbientColor = DirectionalLight.Ambient * TextureColor.rgb * Brightness;

    // Diffuse - Also affected by AO/Brightness
    float DiffuseFactor = max(dot(NormalizedNormal, LightDirection), 0.0);
    vec3 DiffuseColor = DirectionalLight.Diffuse * DiffuseFactor * TextureColor.rgb * Brightness;

    // Specular - Blinn-Phong (Optional: multiply by a specular map if you have one)
    float SpecularFactor = pow(max(dot(NormalizedNormal, HalfwayDir), 0.0), 32.0);
    vec3 SpecularColor = DirectionalLight.Specular * SpecularFactor * SpecularStrength;

    // 4. Final Color
    FragColor = vec4(AmbientColor + DiffuseColor + SpecularColor, 1.0);
}

vec2 AdjustVoxelTextureCoordinate(inout vec2 TextureCoordinate)
{
    TextureCoordinate.x -= TextureCoordinate.x > 1.0 ? int(TextureCoordinate.x) : 0.0;
    TextureCoordinate.y -= TextureCoordinate.y > 1.0 ? int(TextureCoordinate.y) : 0.0;

    return TextureCoordinate;
}

/*
void main()
{
    // ambient
    vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    // vec3 lightDir = normalize(light.position - FragPos);
    vec3 lightDir = normalize(-light.direction);  
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;  
    
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;  
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
} 
*/