#ifndef IRailNetwork_H
#define IRailNetwork_H

#include "IRailComponent.h"

namespace Rail {

    class IRailNetwork {
        public:
        IRailNetwork();
        virtual ~IRailNetwork();

        /**
         *  Creates a new segment and connects it to an existing segment
         * 
         *  @param src A reference to the existing segment to connect to
         *  @param d The direction in which to connect the segment (up side or down side)
         * 
         *  @return A reference to the created segment
         */
        virtual ISegment& AddSegment(ISegment& src, Direction d) = 0;


        /**
         *  Creates a new terminator and connects it to an existing segment
         * 
         *  @param src A reference to the existing segment to connect to
         *  @param d The direction in which to connect the segment (up side or down side)
         * 
         *  @return A reference to the created terminator
         */
        virtual const ITerminator& AddTerminator(ISegment& src, Direction d) = 0;
    };

}

#endif
