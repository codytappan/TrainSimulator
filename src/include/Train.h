#ifndef Train_H
#define Train_H

#include "interfaces/IRailComponent.h"

#include <string>

namespace Train {
    class Train {
        public:
        Train(const std::string& name, const Rail::IComponent *startingComponent, Rail::Direction direction);
        ~Train();

        /**
         *  Train is a stateful object in the simulation.
         *  RUNNING means that the train is still proceeding in the simulation
         *  CRASHED indicates that the train has failed the simulation
         *  SUCCESS indicates that the train has reached its destination without crashing
         */
        typedef enum {
            RUNNING,
            CRASHED,
            SUCCESS
        } State;

        /**
         *  Each call causes the train to travel along the current segment one unit
         */
        void Conduct();

        /**
         *  Gets the name of the train
         */
        const char * const GetName() const {
            return mName.c_str();
        }

        /**
         *  Gets the state of the train
         */
        State GetState() const {
            return mState;
        }

        /**
         *  Reverses the direction of the train
         */
        void ReverseDirection() {
            mDirection = Rail::ReverseDirection(mDirection);
        }

        /**
         *  Gets the current segment of the train
         */
        const Rail::IComponent* GetCurrentComponent() const {
            return mCurrentComponent;
        }

        /**
         *  Notifies the train that a collision has ocurred
         */
        void NotifyCollided(Train* other);

        /**
         *  Checks for a collision with another train
         *  
         *  @param other The other train to check against
         *  @return true if the two trains have collided, false otherwise
         * 
         *  @note If a collision occurs this will update this Train and other Train states accordingly to CRASHED
         */
        bool CheckCollision(Train* other);

        /**
         *  Gets location of train within the current component
         *   
         *  @param d The direction to count from
         *  @return The index of the train within the component in the direction d
         */
        unsigned int GetCurrentLocation(Rail::Direction d) const;

        /**
         *  Prints the current status of the train
         */
        void PrintStatus() const;

        /**
         *  Sets the destination of the train
         */
        void SetDestination(const Rail::IComponent* destination) {
            mDestinationComponent = destination;
        }

        // Helper function to print train state
        static const char * const PrintState(Train::State state) {
        switch (state) {
            case Train::State::RUNNING:
                return "Running";
            case Train::State::CRASHED:
                return "Crashed";
            case Train::State::SUCCESS:
                return "Success";
            default:
                return "Unexpected Train State";
        }
    }

        private:

        // Handles a case where Conduct progesses along the current component
        void handleProgressed();

        // Handles the case where Conduct traverses to a new component
        void handleTraversed(const Rail::IComponent* newComponent);

        // Handles the case where Conduct is called while the train is stopped
        void handleStopped();

        const std::string mName = "DefaultTrainName";

        const Rail::IComponent* mCurrentComponent = nullptr;
        const Rail::IComponent* mDestinationComponent = nullptr;
        Rail::Direction mDirection = Rail::Direction::DOWN;

        // Zero-based index, representing the distance of the train in to
        // its current component in its direction of travel
        unsigned int mSegmentIndex = 0;

        // Tracked for logging metrics
        // Total distance traveled by this train
        unsigned int mDistanceTraveled = 0;
        // Total time stopped for this train
        unsigned int mStoppedTime = 0;

        State mState = State::RUNNING;
    };
}

#endif
