#ifndef ITrafficController_H
#define ITrafficController_H

#include "IRailNetwork.h"

namespace Traffic {

    class ITrafficController {
        /**
         *  Find the optimal path between two points in the network, and toggle junctions such that the path
         *  will be traversed
         */
        virtual void SetOptimalPath(Rail::IConnector& src, Rail::IConnector& dst) = 0;
    };

}

#endif