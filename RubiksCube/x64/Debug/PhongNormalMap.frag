#version 420 core
#extension GL_ARB_bindless_texture : enable
#extension GL_NV_gpu_shader5 : enable
#define MAX_LIGHTS 10

struct PointLight
{
	vec4 pos;
	vec4 color;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 


layout(std140, binding = 30) uniform 
LightData
{
	float ambientStrength;
	vec3 cameraPos;
	PointLight lights[MAX_LIGHTS];
	
} Lights;

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
	vec3 rslt = vec3(0);

	vec3 normal = texture(normalMap, IN.TexCoords).rgb;
	normal = normal * 2.0f - 1.0f;
	normal = normalize(IN.TBN * normal);

	vec3 viewDir = normalize(IN.WorldPos.xyz - Lights.cameraPos);

	for(int i = 0; i<MAX_LIGHTS;++i)
	{
		PointLight light = Lights.lights[i];
		vec3 lightPos = light.pos.xyz;

		float distance    = length(light.pos - IN.WorldPos);
		float attenuation = 1.0 / (distance); 
		
		vec3 lightDir = normalize(IN.WorldPos.xyz - light.pos.xyz);
		
		vec3 reflectDir = reflect(-lightDir, normal);

		float diff = max(dot(normal, lightDir), 0);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);

		vec3 specular = spec * light.color.xyz * IN.Color;
		vec3 diffuse = diff * light.color.xyz * IN.Color;
		vec3 ambient = vec3(Lights.ambientStrength) * IN.Color;

		specular *= attenuation;
		diffuse *= attenuation;
		ambient *= attenuation;
		rslt += (ambient + diffuse + specular);
	}
	rslt *= texture(skybox, reflect(viewDir, normal)).xyz;
	rslt *= texture(opaque, IN.TexCoords).xyz;

	float gamma = 2.2;
    rslt.rgb = pow(rslt.rgb, vec3(1.0/gamma));

	FragColor = vec4(rslt, 1);	
}