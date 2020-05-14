#include "Popdata.h"

Popdata::Popdata()
{
  total = distancing = xlim = ylim = dinfect = dcure
    = dmort = ndinfect = ndcure = ndmort = 0;
}

void Popdata::init(const unsigned int popsize,
    const unsigned int toxlim,
    const unsigned int toylim)
{
  total = popsize;
  xlim = toxlim;
  ylim = toylim;
}

void Popdata::incdistance()
{
  distancing++;
}

void Popdata::incdinfect()
{
  dinfect++;
}

void Popdata::incdcure()
{
  dcure++;
}

void Popdata::incdmort()
{
  dmort++;
}

void Popdata::incndinfect()
{
  ndinfect++;
}

void Popdata::incndcure()
{
  ndcure++;
}

void Popdata::incndmort()
{
  ndmort++;
}


void Popdata::pushframe()
{
  //std::cout << "Pushing: " << dinfect <<  "-dinfect " << dcure << "-dcure " << dmort
    //<< "-dmort " << ndinfect << "-ndinfect "<<  ndcure << "-ndcure " << ndmort 
    //<< "-ndmort\n";
  logs.push_back(Dataframe(dinfect, dcure, dmort,
        ndinfect, ndcure, ndmort));
}

void Popdata::buildframe(cv::Mat& frame, const unsigned int icucap) const
{
	if (((unsigned int)frame.cols != xlim) || 
      ((unsigned int)frame.rows != ylim) || 
      (frame.channels() != 3))
  {
    std::cout << "Cannot build graph data in incorrectly sized frame.\n";
    exit(-1);
  }

  for(unsigned int i = 0; i < xlim; i++)
  {
    unsigned int loc = ftog(i);
    unsigned int infected = (logs[loc].dinfect + logs[loc].ndinfect) -
      (logs[loc].dcure + logs[loc].ndcure + logs[loc].dmort + logs[loc].ndmort);
    unsigned int cured = (logs[loc].dcure + logs[loc].ndcure);
    unsigned int deliminfec = ylim * ((double)(infected)/(double)(total));
    unsigned int delimcure = (ylim * ((double)(cured)/(double)(total))) + deliminfec;
    
    for(unsigned int j = 1; j < ylim; j++)
    {
      if(j > delimcure)
        frame.at<cv::Vec3f>(ylim - j, i) = cv::Vec3f(0.20f, 0.20f, 0.20f);
      else if(j > deliminfec)
        frame.at<cv::Vec3f>(ylim - j, i) = cv::Vec3f(0.00f, 0.80f, 0.20f);
      else
        frame.at<cv::Vec3f>(ylim - j, i) = cv::Vec3f(0.00f, 0.00f, 0.80f);
    }
  }    

  cv::line(frame, cv::Point(0, ylim - (ylim*((double)icucap/100.0))),
      cv::Point(xlim, ylim - (ylim*((double)icucap/100.0))), CV_RGB(80, 80, 80));
}

unsigned int Popdata::ftog(const unsigned int loc) const
{
  return (unsigned int)( (double)loc * ((double)(logs.size()) / (double)(xlim)) );
}
