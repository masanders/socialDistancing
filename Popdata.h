#pragma once

#include <iostream>
#include <vector>

#include <opencv2/opencv.hpp>

struct Dataframe {
  Dataframe() {}
  Dataframe(const unsigned int todi,
      const unsigned int todc,
      const unsigned int todm,
      const unsigned int tondi,
      const unsigned int tondc,
      const unsigned int tondm) :
    dinfect(todi), dcure(todc), dmort(todm),
    ndinfect(tondi), ndcure(tondc), ndmort(tondm)
  {}
  unsigned int dinfect, dcure, dmort,
    ndinfect, ndcure, ndmort;  
};

class Popdata {
  public:
    Popdata();
    Popdata(const int popsize,
        const int toxlim,
        const int toylim) : 
      total(popsize), xlim(toxlim), ylim(toylim)
      {}

    void init(const unsigned int popsize,
        const unsigned int toxlim,
        const unsigned int toylim);

    void incdistance();
    void incdinfect();
    void incdcure();
    void incdmort();
    void incndinfect();
    void incndcure();
    void incndmort();

    void pushframe();
    void buildframe(cv::Mat& frame, const unsigned int icucap) const;
  private:
    unsigned int total, distancing, xlim, ylim;
    unsigned int dinfect, dcure, dmort,
      ndinfect, ndcure, ndmort;  
    std::vector<Dataframe> logs;

    unsigned int ftog(const unsigned int loc) const;
};
