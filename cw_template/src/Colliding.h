#pragma once
#include "Model.h"
#include "ModelInfo.h"
#include "physics.h"
#include "MeshCut.h"
#include <glm/vec3.hpp>
using namespace phys;
using namespace glm;
namespace collision
{
	//class CollisionInfo;
	bool IsColliding(std::vector<Model>&sceneList, std::vector<CollisionInfo> &civ, Model &c1, Model &c2);
}
