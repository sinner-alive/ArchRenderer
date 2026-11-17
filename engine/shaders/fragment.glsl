#version 410 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform vec3 materialDiffuse;
uniform bool hasDiffuseTexture;

void main()
{
    FragColor = hasDiffuseTexture
        ? texture(texture_diffuse1, TexCoords)
        : vec4(materialDiffuse, 1.0);
}