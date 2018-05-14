
#include "Column.h"

#include "Task.h"

Column::Column(std::string title)
: m_title(title) {

}

Column::~Column() {
    for (auto it = m_tasks.begin(); it != m_tasks.end(); ++it) {
        delete *it;
    }
    m_tasks.clear();
}

void Column::addTask(Task *t) {
    m_tasks.push_back(t);
}

bool Column::removeTask(Task *t) {
    for (auto it = m_tasks.begin(); it != m_tasks.end(); ++it) {
        if ((*it) == t) {
            m_tasks.erase(it);
            return true;
        }
    }
    return false;
}

std::vector<Task*> Column::getTasks() {
    return m_tasks;
}
