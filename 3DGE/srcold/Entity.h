#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <vector>

#include "Component.h"

namespace GE {

typedef unsigned long long EntityId;

class Entity {
public:
    Entity();
    ~Entity();

    void addComponent(class Component* component);

    template<class T>
    T* getComponent()
    {
        for (auto it = m_components.begin(); it != m_components.end(); ++it) {
            T* t = dynamic_cast<T*>(*it);
            if (t != nullptr) {
                return t;
            }
        }
        return nullptr;
    }

    template<class T>
    bool hasComponent()
    {
        return getComponent<T>() != nullptr;
    }

protected:
    std::vector<class Component*> m_components;

    EntityId m_id;

private:
    static EntityId m_counter;
};

} // namespace GE

#endif //_ENTITY_H_
