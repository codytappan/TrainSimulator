#ifndef TrainSimulator_H
#define TrainSimulator_H

#include "Train.h"
#include "RailNetwork.h"

namespace Train {

    class Simulator {
        public:
        Simulator();
        ~Simulator();

        /**
         *  Build a rail network and populate it with trains
         */
        void Build();

        /**
         *  Run a built simulation
         */
        void Run();

        /**
         *  Validate the results of a simulation
         */
        void ValidateResults();

        private:
        Rail::RailNetwork mRailNetwork;
    };

}

#endif
