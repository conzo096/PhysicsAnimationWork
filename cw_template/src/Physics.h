#pragma once
#include <glm/glm.hpp>
#include "Model.h"
#include "PlaneCollider.h"
#include "MeshCut.h"
#include "Colliding.h"
using namespace std;
void UpdatePhysics(vector <phys::Model > & physicsScene, const double t, const double dt, phys::PlaneCollider floor);
