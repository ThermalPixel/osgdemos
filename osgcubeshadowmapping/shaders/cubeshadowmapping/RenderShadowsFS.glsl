#version 330
smooth in vec3 ws_pos; 
smooth in vec2 texCoord;

smooth in vec3 normal;
smooth in vec3 ldir;



uniform sampler2D diffuseTex;
uniform vec3 lpos;


float CalculateShadowFactor(vec3 worldPos, vec3 lightPos);

out vec4 color;
void main()
{
	
	vec3 n  = normalize(normal);
        vec4 diffuse = vec4(1,1,1,1) * max(dot(n, normalize(ldir)),0.0);
	
        color =  diffuse * vec4(CalculateShadowFactor(ws_pos, lpos));
//	color = vec4(cm_depth*50.0f);

}
