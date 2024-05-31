#version 420 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexCoords;

out vec2 passTexCoords;

uniform mat4 vp;

void main()
{
	passTexCoords = inTexCoords;
	gl_Position = vp * vec4(inPosition, 1.0);
}
