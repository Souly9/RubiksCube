#version 420 core
#extension GL_ARB_bindless_texture : enable
#extension GL_NV_gpu_shader5 : enable
#define MAX_LIGHTS 1

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
	PointLight lights[MAX_LIGHTS];
	
} Lights;

out vec4 FragColor;

in VertexOut
{
   vec4 vertPos;
   vec4 WorldPos;
   vec3 Normal;
   float delta;
   vec3 Color;
   vec2 TexCoords;
} IN;

uniform Material material;
uniform sampler2D opaque;

void main()
{
	PointLight light;
	vec3 rslt = vec3(0,0,0);
	vec3 normal = normalize(IN.Normal);
	for(int i = 0; i<MAX_LIGHTS;++i)
	{
		light = Lights.lights[i];
		vec3 lightPos = light.pos.xyz;

		vec3 lightDir = normalize(lightPos -IN.WorldPos.xyz);
		vec3 viewDir = normalize(lightPos-IN.WorldPos.xyz);
		vec3 reflectDir = reflect(-lightDir, normal);

		float diff = max(dot(normal, lightDir), 0);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = spec * light.color.xyz * material.specular;
		vec3 diffuse = diff * IN.Color * material.diffuse;

	
		vec3 ambient = Lights.ambientStrength * IN.Color;
		rslt += (ambient + diffuse + specular);
	}
	rslt *= texture(opaque, IN.TexCoords).xyz;
	FragColor = vec4(rslt, 1);	
}