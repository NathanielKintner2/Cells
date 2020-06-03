#include "Organelle.h"
#include "Organism.h"

void Organelle::ConnectTo(Organelle* o)
{
	for (Organelle *oldO : connections)
	{
		if (oldO == o)
		{
			return;
		}
	}
	ConnectOneWay(o);
	o->ConnectOneWay(this);
}

void Organelle::ConnectOneWay(Organelle* o)
{
	connections.push_back(o);
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
	Sector s = Universe::worldHexes[xpos / Universe::sectorPixels + ypos / Universe::sectorPixels * Universe::numxsectors];
	
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

	}
	else //add to structure
	{
		if (!PerformReactionIfGoodEnough(&toReactWith, &structure, rand() % 15 - 5, stability, ielem1, ielem2))
		{
			return;
		}
		
	}
	

	reactants.clear();
}