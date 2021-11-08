//
// Created by rbn on 14/10/21.
//

#pragma once

#include "commons.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>

#define MARGIN 0.001

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

void storeConfiguration(std::string filename, double enclosure_size, double step_time, soaObject& obj);

void checkRebound(soaObject * obj, double size_enclosure, int index);

// Destroy object b and add its mass to object a. Compute final acceleration and velocity.
void objectCollision(soaObject * obj, int index1, int index2);

void executeSimulation(inputParameters params);