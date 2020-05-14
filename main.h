#pragma once

#include <string>

constexpr auto GRIDW      = 400;
constexpr auto GRIDH      = 300;
constexpr auto POPSIZE    = 200;
constexpr auto DISTANCING_COMPLIANCE_RATE = 45;
constexpr auto CONTAGION_RATE             = 10;
constexpr auto INITIAL_INFECTED_RATE      = 15;
constexpr auto MORTALITY_RATE             = 5;
constexpr auto INCUBATION_TIME            = 14;
constexpr auto SPREAD_RANGE               = 4;
constexpr auto ICU_CAPACITY                = 20;

struct Inputdata {
  Inputdata() { delay = 10; rebuild = true; }
  int delay;
  bool rebuild;
};

struct Bardata {
  Bardata() {
    icucap = ICU_CAPACITY;
    n = POPSIZE;
    compliance = DISTANCING_COMPLIANCE_RATE;
    contagion = CONTAGION_RATE;
    initial = INITIAL_INFECTED_RATE;
    mortality = MORTALITY_RATE;
    incubation = INCUBATION_TIME;
    range = SPREAD_RANGE;
  }
  int n, compliance, contagion, initial, mortality,
      incubation, range, icucap;
};

bool proc_input(Inputdata &id);
void buildwindows(std::string& mainwname, std::string& graphwname, Bardata& bd);
