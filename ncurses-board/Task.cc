
#include "Task.h"

Task::Task(std::string title, std::string desc)
: m_title(title)
, m_desc(desc) {

}

Task::~Task() {

}

std::string Task::title() {
    return m_title;
}

std::string Task::desc() {
    return m_desc;
}

/*
void Task::addElement(TaskElement* e) {
    m_elements.push_back(e);
}
*/
