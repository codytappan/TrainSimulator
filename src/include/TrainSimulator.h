#ifndef TrainSimulator_H
#define TrainSimulator_H

#include "Train.h"
#include "RailNetwork.h"

namespace Train {

    class Simulator {
        public:
        Simulator();
        ~Simulator();

        // For now, we use the RailNetwork interface to build the network programatically. 
        // In future we can read this from a file or implement a command line util for users
        Rail::IRailNetwork& GetNetwork() {
            return mRailNetwork;
        }

        /**
         * 
         */
        Train& AddTrain(Rail::ISegment& src, Rail::ITerminator& dst, Rail::Direction direction);

        void RunSimulation();

        private:
        Rail::IRailNetwork& mRailNetwork;
        std::vector<Train> mTrains;
    };

}

#endif
