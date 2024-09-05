#include <cstdio>

#include "TrainSimulator.h"

int main(int argc, char **argv) {
    Train::Simulator simulator;

    // Build the simulation. This will collect user input to build a
    // RailNetwork and populate it with Trains
    simulator.Build();

    // Run the simulation and check the results
    simulator.Run();
    simulator.ValidateResults();

    return 0;
}
