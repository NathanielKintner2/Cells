#include "Compound.h"
#include "HexGrids.h"

bool Compound::idxIsInOuterRingOrFarther(int idx)
{    
    return composition.size() - idx <= rings * 6;
}

void Compound::AddElementAtIndex(Element e, int idx)
{	
    mass += e.red;
    e.currentGreen = 0;
    e.currentBlue = e.blue;
    filledIndices.emplace_back(idx);
    //if the element is in the outermost ring
    while (idxIsInOuterRingOrFarther(idx))
    {
        //add an additional "safety ring", makes some neighborhood problems simpler
        rings++;
        composition.resize(composition.size() + rings * 6);
    }
    composition[idx] = e;
    getPopulatedNeighbors(idx);
    int i = 0;
    while (neighborCache[i] != -1)
    {
        Element* eNeighbor = &composition[neighborCache[i]];
        composition[idx].currentGreen++;
        composition[idx].currentBlue += eNeighbor->blue;
        eNeighbor->currentGreen++;
        eNeighbor->currentBlue += e.blue;
        i++;
    }
}

void Compound::RemoveElementAtIndex(int idx)
{
    Element e = composition[idx];
    mass -= e.red;
    //filledIndices.erase(idx); FILLED INDICES HAS LAZY DELETION NOW, BEWARE
    getPopulatedNeighbors(idx);
    composition[idx].red = 0; //this will be treated as unpopulated

    int i = 0;
    while (neighborCache[i] != -1)
    {
        Element* eNeighbor = &composition[neighborCache[i]];
        eNeighbor->currentGreen--;
        eNeighbor->currentBlue -= e.blue;
        i++;
    }
}

void Compound::getPopulatedNeighbors(int idx)
{
    if (lastNeighborhoodQueried == idx)
    {
        return;
    }
    lastNeighborhoodQueried = idx;
    int filledIdx = 0;
    if (idxIsInOuterRingOrFarther(idx))
    {
        for (int x : Grids::NeighborGrid[idx])
        {
            if (composition.size() > x && composition[x].red != 0)
            {
                neighborCache[filledIdx] = x;
                filledIdx++;
            }
        }
    }
    else
    {
        for (int x : Grids::NeighborGrid[idx])
        {
            if (composition[x].red != 0)
            {
                neighborCache[filledIdx] = x;
                filledIdx++;
            }
        }
    }
    neighborCache[filledIdx] = -1;
}

int Compound::getUnPopulatedNeighborsIndices(int idx, int *ret)
{
    int filledIdx = 0;
    for (int x : Grids::NeighborGrid[idx])
    {
        if (composition.size() > x && composition[x].red == 0)
        {
            ret[filledIdx] = x;
            filledIdx++;
        }
    }
    return filledIdx;
}

int Compound::GetStabilityAtPoint(int idx)
{
    Element e = composition[idx];
    getPopulatedNeighbors(idx);
    int stability = 0;
    if (e.red != 0)
    {
        stability -= e.GetInstability();
    }
    int i = 0;
    while (neighborCache[i] != -1)
    {
        stability -= composition[neighborCache[i]].GetInstability();
        i++;
    }
    return stability;
}

int Compound::GetTotalStability()
{
    int stability = 0;
    int target = filledIndices.size();
    for (int i = 0; i < target; i++)
    {
        stability -= composition[filledIndices[i]].GetInstability();
    }
    return stability;
}

sf::Color Compound::GetColor()
{
    int r = 0;
    int g = 0;
    int b = 0;
    for (int i : filledIndices)
    {
        Element e = composition[i];
        r += e.red;
        g += e.green;
        b += e.blue;
    }
    r = (r * 10) % 255;
    g = (g * 10) % 255;
    b = (b * 10) % 255;
    
    sf::Color ret = sf::Color::Black;
    ret.r += r;
    ret.b += b;
    ret.g += g;
    return ret;
}

Compound* Compound::SplitCompound(int removeIdx, int& numPieces)
{
    getPopulatedNeighbors(removeIdx);
    if (neighborCache[1] == -1 || //if there is only 1 neighbor
        (neighborCache[2] != -1 && neighborCache[3] != -1 && neighborCache[4] != -1)) //or if there are 5 or 6 neighbors
    {
        return nullptr;
    }
    //with 2, 3 or 4 neighbors, it is possible that removing an element at this index would cause the compound to split

    //coming up: the most efficient version of multi-dijkstra i could come up with in an hour
    //this is a very sepcial array. Each index represents a group.
    int flags[4]{ -1,-1,-1,-1 };

    int target = composition.size();
    int* GroupMap = new int[target];
    for (int i = 0; i < target; i++)
    {
        GroupMap[i] = -2;
    }
    for (int i : filledIndices)
    {
        GroupMap[i] = -1;
    }
    std::list<int> borders;
    for (int i = 0; neighborCache[i] != -1; ++i)
    {
        borders.push_back(neighborCache[i]);
        flags[i] = i;
        GroupMap[neighborCache[i]] = i;
    }
    while (borders.size() != 0)
    {
        int curr = borders.front();//current location we are searching from
        borders.pop_front();
        int groupnum = flags[GroupMap[curr]];//group of current location
        for (int x : Grids::NeighborGrid[curr])//all adjacent locations
        {
            int mapval = GroupMap[x];
            switch (mapval)
            {
            case(-2)://not a filled posiiton
                break;
            case(-1):
                borders.push_back(x);
                GroupMap[x] = groupnum;
                break;
            default:
                flags[mapval] = groupnum;
                break;
            }
        }
    }
    //borders now changes meaning, it is instead a list of the pieces of the compound
    for (int i = 0; i < 4; i++)
    {
        if (flags[i] == i)
        {
            borders.push_back(i);
        }
    }
    if (borders.size() == 1)
    {
        return nullptr;
    }
    int flagDests[4]{ -1, -1, -1, -1 };
    Compound* ret = new Compound[borders.size()];
    numPieces = borders.size();
    //for each group
    for (int i = 0; i < numPieces; i++)
    {
        //the groupnumber that dominates this piece
        int group = borders.front();
        borders.pop_front();
        //denote that this group will be at place i in the return array
        flagDests[group] = i;
    }
    for (int i : filledIndices)
    {
        //in the group map at this element, you find a group number.
        //go to flags to see what groupnumber is dominating this groupnumber, if any
        //go to flagdests to see where the dominant number is being returned in ret
        //add the element there
        ret[flagDests[flags[GroupMap[i]]]].AddElementAtIndex(composition[i], i);
    }
    delete[] GroupMap;
    return ret;
}
