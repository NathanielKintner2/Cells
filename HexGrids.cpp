#include "HexGrids.h"

using namespace Grids;

int Grids::NeighborGrid[GRIDSIZE][6];
int Grids::DistanceGrid[GRIDSIZE];
int Grids::XYPositionGrid[GRIDSIZE][2];
int Grids::SectorXYPositionGrid[GRIDSIZE][2];
int Grids::WorldGrid[GRIDSIZE][6];
int Grids::SectorGrid[GRIDSIZE][6];
int Grids::SectorNeighborGrid[GRIDSIZE];
int Grids::SectorEdgePositions;
int Grids::WorldEdgePositions;
int Grids::TotalSectorPositions;


void Printaboi(int pos, int dir)
{
    std::cout << "----------" << std::endl;
    std::cout << SectorGrid[pos][dir] << std::endl;
}
void Printbois(int pos)
{
    std::cout << "----------" << std::endl;
    for (int i = 0; i < 6; i++)
    {
        std::cout << SectorGrid[pos][i] << std::endl;
    }
}

void InitNeighborGrid()
{
    for (int i = 0; i < GRIDSIZE; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            NeighborGrid[i][j] = -1;
        }
    }

    NeighborGrid[0][0] = 1;
    NeighborGrid[1][3] = 0;
    for (int i = 1; i < GRIDSIZE - 1; i++)
    {
        int *curr = NeighborGrid[i];
        int j = 0;
        bool foundFilledValue = false;
        while (true)
        {
            if (foundFilledValue && curr[j] == -1)
            {
                break;
            }
            if (curr[j] != -1)
            {
                foundFilledValue = true;
            }
            j = (j + 1) % 6;
        }
        int k = (j + 5) % 6;
        //j is now an open spot, right next to filled spot k

        //in the direction j, place i+1
        curr[j] = i + 1;

        //at i+1, in the opposite direction to j, place i
        NeighborGrid[i + 1][(j + 3) % 6] = i;

        //at k, say that i+1 in the direction clockwise to j
        NeighborGrid[curr[k]][(j + 1) % 6] = i + 1;
        NeighborGrid[i + 1][(j + 4) % 6] = curr[k];

        if (NeighborGrid[curr[k]][j] != -1)
        {
            NeighborGrid[NeighborGrid[curr[k]][j]][(k + 3) % 6] = i + 1;
            NeighborGrid[i + 1][k] = NeighborGrid[curr[k]][j];
        }
    }	
}

void InitDistanceGridAndXYGrid()
{
    int side = 0;
    int elemsdrawninside = 0;
    int sidelen = 1;
    int curx = -2;
    int cury = -4;
    XYPositionGrid[0][0] = 0;
    XYPositionGrid[0][1] = 0;
    DistanceGrid[0] = 0;
    for (int i = 1; i < GRIDSIZE - 1; i++)
    {
        if (elemsdrawninside == sidelen)
        {
            elemsdrawninside = 0;
            side++;
            if (side == 6)
            {
                side = 0;
                sidelen = sidelen + 1;
                curx -= 2;
                cury -= 4;
            }
        }
        switch (side)
        {
        case 0:
            curx += 4;
            break;
        case 1:
            curx += 2;
            cury += 4;
            break;
        case 2:
            curx -= 2;
            cury += 4;
            break;
        case 3:
            curx -= 4;
            break;
        case 4:
            curx -= 2;
            cury -= 4;
            break;
        case 5:
            curx += 2;
            cury -= 4;
            break;
        }
        XYPositionGrid[i][0] = curx;
        XYPositionGrid[i][1] = cury;
        DistanceGrid[i] = sidelen;
        elemsdrawninside++;
    }
}

void InitSectorXYGrid()
{
    int side = 0;
    int elemsdrawninside = 0;
    int sidelen = 1;
    int cury = -2;
    int curx = -4;
    SectorXYPositionGrid[0][0] = 0;
    SectorXYPositionGrid[0][1] = 0;
    for (int i = 1; i < GRIDSIZE - 1; i++)
    {
        if (elemsdrawninside == sidelen)
        {
            elemsdrawninside = 0;
            side++;
            if (side == 6)
            {
                side = 0;
                sidelen = sidelen + 1;
                cury -= 2;
                curx -= 4;
            }
        }
        switch (side)
        {
        case 0:
            cury += 4;
            break;
        case 1:
            cury += 2;
            curx += 4;
            break;
        case 2:
            cury -= 2;
            curx += 4;
            break;
        case 3:
            cury -= 4;
            break;
        case 4:
            cury -= 2;
            curx -= 4;
            break;
        case 5:
            cury += 2;
            curx -= 4;
            break;
        }
        SectorXYPositionGrid[i][0] = curx;
        SectorXYPositionGrid[i][1] = cury;
        elemsdrawninside++;
    }
}

void ReciprocalPlace(int i, int j, int dir, int arrToFill[][6])
{
    arrToFill[i][(dir + 6) % 6] = j;
    arrToFill[j][(dir + 9) % 6] = i;
}

int InitWorldGrid(int worldsize, int arrToFill[][6])
{
    int remainingspace = worldsize-1;
    int usedspace = 1;
    int rings = 1;
    while (remainingspace > rings * 6)
    {
        usedspace += rings * 6;
        remainingspace -= rings * 6;
        rings++;
    }
    for (int i = 0; i < usedspace; i++)
    {
        for (int j = 0; j < 6; j++)
        {
            arrToFill[i][j] = NeighborGrid[i][j];
        }
    }
    int direction = 1;
    //this puts this cursor on the other end of the macrohex
    int reversei = usedspace - 1 - (rings-1) * 2;
    for (int i = usedspace; i < usedspace + rings * 3 - 1; i++)
    {
        //a few extra cycles, but its a lot easier to just let it happen
        for (int j = 0; j < 6; j++)
        {
            arrToFill[i][j] = NeighborGrid[i][j];
        }
        ReciprocalPlace(i, reversei, direction, arrToFill);
        //WorldGrid[i][(direction + 6) % 6] = reversei;
        //WorldGrid[i][(direction - 1 + 6) % 6] = reversei - 1;
        reversei--;
        if ((i - usedspace + 1) % rings == 0)
        {
            if (direction == 1)
            {
                //first corner
                ReciprocalPlace(i, i + rings * 2 - 1, direction - 1, arrToFill);
                //WorldGrid[i][(direction - 1 + 6) % 6] = i + rings * 2 - 1;
            }
            else
            {
                //second corner (there wont be a third one)
                ReciprocalPlace(i, i - rings * 2 + 1, direction - 1, arrToFill);
                //WorldGrid[i][(direction - 1 + 6) % 6] = i - rings * 2 + 1;
            }
            direction--;
            reversei += (rings) * 2 - 1;
            ReciprocalPlace(i, reversei, direction - 1, arrToFill);
            //WorldGrid[i][(direction - 1 + 6) % 6] = reversei;
        }
        else
        {
            ReciprocalPlace(i, reversei, direction - 1, arrToFill);
        }
        if (i + 1 == usedspace + rings * 3 - 1)
        {
            //we are at the end, fix an edge case
            arrToFill[i][3] = usedspace + rings - 1;
        }
    }
    return usedspace + rings * 3 - 1;
}

void initSectorNeighborGrid()
{
    int neighborNumber = -1;
    int currentPos = 0;
    int sectorPos = 0;
    while (neighborNumber != 6)
    {
        SectorNeighborGrid[sectorPos] = neighborNumber;
        int currentdir = 0;
        while (NeighborGrid[currentPos][currentdir] != currentPos + 1)
        {
            currentdir++;
        }
        currentPos = NeighborGrid[currentPos][currentdir];
        if (SectorGrid[sectorPos][currentdir] != NeighborGrid[sectorPos][currentdir])
        {
            neighborNumber++;
        }
        sectorPos = SectorGrid[sectorPos][currentdir];
    }
}

//will NOT return less than two places
std::vector<int> Grids::spiralFromPoint(int center, int places, int grid[][6])
{
    std::vector<int> ret(0);
    int direction = 4;
    int ring = 1;
    int placesRecorded = 0;
    ret.emplace_back(center);
    bool goInZeroDirection = false;
    //go straight down, then start spiraling out from there
    int currentPos = grid[center][0];
    ret.emplace_back(currentPos);
    for (int i = 0; i < places-2; i++)
    {
        if (goInZeroDirection)
        {
            goInZeroDirection = false;
            currentPos = grid[currentPos][0];
        }
        else
        {
            currentPos = grid[currentPos][direction];
        }        
        placesRecorded++;
        ret.emplace_back(currentPos);
        if (placesRecorded == ring)
        {
            placesRecorded = 0;
            if (direction == 0)//this is the direction we extend in
            {
                goInZeroDirection = true;
                ring++;
            }
            direction = (direction + 5) % 6;
        }
    }
    return ret;
}

void Grids::InitGrids(int sectorSize, int worldSectors)
{
	InitNeighborGrid();    
    InitDistanceGridAndXYGrid();
    InitSectorXYGrid();
    int numsectors = InitWorldGrid(worldSectors, WorldGrid);
    int sectorsize = InitWorldGrid(sectorSize, SectorGrid);
    initSectorNeighborGrid();
    Universe::init(numsectors, sectorsize);
    Grids::TotalSectorPositions = sectorsize;
    int rings = 1;
    while (sectorSize > 0)
    {
        sectorSize -= rings * 6;
        rings++;
    }
    Grids::SectorEdgePositions = rings;
    rings = 1;
    while (numsectors > 0)
    {
        numsectors -= rings * 6;
        rings++;
    }
    Grids::WorldEdgePositions = rings;
}





