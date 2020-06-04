#include "Organelle.h"
#include "Organism.h"


void Organelle::init(Compound struc, int crit, Organism * parentPtr)
{
	parent = parentPtr;
	structure = struc;
	criticalRegion = crit;
	CriticalIdentity = GetCriticalCharge();
	UtilityMarker = 0;
}

void Organelle::ConnectTo(Organelle* o)
{
	if (this == o)
	{
		return;
	}
	for (Organelle *oldO : connections)
	{
		if (oldO == o)
		{
			return;
		}
	}
	unsigned char metaData = o->ConnectOneWay(this, connections.size());
	connections.emplace_back(o);
	connectionMetaData.emplace_back(metaData);
}

//for a basic organelle, the connection metadata simply represents where the connection is sitting
//in that organelle's connections vector. that way, when we want that organelle to disconnect from us,
//they can send them their metadata so that they know exactly where to go to do the disconnect.
//effectively, this makes disconnecting into an O(1) process. Instead of searching through the list of
//connections to find us, we tell that organelle exactly where to find and delete us from their list
unsigned char Organelle::ConnectOneWay(Organelle* o, unsigned char Metadata)
{
	unsigned char ret = connections.size();
	connections.emplace_back(o);
	connectionMetaData.emplace_back(Metadata);
	return ret;
}

//severs all incoming connections, severing all outgoing connections is trivial and generally unnecessary
void Organelle::SeverAllConnections()
{
	int target = connections.size();//force an optimization, we dont care if connections changes size
	for (int i = 0; i < target; i++)
	{
		connections[i]->DisconnectOneWay(connectionMetaData[i]);
	}
}

void Organelle::DisconnectOneWay(unsigned char Metadata)
{
	FastDelete(connections, Metadata);
	FastDelete(connectionMetaData, Metadata);
	if (connections.size() != Metadata)
	{
		//we just moved an entry, so if that entry tries to delete we will not be looking in the wrong place.
		//we send them an updated piece of metadata so that they can find us and then change our delete metadata
		connections[Metadata]->UpdateConnectionMetaData(connectionMetaData[Metadata], Metadata);
	}
}

void Organelle::UpdateConnectionMetaData(unsigned char MetadataToFindEntry, unsigned char MetadataToChangeEntryTo)
{
	connectionMetaData[MetadataToFindEntry] = MetadataToChangeEntryTo;
}

void Organelle::SendRepositionRequests()
{
	for (Organelle* o : connections)
	{
		int xdelta = xpos - o->xpos;
		int ydelta = ypos - o->ypos;
		double distance = sqrt(xdelta * xdelta + ydelta * ydelta);
		int desiredDistance = o->criticalRegion*2 + criticalRegion*2;
		if (abs(distance - desiredDistance) > 15)
		{
			double ratio = desiredDistance / (distance + 1);
			o->ReceiveRepositionRequest(xdelta - xdelta * ratio, ydelta - ydelta * ratio, 1);
		}
	}
	for (Organelle* o : parent->AllOrganelles)
	{
		int xdelta = xpos - o->xpos;
		int ydelta = ypos - o->ypos;
		double distance = sqrt(xdelta * xdelta + ydelta * ydelta);
		int desiredDistance = o->criticalRegion*2 + criticalRegion*2 + 10;
		if (distance < desiredDistance)
		{
			double ratio = desiredDistance / (distance + 1);
			o->ReceiveRepositionRequest(xdelta - xdelta * ratio, ydelta - ydelta * ratio, 3);
		}

	}
}

void Organelle::ReceiveRepositionRequest(int deltax, int deltay, int force)
{
	xposRequest += deltax * force;
	yposRequest += deltay * force;
	totalRequestForce += force;
}

void Organelle::Reposition()
{
	int xmove = xposRequest / (totalRequestForce + 1);
	xpos += xmove;
	int ymove = yposRequest / (totalRequestForce + 1);
	ypos += ymove;
	xposRequest = xmove * 3;//continue to "coast" a little
	yposRequest = ymove * 3;
	totalRequestForce = 3;
}

void Organelle::Activate()
{
	return;
}

int Organelle::size()
{
	return criticalRegion;
}


void Organelle::Display(sf::RenderWindow& window, int zoom, int staticXOffset, int staticYOffset)
{
	sf::CircleShape drawOrg((size() * 1.8) / zoom, 20);
	drawOrg.setFillColor(sf::Color(0, 0, 0, 0));
	drawOrg.setOutlineThickness(10.0 / zoom);
	drawOrg.setOutlineColor(structure.GetColor());
	drawOrg.setPosition(staticXOffset + (xpos - size()) / zoom, staticYOffset + (ypos - size()) / zoom);
	drawOrg.setOrigin(size() * 0.9 / zoom, size() * 0.9 / zoom);
	window.draw(drawOrg);
	for (Organelle* ooo : connections)
	{
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(staticXOffset + xpos / zoom, staticYOffset + ypos / zoom), sf::Color(0,0,0,255)),
			sf::Vertex(sf::Vector2f(staticXOffset + ooo->xpos / zoom, staticYOffset + ooo->ypos / zoom), sf::Color(0,0,0,255))
		};

		window.draw(line, 2, sf::Lines);
	}
}

void Organelle::DoChemistry(std::vector<Compound*>& reactants)
{
	Sector& s = Universe::getSectorAtLocation(xpos, ypos);
	if (s.sol.size() == 0)
	{
		return;
	}
	int randIdx = rand() % s.sol.size();
	Compound &toReactWith = s.sol[randIdx];
	int stability = 0;
	int ielem1;
	int ielem2;
	if (rand() % 2 == 1)//remove from structure
	{
		if (!PerformReactionIfGoodEnough(&structure, &toReactWith, rand() % 15 - 5, stability, ielem1, ielem2))
		{
			return;
		}
		if (structure.mass == 0)
		{
			reactants.emplace_back(&toReactWith);
		}
		else
		{
			int numPieces;
			Compound* pieces = structure.SplitCompound(ielem1, numPieces);
			if(pieces == nullptr){
				reactants.emplace_back(&structure);
				reactants.emplace_back(&toReactWith);
			}
			else
			{
				int largest = -1;
				int size = -1;
				for (int i = 0; i < numPieces; i++)
				{
					if (pieces[i].mass > size)
					{
						largest = i;
						size = pieces[i].mass;
					}
				}
				structure = pieces[largest];
				Compound temp = toReactWith;
				FastDelete(s.sol, randIdx);
				for (int i = 0; i < numPieces; i++)
				{
					if (i != largest)
					{
						s.sol.emplace_back(pieces[i]);
					}
				}
				//note the unusual for loop indexing to avoid a later oboo
				for (int i = 1; i < numPieces; i++)
				{
					reactants.emplace_back(&s.sol[s.sol.size()-i]);
				}
				reactants.emplace_back(&structure);
				s.sol.emplace_back(temp);
				reactants.emplace_back(&s.sol[s.sol.size() - 1]);
			}
		}
	}
	else //add to structure
	{
		if (!PerformReactionIfGoodEnough(&toReactWith, &structure, rand() % 15 - 5, stability, ielem1, ielem2))
		{
			return;
		}
		if (toReactWith.mass == 0)
		{
			FastDelete(s.sol, randIdx);
			reactants.emplace_back(&structure);
		}
		else
		{

			int numPieces;
			Compound* pieces = toReactWith.SplitCompound(ielem1, numPieces);
			if (pieces == nullptr)
			{
				reactants.emplace_back(&structure);
				reactants.emplace_back(&toReactWith);
			}
			else
			{
				FastDelete(s.sol, randIdx);
				for (int i = 0; i < numPieces; i++)
				{
					s.sol.emplace_back(pieces[i]);
				}
				//note the unusual for loop indexing to avoid a later oboo
				for (int i = 1; i < numPieces+1; i++)
				{
					reactants.emplace_back(&s.sol[s.sol.size() - i]);
				}
				reactants.emplace_back(&structure);
			}
		}
	}
	AdjustEnergyValues(stability, reactants);
	reactants.clear();
}

bool Organelle::IsAlive()
{
	Element e = GetCriticalCharge();
	if (e.red == -1)
	{
		return false;
	}
	//as red increases (and therefore so does mass and size), so does the tolerance for damage
	int threshold = CriticalIdentity.red / 4;
	if (abs(e.red - CriticalIdentity.red) > threshold)
	{
		return false;
	}
	if (abs(e.blue - CriticalIdentity.blue) > threshold)
	{
		return false;
	}
	if (abs(e.green - CriticalIdentity.green) > threshold)
	{
		return false;
	}
	return true;
}

Element Organelle::GetCriticalCharge()
{
	Element ret = Element();
	if (structure.composition.size() < criticalRegion)
	{
		ret.red = -1;
		return ret;
	}
	for (int i = 0; i < criticalRegion; i++)
	{
		const Element& e = structure.composition[i];
		if (e.red != 0)
		{
			ret.red += e.red;
			ret.blue += e.blue;
			ret.green += e.green;
		}
	}
	return ret;
}

void Organelle::CheckConnectionDeath(std::list<Organelle*>& border)
{
	for (Organelle* o : connections)
	{
		if (o->UtilityMarker == 0)
		{
			o->UtilityMarker = 2;
			border.push_back(o);
		}
	}
}