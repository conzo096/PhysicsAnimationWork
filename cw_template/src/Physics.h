#pragma once
#include <glm/glm.hpp>
#include "Model.h"
#include "Colliding.h"
using namespace std;
using namespace glm;
void UpdatePhysics(vector <phys::Model > & physicsScene, const double t, const double dt);
