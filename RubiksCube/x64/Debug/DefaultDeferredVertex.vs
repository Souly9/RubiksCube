#version 420 
layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexCoord;
layout(location = 3) in vec3 vColor;
layout(location = 4) in vec3 vTangent;
layout(location = 5) in vec3 vBiTangent;

layout(std140, binding = 10) uniform 
Scenedata
{
	mat4 projMat;
	mat4 viewMat;
	float totalTime;
	float deltaTime;
} SceneD;
uniform mat4 modelMat;

uniform sampler2D normalMap;
out VertexOut
{
   vec4 vertPos;
   vec4 WorldPos;
   vec4 ScreenPos;
   vec3 Normal;
   vec3 Color;
   float delta;
   mat3 TBN;
   vec2 TexCoords;
   mat4 invNormal;

   } OUT;

void main()
{
	OUT.Normal = vNormal;
	OUT.delta = SceneD.deltaTime;

	OUT.WorldPos = modelMat * vec4(vPos, 1);
	OUT.ScreenPos =  SceneD.viewMat * OUT.WorldPos;
	OUT.vertPos = SceneD.projMat * OUT.ScreenPos;

	OUT.TexCoords = vTexCoord;

	vec3 T = normalize(vec3(modelMat * vec4(vTangent,   1.0)));
	// Normals buggy otherwise
	vec3 N = normalize(vNormal);
	vec3 B = normalize(vec3(modelMat * vec4(vBiTangent, 1.0)));
	
	OUT.TBN = mat3(T, B, N);
	OUT.invNormal = inverse(SceneD.projMat);
	OUT.Color = vColor;
	gl_Position = OUT.vertPos;
	
}