#include "RailNetwork.h"

using namespace Rail;

RailNetwork::RailNetwork(const IComponentFactory& f) :
    mComponentFactory(f), mSegments(), mConnectors(), mTerminators()
{

}

RailNetwork::~RailNetwork() {
    // TODO free the created components
}

/**
 *  Create a segment and attach it to an existing segment, in the given direction
 */
ISegment* RailNetwork::AddSegment(ISegment* src, Direction d, unsigned int length) {

    return nullptr;
}

/**
 *  Switch the connector between the given segments so that trains traverse from src to dst
 */
void RouteSegment(ISegment* src, ISegment* dst) {

}

/**
 *  Set the given signal to a specific state
 */
void SetSignal(ISegment* segment, Direction d, SignalState state) {

}

/**
 *  Create a terminator and attach it to an existing segment, in the given direction
 */
void AddTerminator(ISegment* src, Direction d) {

}