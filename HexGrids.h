#pragma once
#include <cstdio>
#include <iostream>
#include "Universe.h"

const int GRIDSIZE = 5000;

namespace Grids
{
	extern int NeighborGrid[GRIDSIZE][6];
	extern int DistanceGrid[GRIDSIZE];
	extern int XYPositionGrid[GRIDSIZE][2];
	extern int SectorXYPositionGrid[GRIDSIZE][2];
	extern int TotalSectorPositions;
	extern int SectorEdgePositions;
	extern int WorldEdgePositions;
	extern int WorldGrid[GRIDSIZE][6];
	extern int SectorGrid[GRIDSIZE][6];
	extern int SectorNeighborGrid[GRIDSIZE];
	std::vector<int> spiralFromPoint(int center, int places, int grid[][6]);
	void InitGrids(int sectorSize, int worldSectors);
}