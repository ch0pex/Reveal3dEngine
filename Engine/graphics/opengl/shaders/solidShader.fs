#version 450 core


in vec4 passColor;
in vec3 passNormal;

out vec4 color; 


uniform vec3 ambientColor;
uniform float ambientLightIntensity;

uniform vec3 sunLightDirection;	
uniform vec3 sunLightColor;
uniform float sunLightIntensity;

void main()
{
	vec4 ambientLight = vec4(ambientColor, 1.0f) * ambientLightIntensity;
	vec3 norm = normalize(passNormal);
	vec3 lightDir = normalize(sunLightDirection);
	float diff = max(dot(norm, lightDir), 0.0f);
	vec4 diffuseLight = vec4(sunLightColor, 1.0f) * diff * sunLightIntensity;

	color = passColor * (ambientLight + diffuseLight);
}
