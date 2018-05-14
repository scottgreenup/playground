#ifndef _COMPONENT_H_
#define _COMPONENT_H_

namespace GE {

class Component  {
public:
    Component();

    /**
     * The component is meant to be inheritented into meaningful components
     */
    virtual ~Component();

    /**
     * Returns a mutable reference to the entity the Component is associated
     * with.
     */
     Entity& entity();

    /**
     * Sets the entity the Component will associated with.
     */
    void setEntity(Entity& entity);

protected:

    // the entity the component is associated with
    Entity& m_entity;
};

}

#endif
