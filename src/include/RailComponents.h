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

        typedef enum State {
            UNKNOWN = 0,
            RED,
            GREEN,
            DISABLED
        };

        State GetState() {
            return mState;
        }

        void SetState(State state) {
            mState = state;
        }

        private:
        State mState = State::UNKNOWN;
    };

    /**
     * A junction is a connector in the rail network which allows a path to be switched to two different segments
     * 
     * This is a special case of a connector
     */
    class Connector : public IComponent {
        public:
        Connector();
        virtual ~Connector();

        private:
        std::vector<const Segment&> mConnectedSegments;
        unsigned int mUpSegmentIndex = -1;
    };

    /**
     * A segment is a length of track within the rail network
     */
    class Segment : public IComponent {
        public:
        Segment(int length);
        ~Segment();

        unsigned int GetLength() {
            return mLength;
        }

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
    class Terminator : public IComponent {
        public:
        Terminator();
        virtual ~Terminator();
    };

} // namespace Rail

#endif
