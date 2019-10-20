#include <iostream>

#include <e3d/ogl/app.hh>



using namespace ogl;



// Shader
std::shared_ptr<Shader> static lambert;;

// Meshes
auto static ground = Mesh();
auto static cube   = Mesh();
auto static sphere = Mesh();



void setup()
{
	// App settings
	app::clear_colour(glm::vec4(0.2F, 0.3F, 0.3F, 1.0F));
	app::camera.position = glm::vec3(0.0F, 20.0F, 5.0F);
	app::camera.look_at(glm::vec3(0.0F));
	app::camera.sensitivity = 0.001F;

	// Shader
	lambert = std::make_shared<Shader>("Lambert");
	lambert->add(GL_VERTEX_SHADER, "resources/shaders/lambert.vert");
	lambert->add(GL_FRAGMENT_SHADER, "resources/shaders/lambert.frag");
	lambert->build();

	// Meshes
	ground.load(Mesh::Quad);
	ground.scale(glm::vec3(5.0F));
	ground.shader = lambert;

	cube.load(Mesh::Cube);
	cube.translate(glm::vec3(1.0F, 0.0F, 1.0F));
	cube.shader = lambert;

	sphere.load(Mesh::File, "resources/models/sphere.obj");
	sphere.translate(glm::vec3(-1.0F, 0.0F, 1.0F));
	sphere.shader = lambert;
}

void render()
{
	app::clear();
	app::draw(sphere);
	app::draw(cube);
	app::draw(ground);
	app::display();
}

auto
main(
	)
	-> int
{
	app::setup_function = setup;
	app::render_function = render;
	app::run();
}
