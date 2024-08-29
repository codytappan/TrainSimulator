#ifndef IRailComponent_H
#define IRailComponent_H

#include <vector>

#include "RailDefinitions.h"

namespace Rail {
    // A general class to represent all components in a network that a train can travel across
    class IComponent {
        /**
         *  Called to get the next component(s) in a given direction, ignoring traversal rules
         *
         *  @param d The direction in which we are traversing the element
         *  @return const ITraversableElement& A reference to the element we end on after traversing one step
         */
        virtual std::vector<const IComponent&> GetNext(Direction d) = 0;

        /**
         *  Called to traverse the element in the given direction
         * 
         *  @param d The direction in which we are traversing the element
         *  @return const ITraversableElement& A reference to the element we end on after traversing one step
         */
        virtual const IComponent& Traverse(Direction d) = 0;
    };

    // An interface which represents a segment in the train network
    class ISegment : public IComponent {
        /**
         * Called to get the length of a given segment
         */
        virtual unsigned int GetLength() = 0;

        /**
         * Get the signal state of a segment in the given direction
         */
        virtual SignalState GetSignalState(Direction d) = 0;
    };

    // An interface which represents connectors in the train network
    class IConnector : public IComponent {};

    // An interface which represents terminators in the train network
    class ITerminator : public IConnector {};
}

#endif
