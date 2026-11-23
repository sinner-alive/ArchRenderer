#version 410 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform vec3 materialDiffuse;
uniform bool hasDiffuseTexture;
uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform vec3 viewPos;

void main()
{
    vec4 baseColor = hasDiffuseTexture
        ? texture(texture_diffuse1, TexCoords)
        : vec4(materialDiffuse, 1.0);

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightDirection);
    float diffuseStrength = max(dot(norm, lightDir), 0.0);
    vec3 ambient = 0.50 * lightColor;
    vec3 diffuse = 0.35 * diffuseStrength * lightColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float specularStrength = 0.12;
    float specularAmount = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specularStrength * specularAmount * lightColor;

    FragColor = vec4((ambient + diffuse) * baseColor.rgb + specular, baseColor.a);
}