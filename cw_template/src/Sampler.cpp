#include "Sampler.h"
namespace phys
{
	double RandomPoint(float range, float minValue)
	{

		// Broke.
		double point;
		srand((int)time(0));
		
		point = ((rand() % (int)(range)) / range*0.5) + minValue;

		
		return point;
	}


	void CreateSplittingPlanes(BoundingBox bb, int numSamples, std::vector<Plane> &splittingPlanes)
	{
		glm::vec3 startPoint(0.0);
		glm::vec3 endPoint(0.0);
		glm::vec3 norm(0.0);
		int i = 0;
		// What face to start ray from.
		int face = 0;
		// Ray to find intersection points.
		while (i < numSamples)
		{
			// Change to a new face.

			srand((time(NULL)));
			face =	(face + (rand() % numSamples)) % 4;
			switch (face)
			{
			case 0: //Cut along y axis.

				/*startPoint.x = RandomPoint(bb.GetLength(),bb.GetFrontBottomRight().x);
				startPoint.y = RandomPoint(bb.GetHeight(), bb.GetFrontBottomRight().y);
				startPoint.z = RandomPoint(bb.GetWidth(), bb.GetFrontBottomRight().z);

				endPoint.x = RandomPoint(bb.GetHeight(), bb.GetFrontTopRight().y);
				endPoint.y = startPoint.y;
				endPoint.z = RandomPoint(bb.GetWidth(), bb.GetFrontTopRight().z);

				norm = glm::normalize(glm::cross(startPoint, endPoint));
				splittingPlanes.push_back(Plane(glm::cross(startPoint, endPoint), norm));*/


				splittingPlanes.push_back(Plane(glm::vec3(0, -0.5, 0.5), glm::vec3(0, 0.5, -0.5), glm::vec3(0, 0.5, 0.5)));

				break;
			case 1: // Cut along x axis.


				//startPoint.x = RandomPoint(bb.GetLength(), bb.GetFrontBottomRight().x);
				//startPoint.y = RandomPoint(bb.GetHeight(), bb.GetFrontBottomRight().y);
				//startPoint.z = RandomPoint(bb.GetWidth(), bb.GetFrontBottomRight().z);

				//endPoint.x = startPoint.x;
				//endPoint.y = RandomPoint(bb.GetHeight(), bb.GetFrontTopLeft().y);
				//endPoint.z = RandomPoint(bb.GetWidth(), bb.GetFrontTopLeft().z);


				//norm = glm::normalize(glm::cross(startPoint, endPoint));
				//splittingPlanes.push_back(Plane(glm::cross(startPoint, endPoint), norm));

				splittingPlanes.push_back(Plane(glm::vec3(0, -0.5, 0.5), glm::vec3(0, 0.5, -0.5), glm::vec3(0, 0.5, 0.5)));

				break;
			case 2: // Cut along z axis.

				/*startPoint.x = RandomPoint(bb.GetLength(), bb.GetFrontBottomRight().x);
				startPoint.y = RandomPoint(bb.GetHeight(), bb.GetFrontBottomRight().y);
				startPoint.z = RandomPoint(bb.GetWidth(), bb.GetFrontBottomRight().z);

				endPoint.x = RandomPoint(bb.GetLength(), bb.GetBackBottomRight().x);
				endPoint.y = RandomPoint(bb.GetWidth(), bb.GetBackBottomRight().y);
				endPoint.z = startPoint.z;
				
				norm = glm::normalize(glm::cross(startPoint, endPoint));
				splittingPlanes.push_back(Plane(glm::cross(startPoint, endPoint), norm));*/

				splittingPlanes.push_back(Plane(glm::vec3(-0.5, 0, 0.5), glm::vec3(0.5, 0, -0.5), glm::vec3(0.5, 0, 0.5)));
				break;
			case 4:
				splittingPlanes.push_back(Plane(glm::vec3(-0.5, 0.2, 0.5), glm::vec3(0.5, 0.2, -0.5), glm::vec3(0.5, 0.2, 0.5)));
				break;
			default: break;

			}
			i++;	
			if (i >= numSamples)
				break;
		}


		//	//Plane testPlane(glm::vec3(0, -0.4, 0.4), glm::vec3(0, 0.4, -0.4), glm::vec3(0, 0.4, 0.4));
		//	//Plane testPlane(glm::vec3(-0.5, -0.2, 0.5), glm::vec3(0.5, -0.2, -0.5), glm::vec3(0.5, -0.2, 0.5));

		//	//Plane testPlane(glm::vec3(-10, -10, 10), glm::vec3(-10, 10, -10), glm::vec3(-10, 10, 10));
		//	//Plane testPlane(glm::vec3(-0.5, 0,0.5), glm::vec3(0.5, 0, -0.5), glm::vec3(0.5, 0, 0.5));

	//	splittingPlanes.push_back(Plane(glm::vec3(-0.5, 0.5, 0), glm::vec3(0.5, -0.5, 0), glm::vec3(0.5, 0.5, 0)));
	//	splittingPlanes.push_back(Plane(glm::vec3(-0.5, 0.2, -0.5), glm::vec3(-0.5, -0.2, -0.5), glm::vec3(0.5, -0.2, -0.5)));
	//	splittingPlanes.push_back(Plane(glm::vec3(-10, -10, 10), glm::vec3(-10, 10, -10), glm::vec3(-10, 10, 10)));
	//	splittingPlanes.push_back(Plane(glm::vec3(-0.5, 0.2, 0.5), glm::vec3(0.5, 0.2, -0.5), glm::vec3(0.5, 0.2, 0.5)));
	}
}