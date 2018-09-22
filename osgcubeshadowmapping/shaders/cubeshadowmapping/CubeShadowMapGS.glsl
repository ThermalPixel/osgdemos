#version 330 core
layout (triangles) in;
layout(triangle_strip, max_vertices=18) out;

uniform mat4 cubeMapMatrices[6];


out vec3 w_pos;

void main()
{
	for (gl_Layer = 0; gl_Layer < 6;++gl_Layer)
	{		
		for (int i =0; i < 3;++i)
		{
			w_pos = gl_in[i].gl_Position.xyz;
		
			gl_Position = cubeMapMatrices[gl_Layer]*  gl_in[i].gl_Position;
			EmitVertex();
		}
		EndPrimitive();
	}
}