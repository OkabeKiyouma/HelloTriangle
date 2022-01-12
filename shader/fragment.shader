"#version 420 core\n\
//#extension GL_ARB_separate_shader_objects : enable\n\
out vec4 FragColor;\
layout (location =3) in vec2 ftex;\
uniform float time;\
uniform sampler2D tsamp;\
void main(){\
	FragColor = texture(tsamp,ftex)*vec4(sin(time), sin(time + 120), sin(time + 240), 1.0f);\
}\0";