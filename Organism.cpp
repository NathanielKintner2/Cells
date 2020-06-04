#include "Organism.h"

void Organism::Reposition()
{
	for (Organelle* o : AllOrganelles)
	{
		o->SendRepositionRequests();
		int jostle = (o->totalRequestForce + 1) * 2;
		o->ReceiveRepositionRequest(rand() % jostle*2 - jostle + 1, rand() % jostle *2 - jostle + 1, 1);
	}
	for (Organelle* o : AllOrganelles)
	{
		o->Reposition();
	}
}
void Organism::DoChemistry(std::vector<Compound*>& reactants)
{
	for (Organelle* o : AllOrganelles)
	{
		o->DoChemistry(reactants);
	}
}

void Organism::DoDeaths()
{
	bool doDeath = false;
	for (Organelle* o : AllOrganelles)
	{
		if (!o->IsAlive())
		{
			doDeath = true;
			o->UtilityMarker = 1;
		}
	}
	if (doDeath)//at least one organelle has failed
	{
		//cut out all dead organelles
		for (Organelle* o : AllOrganelles)
		{
			if (o->UtilityMarker == 1)
			{
				o->SeverAllConnections();				
			}
		}
		//now that we've cut our losses, we need to determine if any organelles are stranded and
		//therefore must also be cut
		if (center->UtilityMarker == 1)
		{
			center = nullptr;//center is dead, kill the whole organism in a later method
			return;
		}
		std::list<Organelle*> border;
		center->UtilityMarker = 2;
		border.push_back(center);
		while (border.size() != 0)
		{
			Organelle* o = border.front();
			border.pop_front();
			//will add to border if there are more connections to check
			o->CheckConnectionDeath(border);
		}
		//now, all living and connected organelles are marked with a 2
		for (int i = 0; i < AllOrganelles.size();)
		{
			if (AllOrganelles[i]->UtilityMarker != 2)
			{
				//free memory and delete from the the list
				delete AllOrganelles[i];
				FastDelete(AllOrganelles, i);
			}
			else
			{
				++i;
			}
		}
	}
}

void Organism::DoActivations()
{
	for (Organelle* o : AllOrganelles)
	{
		o->Activate();
	}
}

void Organism::SetPosition(int xpos, int ypos)
{
	for (Organelle* o : AllOrganelles)
	{
		o->xpos = xpos + rand() % 200 - 100;
		o->ypos = ypos + rand() % 200 - 100;
	}
}

