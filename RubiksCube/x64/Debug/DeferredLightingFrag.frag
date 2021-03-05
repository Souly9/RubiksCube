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
   vec2 TexCoords;
} IN;

uniform Material material;
uniform sampler2D positionBuffer;
uniform sampler2D normalBuffer;
uniform sampler2D albedoBuffer;
uniform samplerCube skybox;

void main()
{
	vec3 rslt = vec3(0);
	vec4 posVals = texture(positionBuffer, IN.TexCoords);
	vec4 worldPos = vec4(posVals.xyz, 1);
	vec3 normal = texture(normalBuffer, IN.TexCoords).rgb;
	vec3 albedo = texture(albedoBuffer, IN.TexCoords).xyz;
	vec3 viewDir = normalize(worldPos.xyz - Lights.cameraPos);

	for(int i = 0; i<MAX_LIGHTS;++i)
	{
		PointLight light = Lights.lights[i];
		vec3 lightPos = light.pos.xyz;

		float distance    = length(light.pos - worldPos);
		float attenuation = 1.0 / (distance); 
		
		vec3 lightDir = normalize(worldPos.xyz - light.pos.xyz);
		
		vec3 reflectDir = reflect(-lightDir, normal);

		float diff = max(dot(normal, lightDir), 0);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);

		vec3 specular = spec * light.color.xyz * albedo;
		vec3 diffuse = diff * light.color.xyz * albedo;
		vec3 ambient = vec3(Lights.ambientStrength) * albedo;

		specular *= attenuation;
		diffuse *= attenuation;
		ambient *= attenuation;
		rslt += (ambient + diffuse + specular);
	}
	rslt *= texture(skybox, reflect(viewDir, normal)).xyz;
	
	float gamma = 2.2;
    rslt.rgb = pow(rslt.rgb, vec3(1.0/gamma));
	if(posVals.w > 0.9)
		rslt = albedo;
	FragColor = vec4(rslt, 1);	
}