#ifndef ITrafficController_H
#define ITrafficController_H

#include "IRailComponent.h"
#include "RailNetwork.h"
#include "Train.h"

namespace Traffic {

    class ITrafficController {
        public:
        /**
         *  Update the rail network switching and signals, based on the currently active trains
         */
        virtual void UpdateRailNetwork(Rail::RailNetwork& network, const std::vector<Train::Train *>& trains) = 0;
    };

}

#endif
