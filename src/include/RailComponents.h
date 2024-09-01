#ifndef RailComponents_H
#define RailComponents_H

#include "interfaces/IRailComponent.h"

#include <string>
#include <set>
#include <map>


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
        Connector(const std::string& name);
        virtual ~Connector();

        /**
         *   Interface Implementations
         */

        // IComponent
        virtual std::string GetName();
        virtual std::string GetInfo();
        virtual const IComponent* Traverse(const IComponent* src, Direction d);

        // IConnector
        virtual std::set<ISegment*> GetNext(Direction d);
        virtual void Connect(ISegment* target, Direction d);
        virtual void Select(ISegment* target);

        private:
        std::string mName = "";

        std::set<ISegment*> mAvailableSegments;
        std::map<Direction, ISegment*> mConnectedSegmentMap;
    };

    /**
     * A terminator is a component of the rail network which represents the destination of a train
     */
    class Terminator : public IConnector {
        public:
        Terminator(const std::string& name);
        virtual ~Terminator();

        /**
         *   Interface Implementations
         */

        // IComponent
        virtual std::string GetName();
        virtual std::string GetInfo();
        virtual const IComponent* Traverse(const IComponent* src, Direction d);


        private:
        std::string mName = "";
    };

    /**
     * A segment is a length of track within the rail network
     */
    class Segment : public ISegment {
        public:
        Segment(unsigned int length);
        Segment(const std::string& name, unsigned int length);
        virtual ~Segment();
        
        unsigned int GetLength() {
            return mLength;
        }

        // Signal Logic
        void AddSignal(Direction d);
        SignalState GetSignalState(Direction d);
        void SetSignalState(SignalState state, Direction d);


        /**
         *   Interface Implementations
         */

        // IComponent
        virtual std::string GetName();
        virtual std::string GetInfo();
        virtual const IComponent* Traverse(const IComponent* src, Direction d);

        // ISegment
        virtual IConnector* GetNext(Direction d);
        virtual void Connect(IConnector* target, Direction d);

        private:
        std::string mName = "";
        unsigned int mLength = 0;

        std::map<Direction, IConnector*> mConnectorMap;
        std::map<Direction, Signal> mSignalMap;
    };

} // namespace Rail

#endif
