#include "RailComponents.h"

using namespace Rail;

/**
 *  Connector Implementation
 */

Connector::Connector(const std::string& name) : mName(name) {
    printf("DEBUG Connector %s created\n", GetName());
}

Connector::~Connector() {

}

const char * const Connector::GetInfo() const {
    return "";
}

const IComponent* Connector::Traverse(const IComponent* src, Direction d) const {
    IComponent* target = nullptr;
    // Connectors are traversed based on src component, not direction
    if(src == mSelectedSegments.first) {
        target = mSelectedSegments.second;
    } else if (src == mSelectedSegments.second) {
        target = mSelectedSegments.first;
    } 
    
    if (target == nullptr) {
        // Traversing network not from a selected segment
        // TODO this crash will not be detected by train
        printf("ERROR CRASH Train crossing improperly switched connector\n");
        return nullptr;
    }

    return target;
}

std::set<ISegment*> Connector::GetNext(ISegment* src) {
    if(mAvailableSegments.find(src) == mAvailableSegments.end()) {
        // Available segments does not contain src
        printf("ERROR GetNext on connector with invalid source segment\n");
        return std::set<ISegment*>();
    }

    // Return a subset of available connectors, without src
    std::set<ISegment*> ret = mAvailableSegments;
    ret.erase(src);
    
    return ret;
}

void Connector::Connect(ISegment* target) {
    // TODO null check
    // Check to make sure we have not already connected to this segment
    if(mAvailableSegments.find(target) != mAvailableSegments.end()) {
        printf("INFO Connector %s has already connected segment %s\n", GetName(), target->GetName());
        return;
    }

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
        printf("ERROR Attempting to select a segment not in the available list\n");
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
    printf("DEBUG Segment %s created\n", GetName());
}

Segment::Segment(unsigned int length) : Segment("DefaultSegmentName", length) {
    mConnectorMap[Direction::UP] = nullptr;
    mConnectorMap[Direction::DOWN] = nullptr;
    mSignalMap[Direction::UP] = Rail::Signal();
    mSignalMap[Direction::DOWN] = Rail::Signal();
}

Segment::~Segment() {

}

void Segment::AddSignal(Direction d) {
    Signal s = Signal();
    s.SetState(SignalState::RED);

    mSignalMap[d] = s;
}

SignalState Segment::GetSignalState(Direction d) const {
    if(mSignalMap.find(d) == mSignalMap.end()) {
        return SignalState::DISABLED;
    }

    return mSignalMap.at(d).GetState();
}

void Segment::SetSignalState(SignalState state, Direction d) {
    if(mSignalMap.find(d) == mSignalMap.end()) {
        printf("ERROR Attempting to set state of nonexistent signal\n");
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
        printf("WARNING Train stopped at red light\n");
        return src;
    }

    return mConnectorMap.at(d)->Traverse(src, d);
}

IConnector* Segment::GetNext(Direction d) {
    return mConnectorMap[d];
}

void Segment::Connect(IConnector* target, Direction d) {
    // TODO Null check
    mConnectorMap[d] = target;
}

/**
 *  Terminator Implementation
 */
Terminator::Terminator(const std::string& name) : Connector(name) {
    printf("DEBUG Terminator %s created\n", GetName());
}

Terminator::~Terminator() {

}

void Terminator::Connect(ISegment* target) {
    if(mConnectedSegment != nullptr) {
        printf("ERROR Attempting to connect already connected Terminator\n");
        return;
    }

    mConnectedSegment = target;
    printf("INFO Terminator %s connected to segment %s\n", GetName(), target->GetName());
}


const IComponent* Terminator::Traverse(const IComponent* src, Direction d) const {
    if(src != mConnectedSegment) {
        printf("ERROR Train traversing to terminator from segment %s, expecting %s\n", 
            src->GetName(), mConnectedSegment->GetName());
        return src;
    }

    return this;
}
