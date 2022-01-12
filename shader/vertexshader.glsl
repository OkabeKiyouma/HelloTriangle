#version 420 core
//#extension GL_ARB_separate_shader_objects : enable  //layout (location) isn't available in lower OGL version
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 tex;
layout (location = 3 )out vec2 ftex;
uniform float time;
void main()
{
   gl_Position = vec4(aPos, 1.0);
ftex=tex;
}