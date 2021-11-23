# Parallel programming: N-body Problem
Universidad Carlos III de Madrid's Bachelor's in Computer Science and Engineering. 

Computer Architecture programming project.

***

### What is this about?
The aim of this project is to solve the N-Body problem using C++ and parallel programming.
The program will obtain a number of celestial bodies (`num_objects`), a `seed` to randomly generate
the objects, a real number (`size_enclosure`) that represents the length of a cube's side,
a period of time (`time_step`) to use as the time it takes between observations and
the total number of observations (`num_iterations`).

### How it works

The program is supposed to receive 5 parameters, so its execution on a terminal, depending on which version of the program is to be executed  should be:

> `$ ./sim-aos <num_objects> <num_iterations> <seed> <size_enclosure> <time_step>`

or
> `$ ./sim-soa <num_objects> <num_iterations> <seed> <size_enclosure> <time_step>`


- num_objects is an int >= 0
- num_iterations is an int >= 0
- seed is an int > 0
- size_enclosure is a real number > 0 (it's the length of a cube's side)
- time_step is a double > 0 that represents the time it takes for each step

The code is to be compiled using CMake. To compile, execute the following commands on a linux terminal:
> `$ cmake CMakeLists.txt`
> 
> `$ make`

This will compile everything and will result in two executable files: _sim-aos_ and _sim-soa_.

The program consists in a _main_ function and three libraries: a _commons_ library for common functionalities, 
a _AOS_ library to execute the program using an Array of Structs (although it was implemented using _std::vector_) and
a _SOA_ library to execute the program using a Struct of Arrays (again, using vectors).

Each data structure is implemented in a different library, but both use an `executeSimulation()` function.
This call will be done from the `main()` function, but depending on the executable a different library will execute the function.

When the program is executed, it will write the mass, initial position, and velocity of each celestial body in a file named _init_config.txt_,
and once the simulation has finished, it will write the final results in the same format in a file named _final_config.txt_ 

### The algorithm

The program will first analyze the given input parametes, checking it is the correct number of inputs
and that the values are valid.
Once this is done, they will be printed on screen.

After all of this has been done, it will call the `executeSimulation()` function,
which will create a list of objects (celestial bodies) using a Marsenne pseudo-random generator
and store it in the corresponding data structure.

Then, the result is stored in the _init_config.txt_ file and  checked so that there are not invalid positions for the objects,
after which a vector to represent the gravitational forces is initialized with all values set to zero.

Once this is done, we arrive to the main loop, which iterates `num_iterations` times, 
inside which we first compute all gravitational forces between all objects, their velocity and position, 
and possible collisions and out of bounds positions are checked.
If an object gets out of the cube space, its corresponding velocity and position 
are reset as if it bounces back on the limit.

At the end of each iteration, we reset the gravitational forces vector to zero values 
to ensure the next iteration's results are valid.
Once the main loop is finished, the final result is stored in _final_config.txt_.
























