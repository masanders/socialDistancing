#ifndef RNUM_H
#define RNUM_H

/*===============================================
 *	RNum.h
 *		Declare the RNum class
 *
 *		Generates random numbers from [low, high]
 */


#include <random>
#include <memory>

class RNum
{
public:
	RNum(const int low = 0, const int high = 255);

	int next() const;
private:
	std::shared_ptr<std::random_device> rd;
	std::shared_ptr<std::mt19937> gen;
	std::shared_ptr<std::uniform_int_distribution<>> dis;
};


#endif /* RNUM_H */