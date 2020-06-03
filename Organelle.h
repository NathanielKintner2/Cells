#pragma once
#include <vector>
#include "Compound.h"
#include "Sector.h"
#include "Universe.h"
#include "ReactionManager.h"

class Organism;

class Organelle
{
public:
	int xpos;
	int ypos;
	int xposRequest;
	int yposRequest;
	int totalRequestForce;
	Organism* parent;
	Compound structure;
	int criticalRegion;
	unsigned char metaData1;
	unsigned char metaData2;
	unsigned char metaData3;
	unsigned char metaData4;
	virtual void ConnectTo(Organelle* o);
	virtual void SendRepositionRequests();
	virtual void ReceiveRepositionRequest(int deltax, int deltay, int force);
	virtual void Reposition();
	virtual void Activate();
	virtual void Display(sf::RenderWindow& window, int zoom, int staticXOffset, int staticYOffset);
	virtual int size();

	virtual void DoChemistry(std::vector<Compound*>& reactants);

private:
	void ConnectOneWay(Organelle* o);
	std::vector<Organelle*> connections;
};


