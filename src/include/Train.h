#ifndef Train_H
#define Train_H

#include "interfaces/IRailComponent.h"

namespace Train {
    class Train {
        public:
        Train();
        ~Train();

        /**
         *  Commands the train to move one position further down the track
         */
        void Move();

        private:
        Rail::ISegment& mCurrentSegment;
        Rail::Direction mCurrentDirection = Rail::Direction::UNKNOWN;
        Rail::ITerminator& mDestination;

        int mSegmentIndex = -1;
    };
}

#endif
