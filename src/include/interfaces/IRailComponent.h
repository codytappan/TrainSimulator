#ifndef IRailComponent_H
#define IRailComponent_H

#include <vector>

namespace Rail {
    /**
     * Rail direction is an arbitrary conctept to outline the direction of travel within the network
     * 
     * Traditionally "Up" refers to "towards city center" and "Down" is the opposite
     */
    typedef enum Direction {
        UNKNOWN = 0,
        UP,
        DOWN
    };

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
}

#endif
