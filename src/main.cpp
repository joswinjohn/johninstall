#include <ncurses.h>

#include "lib.hpp"

int main()
{
  initscr();

  int h, w;
  getmaxyx(stdscr, h, w);

  WINDOW* w_intro = newwin(h - 4, w - 4, 2, 2);
  refresh();

  box(w_intro, 0, 0);

  mvwprintw(w_intro, 0, 1, "Intro");
  mvwprintw(w_intro,
            2,
            4,
            "This is the guided JohnOS installer. to continute or move back "
            "use the \"Next\" and \"Back\" buttons");
  wrefresh(w_intro);

  getch();
  endwin();
  return 0;
}
