#version 420 core
//#extension GL_ARB_separate_shader_objects : enable
out vec4 FragColor;
layout (location =0) in vec3 oCol;
layout (location =1) in vec2 oTex;
uniform sampler2D tsamp1;
uniform sampler2D tsamp2;
void main(){
	FragColor = mix(texture(tsamp1,oTex),texture(tsamp2,oTex),0.3)*vec4(oCol,1);
}