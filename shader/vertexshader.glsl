#version 420 core
//#extension GL_ARB_separate_shader_objects : enable  //layout (location) isn't available in lower OGL version
layout (location = 0) in vec3 iPos;
layout (location = 1) in vec3 iCol;
layout (location = 2) in vec2 iTex;

layout (location = 0 )out vec3 oCol;
layout (location = 1 )out vec2 oTex;
uniform float time;
void main()
{
   gl_Position = vec4(iPos, 1.0);
	oCol=iCol;
	oTex=iTex;
}