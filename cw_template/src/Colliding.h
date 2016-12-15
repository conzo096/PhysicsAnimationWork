/* This class contains the methods for bounding volume detection.*/

#pragma once
#include "Model.h"
#include "ModelInfo.h"
#include "physics.h"
#include "MeshCut.h"
#include "PlaneCollider.h"
#include "glm\gtx\transform.hpp"

namespace collision
{

	bool IsColliding(std::vector<phys::Model>&sceneList, std::vector<phys::CollisionInfo> &civ, phys::Model &c1, phys::Model &c2);
	bool OnFloor(std::vector<phys::CollisionInfo>& collisions, phys::Model &c1, phys::PlaneCollider pc);
}
