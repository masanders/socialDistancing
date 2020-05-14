#include "Population.h"

Population::Population(const int n, 
  const double compliancerate,
  const double contagionrate,
  const double infectedrate,
  const double mortalityrate,
  const unsigned int incubationtime,
  const unsigned int spreadrange,
	const int w, const int h)
{
	RNum wrgen(0, w - 1), hrgen(0, h - 1);
	RNum direcgen(0, 62830); // 2pi = 6.2831
	int compliance = static_cast<int>(1000.0 * compliancerate);
  int infected = static_cast<int>(1000.0 * infectedrate);

	sizeA = static_cast<unsigned int>(n);
	xlim = w;
	ylim = h;
  radius = spreadrange;
  incubation = incubationtime;
  contagion = static_cast<unsigned int>(contagionrate * 1000.0);  
  mortality = static_cast<unsigned int>(mortalityrate * 1000.0);
  cgen = std::make_unique<RNum>(1, 1000);
  auto incgen = [this]() -> unsigned int { 
    return ((0.002*this->cgen->next()) + 0.90) * this->incubation; };
  datalog.init(sizeA, xlim, ylim);

	poplist.reserve(n);
	popgrid.resize(w * h) ;
	
	for (unsigned int i = 0; i < sizeA; i++)
	{
    bool comply = (cgen->next() < compliance);
    bool infect = (cgen->next() < infected);

    if(comply)
    {
      datalog.incdistance();
      if(infect)
        datalog.incdinfect();
    }
    else
      if(infect)
        datalog.incndinfect();

		unsigned int x = wrgen.next(), y = hrgen.next(), direc = direcgen.next();
		double pushx = std::sin(direc), pushy = std::cos(direc);
		poplist.push_back(Person(
      comply,
      infect, 
      infect ? incgen() : 0, 
      xlim, ylim, x, y));

		if (!poplist.empty())
      if(!poplist.back().distancing())
			  poplist.back().shove(pushx, pushy);

		popgrid[(y * xlim) + x].push_back(i);
	}
}

unsigned int Population::size() const
{
	return sizeA;
}

void Population::step()
{
	std::vector<bool> moved(sizeA);
	
	for (size_t j = 0; j < popgrid.size(); j++)
	{
		if (popgrid[j].empty())
			continue;
		
		for (unsigned int i = 0; i < popgrid[j].size();)
		{
			unsigned int loc = popgrid[j][i];
      if (poplist[loc].distancing() || !poplist[loc].alive())
        moved[loc] = true;
			if (moved[loc])
			{
				i++;
				continue;
			}

			moved[loc] = true;
			poplist[loc].step(1.0);

			if ((poplist[loc].ypos() * xlim) + poplist[loc].xpos() == j)
				i++;
			else
			{
				popgrid[(poplist[loc].ypos() * xlim) + poplist[loc].xpos()].
					push_back(loc);
        if(i < popgrid[j].size())
				  popgrid[j].erase(popgrid[j].begin() + i);
			}
		}
	}

  this->spread();
  datalog.pushframe();
}

void Population::spread()
{
  for(unsigned int i = 0; i < popgrid.size(); i++)
  {
    if(popgrid[i].empty())
      continue;

    for(auto j : popgrid[i])
      if(poplist[j].infected())
      {
        poplist[j].infectiontic();
        this->spreadfromloc(i);
        if(poplist[j].daysinfected() < 1)
        {
          if((unsigned int)cgen->next() < mortality)
          {
            if(poplist[j].distancing())
             datalog.incdmort();
            else
             datalog.incndmort(); 
            poplist[j].kill();
          }
          else
          {
            if(poplist[j].distancing())
             datalog.incdcure();
            else
             datalog.incndcure(); 
            poplist[j].cure();
          }
        }
      }
  }
}

void Population::spreadfromloc(const int loc)
{
  int r = (int)radius;
  for(int i = loc - ((int)xlim+r); i <= loc - ((int)xlim-r); i++)
    for(int j = 0; j < r; j++)
    {
      int st = i + (j*xlim);
      if(boundcheck(st))
        if(!popgrid[st].empty())
          infectstack(st);
    }
}

void Population::infectstack(const int loc)
{
  for(auto i : popgrid[loc])
    if(!poplist[i].infected() && !poplist[i].cured() && poplist[i].alive())
      if((unsigned int)cgen->next() < contagion)
      {
        poplist[i].infect(incubation * (0.90 + (cgen->next() * 0.002)));
        if(poplist[i].distancing())
          datalog.incdinfect();
        else
          datalog.incndinfect();
      }
}

bool Population::boundcheck(const int loc)
{
  return (loc > 0 && loc < (int)popgrid.size());
}

void Population::buildFrame(cv::Mat& frame) const
{
	if (((unsigned int)frame.cols != xlim) || 
      ((unsigned int)frame.rows != ylim) || 
      (frame.channels() != 3))
	{
		std::cout << "Unable to build build frame, incorrect matrix size!\n\n";
		exit(-1);
	}

	int j = 0;
	for (auto i = popgrid.begin(); i != popgrid.end(); i++, j++)
	{
		if (i->empty())
			frame.at<cv::Vec3f>(j) = cv::Vec3f(0.25f, 0.25f, 0.25f);
		else
		{
      if (poplist[i->back()].cured())
      {
        if(poplist[i->back()].distancing())
          frame.at<cv::Vec3f>(j) = cv::Vec3f(0.00f, 0.80f, 0.00f);
        else
          frame.at<cv::Vec3f>(j) = cv::Vec3f(0.00f, 0.60f, 0.00f);
      }
      else if (!poplist[i->back()].alive())
      {
        if(poplist[i->back()].distancing())
          frame.at<cv::Vec3f>(j) = cv::Vec3f(0.50f, 0.50f, 0.50f);
        else
          frame.at<cv::Vec3f>(j) = cv::Vec3f(0.10f, 0.10f, 0.10f);
      }
      else if (poplist[i->back()].infected())
      {
        float fade = ((float)poplist[i->back()].daysinfected()/(float)(incubation)) * 0.20;
        frame.at<cv::Vec3f>(j) = cv::Vec3f(0.00f, 0.00f, 0.80f+fade);
      }
      else if (poplist[i->back()].distancing())
				frame.at<cv::Vec3f>(j) = cv::Vec3f(0.80f, 0.30f, 0.25f);
			else
				frame.at<cv::Vec3f>(j) = cv::Vec3f(0.25f, 0.60f, 0.82f);
		}

	}
}

void Population::getGraphFrame(cv::Mat& frame, const unsigned int icucap) const
{
  datalog.buildframe(frame, icucap);
}
