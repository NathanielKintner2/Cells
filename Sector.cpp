#include "Sector.h"
#include "Compound.h"


void Sector::RemoveCompoundAtIdx(int idx)
{
	int location = filledIdxs[idx];
	FastDelete(filledIdxs, idx);
	solution[location] = Compound();
}

void Sector::AddCompoundToLocationInSolution(Compound c, int location)
{
	solution[location] = c;
	filledIdxs.emplace_back(location);

}