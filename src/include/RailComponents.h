#ifndef RailComponents_H
#define RailComponents_H

#include <vector>

#include "interfaces/IRailComponent.h"

namespace Rail {
    /**
     * A signal is a component of the rail network which tells trains whether or not they can proceed across
     * an associated connector
     */
    class Signal {
        public:
        Signal();
        ~Signal();

        SignalState GetState() {
            return mState;
        }

        void SetState(SignalState state) {
            mState = state;
        }

        private:
        SignalState mState = SignalState::UNKNOWN;
    };

    /**
     * A connector is an element in the rail network which connects one or more segments of track
     * 
     * This class represents both direct segment links and junctions
     */
    class Connector : public IConnector {
        public:
        Connector();
        virtual ~Connector();

        // IComponent
        virtual std::vector<const IComponent&> GetNext(Direction d);
        virtual const IComponent& Traverse(Direction d);

        private:
        std::vector<const ISegment&> mConnectedSegments;
        unsigned int mUpSegmentIndex = -1;
    };

    /**
     * A segment is a length of track within the rail network
     */
    class Segment : public ISegment {
        public:
        Segment(int length);
        ~Segment();

        // IComponent
        virtual std::vector<const IComponent&> GetNext(Direction d);
        virtual const IComponent& Traverse(Direction d);
        
        // ISegment
        virtual unsigned int GetLength() {
            return mLength;
        }

        virtual void AddSignal(Direction d);
        virtual SignalState GetSignalState(Direction d);
        virtual void SetSignalState(SignalState state, Direction d);

        private:
        unsigned int mLength = 0;

        Connector &mUpConnector;
        Signal &mUpSignal;

        Connector &mDownConnector;
        Signal &mDownSignal;
    };

    /**
     * A terminator is a component of the rail network which represents the destination of a train
     */
    class Terminator : public ITerminator {
        public:
        Terminator();
        virtual ~Terminator();

        // IComponent
        virtual std::vector<const IComponent&> GetNext(Direction d);
        virtual const IComponent& Traverse(Direction d);
    };

} // namespace Rail

#endif
