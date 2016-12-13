#include "Sampler.h"
namespace phys
{
	glm::vec3 RandomPointOnLine(glm::vec3 maxBound, glm::vec3 minBound)
	{

		// Broke.
		glm::vec3 point = glm::vec3(0, 0, 0);
		
		int range = maxBound.x - minBound.x + 1;
		point.x = rand() % (int)(range + minBound.x);
		range = maxBound.y - minBound.y + 1;
		point.y = rand() % (int)(range + minBound.y);
		range = maxBound.z - minBound.z + 1;
		point.z = rand() % (int)(range + minBound.z);

		return point;
	}


	std::vector<Plane> CreateSplittingPlanes(BoundingBox bb, int numSamples)
	{
		std::vector<Plane> splittingPlanes;
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
			face = (face + 1) % 6;
			switch (face)
			{
			case 0: // bottom face

				startPoint = (bb.GetFrontBottomRight(), bb.GetBackBottomLeft());
				endPoint = RandomPointOnLine(bb.GetFrontTopRight(), bb.GetBackTopLeft());
				endPoint.y = startPoint.y;
				norm = glm::normalize(glm::cross(startPoint, endPoint));
				splittingPlanes.push_back(Plane(glm::cross(startPoint, endPoint), norm));
				
				break;
			case 1: // top face

				startPoint = RandomPointOnLine(bb.GetFrontTopRight(), bb.GetBackTopLeft());
				endPoint = (bb.GetFrontBottomRight(), bb.GetBackBottomLeft());
				endPoint.y = startPoint.y;
				norm = glm::normalize(glm::cross(startPoint, endPoint));
				splittingPlanes.push_back(Plane(glm::cross(startPoint, endPoint), norm));
				
				break;
			case 2: // left face

				startPoint = RandomPointOnLine(bb.GetFrontBottomLeft(), bb.GetBackTopLeft());
				endPoint = RandomPointOnLine(bb.GetFrontBottomRight(), bb.GetBackTopRight());
				endPoint.x = startPoint.x;
				norm = glm::normalize(glm::cross(startPoint, endPoint));
				splittingPlanes.push_back(Plane(glm::cross(startPoint, endPoint), norm));
				
				
				break;
			case 3: // right face

				startPoint = RandomPointOnLine(bb.GetFrontBottomRight(), bb.GetBackTopRight());
				endPoint = RandomPointOnLine(bb.GetFrontBottomLeft(), bb.GetBackTopLeft());
				endPoint.x = startPoint.x;
				norm = glm::normalize(glm::cross(startPoint, endPoint));
				splittingPlanes.push_back(Plane(glm::cross(startPoint, endPoint), norm));
				
				break;
			case 4: // front face

				startPoint = RandomPointOnLine(bb.GetFrontBottomLeft(), bb.GetFrontTopRight());
				endPoint = RandomPointOnLine(bb.GetBackBottomLeft(), bb.GetBackTopRight());
				endPoint.z = startPoint.z;
				norm = glm::normalize(glm::cross(startPoint, endPoint));
				splittingPlanes.push_back(Plane(glm::cross(startPoint, endPoint), norm));
				
				break;
			case 5: // back face

				startPoint = RandomPointOnLine(bb.GetBackBottomLeft(), bb.GetBackTopRight());
				endPoint = RandomPointOnLine(bb.GetFrontBottomLeft(), bb.GetFrontTopRight());
				endPoint.z = startPoint.z;
				norm = glm::normalize(glm::cross(startPoint, endPoint));
				splittingPlanes.push_back(Plane(glm::cross(startPoint, endPoint), norm));

				break;

			default: break;


			}

			i++;
			if (i >= numSamples)
				break;
		}
		return splittingPlanes;
	}
}