#include "DjikstraTrafficController.h"

#include <queue>

using namespace Traffic;

DjikstraController::DjikstraController() {

}

DjikstraController::~DjikstraController() {

}

void DjikstraController::UpdateRailNetwork(Rail::RailNetwork& network, const std::vector<Train::Train *>& trains) {
    for(auto train : trains) {
        if(setPath(network, getPath(train))) {
            return;
        } else {
            printf("WARNING Failed to set optimal path for train %s\n", train->GetName());
        }
    }
}


Path DjikstraController::getPath(Train::Train* train) {
    // Check if we have a cached path for this train
    if(mShortestPaths.count(train)) {
        return mShortestPaths.at(train);
    }

    Path shortestPath = findShortestPath(train);
    if(!shortestPath.empty()) {
        mShortestPaths[train] = shortestPath;
    } else {
        printf("WARNING Could not find shortest path for Train %s\n", train->GetName());
    }

    return shortestPath;
}

Path DjikstraController::findShortestPath(Train::Train* train) {
    // For Djikstras we track a map of visited nodes, and a priority queue of routes to explore
    std::map<Rail::IConnector*, unsigned int> visitedNodes;
    std::priority_queue<PriorityPath, std::vector<PriorityPath>, PriorityPath::CompareFn> priorityPathQueue(PriorityPath::Compare);
    
    // Initialize the visited nodes list and priority queue with 
    // the starting data based off the train's location
    auto initialSegment = dynamic_cast<const Rail::ISegment*>(train->GetCurrentComponent());

    unsigned int currentDistance = train->GetCurrentComponent()->GetLength();
    Path currentPath = Path {initialSegment};
    Rail::IConnector* currentVertex = initialSegment->GetNext(train->GetDirection());

    visitedNodes[currentVertex] = currentDistance;
    priorityPathQueue.push(PriorityPath(currentDistance, currentPath, currentVertex));

    // Now begin iterating through the priority queue of routes to explore
    while(!priorityPathQueue.empty()) {
        // Pop the top of the queue
        PriorityPath next = priorityPathQueue.top();
        priorityPathQueue.pop();

        // If we have our destination at the top of our queue, we have found the shortest path
        if(next.GetVertex() == train->GetDestination()) {
            printf("INFO Path found for Train %s\n", train->GetName());
            return next.GetPath();
        }

        // Otherwise loop over all the next segments and add new Paths for them
        printf("INFO Exploring from %s for Train %s\n", next.GetPath().back()->GetName(), train->GetName());

        auto nextSegments = next.GetVertex()->GetNext(next.GetPath().back());
        for(auto& exploring : nextSegments) {
            // Update the current info according to the explored segment
            currentDistance = next.GetDistance() + exploring->GetLength();
            currentPath = next.GetPath();
            currentPath.push_back(exploring);
            currentVertex = exploring->GetNext(train->GetDirection());

            // Add the explored vertex to our visited nodes if it has not been found
            if(!visitedNodes.count(currentVertex)) {
                visitedNodes[currentVertex] = UINT32_MAX;
            }

            // If this vertex is in the set of visited nodes
            // and the newly explored distance is longer  we don't need to add this new path
            if(currentDistance < visitedNodes[currentVertex]) {
                priorityPathQueue.push(PriorityPath(currentDistance, currentPath, currentVertex));
                visitedNodes[currentVertex] = currentDistance;
                printf("INFO Found a shorter path to %s for Train %s\n", 
                        currentVertex->GetName(), train->GetName());
            } else {
                printf("INFO We already have a shorter path to %s for Train %s\n", 
                        currentVertex->GetName(), train->GetName());
            }
        }
    }

    printf("ERROR No path found for Train %s to destination %s\n", 
            train->GetName(), train->GetDestination()->GetName());
    return Path();
}

bool DjikstraController::setPath(Rail::RailNetwork& network, Path path) {
    bool success = true;
    // Path is a series of Segments that need to be connected, iterate through and route each to the next
    for(int i = 0; i < path.size() - 1; i++) {
        success = success && (network.RouteSegment(path[i], path[i+1]));
    }

    return success;
}