#pragma once

#include <string>

#include <ncurses.h>

class window
{
public:
  WINDOW* win;

  int h, w, y, x;

  // clang-tidy suggestions
  window(const window&) = default;
  window(window&&) = delete;
  auto operator=(const window&) -> window& = default;
  auto operator=(window&&) -> window& = delete;

  window(int height, int width, int beg_y, int beg_x)
      : win(::newwin(height, width, beg_y, beg_x))
      , h(height)
      , w(width)
      , y(beg_y)
      , x(beg_x)
  {
    ::refresh();
  }

  void refresh() const
  {
    ::touchwin(win);
    ::wrefresh(win);
  }

  void keypad(bool v) const { ::keypad(win, v); }

  void box() const
  {
    ::box(win, 0, 0);
    this->refresh();
  }

  void set_size(int height, int width)
  {
    h = height;
    w = width;
    ::wresize(win, h, w);
    this->refresh();
  }

  void print(int pos_y, int pos_x, const std::string& str) const
  {
    ::mvwprintw(win, pos_y, pos_x, str.c_str());
    this->refresh();
  }

  void reverse_on() const { wattron(win, A_REVERSE); }

  void reverse_off() const { wattroff(win, A_REVERSE); }

  void close() const { delwin(win); }

  ~window() { ::delwin(win); }
};
