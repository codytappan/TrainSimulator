#include "Train.h"

namespace Train {

Train::Train(const std::string& name, const Rail::IComponent *startingComponent, Rail::Direction direction) : 
    mName(name), mCurrentComponent(startingComponent), mDirection(direction)
{
    // TODO Null check
}

Train::~Train() {}

void Train::Conduct() {
    if(mState != State::RUNNING) {
        printf("WARNING Conducting a Train that is not RUNNING");
        return;
    }

    if(mSegmentIndex < mCurrentComponent->GetLength()) {
        // If we have not reached the end of a component simply proceed by one
        mSegmentIndex++;
        return;
    }

    // If we have reached the end of a segment, attempt to traverse to the next
    const Rail::IComponent* nextComponent = mCurrentComponent->Traverse(mCurrentComponent, mDirection);

    if(mCurrentComponent == nextComponent) {
        // If we have not moved components, record that we are stopped
        mStoppedTime++;
        printf("INFO Train %s stopped on %p, direction %s", GetName(), mCurrentComponent->GetName(), Rail::PrintDirection(mDirection));
    }

    // We have moved to a new component, update and check success
    mCurrentComponent = nextComponent;
    mSegmentIndex = 0;
    if (mCurrentComponent == mDestinationComponent) {
        // If we have reached our destination, update
        mState = State::SUCCESS;
        printf("SUCCESS Train %s has reached its destination %s", GetName(), mDestinationComponent->GetName());
    }
}

} // namespace Train