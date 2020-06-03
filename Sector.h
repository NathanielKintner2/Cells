#pragma once
#include <vector>
#include "Utilities.h"
#include "Compound.h"



class Compound;
class Sector
{
public:
	Compound solution[25];
	std::vector<int> filledIdxs;
	//THIS IS NOT THE INDEX IN THE SOLUTION, IT IS THE INDEX IN FILLEDIDXS
	void RemoveCompoundAtIdx(int idx);
	void AddCompoundToLocationInSolution(Compound c, int location);

	//temporary, perhaps
	std::vector<Compound> sol;
};

