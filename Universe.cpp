#include "Universe.h"
#include "Organism.h"

using namespace Universe;

std::vector<Sector> Universe::worldHexes(0);
std::vector<Element> Universe::ptable(0);
std::vector<Organism*> Universe::allLife(0);
typedef std::unordered_map<char, Element> hashElement;
hashElement Universe::ptableHash;


int Universe::numxsectors = 0;
int Universe::numysectors = 0;
int Universe::sectorPixels = 320;

void Universe::init(int numxsectors, int numysectors)
{	
	for (int i = 0; i < numxsectors * numysectors; i++)
	{
		worldHexes.emplace_back(Sector());
	}
	Universe::numxsectors = numxsectors;
	Universe::numysectors = numysectors;
}

Compound& Universe::getByRelation(int& editSector, int& editSquare, int direction)
{
	switch (direction)
	{
	case 0://go down
		editSquare += 5;
		if (editSquare >= 25)
		{
			editSquare = editSquare % 25;
			editSector += Universe::numxsectors;
			editSector = editSector % Universe::worldHexes.size();
		}
		break;
	case 1://go left
		editSquare -= 1;
		if ((editSquare + 5) % 5 == 4)
		{
			editSquare = editSquare + 5;
			editSector -= 1;
			if ((editSector + Universe::numxsectors) % Universe::numxsectors == Universe::numxsectors - 1)
			{
				editSector += Universe::numxsectors;
			}
		}
		break;
	case 2://go up
		editSquare -= 5;
		if (editSquare < 0)
		{
			editSquare = editSquare + 25;
			editSector -= Universe::numxsectors;
			if (editSector < 0)
			{
				editSector += Universe::worldHexes.size();
			}
		}
		break;
	case 3://go right
		editSquare += 1;
		if (editSquare % 5 == 0)
		{
			editSquare = editSquare - 5;
			editSector += 1;
			if (editSector % Universe::numxsectors == 0)
			{
				editSector -= Universe::numxsectors;
			}
		}
		break;
	}
	return Universe::worldHexes[editSector].solution[editSquare];
		
}
