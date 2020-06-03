#pragma once

#include <string>
#include <sstream>
#include "Organism.h"
#include <vector>
#include "Compound.h"
#include "Universe.h"
#include "Organelle.h"
#include "Factory.h"

Organelle *ParsePart(std::vector<Compound>& Compounds, unsigned char parts[], unsigned char links[], int idx);

std::string CreateRandomCode();
Organism* ParseCode(std::string code);