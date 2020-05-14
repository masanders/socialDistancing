#pragma once

#include <random>
#include <memory>

class Person
{
public:
	Person(const bool willDistance, const bool isInfected,
    const unsigned int toDaysInfect,
		const unsigned int toxlim, const unsigned int toylim,
		const unsigned int xinit, const unsigned int yinit);

	bool distancing() const;
	bool infected() const;
	bool alive() const;
  bool cured() const;
  unsigned int daysinfected() const;

	void infect(const unsigned int inc);
  void infectiontic();
  void cure();
	void kill();
	void shove(const double x, const double y);
	void halt();

	unsigned int xpos() const;
	unsigned int ypos() const;

	void step(const double dist);

private:
	double xposActual, yposActual;
	double xlim, ylim;
	double xmotion, ymotion;
  unsigned int daysinfectedActual;
	bool distancingActual;
	bool infectedActual;
	bool aliveActual;
  bool curedActual;
};

