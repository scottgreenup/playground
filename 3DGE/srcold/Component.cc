#include "Component.h"

#include "Entity.h"

using namespace GE;

Component::Component()
{

}

Component::~Component()
{

}

Entity& Component::entity()
{
    return m_entity;
}

void Component::setEntity(Entity& entity)
{
    m_entity = entity;
}
