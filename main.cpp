#include <iostream>
#include <string>
#include <chrono>
#include <limits>
#include <memory>

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "Person.h"
#include "Population.h"
#include "main.h"

int main()
{
	cv::Mat data = cv::Mat::zeros(GRIDH, GRIDW, CV_32FC3);
	cv::Mat graphdata = cv::Mat::zeros(GRIDH, GRIDW, CV_32FC3);
	std::string mainwname = "Social Distancing";
  std::string graphwname = "Social Distancing - Graph";
	double tottime = 0.0;
  const size_t cycles = 20000;
  Inputdata id;
  Bardata bd;
  std::unique_ptr<Population> p = 
    std::make_unique<Population>(bd, GRIDW, GRIDH);

	p->buildFrame(data);
  buildwindows(mainwname, graphwname, bd);

	for (unsigned int i = 0; proc_input(id) && i < cycles; i++)
	{
    if(id.rebuild)
    {
      id.rebuild = false;
      p.reset(new Population(bd, GRIDW, GRIDH));
    }

		auto start = std::chrono::high_resolution_clock::now();
		p->step();
		std::chrono::duration<double> steptime =
			std::chrono::high_resolution_clock::now() - start;

		tottime += steptime.count();
    //if(!(i%100))
		  //std::cout << "Step (" << i+1 << '/' << cycles << "): " << steptime.count() 
			  //<< "s - avg: " << tottime/((double)(i+1)) << "s\n";

		p->buildFrame(data);
    p->getGraphFrame(graphdata, bd.icucap);
		cv::imshow(mainwname, data);
    cv::imshow(graphwname, graphdata);
	}

	cv::waitKey(0);

	return 0;
}

bool proc_input(Inputdata &id)
{
  int key = cv::waitKey(id.delay);

  if(key == ' ')
    while(cv::waitKey(1) != ' ');
  else if(key == 'w')
  {
    id.delay = id.delay >= 100 ? 100 : id.delay+1;
    std::cout << "Delay: " << id.delay << '\n';
  }
  else if(key == 's')
  {
    id.delay = id.delay <= 1 ? 1 : id.delay-1;
    std::cout << "Delay: " << id.delay << '\n';
  }
  else if(key == 'r')
    id.rebuild = true;
  else if(key == 'q')
    return false;

  return true;
}

void buildwindows(std::string& mainwname, std::string& graphwname, Bardata& bd)
{
	cv::namedWindow(mainwname, cv::WINDOW_NORMAL);
  cv::namedWindow(graphwname, cv::WINDOW_NORMAL);
	cv::resizeWindow(mainwname, 1200, 900);
	cv::resizeWindow(graphwname, 1200, 900);

  cv::createTrackbar("ICU Capacity", graphwname, &bd.icucap, 100);
  cv::createTrackbar("Pop Size", graphwname, &bd.n, 1000);
  cv::createTrackbar("Distancing Compliance", graphwname, &bd.compliance, 100);
  cv::createTrackbar("Contagion Rate", graphwname, &bd.contagion, 100);
  cv::createTrackbar("Contagion Range", graphwname, &bd.range, 15);
  cv::createTrackbar("Initial Infected", graphwname, &bd.initial, 100);
  cv::createTrackbar("Mortality Rate", graphwname, &bd.mortality, 100);
  cv::createTrackbar("Incubation Time", graphwname, &bd.incubation, 50);
}
