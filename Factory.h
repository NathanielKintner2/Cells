#pragma once
#include "Organelle.h"

class Organelle;

class Factory: public Organelle
{
public:
	Compound goal;
	int currGoalIdx;
	Compound progress;
};