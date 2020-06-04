#include "Factory.h"

void Factory::init(Compound struc, int crit, Organism* parentPtr)
{
	Organelle::init(struc, crit, parentPtr);
	std::string chunk;
	//get the compound section
	std::stringstream code_input(parent->code);
	std::getline(code_input, chunk, '|');
	std::string compound;
	std::vector<Compound> allComps(0);
	ParseComps(chunk, compound, allComps);
	for (int i = 0; i < 4; i++)//very not correct yet
	{
		toDo.push_back(allComps[i]);
	}

	progress = Compound();
	currGoalIdx = 0;
	AdvanceGoalCursor();
}

void Factory::Activate()
{
	Sector& s = Universe::getSectorAtLocation(xpos, ypos);
	if (s.sol.size() == 0)
	{
		return;
	}
	//there is a degree of redundancy here with the reactionmanager functions. However, nowhere else
	//will we want to force a reaction and then simply report on how costly it was.(this may change later, tis
	//easy enough to refactor)
	int chosenComp = rand() % s.sol.size();
	Compound& compEnv = s.sol[chosenComp];
	Compound& compGoal = toDo[currToDoIdx];
	for (int i = 0; i < compEnv.filledIndices.size(); i++)
	{
		int idxidx = compEnv.filledIndices[i];
		Element e = compEnv.composition[idxidx];
		if (e.red == 0)
		{
			continue;
		}
		if (e.name == compGoal.composition[currGoalIdx].name)
		{
			int stability = compEnv.GetStabilityAtPoint(idxidx) + progress.GetStabilityAtPoint(currGoalIdx);
			DoReaction(compEnv, progress, idxidx, currGoalIdx, i);
			stability = compEnv.GetStabilityAtPoint(idxidx) + progress.GetStabilityAtPoint(currGoalIdx) - stability;

			std::vector<Compound*> reactants;
			reactants.emplace_back(&progress);

			if (compEnv.mass == 0)
			{
				FastDelete(s.sol, chosenComp);
			}
			else
			{
				//manage breaking up the compound from the environment
				int numPieces = 0;
				Compound* pieces = compEnv.SplitCompound(idxidx, numPieces);
				if (pieces == nullptr)
				{
					reactants.emplace_back(&compEnv);
				}
				else
				{
					FastDelete(s.sol, chosenComp);
					for (int j = 0; j < numPieces; j++)
					{
						s.sol.emplace_back(pieces[j]);
					}
					int target = s.sol.size() - 1;
					for (int j = 0; j < numPieces; j++)
					{
						reactants.emplace_back(&s.sol[target - j]);
					}
					delete[] pieces;
				}
			}
			//drain energy if this was a disadvantageous reaction
			AdjustEnergyValues(stability, reactants);
			if (compEnv.internalEnergy < 0)
			{
				progress.internalEnergy += compEnv.internalEnergy;
				compEnv.internalEnergy = 0;
			}
			AdvanceGoalCursor();
			break;
		}
	}
}

void Factory::AdvanceGoalCursor()//could be optimized to use filledidx list
{
	while (true)
	{
		if (currGoalIdx >= toDo[currToDoIdx].composition.size())
		{
			complete.push_back(progress);
			progress = Compound();
			currGoalIdx = 0;
			currToDoIdx++;
			if (currToDoIdx == toDo.size())
			{
				currToDoIdx = 0;
				complete.clear();
				Organism* newOrg = ParseCode(parent->code);
				newOrg->code = MutateCode(newOrg->code);
				newOrg->SetPosition(xpos + rand() % 300 - 150, ypos + rand() % 300 - 150);
				Universe::newLife.emplace_back(newOrg);				
			}
		}
		else
		{
			if (progress.composition.size() == currGoalIdx)
			{				
				progress.AddElementAtIndex(Element(), currGoalIdx);//increases size
			}
			if (progress.composition[currGoalIdx].red != 0)//if this position is filled, continue
			{
				currGoalIdx++;
			}
			else if (toDo[currToDoIdx].composition[currGoalIdx].red == 0)//if the goal is empty here, continue
			{
				currGoalIdx++;
			}
			else
			{//the goal is filled and the position is not, stop advancing
				break;
			}
		}
	}
}