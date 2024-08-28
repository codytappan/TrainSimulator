#ifndef RailComponents_H
#define RailComponents_H

namespace Rail {
    /**
     * A connector is a component of a rail network that links two or more other segments
     * so that a train can travel between them
     */
    class Connector {

    };

    /**
     * A signal is a component of the rail network which tells trains whether or not they can proceed across
     * an associated connector
     */
    class Signal {

    };

    /**
     * A segment is a length of track within the rail network
     */
    class Segment {

    };

    /**
     * A junction is a component of the rail network which allows a path to be switched to two different segments
     * 
     * This is a special case of a connector
     */
    class Junction : public Connector {

    };

    /**
     * A terminator is a component of the rail network which represents the destination of a train
     */
    class Terminator {

    };

} // namespace Rail

#endif
