#version 330

layout(location = 0) in vec4 osg_Vertex;
in vec3 osg_Normal;
in vec2 osg_MultiTexCoord0;


uniform mat4 osg_ModelViewProjectionMatrix;
uniform mat4 osg_ViewMatrixInverse;
uniform mat4 osg_ModelViewMatrix;
uniform mat3 osg_NormalMatrix;
uniform vec3 lpos;
smooth out vec3 ws_pos;
smooth out vec2 texCoord;
smooth out vec3 normal;
smooth out vec3 ldir;
void main()
{
	gl_Position =  osg_ModelViewProjectionMatrix* osg_Vertex;
	ws_pos = (osg_ViewMatrixInverse*osg_ModelViewMatrix  * osg_Vertex).xyz;
	texCoord = osg_MultiTexCoord0.xy;
	normal = osg_NormalMatrix * osg_Normal;
	vec4 ecpos = osg_ModelViewMatrix  * osg_Vertex ;
	ecpos.xyz = ecpos.xyz / ecpos.w;
	ldir = normalize(lpos- ecpos.xyz);
}