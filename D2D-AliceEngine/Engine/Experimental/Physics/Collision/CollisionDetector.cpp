#include "pch.h"
#include "CollisionDetector.h"
#include <Component/Collider.h>

bool Physics::FCollisionDetector::BruteForceOverlapCheck(const std::vector<WeakObjectPtr<Collider>>& objects)
{
	for (size_t i = 0; i < objects.size(); ++i)
	{
		if(objects[i].expired()) continue;

		for (size_t j = i + 1; j < objects.size(); ++j)
		{
			if (objects[j].expired()) continue;

			if (objects[i]->IsOverlap(objects[j].Get()))
			{
				std::cout << "[Ãæµ¹] " << objects[i]->name << " " << objects[j]->name << " °ãÄ§\n";
			}
		}
	}

}
