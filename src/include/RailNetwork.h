#ifndef RailNetwork_H
#define RailNetwork_H

#include "RailComponents.h"

#include <vector>

namespace Rail {
    class RailNetwork {
        public:
        RailNetwork(const IComponentFactory& f);
        virtual ~RailNetwork();

        /**
         *  Network building API
         */

        /**
         *  Create a new, unconnected Segment within the network
         * 
         *  @param length The legnth of segment to create.
         *  @return A handle to the new segment
         */
        ISegment* CreateSegment(unsigned int legnth);

        /**
         *  Create a segment and attach it to an existing segment, in the given direction
         * 
         *  @param src The existing segment to connect to.
         *  @param d The side of the existing segment to connect to.
         *  @param length The legnth of segment to create.
         *  @return A handle to the new segment
         */
        ISegment* AttachSegment(ISegment* src, Direction d, unsigned int length);

        /**
         *  Connect two existing segments within the network
         * 
         *  @param s1 A handle to first segment to connect
         *  @param d1 The side of the first segment to connect
         *  @param s2 A handle to second segment to connect
         *  @param d2 The side of the second segment to connect
         */
        void ConnectSegments(ISegment* s1, Direction d1, ISegment* s2, Direction d2);

        /**
         *  Add a signal to a segment in the given direction
         */
        void AddSignal(ISegment* segment, Direction d, SignalState state);

        /**
         *  Create a terminator and attach it to an existing segment, in the given direction
         */
        IConnector* AddTerminator(ISegment* src, Direction d);

        /**
         *  Network Traversal API
         */

        /**
         *  Switch the connector between the given segments so that trains traverse from src to dst
         */
        void RouteSegment(ISegment* src, ISegment* dst);

        /**
         *  Set the given signal to a specific state
         */
        void SetSignal(ISegment* segment, Direction d, SignalState state);

        private:
        const IComponentFactory& mComponentFactory;

        std::vector<ISegment*> mSegments;
        std::vector<IConnector*> mConnectors;
        std::vector<IConnector*> mTerminators;
    };

}


#endif
