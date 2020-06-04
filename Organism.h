#pragma once

#include <vector>
#include "Compound.h"
#include "Organelle.h"

class Organism
{
public:
	std::vector<Organelle*> AllOrganelles;
	Organelle* center;
	std::string code;

	void SetPosition(int xpos, int ypos);

	void Reposition();
	void DoChemistry(std::vector<Compound*>& reactants);
	void DoDeaths();
	void DoActivations();

	~Organism()
	{
		for (Organelle* o : AllOrganelles)
		{
			delete o;
		}
	}
};

