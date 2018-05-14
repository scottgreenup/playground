#ifndef _SYSTEM_BLACKBOARD_H_
#define _SYSTEM_BLACKBOARD_H_

#include "Entity.h"

#include <list>

namespace GE {

    class SystemBlackboard {
    public:
        SystemBlackboard();
        ~SystemBlackboard();

        /**
         * Writes the EntityId onto the blackboard for reading.
         */
        void write(EntityId id);

        /**
         * Reads the list of EntityIds that are on the board.
         */
        const std::list<EntityId>& read();

        /**
         * Wipes the board clean.
         */
        void wipe();

    private:
        std::list<EntityId> m_entities;

    };

}

#endif

/*

The system could hold a list of IDs. If it does, then it has to grab the actual
entity from a lookup table some where. Which would have to be fast and be able
to store a shit-ton of game objects. The ID should be an integer. The storage
could be a map.

The map would have to be shared between the Systems. What can delete an entity
from the map though? A system can't?
*/
