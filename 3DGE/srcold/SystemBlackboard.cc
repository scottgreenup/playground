#include "SystemBlackboard.h"

using namespace GE;

SystemBlackboard::SystemBlackboard() {

}

SystemBlackboard::~SystemBlackboard() {
    m_entities.clear();
}

void SystemBlackboard::write(EntityId id) {
    m_entities.push_back(id);
}

const std::list<EntityId>& SystemBlackboard::read() {
    return m_entities;
}

void SystemBlackboard::wipe() {
    m_entities.clear();
}
