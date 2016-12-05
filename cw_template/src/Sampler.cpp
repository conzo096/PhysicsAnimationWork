#include "Sampler.h"
namespace phys
{
	Sampler::Sampler()
	{
	}

	Sampler::~Sampler()
	{
	}

	std::vector<glm::vec3> Sampler::SamplePoints(Model model, int numSamples)
	{
		std::vector<glm::vec3> samplePoints = std::vector<glm::vec3>();
		// How many rays to be shot through each face?
		int raysPerFace = std::min(numSamples, std::max(5, numSamples / 3));
		int i = 0;
		// What face to start ray from.
		int face = 0;
		// Ray to find intersection points.
		RayCast ray;
		while (i < numSamples)
		{
			// Change to a new face.
			face = (face + 1) % 6;
			switch (face)
			{
			case 0: // bottom face

				ray.SetRandomOrigin(model.GetBoundingBox().GetFrontBottomRight(), model.GetBoundingBox().GetBackBottomLeft());
				ray.SetDirection(glm::vec3(0, 1, 0));
				ray.SetRange(glm::distance(model.GetBoundingBox().GetBackBottomRight(), model.GetBoundingBox().GetBackTopRight()));
				break;
			case 1: // top face

				ray.SetRandomOrigin(model.GetBoundingBox().GetFrontTopRight(), model.GetBoundingBox().GetBackTopLeft());
				ray.SetDirection(glm::vec3(0, -1, 0));
				ray.SetRange(glm::distance(model.GetBoundingBox().GetBackBottomRight(), model.GetBoundingBox().GetBackTopRight()));

				break;
			case 2: // left face

				ray.SetRandomOrigin(model.GetBoundingBox().GetFrontBottomLeft(), model.GetBoundingBox().GetBackTopLeft());
				ray.SetDirection(glm::vec3(1, 0, 0));
				ray.SetRange(glm::distance(model.GetBoundingBox().GetBackBottomRight(), model.GetBoundingBox().GetBackBottomLeft()));

				break;
			case 3: // right face

				ray.SetRandomOrigin(model.GetBoundingBox().GetFrontBottomRight(), model.GetBoundingBox().GetBackTopRight());
				ray.SetDirection(glm::vec3(-1, 0, 0));
				ray.SetRange(glm::distance(model.GetBoundingBox().GetBackBottomRight(), model.GetBoundingBox().GetBackBottomLeft()));

				break;
			case 4: // front face

				ray.SetRandomOrigin(model.GetBoundingBox().GetFrontBottomLeft(), model.GetBoundingBox().GetFrontTopRight());
				ray.SetDirection(glm::vec3(0, 0, 1));
				ray.SetRange(glm::distance(model.GetBoundingBox().GetBackBottomRight(), model.GetBoundingBox().GetFrontBottomRight()));
				break;
			case 5: // back face

				ray.SetRandomOrigin(model.GetBoundingBox().GetBackBottomLeft(), model.GetBoundingBox().GetBackTopRight());
				ray.SetDirection(glm::vec3(0, 0, -1));
				ray.SetRange(glm::distance(model.GetBoundingBox().GetBackBottomRight(), model.GetBoundingBox().GetFrontBottomRight()));
				break;
		
			default: break;
			}
			// Sample points from the ray.
			for (int i = 0; i < raysPerFace; i++)
			{
				samplePoints.push_back(SampleRandomPoint(model, ray));
				if (i >= numSamples)
					break;

			}
		}

		return samplePoints;
	}

	std::vector<glm::vec3> Sampler::SampleShell(Model model)
	{
		return std::vector<glm::vec3>();
	}

	std::vector<glm::vec3> Sampler::SampleInterior(Model model)
	{
		return std::vector<glm::vec3>();
	}

	glm::vec3 Sampler::SampleRandomPoint(Model model, RayCast ray)
	{
		glm::vec3 point1, point2, point3;

		return glm::vec3();
	}
}