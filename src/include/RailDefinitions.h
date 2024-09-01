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

    /**
     * Helper function, to reverse rail direction
     */
    inline Direction ReverseDirection(Direction d) {
        return (d == UP) ? DOWN : UP;
    }

    /**
     * Helper function to format a direction for printing
     */
    inline const char * PrintDirection(Direction d)
    {
        switch (d) {
            case Direction::UNKNOWN:
                return "Unknown";
            case Direction::UP:
                return "Up";
            case Direction::DOWN:
                return "Down";
            default:
                return "Unexpected Direction";
         }
    }

    typedef enum SignalState {
        UNKNOWN = 0,
        RED,
        GREEN,
        DISABLED
    };

    /**
     * Helper function to format a signal state for printing
     */
    inline const char * PrintDirection(SignalState d)
    {
        switch (d) {
            case SignalState::UNKNOWN:
                return "Unknown";
            case SignalState::RED:
                return "Red";
            case SignalState::GREEN:
                return "Green";
            case SignalState::DISABLED:
                return "Disabled";
            default:
                return "Unexpected Signal State";
         }
    }
}

#endif
