#version 420 core
layout (location = 0) out vec4 gWorldPos;
layout (location = 2) out vec4 gAlbedo;
in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{    
    gAlbedo.xyz = texture(skybox, TexCoords).rgb;
    gWorldPos = vec4(1);
}