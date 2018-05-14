#ifndef _BOARD_H_
#define _BOARD_H_

#include <vector>

class Board {
public:
    Board();
    ~Board();

    inline bool empty() {
        return m_columns.size() == 0;
    }

    inline int size() {
        return m_columns.size();
    }

    void addColumn(class Column* c);
    bool removeColumn(class Column *c);
    std::vector<class Column*> getColumns();
    class Column* get(int i);


private:
    std::vector<class Column*> m_columns;
};

#endif
