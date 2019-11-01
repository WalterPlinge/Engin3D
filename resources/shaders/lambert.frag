#version 410

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_uv;

out vec4 out_colour;

struct DirectionalLight
{
	vec3 colour;
	vec3 direction;
};

// This shader applies a Lambert shading model and uses a directional light
void main()
{
	// Directional light properties
	DirectionalLight light;
	light.colour    = normalize(vec3(0.5F, 0.4F, 0.3F));
	light.direction = normalize(vec3(1.0F, 2.0F, 3.0F));

	// Ambient
	float ambientIntensity = 0.5F;
	vec3 ambient = light.colour * ambientIntensity;

	// Diffuse
	float diffuseIntensity = 1.0F;
	vec3 diffuse =
		0.2F
		* max(dot(light.direction, in_normal), 0.0F)
		* light.colour
		* diffuseIntensity;

	// Final colour
	out_colour = vec4(ambient + diffuse, 1.0F);
}
