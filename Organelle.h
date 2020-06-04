#pragma once
#include <vector>
#include "Compound.h"
#include "Sector.h"
#include "Universe.h"
#include "ReactionManager.h"
#include "Element.h"

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
	char UtilityMarker = 0;
	virtual void ConnectTo(Organelle* o);
	virtual void SeverAllConnections();
	virtual void SendRepositionRequests();
	virtual void ReceiveRepositionRequest(int deltax, int deltay, int force);
	virtual void Reposition();
	virtual void Activate();
	virtual void Display(sf::RenderWindow& window, int zoom, int staticXOffset, int staticYOffset);
	virtual int size();
	virtual bool IsAlive();
	virtual Element GetCriticalCharge();
	virtual void init(Compound struc, int crit, Organism* parentPtr);

	virtual void DoChemistry(std::vector<Compound*>& reactants);
	virtual void CheckConnectionDeath(std::list<Organelle*>& border);

private:
	unsigned char ConnectOneWay(Organelle* o, unsigned char Metadata);	
	void DisconnectOneWay(unsigned char Metadata);
	void UpdateConnectionMetaData(unsigned char MetadataToFindEntry, unsigned char MetadataToChangeEntryTo);
	std::vector<Organelle*> connections;
	std::vector<unsigned char> connectionMetaData;
	Element CriticalIdentity;
};


