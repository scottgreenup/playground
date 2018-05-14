#ifndef _TASK_H_
#define _TASK_H_

#include <string>
#include <vector>

class Task {
public:
    Task(std::string title, std::string desc);
    ~Task();

    //void addElement(TaskElement* e);

    std::string title();
    std::string desc();

private:
    std::string m_title;
    std::string m_desc;

    //std::vector<TaskElement> m_elements;
};

#endif

