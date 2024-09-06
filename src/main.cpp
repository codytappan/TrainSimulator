#include <cstdio>

#include "TrainSimulator.h"


int main(int argc, char **argv) {
    Train::Simulator simulator;

    // Run a simple network
    // Test a network where collision happens
    // Run a large network

    simulator.Build();
    simulator.Run();
    simulator.ValidateResults();

    return 0;
}
