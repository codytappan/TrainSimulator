#include "TrainSimulator.h"
#include "DjikstraTrafficController.h"
#include "RailNetwork.h"

using namespace Train;

Simulator::Simulator() {
    mRailNetwork = new Rail::RailNetwork(new Rail::ComponentFactory());
    mTrafficController = new Traffic::DjikstraController();
}

Simulator::~Simulator() {
    free(mRailNetwork);
    free(mTrafficController);
}

/**
 *  Build a rail network and populate it with trains
 */
void Simulator::Build() {
    // For now make a simple test Network. We'll add user input later

    // TermA --> SegA 20 --> SegB 20 --> SegC 10 --> TermB
    //                   \-- SegD 10 --/ 
    auto segA = mRailNetwork->CreateSegment("SegA", 20);
    auto segB = mRailNetwork->AttachSegment(segA, Rail::Direction::UP, "SegB", 20);
    auto segC = mRailNetwork->AttachSegment(segB, Rail::Direction::UP, "SegC", 10);
    auto segD = mRailNetwork->AttachSegment(segA, Rail::Direction::UP, "SegD", 10);

    // Connect segD up to segC down
    mRailNetwork->ConnectSegments(segD, Rail::Direction::UP, segC, Rail::Direction::DOWN);

    // Terminate Network
    auto termA = mRailNetwork->AddTerminator(segA, Rail::Direction::DOWN, "TermA");
    auto termB = mRailNetwork->AddTerminator(segC, Rail::Direction::UP, "TermB");

    // Add a train to the network
    Train* testTrain = new Train("TestTrain", segA, Rail::Direction::UP);
    testTrain->SetDestination(termB);

    mRunningTrains.push_back(testTrain);
}

/**
 *  Run a built simulation
 */
void Simulator::Run() {
    // As long as trains are still in the simulator, tick the simulation
    while(!mRunningTrains.empty()) {
        // Let the traffic controller update the rail network
        updateRailNetwork();

        // Conduct each train forward
        for(auto train: mRunningTrains) {
            train->Conduct();

            if(checkTrainCollision(train)) {
                // A train has crashed!
            } else if (checkTrainSucceeded(train)) {
                // A train has made it to the end!
            }
        }

        // Remove any trains that have finished their simulation
        removeFinishedTrains();
    }
}

/**
 *  Validate the results of a simulation
 */
bool Simulator::ValidateResults() {
    bool success = true;
    for(auto train: mFinishedTrains) {
        train->PrintStatus();
        success = success && (train->GetState() == Train::State::SUCCESS);
    }

    printf("INFO Simulation Results: \n");
    printf("INFO All trains safe? %s \n", success ? "YES" : "NO");
    printf("INFO All trains finished? %s \n", mRunningTrains.empty() ? "YES" : "NO");

    return success && mRunningTrains.empty();
}

/**
 *  Checks to see if the train has collided with any other trains
 */
bool Simulator::checkTrainCollision(Train* train) {
    for(auto other: mRunningTrains) {
        if(train != other && train->CheckCollision(other)) {
            // Trains can only collide 1:1 so we early return on any collision
            return true;
        }
    }

    return false;
}

/**
 *  Checks to see if a train has arrived at its destination
 */
bool Simulator::checkTrainSucceeded(const Train* train) {
    return train->GetState() == Train::State::SUCCESS;
}

/**
 *  Removes finished trains from the running simulation
 */
void Simulator::removeFinishedTrains() {
    for(auto iter = mRunningTrains.begin(); iter != mRunningTrains.end(); ) {
        if((*iter)->GetState() != Train::State::RUNNING) {
            // Find any trains that are not RUNNING
            printf("INFO Removing Train %s from simulation\n", (*iter)->GetName());

            // and move them to finished trains
            mFinishedTrains.push_back(*iter);
            iter = mRunningTrains.erase(iter);
        } else {
            iter++;
        }
    }
}

/**
 *  Updates simulator's rail network accoring to the Traffic Controller
 */
void Simulator::updateRailNetwork() {
    mTrafficController->UpdateRailNetwork(*mRailNetwork, mRunningTrains);
}
