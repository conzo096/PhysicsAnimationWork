/*
		CURRENT ISSUE. 
		Need to be able to change camera and mvp matrix. Might have to rewrite that segment as well.
*/
#include <glm/glm.hpp>
#include <graphics_framework.h>
#include <phys_utils.h>
#include "Model.h"
#include "PlaneCollider.h"
#include "ModelInfo.h"
#include "Colliding.h"
#include "MeshCut.h"

using namespace std;
using namespace graphics_framework;
using namespace glm;
using namespace phys;
#define physics_tick 1.0 / 60.0


// Holds all the Models in the scene.
static vector<Model> sceneList;
// Need to turn this to an object and add to sceneList.
PlaneCollider sceneFloor;
// Effect that is used to render.
effect eff;
// Camera to navigate the scene.
free_camera cam;
// Projection-view matrix, this should be moved to the render?
glm::mat4 PV;
// Basic directional lighting of the scene.
directional_light light;
// Material all objects are made from.
material mat;
// Ensures space function only executed once. Remove once destrution by collisions is done.
int counter;
int prevCounter;
// Hold x,y positions of the cursor.
double cursor_x, cursor_y;
bool load_content()
{

	// Remove cursor from simulation.
	glfwSetInputMode(renderer::get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	phys::Init();
	
	// Create first model for simulation.
	Model test;
	test.SetModelInfo(LoadCube(glm::vec3(1, 1, 1)));
	test.GetRigidBody().SetInitialPosition(glm::dvec3(0, 10, 0));
	test.GetRigidBody().SetMass(10);
	test.CreateBuffers();
	CreateSplittingPlanes(test.GetBoundingBox(), 4, test.GetSplittingPlanes());
	sceneList.push_back(test);

	// Create second model for simulation.
	Model test1;
	test1.SetModelInfo(LoadCube(glm::vec3(1, 1, 1)));
	test1.GetRigidBody().SetInitialPosition(glm::vec3(-3, 45, 0));
	test1.GetRigidBody().AddLinearImpulse(glm::dvec3(0, -0.5, 0));
	CreateSplittingPlanes(test1.GetBoundingBox(), 4, test1.GetSplittingPlanes());
	test1.CreateBuffers();
	sceneList.push_back(test1); 

	// Set the position of the floor.
	sceneFloor.SetPosition(glm::dvec3(0, -6, 0));


	// Create shader for objects.
	eff = effect();
	eff.add_shader("shaders/phys_phong.vert", GL_VERTEX_SHADER);
	eff.add_shader("shaders/phys_phong.frag", GL_FRAGMENT_SHADER);
	eff.build();
	// Initilize camera.
	cam.set_position(vec3(10.0f, 10.0f, 20.0f));
	cam.set_target(vec3(-5.0f, 10.0f, 0.0f));
	auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
	cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);
	// Set the material and lighting information.
	light.set_ambient_intensity(vec4(0.5f, 0.5f, 0.5f, 1.0f));
	light.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	light.set_direction(vec3(0.0f, 1.0f, 0.0f));
	mat = material(vec4(0.0f, 0.0f, 0.0f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f), 25.0f);
	return true;
}


bool update(float delta_time)
{
	static double t = 0.0;
	static double accumulator = 0.0;
	accumulator += delta_time;

	// The ratio of pixels to rotation - remember the fov
	float ratio_width = glm::quarter_pi<float>() / static_cast<float>(renderer::get_screen_width());
	float ratio_height = glm::quarter_pi<float>() / static_cast<float>(renderer::get_screen_height());
	// Stores last updates cursor position as well as updating this frames cursor position.
	double current_x, current_y;
	// Get cursor position.
	glfwGetCursorPos(renderer::get_window(), &current_x, &current_y);
	// Calculate delta of cursor positions from last frame
	double delta_x = current_x - cursor_x;
	double delta_y = current_y - cursor_y;
	// Multiply deltas by ratios - gets actual change in orientation
	delta_x *= ratio_width;
	delta_y *= ratio_height;
	cam.rotate(static_cast<float>(delta_x), static_cast<float>(-delta_y)); // flipped y to revert the invert.
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_UP))
		cam.move(glm::vec3(0.0f, 0.0f, 5.0f)*delta_time);
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_DOWN))
		cam.move(glm::vec3(0.0f, 0.0f, -5.0f)*delta_time);
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_LEFT))
		cam.move(glm::vec3(-5.0f, 0.0f, 0.0f)*delta_time);
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_RIGHT))
		cam.move(glm::vec3(5.0f, 0.0f, 0.0f)*delta_time);

	cam.update(delta_time);
	glfwGetCursorPos(renderer::get_window(), &cursor_x, &cursor_y);

	if (glfwGetKey(renderer::get_window(), GLFW_KEY_W))
		sceneList[0].GetRigidBody().AddLinearImpulse(glm::vec3(0.0f, 1, 0.0f)*delta_time);
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_S))
		sceneList[0].GetRigidBody().AddLinearImpulse(glm::vec3(0.0f, -1, 0.0f)*delta_time);
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_A))
		sceneList[0].GetRigidBody().AddLinearImpulse(glm::vec3(-1.0f, 0, 0.0f)*delta_time);
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_D))
		sceneList[0].GetRigidBody().AddLinearImpulse(glm::vec3(1.0f, 0, 0.0f)*delta_time);
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_Q))
		sceneList[0].GetRigidBody().AddAngularForce(glm::vec3(0, 0, 5.0));
	if(glfwGetKey(renderer::get_window(), GLFW_KEY_SPACE))
	{
		for (auto & m : sceneList)
			m.GetRigidBody().AddLinearImpulse(glm::vec3(0, 2, 0)*delta_time);
	}

	PV = cam.get_projection() * cam.get_view();
	cam.update(static_cast<float>(delta_time));
	phys::SetCamera(cam);
	phys::SetCameraPos(cam.get_position());
	renderer::setClearColour(0, 0, 0);
	

	while (accumulator > physics_tick)
	{
		UpdatePhysics(sceneList,t, physics_tick, sceneFloor);
		accumulator -= physics_tick;
		t += physics_tick;
	}

	for (auto &e : sceneList)
	{
		e.Update(delta_time);
	}
	phys::Update(delta_time);
	prevCounter = counter;
	return true;
}

bool render()
{
	renderer::clear();
	renderer::bind(eff);
	// Render rest of scene.
	for (auto &e : sceneList)
	{
		RGBAInt32 col = GREY;
		mat.set_diffuse(col.tovec4());
		renderer::bind(mat, "mat");
		renderer::bind(light, "light");
		mat4 M = e.GetRigidBody().get_transform_matrix();
		mat3 N = e.GetRigidBody().get_normal_matrix();
		glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(PV * M));
		glUniformMatrix4fv(eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
		glUniformMatrix3fv(eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(N));
		e.Render();
	}
	// Render floor.
	phys::DrawScene();
	return true;
}

void main()
{
	// Create application
	app application;
	// Set load content, update and render methods
	application.set_load_content(load_content);
	application.set_update(update);
	application.set_render(render);
	// Run application
	application.run();
}