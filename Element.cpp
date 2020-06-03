#include "Element.h"
#include <cstdlib>

int Element::GetInstability()
{
    int bonddiff = green - currentGreen;
    bonddiff = 10 * bonddiff * bonddiff - 5 * bonddiff; // if bonddiff is positive, i.e. less bonds than desired, cost is
    //slightly softened. diff is negative and there are too many bonds, cost is extra high
    int chargeval = abs(currentBlue) - 2;
    chargeval = chargeval + abs(chargeval); //0, 1 and 2 will all =0, 3=2, 4=4, 5=6, 6=8, etc
    return (bonddiff + chargeval) * red; //bigger element=more impact on stability
}