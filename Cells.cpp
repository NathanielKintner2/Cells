// Cells.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "Cells.h"

int main()
{
    srand(time(NULL));
    Grids::InitGrids(30, 30);
    Element e;
    e.red = 1;
    e.blue = 1;
    e.green = 1;
    e.name = 'a';
    Universe::ptable.emplace_back(e);
    e.red = 2;
    e.blue = 2;
    e.green = 2;
    e.name = 'b';
    Universe::ptable.emplace_back(e);
    e.red = 2;
    e.blue = -2;
    e.green = 2;
    e.name = 'c';
    Universe::ptable.emplace_back(e);
    e.red = 2;
    e.blue = -1;
    e.green = 3;
    e.name = 'd';
    Universe::ptable.emplace_back(e);
    e.red = 2;
    e.blue = 1;
    e.green = 3;
    e.name = 'e';
    Universe::ptable.emplace_back(e);
    for (int i = 1; i <= 10; i++)
    {
        e.red = rand() % 3 + 1;
        e.blue = rand() % 8 - 4;
        e.green = rand() % 6;
        e.name = 'e' + i;
        Universe::ptable.emplace_back(e);
    }

    for (Element e : Universe::ptable)
    {
        Universe::ptableHash[e.name] = e;
    }

    for (int i = 0; i < 1; i++)
    {
        for (int j = 0; j < 1; j++)
        {
            Organism* o;
            do
            {
                std::string newcell = CreateRandomCode();
                o = ParseCode(newcell);
                std::cout << o->AllOrganelles.size() << std::endl;
            } while (o->AllOrganelles.size() < 1 || o->AllOrganelles.size() > 24);

            Universe::allLife.emplace_back(o);


            for (Organelle* oo : o->AllOrganelles)
            {
                oo->xpos = 500*i + rand() % 800;
                oo->ypos = 500*j + rand() % 800;
            }
        }
    }

    std::vector<Compound> env;

    
    for (int i = 0; i < 100; i++)
    {
        Compound c;
        for (int j = 0; j < 4; j++)
        {
            c.AddElementAtIndex(Universe::ptable[rand() % Universe::ptable.size()], j);
        }
        c.id = i;
        env.emplace_back(c);
    }
    int stability = 0;

    for (Compound c : env)
    {
        stability += c.GetTotalStability();
    }
    std::cout << stability << std::endl;


    std::clock_t start;
    double duration;

    start = std::clock();

    /* Your algorithm here */

    for (int i = 0; i < 20000000; i++) //6.661 secs benchmark, debugging seems to take 140X longer or so (oof)
    //for (int i = 0; i < 20000; i++)
    {
        std::vector<Compound*> asdf;
        AttemptReaction(env, 10, asdf);
        
    }

    duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;

    std::cout << "duration: " << duration << '\n';
    stability = 0;

    for (Compound c : env)
    {
        stability += c.GetTotalStability();
    }
    std::cout << stability << std::endl;

    for (Sector &s : Universe::worldHexes)
    {
        for (int i = 0; i < 5; i++)
        {
            //extremely simple, probably temporary
            s.sol.emplace_back(env[rand() % env.size()]);
            continue;
            int randlocation = rand() % 25;
            if (s.solution[randlocation].mass == 0)
            {
                s.AddCompoundToLocationInSolution(env[rand() % env.size()], randlocation);
            }
        }
    }
    //Universe::worldHexes[0].AddCompoundToLocationInSolution(env[rand() % env.size()], rand() % 25);
    sf::RenderWindow window(sf::VideoMode(WINDOWX, WINDOWY), "Squishy dudes, one day");

    std::vector<int> sectorsToVisit(0);
    start = std::clock();
    std::vector<Compound*> reactants;
    while (window.isOpen())
    {
        while (std::clock() - start < 30)//keep a consistent minimum of 30 millis between steps
        {
            Sleep(1);
        }
        start = std::clock();

        for (Organism* o : Universe::allLife)
        {
            o->Reposition();
        }
        
        for (size_t i = 0; i < Universe::worldHexes.size(); i++)
        {
            sectorsToVisit.emplace_back(i);
        }
        while (sectorsToVisit.size() != 0)
        {
            size_t randpick = rand() % sectorsToVisit.size();
            int sectorWeAreVisitingNow = sectorsToVisit[randpick];
            FastDelete(sectorsToVisit, randpick);
            //for (size_t j = 0; j < Universe::worldHexes[sectorWeAreVisitingNow].filledIdxs.size(); j++);
            while (Universe::worldHexes[sectorWeAreVisitingNow].sol.size() > 25)
            {
                DoEntropy(sectorWeAreVisitingNow);
                int energy = AttemptReaction(Universe::worldHexes[sectorWeAreVisitingNow].sol, rand() % 20 - 5, reactants);
                AdjustEnergyValues(energy, reactants);
                reactants.clear();
            }
            for (size_t j = 0; j < Universe::worldHexes[sectorWeAreVisitingNow].sol.size()/2; j++);
            {
                DoEntropy(sectorWeAreVisitingNow);
                int energy = AttemptReaction(Universe::worldHexes[sectorWeAreVisitingNow].sol, rand() % 20 - 5, reactants);
                AdjustEnergyValues(energy, reactants);
                reactants.clear();
            }
        }
        DisplayAll(window);
    }

    return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
