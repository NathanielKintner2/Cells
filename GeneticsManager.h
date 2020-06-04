#pragma once

#include <string>
#include <sstream>
#include "Organism.h"
#include <vector>
#include "Compound.h"
#include "Universe.h"
#include "Organelle.h"
#include "Factory.h"
#include "ReactionManager.h"

Organelle* ParsePart(std::vector<Compound>& Compounds, unsigned char parts[], unsigned char links[], int idx, Organism* parentPtr);
void ParseComps(std::string compound_chunk, std::string compound, std::vector<Compound>& allComps);

std::string CreateRandomCode();
Organism* ParseCode(std::string code);
std::string MutateCode(std::string code);