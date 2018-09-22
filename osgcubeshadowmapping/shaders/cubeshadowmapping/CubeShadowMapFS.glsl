#version 330 core
in vec3 w_pos;
in vec2 texCoord;
uniform vec3 lpos;
uniform float far;

void main()
{
	float w_dist = distance(w_pos, lpos);
	float w_dist_norm = w_dist / far;
	gl_FragDepth = w_dist_norm;
}