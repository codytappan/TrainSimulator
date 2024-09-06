#include <cstdio>

#include "TrainSimulator.h"


int main(int argc, char **argv) {
    Train::Simulator simulator;

    printf("\n- TrainSimulator ready to run SimpleNetworkTest -\n");
    getchar();

    simulator.RunSimpleNetworkTest();

    printf("\n- TrainSimulator ready to run CollisionTest -\n");
    getchar();

    simulator.RunCollisionTest();

    return 0;
}
