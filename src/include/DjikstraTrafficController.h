#ifndef DjikstraTrafficController_H
#define DjikstraTrafficController_H

#include "interfaces/ITrafficController.h"

namespace Traffic {
    
    class DjikstraController : public ITrafficController {
        public:
        DjikstraController();
        virtual ~DjikstraController();

        // ITrafficController
        virtual void UpdateRailNetwork(Rail::RailNetwork& network, const std::vector<Train::Train *>& trains);
    };

}

#endif