# version 450 core

layout (location = 0) in vec3 inPosition; 
layout (location = 1) in vec4 inColor;
layout (location = 2) in vec3 inNormal;

out vec4 passColor;
out vec3 passNormal;

uniform mat4 vp; 

void main()
{
	gl_Position = vp * vec4(inPosition, 1.0);
	passColor = inColor;
	passNormal = inNormal;
}
