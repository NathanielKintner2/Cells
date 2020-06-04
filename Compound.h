#pragma once
#include <vector>
#include <list>
#include <SFML/Graphics.hpp>
#include "Element.h"
#include "Utilities.h"

class Compound
{
public:
    int id;
    Compound() {
        composition = std::vector<Element>(7);
        filledIndices = std::vector<int>(0);
        rings = 1;
        neighborCache[6] = -1;
        Vvert = 0;
        Vhoriz = 0;
        mass = 0;
        internalEnergy = 0;
        lastNeighborhoodQueried = -1;
    };
    std::vector<Element> composition;
    std::vector<int> filledIndices;
    int mass;
    int rings;  
    int neighborCache[7];
    int lastNeighborhoodQueried;

    int Vvert;
    int Vhoriz;

    int internalEnergy;


    void AddElementAtIndex(Element e, int idx);
    void RemoveElementAtIndex(int idx);    
    //ret should be an array of length 6 at least, returns number of array indeces filled
    int getUnPopulatedNeighborsIndices(int idx, int* ret);
    int GetStabilityAtPoint(int idx);
    int GetTotalStability();

    sf::Color GetColor();

    Compound* SplitCompound(int removeIdx, int& numPieces);
    std::string ChemicalString();

private:
    void getPopulatedNeighbors(int idx);
    bool idxIsInOuterRingOrFarther(int idx);
};

