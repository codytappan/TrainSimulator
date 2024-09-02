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
         *  Create a segment and attach it to an existing segment, in the given direction
         */
        ISegment* AddSegment(ISegment* src, Direction d, unsigned int length);

        /**
         *  Switch the connector between the given segments so that trains traverse from src to dst
         */
        void RouteSegment(ISegment* src, ISegment* dst);

        /**
         *  Set the given signal to a specific state
         */
        void SetSignal(ISegment* segment, Direction d, SignalState state);

        /**
         *  Create a terminator and attach it to an existing segment, in the given direction
         */
        void AddTerminator(ISegment* src, Direction d);

        private:
        const IComponentFactory& mComponentFactory;

        std::vector<ISegment*> mSegments;
        std::vector<IConnector*> mConnectors;
        std::vector<IConnector*> mTerminators;
    };

}


#endif
