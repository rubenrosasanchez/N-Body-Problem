//
// Created by rbn on 10/10/21.
//


#pragma once

#include "commons.hpp"
#include "math.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <omp.h>

//#include <string>


struct spaceObject{
    double mass;
    // Position
    double px;
    double py;
    double pz;
    // Velocity
    double vx;
    double vy;
    double vz;
};

void executeSimulation(inputParameters params);