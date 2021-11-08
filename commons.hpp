//
// Created by rbn on 15/10/21.
//

#pragma once

#include "math.h"
#include <iostream>
#include <random>

#define computeVelocity(vi,acc,t) vi+(acc*t)
#define computePosition(pi,v,t) pi+(v*t)

struct inputParameters{
    int num_objects;
    int num_iterations;
    int seed;
    double size_enclosure;
    double time_step;
};

struct spaceVector{
    double x;
    double y;
    double z;
};


const double G_const = 6.674*(std::pow(10, -11));

double power(double base, int exponent);

double norm(double x, double y, double z);

spaceVector gravitationalForce(double m1, double m2, double x1, double y1, double z1, double x2, double y2, double z2);

bool isCollision(double x1, double y1, double z1, double x2, double y2, double z2);