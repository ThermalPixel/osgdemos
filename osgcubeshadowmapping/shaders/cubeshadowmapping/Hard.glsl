#version 330

uniform samplerCube cubeMap;
uniform float far;

#define BIAS 0.1

float CalculateShadowFactor(vec3 worldPos, vec3 lightPos)
{
	vec3 vert_to_light = worldPos - lightPos;
	float cm_depth = texture(cubeMap,vert_to_light).r;
	float depth = length(vert_to_light);
	cm_depth *= far;
	float sf = 1.0f;
	if(cm_depth+BIAS  <= depth)
	{
		sf = 0.0f;
	}
	return sf;
}
