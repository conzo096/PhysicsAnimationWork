#include "stdafx.h"

namespace phys
{
	class Triangle
	{
		glm::vec3 a, b, c;
		public:
			Triangle() {};
			Triangle(glm::vec3 A, glm::vec3 B, glm::vec3 C) { a = A; b = B; c = C; };
		~Triangle() {};

		glm::vec3 GetA() { return a; }
		glm::vec3 GetB() { return b; }
		glm::vec3 GetC() { return c; }
	};
}