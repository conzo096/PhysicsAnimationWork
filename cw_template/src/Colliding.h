#pragma once
#include "Model.h"
#include "ModelInfo.h"
#include "physics.h"
#include "MeshCut.h"
#include "PlaneCollider.h"
using namespace phys;
using namespace glm;
namespace collision
{

	bool IsColliding(std::vector<Model>&sceneList, std::vector<CollisionInfo> &civ, Model &c1, Model &c2);
	bool OnFloor(std::vector<CollisionInfo>& collisions, Model &c1, PlaneCollider pc);
}
