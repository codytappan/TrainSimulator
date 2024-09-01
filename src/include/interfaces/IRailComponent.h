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

    class IConnector : public IComponent {
        /**
         *  Called to get the next segments(s) in a given direction, ignoring traversal rules
         *
         *  @param d The direction in which we are traversing the element
         *  @return A vector of components connected to this component
         */
        virtual std::set<ISegment*> GetNext(Direction d) = 0;

        /**
         *  Called to connect to a segment in the network
         */
        virtual void Connect(ISegment* target, Direction d) = 0;

        /**
         *  Called to select between connected segments in a network
         */
        virtual void Select(ISegment* target) = 0;
    };

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
         */
        virtual void Connect(IConnector* target, Direction d) = 0;
    };
}

#endif
