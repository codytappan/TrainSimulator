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
        virtual const char * const GetName() const = 0;

        /**
         * Called to get info on the component, for debug and logging purposes
         */
        virtual const char * const GetInfo() const = 0;

        /**
         *  Get the length of the component
         */
        virtual unsigned int GetLength() const = 0;

        /**
         *  Called when a train wants to traverse to the next Component in the given direction
         * 
         *  @param src The source element from which the train is arriving
         *  @param d The direction in which we are traversing the element
         *  @return const IComponent* A pointer to the element we end on after traversing
         * 
         *  @note If the Traverse is blocked the src component will be returned to indicate
         *        no traversal ocurred
         */
        virtual const IComponent* Traverse(const IComponent* src, Direction d) const = 0;
    };

    // Forward declaration of the Connector interface
    class IConnector;

    // Interface representing a segment of track in the RailNetwork
    class ISegment : public IComponent {
        public:

        /**
         *  Add a signal to the segment, before the connector in the given direction
         * 
         *  @param d The side of the track to add the signal to
         *  @note The default signal state will be RED for safety sake
         */
        virtual void AddSignal(Direction d) = 0;

        /**
         *  Gets the current state of the signal in the given direction
         * 
         *  @return SignalState If the segment contains a signal in the given direction return RED or GREEN,
         *                      otherwise DISABLED is returned
         */
        virtual SignalState GetSignalState(Direction d) const = 0;

        /**
         *  Set the state of the signal in the given direction
         * 
         *  @param state The state to set
         *  @param d The side of the track on which to set the signal
         */
        virtual void SetSignalState(SignalState state, Direction d) = 0;

        /**
         *  Called to get the next connector(s) in a given direction, ignoring traversal rules
         *
         *  @param d The direction in which we are traversing the element
         *  @return A pointer to the connector in the given direction on this segment, or nullptr if not connected
         */
        virtual IConnector* GetNext(Direction d) const = 0;
    
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
        public:
        /**
         *  Called to get the next segments(s), ignoring traversal rules
         *
         *  @param d The direction in which we are traversing the element
         *  @return src The source segment we are looking from
         */
        virtual std::set<const ISegment*> GetNext(const ISegment *src) = 0;

        /**
         *  Called to connect to a segment in the network
         *  @param target The Segment to connect to
         */
        virtual void Connect(ISegment* target) = 0;

        /**
         *  Called to select connected segments in a connector
         *  @param s1, s2  The segments to connect, which will be traversed when traveling across the connector
         *  @return true if the elements could be routed, false otherwise
         */
        virtual bool Select(const ISegment* s1, const ISegment* s2) = 0;

        /**
         *  Called to fix a segment in a connector. The connector must always connect to this segment
         */
        virtual void Fix(ISegment* target) = 0;
    };

    // Class interface to handle dependency injection of component types
    class IComponentFactory {
        public:
        /**
         * Create a new Segment
         */
        virtual ISegment* NewSegment(const std::string& name, unsigned int length) const = 0;

        /**
         * Create a new Connector
         */
        virtual IConnector* NewConnector(const std::string& name) const = 0;

        /**
         * Create a new Terminator
         */
        virtual IConnector* NewTerminator(const std::string& name) const = 0;
    };
}

#endif
