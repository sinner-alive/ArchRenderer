#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    vec3 skyColor = texture(skybox, TexCoords).rgb;
    FragColor = vec4(min(skyColor * 1.15, vec3(1.0)), 1.0);
}