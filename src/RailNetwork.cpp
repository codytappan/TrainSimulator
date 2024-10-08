#include "RailNetwork.h"

using namespace Rail;

RailNetwork::RailNetwork(const IComponentFactory* f) :
    mComponentFactory(f), mSegments(), mConnectors(), mTerminators()
{

}

RailNetwork::~RailNetwork() {
    // Free the created components
    for(auto s : mSegments) {
        free(s);
    }

    for(auto c : mConnectors) {
        free(c);
    }

    for(auto t : mTerminators) {
        free(t);
    }
}

ISegment* RailNetwork::CreateSegment(const std::string& name, unsigned int length) {
    ISegment *segment = mComponentFactory->NewSegment(name, length);
    mSegments.push_back(segment);

    return segment;
}

ISegment* RailNetwork::AttachSegment(ISegment* src, Direction d, const std::string& name, unsigned int length) {
    ISegment *segment = CreateSegment(name, length);

    // Where possible we connect segments UP<->DOWN sides to ease logic of traversing network
    ConnectSegments(src, d, segment, ReverseDirection(d));

    return segment;
}

void RailNetwork::ConnectSegments(ISegment* s1, Direction d1, ISegment* s2, Direction d2) {
    IConnector* c1 = s1->GetNext(d1);
    IConnector* c2 = s2->GetNext(d2);
    IConnector* target = nullptr;

    if(c1 != nullptr && c2 != nullptr) {
        // If both segments are already connected to other segments in the given directions
        // We cannot complete this operation
        printf("ERROR Connecting two already connected segments");
        return;
    }

    if( c1 == nullptr && c2 == nullptr) {
        // If neither segment is connected we need to create a new connector for this op
        target = mComponentFactory->NewConnector("DefaultConnectorName");
        mConnectors.push_back(target);
    } else {
        // Target the existing connector
        target = (c1 != nullptr ? c1 : c2);
    }

    // Connect the target connector to both segments
    target->Connect(s1);
    target->Connect(s2);

    if(c1 == nullptr) {
        s1->Connect(target, d1);
    }

    if(c2 == nullptr) {
        s2->Connect(target, d2);
    }
}

/**
 *  Switch the connector between the given segments so that trains traverse from src to dst
 */
bool RailNetwork::RouteSegment(const ISegment* src, const ISegment* dst) {
    // Attempt the up facing connection first
    IConnector* upConnector = src->GetNext(Direction::UP);
    if(upConnector->Select(src, dst)) {
        return true;
    }

    // If that failed, we may be routing down
    IConnector* downConnector = src->GetNext(Direction::DOWN);
    if(downConnector->Select(src, dst)) {
        return true;
    }

    printf("WARNING Failed to route segments %s and %s\n",
            src->GetName(), dst->GetName());
    return false;
}

void RailNetwork::AddSignal(ISegment* segment, Direction d, SignalState state) {
    SignalState currentState = segment->GetSignalState(d);
    if (currentState != SignalState::DISABLED) {
        printf("ERROR Adding signal to location where signal has already been added");
        return;
    }

    segment->AddSignal(d);
    segment->SetSignalState(state, d);
}

void RailNetwork::SetSignal(ISegment* segment, Direction d, SignalState state) {
    SignalState currentState = segment->GetSignalState(d);
    if (currentState == SignalState::DISABLED) {
        printf("ERROR Setting signal state in location where no signal exits");
        return;
    }

    segment->SetSignalState(state, d);
}

IConnector* RailNetwork::AddTerminator(ISegment* src, Direction d, const std::string& name) {
    if(src->GetNext(d) != nullptr) {
        printf("ERROR Connecting terminator to connected segment");
        return nullptr;
    }

    IConnector* terminator = mComponentFactory->NewTerminator(name);
    src->Connect(terminator, d);
    terminator->Connect(src);

    // Save the new terminator
    mTerminators.push_back(terminator);
    return terminator;
}