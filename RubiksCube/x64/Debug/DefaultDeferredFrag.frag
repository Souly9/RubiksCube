#version 420 core
#extension GL_ARB_bindless_texture : enable
#extension GL_NV_gpu_shader5 : enable
#define MAX_LIGHTS 10

layout(location = 0) out vec4 gPosition;
layout(location = 1) out vec3 gNormal;
layout(location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec4 gInfo;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

out vec4 FragColor;

in VertexOut
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
} IN;

uniform Material material;
uniform sampler2D opaque;
uniform sampler2D normalMap;
uniform sampler2D displacement;
uniform samplerCube skybox;

void main()
{
    gPosition.xyz = IN.vertPos.xyz;
	vec3 normal = texture(normalMap, IN.TexCoords).rgb;
	normal = normal * 2.0f - 1.0f;
	gNormal = normalize(IN.TBN * normal);
    gAlbedoSpec.rgb = texture(opaque, IN.TexCoords).xyz * IN.Color;
    gPosition.w = 0;
}