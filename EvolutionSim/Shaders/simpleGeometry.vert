#version 330 core

in vec2 aPos;

uniform mat4 P;

void main()
{
    gl_Position.xy = (P * vec4(aPos, 0.0, 1.0)).xy;
	gl_Position.z = 0.0;
	gl_Position.w = 1.0;
	
}