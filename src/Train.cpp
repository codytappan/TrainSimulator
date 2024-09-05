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
    
    // If we have not reached the end of a component simply progress by one unit
    if(mSegmentIndex < mCurrentComponent->GetLength()) {
        handleProgressed();
        return;
    }

    // If we have reached the end of a segment, attempt to traverse the network
    const Rail::IComponent* nextComponent = mCurrentComponent->Traverse(mCurrentComponent, mDirection);
    handleTraversed(nextComponent);
}

void Train::NotifyCollided(Train* other) {
    //TODO null check
    printf("ERROR Train %s collided with %s on component %s", GetName(), other->GetName(), mCurrentComponent->GetName());
    mState = State::CRASHED;
}

bool Train::CheckCollision(Train* other) {
    //TODO null check
    if(GetCurrentComponent() == other->GetCurrentComponent() &&
       GetCurrentLocation(mDirection) == other->GetCurrentLocation(mDirection)) {
        // If the trains are at the same location on the same connector, then
        // A collision has ocurred
        NotifyCollided(other);
        other->NotifyCollided(this);
        return true;
    }

    return false;
}

unsigned int Train::GetCurrentLocation(Rail::Direction d) const {
    if(mDirection == d) {
        return mSegmentIndex;
    } else {
        return mCurrentComponent->GetLength() - mSegmentIndex + 1;
    }
}

void Train::PrintStatus() const {
    printf("INFO Train %s: %s at %s\n", 
            GetName(), PrintState(mState), mCurrentComponent->GetName());
    printf("INFO Train %s travelled: %d units\n", GetName(), mDistanceTraveled);
    printf("INFO Train %s stopped time: %d units\n\n", GetName(), mStoppedTime);
}


// Handles a case where Conduct progesses along the current component
void Train::handleProgressed() {
    mSegmentIndex++;
    mDistanceTraveled++;
}

// Handles the case where Conduct traverses to a new component
void Train::handleTraversed(const Rail::IComponent* newComponent) {
    // TODO nullcheck
    // If we have not moved components, record that we are stopped
    if(mCurrentComponent == newComponent) {
        handleStopped();
        return;
    }

    // We have moved to a new component, update data
    mSegmentIndex = 0;
    mCurrentComponent = newComponent;

    // Printing every transition for debug
    PrintStatus();

    // Then we need to check if we are at our destination
    if (mCurrentComponent == mDestinationComponent) {
        mState = State::SUCCESS;
        printf("SUCCESS Train %s has reached its destination %s", GetName(), mDestinationComponent->GetName());
    }
}

// Handles the case where Conduct is called while the train is stopped
void Train::handleStopped() {
    mStoppedTime++;
    printf("INFO Train %s stopped on %p, direction %s", 
        GetName(), mCurrentComponent->GetName(), Rail::PrintDirection(mDirection));
}

} // namespace Train