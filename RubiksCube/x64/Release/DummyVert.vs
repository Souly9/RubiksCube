#version 420 
layout(location = 0) in vec3 vPos;
layout(std140, binding = 10) uniform 
Scenedata
{
	mat4 projMat;
	mat4 viewMat;
	float totalTime;
	float deltaTime;
} SceneD;

out VertexOut
{
   vec2 TexCoords;
} OUT;

void main()
{
	OUT.TexCoords = (vPos.xy + 1) * 0.5;
	gl_Position = vec4(vPos, 1);	
}