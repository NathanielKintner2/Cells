#pragma once
#include "Sector.h"
#include "Compound.h"
#include "ReactionManager.h"
#include "HexGrids.h"
#include <vector>
#include <unordered_map>

class Sector;
class Organism;
namespace Universe
{
	extern std::vector<Sector> worldHexes;
	extern std::vector<Element> ptable;
	extern std::unordered_map<char, Element> ptableHash;
	extern std::vector<Organism*> allLife;
	void init(int numsectors, int sectorsize);
	Compound& getByRelation(int& editSector, int& editSquare, int direction);
	extern int numxsectors;
	extern int numysectors;
	extern int sectorPixels;
}

