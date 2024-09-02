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

/**
 *  Create a segment and attach it to an existing segment, in the given direction
 */
ISegment* RailNetwork::AddSegment(ISegment* src, Direction d, unsigned int length) {
    IConnector* connector = src->GetNext(d);
    if (connector == nullptr) {
        // If the segment does not already have a connector in this direction, create one
        connector = mComponentFactory.NewConnector("DefaultConnectorName");
        src->Connect(connector, d);

        // Save the new Connector
        mConnectors.push_back(connector);
    }

    ISegment *segment = mComponentFactory.NewSegment("DefaultSegmentName", length);
    segment->Connect(connector, ReverseDirection(d));

    // Save the new Segment
    mSegments.push_back(segment);

    return segment;
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

/**
 *  Set the given signal to a specific state
 */
void RailNetwork::SetSignal(ISegment* segment, Direction d, SignalState state) {
    //TODO add signal logic to segment interface and use here
}

/**
 *  Create a terminator and attach it to an existing segment, in the given direction
 */
void RailNetwork::AddTerminator(ISegment* src, Direction d) {
    if(src->GetNext(d) != nullptr) {
        printf("ERROR Connecting terminator to connected segment");
    }

    IConnector* terminator = mComponentFactory.NewTerminator("DefaultTerminatorName");
    src->Connect(terminator, d);

    // Save the new terminator
    mTerminators.push_back(terminator);
}