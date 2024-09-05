#ifndef TrainSimulator_H
#define TrainSimulator_H

#include "Train.h"
#include "RailNetwork.h"
#include "interfaces/ITrafficController.h"

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
        bool ValidateResults();

        private:
        /**
         *  Checks to see if the train has collided with any other trains
         */
        bool checkTrainCollision(Train* train);

        /**
         *  Checks to see if a train has arrived at its destination
         */
        bool checkTrainSucceeded(const Train* train);

        /**
         *  Removes finished trains from the running simulation
         */
        void removeFinishedTrains();

        /**
         *  Updates rail network accoring to the Traffic Controller
         */
        void updateRailNetwork();

        Rail::RailNetwork* mRailNetwork;
        Traffic::ITrafficController* mTrafficController;

        std::vector<Train*> mRunningTrains;
        std::vector<Train*> mFinishedTrains;
    };

}

#endif
