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

