#pragma once

#include <vector>
#include "Compound.h"
#include "Organelle.h"

class Organism
{
public:
	std::vector<Organelle*> AllOrganelles;
	Organelle* center;

	void Reposition();

	~Organism()
	{
		for (Organelle* o : AllOrganelles)
		{
			delete o;
		}
	}
};

