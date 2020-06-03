#pragma once
#include <vector>
#include "Compound.h"
#include "Utilities.h"

class Compound;
//if null, failed to react. otherwise, first int is difference in stability and subsequent ints are compounds involved in reaction
int AttemptReaction(std::vector<Compound> &initial, int threshold, std::vector<Compound*>& reactants);
void AdjustEnergyValues(int stabilityChange, std::vector<Compound*>& reactants);
void DoReaction(Compound &c1, Compound &c2, int idx1, int idx2, int idxidxtoremove);
bool PerformReactionIfGoodEnough(Compound* comp1, Compound* comp2, int threshold, int& stabilityChange, int& ielem1, int& ielem2);
int FindElementIdxByFalling(Compound &c);