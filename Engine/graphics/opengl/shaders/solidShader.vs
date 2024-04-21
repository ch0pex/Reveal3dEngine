# version 450 core

layout (location = 0) in vec3 inPosition; 
layout (location = 1) in vec4 inColor;
layout (location = 2) in vec3 inNormal;

out vec4 passColor;
out vec3 passNormal;

uniform mat4 vp;
uniform mat4 model;

void main()
{
	gl_Position = vp * model * vec4(inPosition, 1.0);
	passColor = inColor;
	passNormal = inNormal;
}
