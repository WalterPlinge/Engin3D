#version 410

layout (location = 0) in vec3 fPosition;

layout (location = 0) out vec4 fColour;

in vec3 normal;

struct DirectionalLight {
    vec3 colour;
    vec3 direction;
};

//This shader applies a Lambert shading model and uses a directional light as single light source.
void main() {
	// directional light properties
	DirectionalLight light;
	light.colour = vec3(0.5f, 0.4f, 0.3f);
	//light.direction = vec3(-2.0f, 0.5f, 3.7f);
	light.direction = vec3(1.0f, 2.0f, 3.0f);

    // ambient 
    float ambientIntensity = 0.5f;
    vec3 ambient = light.colour * ambientIntensity;

    // diffuse
    float diffuseIntensity = 1.0f;
	vec3 diffuse = 0.2f * max(dot(light.direction, normal), 0.0f) * light.colour * diffuseIntensity;

	// final colour
    fColour = vec4(ambient + diffuse, 1.0f);
}