#pragma once

#include <vector>
#include <iostream>
#include <cmath>
#include <memory>

#include <opencv2/opencv.hpp>

#include "Person.h"
#include "Popdata.h"
#include "RNum.h"
#include "main.h"

class Population
{
public:
	Population(const int n, 
    const double compliancerate,
    const double contagionrate,
    const double infectedrate,
    const double mortalityrate,
    const unsigned int incubationtime,
    const unsigned int spreadrange,
		const int w, const int h);
  Population(Bardata& bd, const int w, const int h) :
    Population(
        bd.n,
        (double)(bd.compliance)/100.0, 
        (double)(bd.contagion)/100.0, 
        (double)(bd.initial)/100.0,
        (double)(bd.mortality)/100.0,
        bd.incubation*40,
        bd.range,
        w, h) {}

	unsigned int size() const;
	void step();

	void buildFrame(cv::Mat& frame) const;
  void getGraphFrame(cv::Mat& frame, const unsigned int icucap) const;

private:
	unsigned int sizeA, xlim, ylim, contagion, mortality, incubation, radius;
	std::vector<Person> poplist;
	std::vector<std::vector<unsigned int>> popgrid;
  std::unique_ptr<RNum> cgen;
  Popdata datalog;

  void spread();
  void spreadfromloc(const int loc);
  void infectstack(const int loc);
  bool boundcheck(const int loc);
};

