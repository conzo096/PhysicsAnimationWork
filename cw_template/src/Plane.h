#include "stdafx.h"

namespace phys
{
	enum side {BEHIND = -1, ON = 0, INFRONT = 1};

	class Plane
	{
		glm::vec3 point;
		glm::vec3 normal;
	
		public:
		Plane() {};
		~Plane() {};

		int SideOfPlane(glm::vec3 p);

		void SetPoint(glm::vec3 p){point = p;}
		void SetNormal(glm::vec3 n){normal = n;}

		glm::vec3 GetPoint(){return point;}
		glm::vec3 GetNormal(){	return normal; }

		
	};
}