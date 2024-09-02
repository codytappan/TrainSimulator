#ifndef IRailComponent_H
#define IRailComponent_H

#include <set>

#include "RailDefinitions.h"

namespace Rail {
    // A general class to represent all components in a network that a train can travel across
    class IComponent {
        public:
        /**
         * Called to get the name of the component, for debug and logging purposes
         */
        virtual std::string GetName() = 0;

        /**
         * Called to get info on the component, for debug and logging purposes
         */
        virtual std::string GetInfo() = 0;

        /**
         *  Called when a train wants to traverse the element in the given direction
         * 
         *  @param src The source element from which the train is arriving
         *  @param d The direction in which we are traversing the element
         *  @return const ITraversableElement& A reference to the element we end on after traversing one step
         */
        virtual const IComponent* Traverse(const IComponent* src, Direction d) = 0;
    };

    // Forward declaration of the Connector interface
    class IConnector;

    // Interface representing a segment of track in the RailNetwork
    class ISegment : public IComponent {
        /**
         *  Called to get the next connector(s) in a given direction, ignoring traversal rules
         *
         *  @param d The direction in which we are traversing the element
         *  @return A vector of components connected to this component
         */
        virtual IConnector* GetNext(Direction d) = 0;
    
        /**
         *  Called to connect to a connector in the network
         * 
         *  @param target The connector to connect to the end of the segment
         *  @param d The end of the segment to connect to
         */
        virtual void Connect(IConnector* target, Direction d) = 0;
    };

    // Interface representing connections between different segments within the RailNetwork
    class IConnector : public IComponent {
        /**
         *  Called to get the next segments(s), ignoring traversal rules
         *
         *  @param d The direction in which we are traversing the element
         *  @return src The source segment we are looking from
         */
        virtual std::set<ISegment*> GetNext(ISegment *src) = 0;

        /**
         *  Called to connect to a segment in the network
         *  @param target The Segment to connect to
         */
        virtual void Connect(ISegment* target) = 0;

        /**
         *  Called to select connected segments in a connector
         *  @param s1, s2  The segments to connect, which will be traversed when traveling across the connector
         */
        virtual void Select(ISegment* s1, ISegment* s2) = 0;

        /**
         *  Called to fix a segment in a connector. The connector must always connect to this segment
         */
        virtual void Fix(ISegment* target) = 0;
    };

    // Class interface to handle dependency injection of component types
    class IComponentFactory {
        /**
         * Create a new Segment
         */
        virtual ISegment* NewSegment(const std::string& name, unsigned int length) = 0;

        /**
         * Create a new Connector
         */
        virtual IConnector* NewConnector(const std::string& name) = 0;

        /**
         * Create a new Terminator
         */
        virtual IConnector* NewTerminator(const std::string& name) = 0;
    };
}

#endif
