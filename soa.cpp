#include "soa.hpp"


using namespace std;


void checkRebound(soaObject * obj, double size_enclosure, int index){

    // Check lower limits
    if (obj->px[index] < 0 && obj->vx[index] < 0){
        obj->px[index] = 0;
        obj->vx[index] *= -1;
    }
    if (obj->py[index] < 0 && obj->vy[index] < 0){
        obj->py[index] = 0;
        obj->vy[index] *= -1;
    }
    if (obj->pz[index] < 0 && obj->vz[index] < 0){
        obj->pz[index] = 0;
        obj->vz[index] *= -1;
    }
    // Check upper limits
    if (obj->px[index] > size_enclosure && obj->vx[index] > 0){
        obj->px[index] = size_enclosure;
        obj->vx[index] *= -1;
    }
    if (obj->py[index] > size_enclosure && obj->vy[index] > 0){
        obj->py[index] = size_enclosure;
        obj->vy[index] *= -1;
    }
    if (obj->pz[index] > size_enclosure && obj->vz[index] > 0){
        obj->pz[index] = size_enclosure;
        obj->vz[index] *= -1;
    }
}


soaObject getInitialBodies(inputParameters params){
    soaObject obj;

    // Prepare the Mersenne generator engine
    mt19937_64 engine{ (unsigned int) params.seed};
    uniform_real_distribution <double> positionGenerator{0, params.size_enclosure};
    normal_distribution <double> massGenerator{ pow(10, 21), pow(10, 15)};

    // Now we use a loop to generate random numbers for each object (only mass and position)
    for(int ii = 0; ii < params.num_objects; ++ii){
        // Generate position values
        obj.px.emplace_back(positionGenerator(engine));
        obj.py.emplace_back(positionGenerator(engine));
        obj.pz.emplace_back(positionGenerator(engine));
        // Generate mass of the object
        obj.mass.emplace_back(massGenerator(engine));
    }
    // Initialize velocity to 0
    obj.vx = std::vector<double>(params.num_objects,0);
    obj.vy = std::vector<double>(params.num_objects,0);
    obj.vz = std::vector<double>(params.num_objects,0);

    return obj;
}



void storeConfiguration(std::string filename, double enclosure_size, double step_time, soaObject *obj){
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
        f << enclosure_size << " " << step_time << " " << (int) obj->mass.size() << endl;

        for(int ii = 0; (unsigned long) ii < obj->mass.size(); ++ii){
            f << obj->px[ii] << " " << obj->py[ii] << " " << obj->pz[ii] << " " << obj->vx[ii] << " " << obj->vy[ii] << " " << obj->vz[ii] << " " << obj->mass[ii] << endl;
        }

        f.close();

    }else{
        cerr << "Error: couldn't open " << filename << endl;
    }
}

void objectCollision(soaObject *obj, int index1, int index2){
    obj->mass[index1] += obj->mass[index2];
    obj->vx[index1] += obj->vx[index2];
    obj->vy[index1] += obj->vy[index2];
    obj->vz[index1] += obj->vz[index2];
}



void checkForInitialCollisions(soaObject &obj){
    bool deletedObject = false;

    #pragma omp parallel for
    for(unsigned long a = 0; a < SOAOBJLEN(obj); ++a){

        for(unsigned long b = a+1; b < SOAOBJLEN(obj); ++b){
            if(isCollision(obj.px[a], obj.px[b], obj.py[a], obj.py[b], obj.pz[a], obj.pz[b])){
                #pragma omp critical
                {
                    objectCollision(&obj, a, b);
                    obj.mass[b] = 0;
                    deletedObject = true;
                };
            }
        }
    }

    if(deletedObject){
        #pragma omp parallel
        for(unsigned long b = 0; b < SOAOBJLEN(obj); ++b){
            if(obj.mass[b] == 0){
                #pragma omp critical
                {
                    obj.px.erase(obj.px.begin() + b);
                    obj.py.erase(obj.py.begin() + b);
                    obj.pz.erase(obj.pz.begin() + b);
                    obj.vx.erase(obj.vx.begin() + b);
                    obj.vy.erase(obj.vy.begin() + b);
                    obj.vz.erase(obj.vz.begin() + b);
                    obj.mass.erase(obj.mass.begin() + b);
                    --b;
                };
            }

        }
    }
}

void computeForces(soaObject &obj, std::vector<spaceVector> &gForces){
#pragma omp parallel for collapse(2)
    for(unsigned long a = 0; a < SOAOBJLEN(obj); ++a){

        // Before computing velocity and position, we need Force applied from every body in the system
        for(unsigned long b = 0; b < SOAOBJLEN(obj); ++b){

            if(a != b){
                // Obtain gravitational force vector
                spaceVector force = gravitationalForce(obj.mass[a], obj.mass[b], obj.px[a], obj.py[a], obj.pz[a], obj.px[b], obj.py[b], obj.pz[b]);

                gForces[a].x += force.x;
                gForces[a].y += force.y;
                gForces[a].z += force.z;
            }
        }
    }
}

void computeVelocitiesAndPositions(soaObject &obj, std::vector<spaceVector> &gForces, inputParameters params){
    #pragma omp parallel for
    for(unsigned long a = 0; a < SOAOBJLEN(obj); ++a){
        // velocity on body A
        obj.vx[a] = computeVelocity(obj.vx[a], (gForces[a].x / obj.mass[a]), params.time_step);
        obj.vy[a] = computeVelocity(obj.vy[a], (gForces[a].y / obj.mass[a]), params.time_step);
        obj.vz[a] = computeVelocity(obj.vz[a], (gForces[a].z / obj.mass[a]), params.time_step);
        // position on body A
        obj.px[a] = computePosition(obj.px[a], obj.vx[a], params.time_step);
        obj.py[a] = computePosition(obj.py[a], obj.vy[a], params.time_step);
        obj.pz[a] = computePosition(obj.pz[a], obj.vz[a], params.time_step);
    }
}


void checkForCollisions(soaObject &obj, inputParameters params){
    bool deletedObject = false;

    #pragma omp parallel for
    for(unsigned long a = 0; a < SOAOBJLEN(obj); ++a){

        for(unsigned long b = a+1; b < SOAOBJLEN(obj); ++b){
            if(isCollision(obj.px[a], obj.py[a], obj.pz[a], obj.px[b], obj.py[b], obj.pz[b])){
                // Do something about the collision
                #pragma omp critical
                {
                    objectCollision(&obj, a, b);
                    obj.mass[b] = 0;
                    deletedObject = true;
                };
            }
        }
        //#pragma omp critical
        checkRebound(&obj, params.size_enclosure, a);
    }

    if(deletedObject){
        #pragma omp parallel for
        for(unsigned long b = 0; b < SOAOBJLEN(obj); ++b){
            if(obj.mass[b] == 0){
                #pragma omp critical
                {
                    obj.mass.erase(obj.mass.begin() + b);
                    obj.px.erase(obj.px.begin() + b);
                    obj.py.erase(obj.py.begin() + b);
                    obj.pz.erase(obj.pz.begin() + b);
                    obj.vx.erase(obj.vx.begin() + b);
                    obj.vy.erase(obj.vy.begin() + b);
                    obj.vz.erase(obj.vz.begin() + b);
                    --b;
                }
            }
        }
    }
}

void executeSimulation(inputParameters params){
    soaObject obj = getInitialBodies(params);

    storeConfiguration("init_config.txt", params.size_enclosure, params.time_step, &obj);

    // First, let's check the randomly generated set in case there are collisions in the first place
    checkForInitialCollisions(obj);

    // Start a vector to store accelerations and initialize it with an empty value
    spaceVector toFill;
    toFill.x = 0;
    toFill.y = 0;
    toFill.z = 0;
    std::vector <spaceVector> gForces = fillVector(toFill, SOAOBJLEN(obj));


    // Each iteration of this loop is an iteration of params.time_step seconds up to params.num_iterations times
    for(int index = 0; index < params.num_iterations; ++index){
        computeForces(obj, gForces);
        computeVelocitiesAndPositions(obj, gForces, params);
        checkForCollisions(obj, params);
        // Empty the forces computed in the iteration to prevent errors in the next one
        eraseForces(gForces, toFill);
    }
    storeConfiguration("final_config.txt", params.size_enclosure, params.time_step, &obj);
}
