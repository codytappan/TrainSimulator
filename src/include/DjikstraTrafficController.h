#ifndef DjikstraTrafficController_H
#define DjikstraTrafficController_H

#include "interfaces/ITrafficController.h"

namespace Traffic {
    
    class DjikstraController : public ITrafficController {
        public:
        DjikstraController();
        virtual ~DjikstraController();

        // ITrafficController
        virtual void UpdateRailNetwork(Rail::RailNetwork& network, const std::vector<Train::Train *>& trains);

        private:
        using Path = std::vector<const Rail::ISegment*>;

        /**
         *  Find the shortest path across the network for the given train
         * 
         *  @param train The Train to find the shortest path for
         *  @return The shortest path found using Djikstra's algorithm
         * 
         *  @note Paths found for this method are cached in mShortestPaths, and must
         *        be manually cleared if we want to subsequently recalculate the path
         */
        Path findShortestPath(Train::Train* train);

        /**
         *  Links junctions to create the given path in the network
         * 
         *  @param network The network to set the path on
         *  @param path The path of components to set in the network
         */
        bool setPath(Rail::RailNetwork& network, Path path);

        std::map<Train::Train*, Path> mShortestPaths;
    };

}

#endif