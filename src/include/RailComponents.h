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
        Signal() {};
        ~Signal() {};

        SignalState GetState() const {
            return mState;
        }

        void SetState(SignalState state) {
            mState = state;
        }

        private:
        SignalState mState = SignalState::DISABLED;
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
        virtual const char * const GetName() const {
            return mName.c_str();
        }

        virtual const char * const GetInfo() const;

        virtual unsigned int GetLength() const {
            return 0;
        }

        virtual const IComponent* Traverse(const IComponent* src, Direction d) const;

        // IConnector
        virtual std::set<ISegment*> GetNext(ISegment* src);
        virtual void Connect(ISegment* target);
        virtual void Select(ISegment* s1, ISegment* s2);
        virtual void Fix(ISegment* src);

        private:
        std::string mName = "";

        std::set<ISegment*> mAvailableSegments;
        std::pair<ISegment*, ISegment*> mSelectedSegments;
    };

    /**
     * A segment is a length of track within the rail network
     */
    class Segment : public ISegment {
        public:
        Segment(unsigned int length);
        Segment(const std::string& name, unsigned int length);
        virtual ~Segment();

        /**
         *   Interface Implementations
         */

        // IComponent
        virtual const char * const GetName() const {
            return mName.c_str();
        }

        virtual const char * const GetInfo() const;

        virtual unsigned int GetLength() const {
            return mLength;
        }

        virtual const IComponent* Traverse(const IComponent* src, Direction d) const;

        // ISegment
        virtual void AddSignal(Direction d);
        virtual SignalState GetSignalState(Direction d) const;
        virtual void SetSignalState(SignalState state, Direction d);
        virtual IConnector* GetNext(Direction d);
        virtual void Connect(IConnector* target, Direction d);

        private:
        std::string mName = "";
        unsigned int mLength = 0;

        std::map<Direction, IConnector*> mConnectorMap;
        std::map<Direction, Signal> mSignalMap;
    };

    /**
     * A terminator is a component of the rail network which represents the destination of a train
     */
    class Terminator : public Connector {
        public:
        Terminator(const std::string& name);
        virtual ~Terminator();

        /**
         *   Interface Implementations
         */

        virtual const IComponent* Traverse(const IComponent* src, Direction d) const override;
    };

    class ComponentFactory : public IComponentFactory {
        public: 
        
        ComponentFactory() {}
        virtual ~ComponentFactory() {}

        /**
         * Create a new Segment
         */
        virtual ISegment* NewSegment(const std::string& name, unsigned int length) const {
            return new Segment(name, length);
        }

        /**
         * Create a new Connector
         */
        virtual IConnector* NewConnector(const std::string& name) const {
            return new Connector(name);
        }

        /**
         * Create a new Terminator
         */
        virtual IConnector* NewTerminator(const std::string& name) const {
            return new Terminator(name);
        }
    };

} // namespace Rail

#endif
