/*
		CURRENT ISSUE. 
		Need to be able to change camera and mvp matrix. Might have to rewrite that segment as well.
*/


#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
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

static vector<Model> sceneList;
PlaneCollider sceneFloor;
effect eff;
target_camera cam;
glm::mat4 PV;
directional_light light;
material mat;

bool load_content()
{
	phys::Init();

	Model test;
	test.SetModelInfo(LoadCube(glm::vec3(1, 1, 1)));
	test.GetRigidBody().SetInitialPosition(glm::dvec3(0, 10, 0));
	test.GetRigidBody().SetMass(10);
	test.CreateBuffers();
	sceneList.push_back(test);

	Model test1;
	test1.SetModelInfo(LoadCube(glm::vec3(1, 1, 1)));
	test1.GetRigidBody().SetInitialPosition(glm::vec3(0, 0, 0));
	test1.CreateBuffers();
	sceneList.push_back(test1); 

	sceneFloor = PlaneCollider();


	eff = effect();
	eff.add_shader("shaders/phys_phong.vert", GL_VERTEX_SHADER);
	eff.add_shader("shaders/phys_phong.frag", GL_FRAGMENT_SHADER);
	eff.build();
	cam.set_position(vec3(10.0f, 10.0f, 10.0f));
	cam.set_target(vec3(0.0f, 0.0f, 0.0f));
	auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
	cam.set_projection(quarter_pi<float>(), aspect, 2.414f, 1000.0f);
	light.set_ambient_intensity(vec4(0.5f, 0.5f, 0.5f, 1.0f));
	light.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	light.set_direction(vec3(0.0f, 1.0f, 0.0f));
	mat = material(vec4(0.0f, 0.0f, 0.0f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f), vec4(1.0f, 1.0f, 1.0f, 1.0f), 25.0f);
	return true;
}


bool update(float delta_time)
{
	bool spacePressed = false;
	static double t = 0.0;
	static double accumulator = 0.0;
	accumulator += delta_time;

	static float rot = 0.0f;
	rot += 0.2f * delta_time;
	phys::SetCameraPos(rotate(vec3(15.0f, 12.0f, 15.0f), rot, vec3(0, 1.0f, 0)));
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_W))
		sceneList[0].GetRigidBody().AddLinearImpulse(glm::vec3(0.0f, 1, 0.0f)*delta_time);
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_S))
		sceneList[0].GetRigidBody().AddLinearImpulse(glm::vec3(0.0f, -1, 0.0f)*delta_time);
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_A))
		sceneList[0].GetRigidBody().AddLinearImpulse(glm::vec3(-1.0f, 0, 0.0f)*delta_time);
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_D))
		sceneList[0].GetRigidBody().AddLinearImpulse(glm::vec3(1.0f, 0, 0.0f)*delta_time);
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_Q))
		sceneList[sceneList.size()-1].GetRigidBody().AddAngularForce(glm::vec3(0, 0, 5.0));
	if(spacePressed == false)
	if(glfwGetKey(renderer::get_window(), GLFW_KEY_SPACE))
	{
		spacePressed = true;
		std::cout << "HERE WE GO" << std::endl;
		Plane testPlane(glm::vec3(0, -0.5, 0.5), glm::vec3(0, 0.5, -0.5), glm::vec3(0, 0.5, 0.5));
		//Plane testPlane(glm::vec3(-10, -10, 10), glm::vec3(-10, 10, -10), glm::vec3(-10, 10, 10));
		//Plane testPlane(glm::vec3(-0.5, 0,0.5), glm::vec3(0.5, 0, -0.5), glm::vec3(0.5, 0, 0.5));
		Model test;
		info i = SliceModel(sceneList[1], testPlane);
		test.SetModelInfo(i);
		test.GetRigidBody().SetInitialPosition(sceneList[1].GetRigidBody().GetPosition() + glm::dvec3(-3,0,0));
		std::cout << to_string(test.GetRigidBody().position) << std::endl;
		//test.SetBoundingBox(BoundingBox(test.GetModelInfo().positions));
		//test.SetSphereCollider(SphereCollider(test.GetModelInfo().positions));
		test.CreateBuffers();
		sceneList.push_back(test);
	}


	PV = cam.get_projection() * cam.get_view();
	cam.update(static_cast<float>(delta_time));
	renderer::setClearColour(0, 0, 0);
	

	while (accumulator > physics_tick)
	{
		UpdatePhysics(sceneList,t, physics_tick);
		accumulator -= physics_tick;
		t += physics_tick;
	}

	for (auto &e : sceneList)
	{
		e.Update(delta_time);
	}
	phys::Update(delta_time);
	return true;
}

bool render()
{
	renderer::clear();
	renderer::bind(eff);
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