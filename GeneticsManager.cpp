#include "GeneticsManager.h"

std::string CreateRandomCode()
{
	char buf[5000];
	int bufspot = 0;

	//compound layer
	for (int i = 0; i < 256; i++)
	{
		for (int j = rand() % 10; j >= 0; j--)
		{
			buf[bufspot] = 'a' + (char)(rand() % Universe::ptable.size());
			bufspot++;
		}
		buf[bufspot] = ';';
		bufspot++;
	}
	buf[bufspot] = '|';
	bufspot++;
	//parts layer
	for (int i = 0; i < 256 * 7; i++)
	{
		//ensure no EOL chars and no | chars
		buf[bufspot] = 1 + rand() % 255;
		while (buf[bufspot] == 124)
		{
			buf[bufspot] = 1 + rand() % 255;
		}
		bufspot++;
	}

	buf[bufspot] = '|';
	bufspot++;
	//link layer
	for (int i = 0; i < 30; i++)
	{
		//ensure no EOL chars and no | chars
		buf[bufspot] = 1 + rand() % 255;
		while (buf[bufspot] == 124)
		{
			buf[bufspot] = 1 + rand() % 255;
		}
		bufspot++;
	}
	buf[bufspot] = '\0';
	std::string ret(buf);
	return ret;
}

Organism* ParseCode(std::string code)
{
	std::string chunk;
	//get the compound section
	std::stringstream code_input(code);
	std::getline(code_input, chunk, '|');
	std::stringstream compound_input(chunk);
	std::string compound;
	std::vector<Compound> allComps(0);
	while (std::getline(compound_input, compound, ';'))
	{
		Compound newComp;

		for (size_t i = 0; i < compound.size(); i++)
		{
			newComp.AddElementAtIndex(Universe::ptableHash[compound[i]], i);
		}
		allComps.emplace_back(newComp);
	}
	std::string sparts;
	std::getline(code_input, sparts, '|');
	unsigned char* parts;
	parts = (unsigned char*)sparts.c_str();
	std::string slinks;
	std::getline(code_input, slinks, '|');
	unsigned char* links;
	links = (unsigned char*)slinks.c_str();

	Organism* ret = new Organism();
	Organelle* start = ParsePart(allComps, parts, links, 0);

	std::vector<int> pendingLinks(0);
	pendingLinks.emplace_back(1);
	ret->AllOrganelles.emplace_back(start);
	ret->center = ret->AllOrganelles[0];
	ret->center->parent = ret;
	for (size_t idx = 0; idx < ret->AllOrganelles.size(); idx++)
	{
		//idx in links where this link is
		size_t currentlinkidx = pendingLinks[idx];
		//the link itself, which points into parts
		unsigned char currentlink = links[currentlinkidx];
		//an indicator of whether we should add more parts or not
		unsigned char operation = ((parts[currentlink] % 8) + 3) / 5;
		int target = 0;
		switch (operation)
		{
		case(0):
			//loop back
			target = parts[currentlink + 1] % 4 - parts[currentlink + 2] % 4;
			target = (idx + target) % ret->AllOrganelles.size();
			if (target != idx)
			{
				ret->AllOrganelles[idx]->ConnectTo(ret->AllOrganelles[target]);
			}
			break;
		case(1):
			//branch out
			target = (parts[currentlink + 1] % 2 + 1) * parts[currentlink + 1] % 4 + 1;
			for (int i = 0; i < target; i++)
			{
				currentlinkidx += (parts[(currentlink + 2 + 2 * i) % 256] % 8) * (parts[(currentlink + 3 + 2 * i) % 256] % 8);
				if (currentlinkidx >= slinks.size())
				{
					break;
				}
				Organelle* newOrg = ParsePart(allComps, parts, links, currentlinkidx);
				ret->AllOrganelles.emplace_back(newOrg);
				newOrg->parent = ret;
				ret->AllOrganelles[idx]->ConnectTo(ret->AllOrganelles[ret->AllOrganelles.size() - 1]);
				pendingLinks.emplace_back(currentlinkidx + 1);
			}
			break;
		case(2):
			//end, do nothing. perhaps there will be something to do here one day
			break;
		}
	}
	return ret;

}

Organelle* ParsePart(std::vector<Compound>& Compounds, unsigned char parts[], unsigned char links[], int idx)
{
	Organelle* ret = new Organelle;
	unsigned char partIdx = links[idx];
	ret->structure = Compounds[parts[partIdx]];
	ret->criticalRegion = parts[partIdx + 1] % ret->structure.composition.size();
	ret->metaData1 = parts[partIdx + 2];
	ret->metaData2 = parts[partIdx + 3];
	ret->metaData3 = parts[partIdx + 4];
	ret->metaData4 = parts[partIdx + 5];
	return ret;
}
