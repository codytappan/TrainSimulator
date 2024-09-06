#ifndef DjikstraTrafficController_H
#define DjikstraTrafficController_H

#include "interfaces/ITrafficController.h"

namespace Traffic {
    
    using Path = std::vector<const Rail::ISegment*>;

    class DjikstraController : public ITrafficController {
        public:
        DjikstraController();
        virtual ~DjikstraController();

        // ITrafficController
        virtual void UpdateRailNetwork(Rail::RailNetwork& network, const std::vector<Train::Train *>& trains);

        private:

        /**
         *  Get the shortest path for the given train.
         * 
         *  @note Paths returned by this method are cached in mShortestPaths, and must
         *        be manually cleared if we want to subsequently recalculate the path
         */
        Path getPath(Train::Train* train);

        /**
         *  Find the shortest path across the network for the given train
         * 
         *  @return The shortest path found using Djikstra's algorithm
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

    class PriorityPath {
        public:
        PriorityPath(unsigned int distance, Path path, Rail::IConnector* vertex) : 
            mDistance(distance), mPath(path), mVertex(vertex) {}
    
        ~PriorityPath() {}

        using CompareFn = std::function<bool(PriorityPath, PriorityPath)>;

        static bool Compare(PriorityPath below, PriorityPath above) {
            return below.mDistance > above.mDistance;
        }

        unsigned int GetDistance() {
            return mDistance;
        }

        const Path GetPath() {
            return mPath;
        }

        void AppendPath(Rail::ISegment* segment) {
            mPath.push_back(segment);
        }

        Rail::IConnector* GetVertex() {
            return mVertex;
        }

        private:
        unsigned int mDistance = UINT32_MAX;
        Path mPath;
        Rail::IConnector* mVertex = nullptr;
    };

}

#endif