//
// Created by rbn on 10/10/21.
//


#pragma once

#include "commons.hpp"
#include "math.h"
#include <fstream>
#include <iostream>
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

void storeConfiguration(std::string filename, double enclosure_size, double step_time, std::vector<spaceObject> *objs);

void checkRebound(spaceObject * obj, double size_enclosure);

// Destroy object b and add its mass to object a. Compute final acceleration and velocity.
void objectCollision(spaceObject *a, spaceObject *b);

void executeSimulation(inputParameters params);