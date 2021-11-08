#include <iostream>
#include <stdio.h>
#include <cstring>
// This library was created to avoid redundant code within the two versions of this program
#include "commons.hpp"

//The following lines are to be commented or uncommented to compile this code into Struct Of Arrays or Array of Structs
#include "aos.hpp"
#include "soa.hpp"

#define NUM_PARAMETERS 6
using namespace std;



int convertCharsToInt(char *str){
    int num;
    sscanf(str, "%d", &num);
    return num;
}

bool stringIsNumber(char * s){
    bool ret = true;
    for (int i = 0; i < (int) strlen(s); ++i) {
        if( !(isdigit(s[i]) || s[i] == '.' ) ){
            ret = false;
        }
    }
    return ret;
}

void storeParameterValues(int argc, char *argv[], struct inputParameters &p){

    for (int i = 1; i < NUM_PARAMETERS; ++i) {
        if (i < argc){
            switch (i) {
                case 1:
                    if(stringIsNumber(argv[i])){
                        p.num_objects = convertCharsToInt(argv[i]);
                    }else{
                        p.num_objects = -1; // Set it to an invalid value to display later
                    }
                    break;
                case 2:
                    if(stringIsNumber(argv[i])){
                        p.num_iterations = convertCharsToInt(argv[i]);
                    }else{
                        p.num_iterations = -1; // Set it to an invalid value to display later
                    }
                    break;
                case 3:
                    if(stringIsNumber(argv[i])){
                        p.seed = convertCharsToInt(argv[i]);
                    }else{
                        p.seed = -1; // Set it to an invalid value to display later
                    }
                    break;
                case 4:
                    if(stringIsNumber(argv[i])){
                        p.size_enclosure = strtod(argv[i], NULL);
                    }else{
                        p.size_enclosure = -1; // Set it to an invalid value to display later
                    }
                    break;
                case 5:
                    if(stringIsNumber(argv[i])){
                        p.time_step = strtod(argv[i], NULL);
                    }else{
                        p.time_step = -1; // Set it to an invalid value to display later
                    }
                    break;
            }
        }
    }

}

int checkParameterValidity(struct inputParameters &p){
    int ret = 0;
    if (p.num_objects < 0){
        cerr << "Error: Invalid number of objects" << endl; // cerr ???
        ret = -2;
    }
    if(p.num_iterations < 0){
        cerr << "Error: Invalid number of iterations" << endl; // cerr ???
        ret = -2;
    }
    if (p.seed <= 0){
        cerr << "Error: Invalid seed" << endl; // cerr ???
        ret = -2;
    }
    if (p.size_enclosure <= 0){
        cerr << "Error: Invalid enclosure size" << endl; // cerr ???
        ret = -2;
    }
    if (p.time_step <= 0){
        cerr << "Error: Invalid step time" << endl; // cerr ???
        ret = -2;
    }
    return ret;
}

void displayParameters(int argc, char *argv[]){

    for (int i = 1; i < NUM_PARAMETERS; ++i) {

        if (i < argc){

            switch (i) {
                case 1: cout << "num_objects: " << argv[i] << endl;
                    break;
                case 2: cout << "num_iterations: " << argv[i] << endl;
                    break;
                case 3: cout << "seed: " << argv[i] << endl;
                    break;
                case 4: cout << "size_enclosure: " << argv[i] << endl;
                    break;
                case 5: cout << "time_step: " << argv[i] << endl;
                    break;
                    //default: cout << "Too many parameters" << endl;
            }
        }else{

            switch (i) {
                case 1: cout << "num_objects: ?" << endl;
                    break;
                case 2: cout << "num_iterations: ?" << endl;
                    break;
                case 3: cout << "seed: ?" << endl;
                    break;
                case 4: cout << "size_enclosure: ?" << endl;
                    break;
                case 5: cout << "time_step: ?" << endl;
                    break;
                    //default: cout << "Too many parameters!" << endl;
            }

        }

    }
}

int checkAndSetParameters(int argc, char *argv[], struct inputParameters &p){

    int msg = 0; // Message to return for error handling
    storeParameterValues(argc, argv,p);
    int val = checkParameterValidity(p);
    cout << argv[0] << " invoked with " << (argc - 1) << " parameters." << endl; //////////////////////////
    displayParameters(argc, argv);

    if (argc != NUM_PARAMETERS){
        if (argc > NUM_PARAMETERS){
            cerr << "Too many parameters!" << endl;
        }
        msg = -1;
    }else if (val){
        msg = -2;
    }
    return msg;

}


int main(int argc, char *argv[]){
    /*
        The program is supposed to receive 5 parameters, so its execution on a terminal should be:
        ./sim-aos <num_objects> <num_iterations> <seed> <size_enclosure> <time_step>

        argv[1] == num_objects is an int >= 0
        argv[2] == num_iterations is an int >= 0
        argv[3] == seed is an int > 0
        argv[4] == size_enclosure is a double > 0 --> it's the length of a cube's side
        argv[5] == time_step is a double > 0 that represents the time it takes for each step

     */

    struct inputParameters params; // To store all parameters in a single data structure

    // If parameters are not valid, exit program with error code
    int p = checkAndSetParameters(argc, argv, params);
    if (p == -1){
        return -1;
    }else if (p == -2){
        return -2;
    }


    // timestamp
    executeSimulation(params);
    // executionTime = currentTime - timestamp

    return EXIT_SUCCESS;

}