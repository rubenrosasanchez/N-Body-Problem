#!/bin/bash

# Variables
SEED=1
SIZE_ENCLOSURE=1000000
TIME_STEP=1
POPULATION="400"		# Number of objects
ITERATIONS="100"		# Number of iterations
NUM_THREADS="1 2 4"	# Number of threads
EXECUTABLES="paos psoa"		# Executables to test

for E in $EXECUTABLES; do
	for P in $POPULATION; do
		for I in $ITERATIONS; do
			for N in $NUM_THREADS; do
				# Set the number of threads
				export OMP_NUM_THREADS=$N
				echo "Running with exe=$E objs=$P iters=$I threads=$N"
				for T in {1..4}; do
					echo "$T -> `grep elapsed <(perf stat ./$E $P $I $SEED $SIZE_ENCLOSURE $TIME_STEP 2>&1)`"
				done
			done
		done
	done
done
