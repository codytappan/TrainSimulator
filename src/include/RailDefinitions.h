#ifndef RailDefinitions_H
#define RailDefinitions_H

namespace Rail {
    /**
     * Rail direction is an arbitrary conctept to outline the direction of travel within the network
     * Traditionally "Up" refers to "towards city center" and "Down" is the opposite
     */
    typedef enum Direction {
        UNKNOWN = 0,
        UP,
        DOWN
    };

    typedef enum SignalState {
        UNKNOWN = 0,
        RED,
        GREEN,
        DISABLED
    };
}

#endif
