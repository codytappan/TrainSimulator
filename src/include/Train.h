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
         *  Reverses the direction of the train
         */
        void ReverseDirection() {
            mDirection = Rail::ReverseDirection(mDirection);
        }

        /**
         *  Gets the current segment of the train
         */
        const Rail::IComponent* GetCurrentComponent() {
            return mCurrentComponent;
        }

        /**
         *  Sets the destination of the train
         */
        void SetDestination(const Rail::IComponent* destination) {
            mDestinationComponent = destination;
        }

        /**
         *  Gets the name of the train
         */
        const char * const GetName() {
            return mName.c_str();
        }

        /**
         *  Gets the state of the train
         */
        State GetState() {
            return mState;
        }

        private:
        const std::string& mName = "DefaultTrainName";

        const Rail::IComponent* mCurrentComponent = nullptr;
        const Rail::IComponent* mDestinationComponent = nullptr;
        Rail::Direction mDirection = Rail::Direction::DOWN;

        unsigned int mSegmentIndex = 0;
        unsigned int mStoppedTime = 0;

        State mState = State::RUNNING;
    };
}

#endif
