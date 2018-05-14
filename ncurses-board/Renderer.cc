#include "Renderer.h"

#include <ncurses.h>

#include "Column.h"
#include "Task.h"

Renderer::Renderer(Board* board)
: m_board(board) {

    init_pair(1, COLOR_RED, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);

    int x = 0;
    int w = 60;
    int h = 35;

    // create a window for every column in the board
    std::vector<Column*> columns = board->getColumns();

    for (auto c = columns.begin(); c != columns.end(); ++c) {
        std::vector<Task*> tasks = (*c)->getTasks();

        ColumnData cd;

        cd.border = newwin(h+2, w+2, 0, x);
        box(cd.border, 0 , 0);
        wrefresh(cd.border);

        cd.content = newwin(h, w, 1, x+1);
        wattron(cd.content, COLOR_PAIR(2));
        wattron(cd.content, A_BOLD);
        mvwprintw(cd.content, 0, 0, (*c)->title().c_str());
        wmove(cd.content, 2, 0);
        wattroff(cd.content, A_BOLD);
        wattroff(cd.content, COLOR_PAIR(2));

        x += (w + 2);

        for (auto t = tasks.begin(); t != tasks.end(); ++t) {
            wprintw(cd.content, (*t)->title().c_str());
            wprintw(cd.content, "\n");
        }

        wrefresh(cd.content);

        m_columns.push_back(cd);
    }
}

Renderer::~Renderer() {

}

void Renderer::render(Cursor const & cursor) {
    // what happens when you remove a column?
    // or remove a task?
    // renderer should detect the change
    // and apply the change
    // let's assume no change at the moment
}

