#include "DjikstraTrafficController.h"

#include <queue>

using namespace Traffic;

DjikstraController::DjikstraController() {

}

DjikstraController::~DjikstraController() {

}

void DjikstraController::UpdateRailNetwork(Rail::RailNetwork& network, const std::vector<Train::Train *>& trains) {
    // For each train, find and set the shortest path
    for(auto train : trains) {
        // Only find the shortest path once for each train
        Path shortestPath;
        if(mShortestPaths.count(train) == 0) {
            shortestPath = findShortestPath(train);
            if (!shortestPath.empty()) {
                mShortestPaths[train] = shortestPath;
            }
        } else {
            shortestPath = mShortestPaths.at(train);
        }

        // If we can appropriately set the path for one train, return
        // We can extend this with multi-train optimizations at a later date
        if(setPath(network, shortestPath)) {
            return;
        } else {
            printf("WARNING Failed to set optimal path for train %s\n", train->GetName());
        }
    }
}

DjikstraController::Path DjikstraController::findShortestPath(Train::Train* train) {
    using Path = DjikstraController::Path;
    typedef struct {
        unsigned int distance = UINT32_MAX;
        Path path;
        Rail::IConnector* vertex = nullptr;
    } PriorityPath;

    using CompareFn = std::function<bool(PriorityPath, PriorityPath)>;
    CompareFn compare (
        [](PriorityPath below, PriorityPath above) -> bool {
            return below.distance > above.distance;
        }
    );

    std::map<Rail::IConnector*, unsigned int> visitedNodes;

    std::priority_queue<PriorityPath, std::vector<PriorityPath>, CompareFn> pathsQueue(compare);

    auto sourceSeg = dynamic_cast<const Rail::ISegment*>(train->GetCurrentComponent());

    // Push the starting Node in to the priority queue
    // The initial cost is the length of the starting segment
    PriorityPath initialPath {
        .distance = train->GetCurrentComponent()->GetLength(),
        .path =  Path(),
        .vertex = sourceSeg->GetNext(train->GetDirection()),
    };

    initialPath.path.push_back(sourceSeg);
    visitedNodes[initialPath.vertex] = initialPath.distance;

    pathsQueue.push(initialPath);

    while(!pathsQueue.empty()) {
        // Pop the top of the queue
        PriorityPath next = pathsQueue.top();
        pathsQueue.pop();

        // If we have the destination at the top of our queue, we have found the shortest path
        if(next.vertex == train->GetDestination()) {
            printf("INFO Path found for Train %s\n", train->GetName());
            return next.path;
        }

        // Otherwise we loop over all the next segments and add new Paths for them
        printf("INFO Exploring from %s for Train %s\n", next.path.back()->GetName(), train->GetName());

        auto nextSegments = next.vertex->GetNext(next.path.back());
        for(auto& exploredSeg : nextSegments) {
            auto exploredDistance = next.distance + exploredSeg->GetLength();

            auto exploredPath = next.path;
            exploredPath.push_back(exploredSeg);

            auto exploredVertex = exploredSeg->GetNext(train->GetDirection());

            // Add the explored vertex to our visited nodes if it has not been found
            if(!visitedNodes.count(exploredVertex)) {
                visitedNodes[exploredVertex] = UINT32_MAX;
            }

            PriorityPath newPath {
                .distance = exploredDistance,
                .path = exploredPath,
                .vertex = exploredVertex
            };

            // If this vertex is in the set of visited nodes
            // and the newly explored distance is longer  we don't need to add this new path
            if(exploredDistance < visitedNodes[exploredVertex]) {
                pathsQueue.push(newPath);
                visitedNodes[exploredVertex] = exploredDistance;
                printf("INFO Found a shorter path to %s for Train %s\n", exploredVertex->GetName(), train->GetName());
            } else {
                printf("INFO We already have a shorter path to %s for Train %s\n", exploredVertex->GetName(), train->GetName());
            }
        }
    }

    printf("ERROR No path found for Train %s to destination %s\n", 
            train->GetName(), train->GetDestination()->GetName());
    return Path();
}

bool DjikstraController::setPath(Rail::RailNetwork& network, DjikstraController::Path path) {
    bool success = true;
    // Path is a series of Segments that need to be connected, iterate through and route each to the next
    for(int i = 0; i < path.size() - 1; i++) {
        success = success && (network.RouteSegment(path[i], path[i+1]));
    }

    return success;
}