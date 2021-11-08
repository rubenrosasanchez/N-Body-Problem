//
// Created by rbn on 10/10/21.
//


#include <iomanip>
#include "aos.hpp"

using namespace std;


void checkRebound(spaceObject * obj, double size_enclosure){

    // Check lower limits
    if (obj->px < 0 && obj->vx < 0){
        obj->px = 0;
        //obj->vx = -obj->vx;
        obj->vx *= -1;
    }
    if (obj->py < 0 && obj->vy < 0){
        obj->py = 0;
        //obj->vy = -obj->vy;
        obj->vy *= -1;
    }
    if (obj->pz < 0 && obj->vz < 0){
        obj->pz = 0;
        //obj->vz = -obj->vz;
        obj->vz *= -1;
    }
    // Check upper limits
    if (obj->px > size_enclosure && obj->vx > 0){
        obj->px = size_enclosure;
        //obj->vx = -obj->vx;
        obj->vx *= -1;
    }
    if (obj->py > size_enclosure && obj->vy > 0){
        obj->py = size_enclosure;
        //obj->vy = -obj->vy;
        obj->vy *= -1;
    }
    if (obj->pz > size_enclosure && obj->vz > 0){
        obj->pz = size_enclosure;
        //obj->vz = -obj->vz;
        obj->vz *= -1;
    }
}


vector <spaceObject> getInitialBodies(inputParameters params){

    vector <spaceObject> bodies;

    // Prepare the Mersenne generator engine
    mt19937_64 engine{ (unsigned int) params.seed};
    uniform_real_distribution <double> positionGenerator{0, params.size_enclosure};
    normal_distribution <double> massGenerator{ pow(10, 21), pow(10, 15)};

    // Now we use a loop to generate random numbers for each object (only mass and position)
    for(int ii = 0; ii < params.num_objects; ++ii){
        spaceObject obj;
        // Generate position values
        obj.px = positionGenerator(engine);
        obj.py = positionGenerator(engine);
        obj.pz = positionGenerator(engine);
        // Generate mass of the object
        obj.mass = massGenerator(engine);
        // Initialize velocity to 0
        obj.vx = 0;
        obj.vy = 0;
        obj.vz = 0;
        // Now put the new object into the vector
        bodies.push_back(obj);
    }

    return bodies;
}

void storeConfiguration(string filename, double enclosure_size, double step_time, std::vector<spaceObject> *objs){

    // Open file to truncate and create one if it doesn't exist
    std::fstream f;
    f.open(filename, std::fstream::out | std::fstream::app);
    f.close();
    f.open(filename, std::fstream::out | std::fstream::trunc);

    // Now write data into file
    if(f.is_open() && f.good()){

        // Print no more than 3 digits
        f << std::fixed;
        f << std::setprecision(3);

        // Put first line of file
        f << enclosure_size << " " << step_time << " " << (int) objs->size() << endl;

        for(int ii = 0; (unsigned long) ii < objs->size(); ++ii){
            f << objs->at(ii).px << " " << objs->at(ii).py << " " << objs->at(ii).pz << " " << objs->at(ii).vx << " " << objs->at(ii).vy << " " << objs->at(ii).vz << " " << objs->at(ii).mass << endl;
        }

        f.close();

    }else{
        cerr << "Error: couldn't open " << filename << endl;
    }

}

void objectCollision(spaceObject *a, spaceObject *b){
    a->mass += b->mass;
    a->vx += b->vx;
    a->vy += b->vy;
    a->vz += b->vz;
}

void executeSimulation(inputParameters params){

    // obtain a vector of celestial bodies
    vector <spaceObject> cb = getInitialBodies(params);

    storeConfiguration("init_config.txt", params.size_enclosure, params.time_step, &cb);

    // First, let's check the randomly generated set in case there are collisions in the first place
    for(int a = 0; (unsigned long) a < cb.size(); ++a){
        for(int b = a+1; (unsigned long) b < cb.size(); ++b){
            if(isCollision(cb[a].px, cb[a].py, cb[a].pz, cb[b].px, cb[b].py, cb[b].pz)){
                objectCollision(&cb[a], &cb[b]);
                cb.erase(cb.begin() + b);
            }
        }
    }

    cout << params.size_enclosure << " " << params.time_step << " " << (int) cb.size() << endl;

    // Start a vector to store accelerations and initialize it with an empty value
    spaceVector toFill;
    toFill.x = 0;
    toFill.y = 0;
    toFill.z = 0;
    vector <spaceVector> gForces;
    gForces.push_back(toFill);

    // Each iteration of this loop is an iteration of params.time_step seconds up to params.num_iterations times
    for(int index = 0; index < params.num_iterations; ++index){

        // Compute all values before checking for collisions
        for(int a = 0; (unsigned long) a < cb.size(); ++a){

            // Before computing velocity and position, we need Force applied from every body in the system
            for(int b = a+1; (unsigned long) b < cb.size(); ++b){

                if(a == 0){ // fill the vector with empty values if more are needed
                    gForces.push_back(toFill);
                }

                // Obtain gravitational force vector
                spaceVector force = gravitationalForce(cb[a].mass, cb[b].mass, cb[a].px, cb[a].py, cb[a].pz, cb[b].px, cb[b].py, cb[b].pz);
                // force on body A
                gForces[a].x += force.x;
                gForces[a].y += force.y;
                gForces[a].z += force.z;
                // force on body B - force is same magnitude, but opposite direction
                gForces[b].x -= force.x;
                gForces[b].y -= force.y;
                gForces[b].z -= force.z;

            }

            // compute velocity on body A
            cb[a].vx = computeVelocity(cb[a].vx, (gForces[a].x / cb[a].mass), params.time_step);
            cb[a].vy = computeVelocity(cb[a].vy, (gForces[a].y / cb[a].mass), params.time_step);
            cb[a].vz = computeVelocity(cb[a].vz, (gForces[a].z / cb[a].mass), params.time_step);
            // compute position on body A
            cb[a].px = computePosition(cb[a].px, cb[a].vx, params.time_step);
            cb[a].py = computePosition(cb[a].py, cb[a].vy, params.time_step);
            cb[a].pz = computePosition(cb[a].pz, cb[a].vz, params.time_step);

        }

        // Uncomment this to check values on each iteration
        /*
        cout << endl;
        cout << "ITERATION " << index << endl;
        cout << endl;
        */

        // Now that all factors have been computed, we check for collisions
        for(int a = 0; (unsigned long) a < cb.size(); ++a){
            for(int b = a+1; (unsigned long) b < cb.size(); ++b){

                if(isCollision(cb[a].px, cb[a].py, cb[a].pz, cb[b].px, cb[b].py, cb[b].pz)){
                    // Do something about the collision
                    objectCollision(&cb[a], &cb[b]);
                    cb.erase(cb.begin() + b);
                    --b;
                }else{
                    checkRebound(&cb[b], params.size_enclosure);
                }
                checkRebound(&cb[a], params.size_enclosure);
            }
        }

        // Uncomment this to check values on each iteration
        /*
        for(int ii = 0; (unsigned long) ii < cb.size(); ++ii){
            cout << "Body " << ii << ":" << endl;
            //cout << "\Gravitatinal Forces:" << endl;
            cout << "\tFx = " << gForces[ii].x << " \tFy == " << gForces[ii].y << " \tFz == " << gForces[ii].z << endl;
            //cout << "\tPositions:" << endl;
            cout << "\tPx == " << cb.at(ii).px << " \tPy == " << cb.at(ii).py << " \tPz == " << cb.at(ii).pz << endl;
            //cout << "\tVelocities:" << endl;
            cout <<  "\tVx == " << cb.at(ii).vx << " \tVy == " << cb.at(ii).vy << " \tVz == " << cb.at(ii).vz << endl;
            cout << "\tMass == " << cb.at(ii).mass << endl;
        }
        */

        // Empty the forces computed in the iteration to prevent errors in the next one
        for(int a = 0; (unsigned long) a < gForces.size(); ++a){
            gForces[a].x = 0;
            gForces[a].y = 0;
            gForces[a].z = 0;
        }




    }

    storeConfiguration("final_config.txt", params.size_enclosure, params.time_step, &cb);

}


