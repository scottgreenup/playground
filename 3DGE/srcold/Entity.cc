#include "Entity.h"
using namespace GE;

EntityId Entity::m_counter = 0;

Entity::Entity()
: m_id(m_counter++)
{

}

Entity::~Entity()
{
    for (auto it = m_components.begin(); it != m_components.end(); ++it) {
        delete *it;
    }
    m_components.clear();
}

void Entity::addComponent(Component* component)
{
    m_components.push_back(component);
}


