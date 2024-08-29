#ifndef RailNetwork_H
#define RailNetwork_H

#include "interfaces/IRailNetwork.h"

#include "RailComponents.h"

namespace Rail {

    class RailNetwork : public IRailNetwork {
        public:
        RailNetwork(IComponentFactory& ComponentFactory);
        virtual ~RailNetwork();

        private:
        std::vector<ISegment*> mSegments;
        std::vector<IConnector*> mConnectors;
        std::vector<ITerminator*> mTerminators;

        IComponentFactory& mComponentFactory;
    };

    class ComponentFactory : public IComponentFactory {
        public:
        ComponentFactory() {};
        virtual ~ComponentFactory() {};

        virtual ISegment* NewSegment(int length) const {
            return new Segment(length);
        }
        virtual IConnector NewConnector() const {
            return new Connector();
        };
        virtual ITerminator NewTerminator() const {
            return new Terminator();
        };
    }

}


#endif
