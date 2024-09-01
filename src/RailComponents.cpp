#include "RailComponents.h"

using namespace Rail;

/**
 *  Connector Implementation
 */

Connector::Connector(const std::string& name) : mName(name) {

}

Connector::~Connector() {

}

std::string Connector::GetName() {
    return mName;
}

std::string Connector::GetInfo() {
    return "";
}

const IComponent* Connector::Traverse(const IComponent* src, Direction d) {
    // Check to make sure that the train is arriving from the connected component
    if(src != mConnectedSegmentMap.at(ReverseDirection(d))) {
        printf("ERROR CRASH Train crossing improperly switched connector");
        return src;
    }

    return mConnectedSegmentMap.at(d);
}

std::set<ISegment*> Connector::GetNext(Direction d) {
    // If we're looking "Up" return a list of connected components
    if (d == Direction::UP) {
        return mAvailableSegments;
    }

    // If we're looking "Down" return the single connected component
    return std::set<ISegment*>() = { mConnectedSegmentMap.at(Direction::DOWN) };
}

void Connector::Connect(ISegment* target, Direction d) {
    // TODO null check   
    if (d == Direction::UP) {
        // If we're connecting "Up" side, append the target to our available segments
        mAvailableSegments.insert(target);
    } 
    
    mConnectedSegmentMap.at(d) = target;
}

void Connector::Select(ISegment *target) {
    // TODO null check
    // Check to make sure that our target is valid within our connected segments
    if(std::find(mAvailableSegments.begin(), mAvailableSegments.end(), target) == mAvailableSegments.end()) {
        printf("ERROR Attempting to select a segment not in the available list");
        return;
    }

    mConnectedSegmentMap.at(Direction::UP) = target;
}


/**
 * Segment Implementation
 */
Segment::Segment(const std::string& name, unsigned int length) : 
    mLength(length), mName(name) {

}

Segment::Segment(unsigned int length) : Segment("DefaultSegmentName", length) {

}

Segment::~Segment() {

}

void Segment::AddSignal(Direction d) {
    Signal s = Signal();
    s.SetState(SignalState::RED);

    mSignalMap.at(d) = s;
}

SignalState Segment::GetSignalState(Direction d) {
    if(mSignalMap.find(d) == mSignalMap.end()) {
        return SignalState::DISABLED;
    }

    return mSignalMap.at(d).GetState();
}

void Segment::SetSignalState(SignalState state, Direction d) {
    if(mSignalMap.find(d) == mSignalMap.end()) {
        printf("ERROR Attempting to set state of nonexistent signal");
        return;
    }

    mSignalMap.at(d).SetState(state);
}

std::string Segment::GetName() {
    return mName;
}

std::string Segment::GetInfo() {
    return "";
}

const IComponent* Segment::Traverse(const IComponent* src, Direction d) {
    // Check the signal in the departing direction
    if(GetSignalState(d) == SignalState::RED) {
        printf("WARNING Train stopped at red light");
        return src;
    }

    return mConnectorMap.at(d)->Traverse(src, d);
}

IConnector* Segment::GetNext(Direction d) {
    return mConnectorMap.at(d);
}

void Segment::Connect(IConnector* target, Direction d) {
    // TODO Null check
    mConnectorMap.at(d) = target;
}
