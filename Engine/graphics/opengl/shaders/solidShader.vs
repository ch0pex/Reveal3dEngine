# version 450 core

layout (location = 0) in vec3 inPosition; 
layout (location = 1) in vec2 inTexCoords;
layout (location = 2) in vec3 inNormal;

out vec2 passTexCoords;
out vec3 passNormal;

uniform mat4 vp; 

void main()
{
	gl_Position = vp * vec4(inPosition, 1.0);
	passTexCoords = inTexCoords; 
	passNormal = inNormal;
}
