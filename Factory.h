#pragma once
#include "Organelle.h"
#include "Organism.h"
#include "GeneticsManager.h"
#include <string>
#include <sstream>
#include <vector>
#include <list>

class Organelle;

class Factory: public Organelle
{
public:
	std::vector<Compound> toDo;
	std::list<Compound> complete;
	int currToDoIdx;
	int currGoalIdx;
	Compound progress;

	void Activate();
	void init(Compound struc, int crit, Organism* parentPtr);
	void AdvanceGoalCursor();
};