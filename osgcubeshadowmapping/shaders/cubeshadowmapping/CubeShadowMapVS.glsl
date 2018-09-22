#version 330 core
in vec4 osg_Vertex;
uniform mat4 osg_ModelViewMatrix;

void main()
{
	gl_Position =  osg_ModelViewMatrix * osg_Vertex;

}
