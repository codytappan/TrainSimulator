#include <cstdio>

#include "TrainSimulator.h"

int main(int argc, char **argv) {
    Train::Simulator simulator;

    simulator.Build();
    simulator.Run();
    simulator.ValidateResults();

    return 0;
}
