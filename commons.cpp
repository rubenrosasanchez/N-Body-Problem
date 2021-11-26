//
// Created by rbn on 15/10/21.
//
#include "commons.hpp"


double norm(double x, double y, double z){
    return std::sqrt(std::pow(x, 2) + std::pow(y, 2) + std::pow(z, 2));
}

spaceVector gravitationalForce(double m1, double m2, double x1, double y1, double z1, double x2, double y2, double z2){

    spaceVector force;

    // Get vector components
    double x_comp = x2-x1;
    double y_comp = y2-y1;
    double z_comp = z2-z1;

    // Compute the denominator
    double norma = norm(x_comp, y_comp, z_comp);
    double cube = norma*norma*norma;

    // Obtain each component of the Gravitational Force
    force.x = ((G_const*m1*m2)*(x_comp)) / (cube);
    force.y = ((G_const*m1*m2)*(y_comp)) / (cube);
    force.z = ((G_const*m1*m2)*(z_comp)) / (cube);

    return force;
}

bool isCollision(double x1, double y1, double z1, double x2, double y2, double z2){

    if(x1 == x2 && y1 == y2 && z1 == z2){
        return true;
    }
    return false;
}


std::vector <spaceVector> fillVector(spaceVector toFill, int size){

    std::vector <spaceVector> v;

    for(int a = 0; a < size; ++a) {

        // fill the vector with empty values if more are needed
        //v.push_back(toFill);
        v.emplace_back(toFill);

    }

    return v;

}

void eraseForces(std::vector <spaceVector> &v, spaceVector toFill){
    int size = v.size();
    v.erase(v.begin(), v.end());
    v.assign(size, toFill);

}