#include "Sampler.h"
namespace phys
{
	Sampler::Sampler()
	{
	}

	Sampler::~Sampler()
	{
	}

	std::vector<glm::vec3> Sampler::SamplePoints(Model model, int samples)
	{
		// How many rays to be shot through each face?
		int raysPerFace = std::min(samples, std::max(5, samples / 3));

		int i = 0;
		int face = 0;
		glm::vec3 o, u, v, w;
		while (i < samples)
		{
			face = (face + 1) % 6;
			switch (face)
			{
			case 0: // bottom face
				o = bounds.min()
				u = glm::vec3(extents.x, 0, 0);
				v = glm::vec3(0, 0, extents.z);
				w = glm::vec3(0, 1, 0);
				break;
			case 1: // top face
				o.x = bounds.min().x; o.y = bounds.max().y; o.z = bounds.min().z;
				u = glm::vec3(extents.x, 0, 0);
				v = glm::vec3(0, 0, extents.z);
				w = glm::vec3(0, -1, 0);
				break;
			case 2: // left face
				o = bounds.min();
				u = glm::vec3(0, extents.y, 0);
				v = glm::vec3(0, 0, extents.z);
				w = glm::vec3(1, 0, 0);
				break;
			case 3: // right face
				o.x = bounds.max().x; o.y = bounds.min().y; o.z = bounds.min().z;
				u = glm::vec3(0, extents.y, 0);
				v = glm::vec3(0, 0, extents.z);
				w = glm::vec3(-1, 0, 0);
				break;
			case 4: // front face
				o = bounds.min();
				u = glm::vec3(extents.x, 0, 0);
				v = glm::vec3(0, extents.y, 0);
				w = glm::vec3(0, 0, 1);
				break;
			case 5: // back face
				o.x = bounds.min().x; o.y = bounds.min().y; o.z = bounds.max().z;
				u = glm::vec3(extents.x, 0, 0);
				v = glm::vec3(0, extents.y, 0);
				w = glm::vec3(0, 0, -1);
				break;
			default: break;
			}

			RayCast ray;
			for (int i = 0; i < raysPerFace; i++)
			{
				if (originOffset < numOffsetSamples)
				{
					ray.SetOrigin(o + w * safetyOffset + u * originOffsets[2 * originOffset] + v * originOffsets[2 * originOffset + 1]);
					originOffset++;
				}
				else
				{
					ray.SetOrigin(o + w * safetyOffset + u * randomFloat01() + v * randomFloat01());
				}
				ray.SetDirection = w;
				ray.SetRange(99999);

				samples += ray.TraceRay(Model, depthStep, points, normals);
				if (i >= samples)
					break;

		}

		return std::vector<glm::vec3>();
	}

	std::vector<glm::vec3> Sampler::SampleShell(Model model)
	{
		return std::vector<glm::vec3>();
	}

	std::vector<glm::vec3> Sampler::SampleInterior(Model model)
	{
		return std::vector<glm::vec3>();
	}
}