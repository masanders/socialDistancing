#include <cmath>
#include <iostream>

#include "Person.h"

Person::Person(const bool willDistance, const bool isInfected, 
  const unsigned int toDaysInfect,
	const unsigned int toxlim, const unsigned int toylim,
	const unsigned int xinit, const unsigned int yinit)
{
	xmotion = ymotion = 0.0;
	xlim = toxlim; ylim = toylim;

	if(xinit >= xlim)
		xposActual = (double)xlim;
	else
		xposActual = (double)xinit;

	if (yinit >= ylim)
		yposActual = (double)ylim;
	else
		yposActual = (double)yinit;

  daysinfectedActual = toDaysInfect;
	aliveActual = true;
	distancingActual = willDistance;
	infectedActual = isInfected;
  curedActual = false;
}

bool Person::distancing() const
{
	return distancingActual;
}

bool Person::infected() const
{
	return infectedActual;
}

bool Person::alive() const
{
	return aliveActual;
}

bool Person::cured() const
{
  return curedActual;
}

unsigned int Person::daysinfected() const
{
  return daysinfectedActual;
}

void Person::infect(const unsigned int inc)
{
  if(curedActual || !aliveActual)
    return;
	infectedActual = true;
  daysinfectedActual = inc;
}

void Person::infectiontic()
{
  if(infectedActual)
    daysinfectedActual--;
}

void Person::cure()
{
  curedActual = true;
  infectedActual = false;
  daysinfectedActual = 0;
}

void Person::kill()
{
  infectedActual = false;
  aliveActual = false;
  daysinfectedActual = 0;
  this->halt();
}

void Person::shove(const double x, const double y)
{
	xmotion = x;
	ymotion = y;
}

void Person::halt()
{
	xmotion = 0.0;
	ymotion = 0.0;
}

unsigned int Person::xpos() const
{
	return (unsigned int)xposActual;
}

unsigned int Person::ypos() const
{
	return (unsigned int)yposActual;
}

void Person::step(const double dist)
{
	double xmove = dist * (xmotion / (std::abs(xmotion) + std::abs(ymotion)));
	double ymove = dist * (ymotion / (std::abs(xmotion) + std::abs(ymotion)));

	if ((xposActual + xmove) < 0.0)
	{
		xposActual = std::abs(xposActual + xmove);
		xmotion = -xmotion;
	}
	else if ((xposActual + xmove) >= xlim)
	{
		xposActual = xlim - ((xposActual + xmove) - xlim);
		xmotion = -xmotion;
		if (xposActual == xlim)
			xposActual -= xmove;
	}
	else
		xposActual += xmove;

	if ((yposActual + ymove) < 0.0)
	{
		yposActual = std::abs(yposActual + ymove); 
		ymotion = -ymotion;
	}
	else if ((yposActual + ymove) >= ylim)
	{
		yposActual = ylim - ((yposActual + ymove) - ylim);
		ymotion = -ymotion;
		if (yposActual == ylim)
			yposActual -= ymove;
	}
	else
		yposActual += ymove;
}
