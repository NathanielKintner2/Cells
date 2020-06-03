#include "EntropyManager.h"

bool ResolveMovement(int direction, int& startSector, int& startLocation, Compound& movingComp);

void DoEntropy(int secIdx)
{
	Sector& s = Universe::worldHexes[secIdx];
	//a temporary, extremely simple, extremely random entropy routine
	if (s.sol.size() != 0)
	{
		int delidx = rand() % s.sol.size();
		Compound c = s.sol[delidx];
		FastDelete(s.sol, delidx);
		Universe::worldHexes[rand() % Universe::worldHexes.size()].sol.emplace_back(c);		
	}
	return;
	if (s.filledIdxs.size() == 0)
	{
		return;
	}
	int entropyIdx = rand() % s.filledIdxs.size();

	int currsector = secIdx;
	int currlocation = s.filledIdxs[entropyIdx];

	Compound c = s.solution[currlocation];
	s.RemoveCompoundAtIdx(entropyIdx);

	int currVert = abs(c.Vvert / (c.mass * 10));
	int currHoriz = abs(c.Vhoriz / (c.mass * 10));
	int total = currHoriz + currVert;

	while (total != 0)
	{
		int selection = rand() % total;
		selection = selection - currHoriz;
		if (selection < 0)
		{
			currHoriz--;
			int direction = 0;
			if (c.Vhoriz < 0)
			{
				direction = 1;
			}
			else
			{
				direction = 3;
			}
			if (!ResolveMovement(direction, currsector, currlocation, c))
			{
				//Universe::worldHexes[currsector].AddCompoundToLocationInSolution(c, currlocation);
				//c = Universe::getByRelation(direction, currsector, currlocation);
				break;
			}
		}
		else
		{
			currVert--;
			int direction = 0;
			if (c.Vvert < 0)
			{
				direction = 0;
			}
			else
			{
				direction = 2;
			}
			if (!ResolveMovement(direction, currsector, currlocation, c))
			{
				//Universe::worldHexes[currsector].AddCompoundToLocationInSolution(c, currlocation);
				//c = Universe::getByRelation(direction, currsector, currlocation);
				break;
			}
		}
		total--;		
	}


	int xpos = currsector % Universe::numxsectors;
	if (xpos > Universe::numxsectors / 2)
	{
		c.Vhoriz += c.mass;
	}
	else
	{
		c.Vhoriz -= c.mass;
	}
	int ypos = currsector / Universe::numxsectors;
	if (ypos > Universe::numysectors / 2)
	{
		c.Vvert -= c.mass;
	}
	else
	{
		c.Vvert += c.mass;
	}
	if (abs(xpos - Universe::numxsectors / 2) > abs(ypos - Universe::numysectors / 2))
	{
		if (xpos > Universe::numxsectors / 2)
		{
			c.Vhoriz += c.mass;
		}
		else
		{
			c.Vhoriz -= c.mass;
		}
	}
	else
	{
		if (ypos > Universe::numysectors / 2)
		{
			c.Vvert -= c.mass;
		}
		else
		{
			c.Vvert += c.mass;
		}
	}
	c.Vvert = (rand() % 50 - 25) * c.mass;//GET RID OF THESE TO REACTIVATE GRAVITY
	c.Vhoriz = (rand() % 50 - 25) * c.mass;//GET RID OF THESE TO REACTIVATE GRAVITY
	c.Vvert += rand() % 50 - 25;
	c.Vhoriz += rand() % 50 - 25;
	Universe::worldHexes[currsector].AddCompoundToLocationInSolution(c, currlocation);
}

bool ResolveMovement(int direction, int& startSector, int &startLocation, Compound& movingComp)
{
	int editsecnum = startSector;
	int editlocation = startLocation;
	Compound &runinto = Universe::getByRelation(editsecnum, editlocation, direction);
	if (runinto.mass == 0)
	{
		startSector = editsecnum;
		startLocation = editlocation;
		return true;
	}
	else
	{
		///resolve bumping into something
		int horizVelDiff = runinto.Vhoriz - movingComp.Vhoriz;
		int vertVelDiff = runinto.Vvert - movingComp.Vvert;

		runinto.Vvert -= vertVelDiff;
		runinto.Vhoriz -= horizVelDiff;

		movingComp.Vvert += vertVelDiff;
		movingComp.Vhoriz += horizVelDiff;
		
		return false;
	}
}
