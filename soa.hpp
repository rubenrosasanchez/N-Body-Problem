//
// Created by rbn on 14/10/21.
//

#pragma once

#include "commons.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <omp.h>


#define SOAOBJLEN(o) (o.mass.size())
//#define MARGIN 0.001

struct soaObject{
    // Mass
    std::vector <double> mass;
    // Position
    std::vector <double> px;
    std::vector <double> py;
    std::vector <double> pz;
    // Velocity
    std::vector <double> vx;
    std::vector <double> vy;
    std::vector <double> vz;
};

void executeSimulation(inputParameters params);