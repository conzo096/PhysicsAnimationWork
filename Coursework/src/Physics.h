// Physics class is the central update feature for rigid bodies and collision resolving. 
#pragma once
#include <glm/glm.hpp>
#include "Model.h"
#include "PlaneCollider.h"
#include "MeshCut.h"
#include "Colliding.h"
void UpdatePhysics(std::vector<phys::Model > & physicsScene, const double t, const double dt, phys::PlaneCollider floor);
