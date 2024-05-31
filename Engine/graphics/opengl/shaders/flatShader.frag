#version 420 core

out vec4 color;

void main()
{
	color = texture(Texture, passTexCoords);
}
