#ifndef _COLUMN_H_
#define _COLUMN_H_

#include <string>
#include <vector>

class Column {
public:
    Column(std::string title);
    ~Column();


    void addTask(class Task *t);
    bool removeTask(class Task *t);

    std::vector<class Task*> getTasks();

    inline class Task* get(int i) {
        return m_tasks.at(i);
    }

    inline size_t size() {
        return m_tasks.size();
    }

    inline std::string title() {
        return m_title;
    }

    inline bool empty() {
        return m_tasks.size() == 0;
    }

private:
    std::string m_title;
    std::vector<class Task*> m_tasks;
};

#endif
