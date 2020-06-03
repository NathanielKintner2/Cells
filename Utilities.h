#pragma once
#include <vector>
#include "Compound.h"
#include "Element.h"

template <class T>
void FastDelete(std::vector<T> &vec, int idx);

template <class T>
void FastDelete(std::vector<T> &vec, int idx)
{
	vec[idx] = vec.back();
	vec.pop_back();
}