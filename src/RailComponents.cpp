#include "RailComponents.h"

using namespace Rail;

/**
 *  Connector Implementation
 */

Connector::Connector(const std::string& name) : mName(name) {

}

Connector::~Connector() {

}

const char * const Connector::GetInfo() const {
    return "";
}

const IComponent* Connector::Traverse(const IComponent* src, Direction d) const {
    IComponent* target = nullptr;
    if(src == mSelectedSegments.first) {
        target = mSelectedSegments.second;
    } else if (src == mSelectedSegments.second) {
        target = mSelectedSegments.first;
    } 
    
    if (target == nullptr) {
        // Traversing network not from a selected segment
        printf("ERROR CRASH Train crossing improperly switched connector");
        return src;
    }

    return target;
}

std::set<ISegment*> Connector::GetNext(ISegment* src) {
    if(mAvailableSegments.find(src) == mAvailableSegments.end()) {
        // Available segments does not contain src
        printf("ERROR GetNext on connector with invalid source segment");
        return std::set<ISegment*>();
    }

    // Return a subset of available connectors, without src
    std::set<ISegment*> ret = mAvailableSegments;
    ret.erase(src);
    
    return ret;
}

void Connector::Connect(ISegment* target) {
    // TODO null check
    mAvailableSegments.insert(target);

    // Automatically select segments as they are connected
    if(mSelectedSegments.first == nullptr) {
        mSelectedSegments.first = target;
    } else if (mSelectedSegments.second == nullptr) {
        mSelectedSegments.second = target;
    }
}

void Connector::Select(ISegment *s1, ISegment *s2) {
    // TODO null check
    // Check to make sure that our targets are valid within our connected segments
    if(mAvailableSegments.find(s1) == mAvailableSegments.end() || mAvailableSegments.find(s2) == mAvailableSegments.end()) {
        printf("ERROR Attempting to select a segment not in the available list");
        return;
    }

    mSelectedSegments = std::pair<ISegment*, ISegment*>(s1, s2);
}


void Connector::Fix(ISegment* src) {
    //TODO implement logic to fix a specific segment
}


/**
 * Segment Implementation
 */
Segment::Segment(const std::string& name, unsigned int length) : 
    mName(name), mLength(length) {

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

SignalState Segment::GetSignalState(Direction d) const {
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

const char * const Segment::GetInfo() const {
    return "";
}

const IComponent* Segment::Traverse(const IComponent* src, Direction d) const {
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

/**
 *  Terminator Implementation
 */

const IComponent* Terminator::Traverse(const IComponent* src, Direction d) const {
    // TODO Handle case where we are traversing in to a terminator

    // The TrainSimulator needs to be notified that a Train has reached a destination
    return nullptr;
}
