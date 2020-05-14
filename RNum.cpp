#include "RNum.h"

RNum::RNum(const int low, const int high)
{
	rd = std::make_shared<std::random_device>();
	gen = std::make_shared<std::mt19937>((*rd)());
	dis = std::make_shared<std::uniform_int_distribution<>>(low, high);
}

int RNum::next() const
{
	return (*dis)(*gen);
}


