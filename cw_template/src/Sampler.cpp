#include "Sampler.h"
namespace phys
{
	glm::vec3 RandomPoint(glm::vec3 minValue, glm::vec3 maxValue)
	{
	
		glm::vec3 direction = maxValue - minValue;
		direction *= 0.5; // Midway.
		glm::vec3 midPoint = minValue + direction;
		return midPoint;
	}


	void CreateSplittingPlanes(BoundingBox bb, int numSamples, std::vector<Plane> &splittingPlanes)
	{
		glm::vec3 midPoint(0.0);
		glm::vec3 norm(0.0);
		std::vector<glm::vec3> linePoints;
		int i = 0;
		// What face to start ray from.
		int face = 0;
		// Ray to find intersection points.
		while (i < numSamples)
		{
			// Change to a new face.

			//srand((time(NULL)));
			face =	(face + 1);
			switch (face)
			{
			case 0: //Cut along y axis.

				linePoints.clear();
				linePoints.push_back(bb.GetBackBottomRight());
				linePoints.push_back(bb.GetBackTopLeft());
				for (int i = 0; i < 3; i++)
				{
					linePoints.push_back(RandomPoint(bb.GetBackBottomRight(), bb.GetBackTopRight()));
					std::shuffle(linePoints.begin(),linePoints.end(),std::default_random_engine());
				}

				norm = glm::vec3(0,1,0);
				// Shuffle is random so just take first point.
				splittingPlanes.push_back(Plane(linePoints[0], norm));
				splittingPlanes.push_back(Plane(linePoints[2], norm));

				//splittingPlanes.push_back(Plane(glm::vec3(0, -0.5, 0.5), glm::vec3(0, 0.5, -0.5), glm::vec3(0, 0.5, 0.5)));

				break;
			case 1: // Cut along x axis.

				linePoints.clear();
				linePoints.push_back(bb.GetBackBottomRight());
				linePoints.push_back(bb.GetBackTopLeft());
				for (int i = 0; i < 3; i++)
				{
					linePoints.push_back(RandomPoint(bb.GetBackBottomRight(), bb.GetBackTopRight()));
					std::shuffle(linePoints.begin(), linePoints.end(),
						std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
				}

				norm = glm::vec3(1, 0, 0);
				// Shuffle is random so just take first point.
				splittingPlanes.push_back(Plane(linePoints[0], norm));
				splittingPlanes.push_back(Plane(linePoints[2], norm));

			//	splittingPlanes.push_back(Plane(glm::vec3(0, -0.5, 0.5), glm::vec3(0, 0.5, -0.5), glm::vec3(0, 0.5, 0.5)));

				break;
			case 2: // Cut along z axis.

				linePoints.clear();
				linePoints.push_back(bb.GetBackBottomRight());
				linePoints.push_back(bb.GetBackTopLeft());
				for (int i = 0; i < 3; i++)
				{
					linePoints.push_back(RandomPoint(bb.GetBackBottomRight(), bb.GetBackTopRight()));
					std::shuffle(linePoints.begin(), linePoints.end(),
						std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
				}

				norm = glm::vec3(0, 0, 1);
				// Shuffle is random so just take first point.
				splittingPlanes.push_back(Plane(linePoints[0], norm));
				splittingPlanes.push_back(Plane(linePoints[2], norm));

				//				splittingPlanes.push_back(Plane(glm::vec3(-0.5, 0, 0.5), glm::vec3(0.5, 0, -0.5), glm::vec3(0.5, 0, 0.5)));
				
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

		// Shuffle the order of the splitting planes. Should make each fracture unique.
		std::shuffle(splittingPlanes.begin(), splittingPlanes.end(),
			std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));

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