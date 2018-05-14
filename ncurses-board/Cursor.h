#ifndef _CURSOR_H_
#define _CURSOR_H_

/*
 * On the board screen a task is selected at any one time.
 *
 * On the task screen, a menu option is selected at any one time.
 *
 * This class has to have subclasses for behaviour.
 * - BoardCursor
 * - TaskCursor
 */

/*
   Now... one option is...
    1. A cursor holds the item that is selected. The renderer will alter that
       item.
    2. The cursor holds the board and the render uses the cursor.. no..

    The cursor should maintain a pointer to the board.
    The cursor should tell an item what colour it is.
    An item should have a colour. A selectable should have a colour.
 */

#include "Board.h"
#include "Column.h"

class Cursor {
public:
    Cursor() {}
    virtual ~Cursor() {}

private:
};

class BoardCursor : public Cursor {
public:
    BoardCursor(Board *board)
    : m_board(board)
    , m_column(0)
    , m_task(0) {

    }

    ~BoardCursor() {
        m_board = nullptr;
    }

    void keyEvent(int ch) {
        switch(ch) {
        case 'h':
            moveLeft();
            break;
        case 'j':
            moveDown();
            break;
        case 'k':
            moveUp();
            break;
        case 'l':
            moveRight();
            break;
        };
    }

private:

    void moveLeft() {
        if (m_column > 0) {
            m_column--;
        }

        size_t task_size = m_board->get(m_column)->size();

        if (m_task > task_size
        && task_size != 0) {
            m_task = task_size - 1;
        }
    }

    void moveRight() {
        if (m_column < m_board->size() - 1) {
            m_column++;
        }
    }

    void moveUp() {
        size_t task_size = m_board->get(m_column)->size();

        if (m_task > 0) {
            m_task--;
        }
    }

    void moveDown() {
        size_t task_size = m_board->get(m_column)->size();

        if (m_task < task_size + 1) {
            m_task++;
        }
    }


    Board *m_board;

    int m_column;
    int m_task;

};

#endif
