#pragma once

#include <utility>

#include <ncurses.h>

class curse
{
public:
  static std::pair<int, int> get_size()
  {
    int h;
    int w;

    getmaxyx(stdscr, h, w);
    return {h, w};
  }

  static void start_curses()
  {
    initscr();
    cbreak();
    curs_set(0);
    noecho();
  }

  static void stop_curses()
  {
    endwin();
  }
};
