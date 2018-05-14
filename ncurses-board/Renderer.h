#ifndef _RENDERER_H_
#define _RENDERER_H_

#include <ncurses.h>
#include <vector>

#include "Board.h"
#include "Cursor.h"

class Renderer {
public:
    Renderer(Board* board);
    ~Renderer();

    void render(Cursor const & cursor);

private:
    struct ColumnData {
        WINDOW* border;
        WINDOW* content;
    };

    Board* m_board;

    std::vector<ColumnData> m_columns;
};

#endif

