#version 420 
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexCoords;
layout(location = 3) in vec3 vColor;
layout(std140, binding = 10) uniform 
Scenedata
{
	mat4 projMat;
	mat4 viewMat;
	float totalTime;
	float deltaTime;
} SceneD;

uniform mat4 modelMat;
uniform sampler2D displacement;

out VertexOut
{
   vec4 vertPos;
   vec4 WorldPos;
   vec3 Normal;
   float delta;
   vec3 Color;
   vec2 TexCoords;
} OUT;

void main()
{
	OUT.Normal = mat3(transpose(inverse(modelMat))) * vNormal;
	OUT.delta = SceneD.deltaTime;
	OUT.WorldPos =  modelMat * vec4(vPos, 1); //* vec4(vPos.r + texture(displacement, vTexCoords).r, vPos.yz, 1);
	OUT.vertPos = SceneD.projMat * SceneD.viewMat * OUT.WorldPos;
	OUT.TexCoords = vTexCoords;
	OUT.Color = vColor;
	gl_Position = OUT.vertPos;
	
}