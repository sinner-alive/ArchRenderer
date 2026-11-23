#version 410 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform bool hasDiffuseTexture;

void main() {
    vec3 baseColor =  texture(texture_diffuse1, TexCoords).rgb;
    vec3 sunEmission = baseColor * 2.0;
    FragColor = vec4(sunEmission, 1.0);
}