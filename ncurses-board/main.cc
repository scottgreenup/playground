
#include <ncurses.h>

#include "Board.h"
#include "Column.h"
#include "Cursor.h"
#include "Task.h"
#include "Renderer.h"

int main() {

    Board board;

    Column* colA = new Column("Foo");
    colA->addTask(new Task("Do everything.", "This is a task related to doing everything."));
    colA->addTask(new Task("Do nothing.", "This is a task related to doing everything."));
    board.addColumn(colA);

    Column* colB = new Column("Bar");
    colB->addTask(new Task("Do something.", "This is a task related to doing something."));
    colB->addTask(new Task("Do that thing.", "This is a task related to doing that thing."));
    board.addColumn(colB);

    // start curses mode
    initscr();

    // disable line buffering
    cbreak();

    // enable the keyboard
    keypad(stdscr, TRUE);

    start_color();

    // don't echo user input
    noecho();
    refresh();

    Renderer renderer(&board);

    int ch = 0;

    Cursor cursor;

    while((ch = getch()) != 'q') {
        renderer.render(cursor);
    }

    // leave curses mode
    endwin();

    return 0;
}

