#include "RailNetwork.h"

using namespace Rail;

RailNetwork::RailNetwork(const IComponentFactory& f) :
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

ISegment* RailNetwork::CreateSegment(unsigned int length) {
    ISegment *segment = mComponentFactory.NewSegment("DefaultSegmentName", length);
    mSegments.push_back(segment);

    return segment;
}

ISegment* RailNetwork::AttachSegment(ISegment* src, Direction d, unsigned int length) {
    ISegment *segment = CreateSegment(length);

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
        target = mComponentFactory.NewConnector("DefaultConnectorName");
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
void RailNetwork::RouteSegment(ISegment* src, ISegment* dst) {
    // TODO this can be improved by testing the result of Select on the upConnector and only switching the DOWN if needed
    IConnector* upConnector = src->GetNext(Direction::UP);
    upConnector->Select(src, dst);

    IConnector* downConnector = src->GetNext(Direction::DOWN);
    downConnector->Select(src, dst);
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

void RailNetwork::AddTerminator(ISegment* src, Direction d) {
    if(src->GetNext(d) != nullptr) {
        printf("ERROR Connecting terminator to connected segment");
        return;
    }

    IConnector* terminator = mComponentFactory.NewTerminator("DefaultTerminatorName");
    src->Connect(terminator, d);

    // Save the new terminator
    mTerminators.push_back(terminator);
}