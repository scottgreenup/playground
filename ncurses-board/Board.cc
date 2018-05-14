#include "Board.h"

#include "Column.h"

Board::Board() {

}

Board::~Board() {
    for (auto it = m_columns.begin(); it != m_columns.end(); ++it) {
        delete *it;
    }
    m_columns.clear();
}

void Board::addColumn(Column *c) {
    m_columns.push_back(c);
}

bool Board::removeColumn(Column *c) {
    for (auto it = m_columns.begin(); it != m_columns.end(); ++it) {
        if ((*it) == c) {
            m_columns.erase(it);
            return true;
        }
    }
    return false;
}

std::vector<class Column*> Board::getColumns() {
    return m_columns;
}

Column* Board::get(int i) {
    return m_columns.at(i);
}
