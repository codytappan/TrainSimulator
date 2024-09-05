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
        mDistanceTraveled++;
        return;
    }

    // If we have reached the end of a segment, attempt to traverse to the next
    const Rail::IComponent* nextComponent = mCurrentComponent->Traverse(mCurrentComponent, mDirection);

    if(mCurrentComponent == nextComponent) {
        // If we have not moved components, record that we are stopped
        mStoppedTime++;
        printf("INFO Train %s stopped on %p, direction %s", 
            GetName(), mCurrentComponent->GetName(), Rail::PrintDirection(mDirection));
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
            GetName(), GetState(), mCurrentComponent->GetName());
    printf("INFO Train %s travelled: %d units\n", GetName(), mDistanceTraveled);
    printf("INFO Train %s stopped time: %d units\n\n", GetName(), mStoppedTime);
}

} // namespace Train