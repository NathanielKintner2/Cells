#pragma once
class Element
{
public:
	short red; //mass, between 1 and 5, and how badly this wants to "have its way". instability *= red for this element
	//captures things like flourine being oxygen but MORE, or the difference between sodium/potassium/cesium

	short green; //between 0 and 6, tries to have this many neighbors, instability += (green-neighbors) * (green-neighbors)
	//essentially, number of bonds

	short blue; // between -4 and 4, adds own charge to neighbors, instability += |blue|-2 + ||blue|-2| (makes 0=1=2!=3!=4 etc)
	//sort of "charge". doesn't really work the same way as normal chemistry, but captures the "happier with the opposite" effect

	short currentGreen; //# of neighboring elements in the compound, wants to be = Green
	short currentBlue; //blue + blue of all neighboring elements in the compound, wants to be -2, -1, 0, 1, or 2

	char name; //just an indentifier

	int GetInstability();
};

