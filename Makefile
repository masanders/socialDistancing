# *****************************************************
# Variables to control Makefile operation

CXX = g++
CXXFLAGS = -std=c++14 -Wall
CVFLAG = `pkg-config --libs --cflags opencv`

# ****************************************************
# Targets needed to bring the executable up to date

socialDistancing: main.cpp Person.cpp Person.h Population.cpp \
		Population.h RNum.cpp RNum.h Popdata.cpp Popdata.h
	$(CXX) $(CXXFLAGS) main.cpp Person.cpp Population.cpp \
		Popdata.cpp RNum.cpp \
		$(CVFLAG) -o socialDistancing
