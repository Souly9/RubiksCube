#version 420 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

layout(std140, binding = 10) uniform 
Scenedata
{
	mat4 projMat;
	mat4 viewMat;
	float totalTime;
	float deltaTime;
} SceneD;

void main()
{
    TexCoords = aPos;
    gl_Position = SceneD.projMat * mat4(mat3(SceneD.viewMat)) * vec4(aPos, 1.0);
}