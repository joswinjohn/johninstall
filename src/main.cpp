#include <string>
#include <vector>

#include <ncurses.h>

#include "lib.hpp"

#define GRID_SPLIT 5

int main()
{
  initscr();
  curs_set(0);

  int h, w;
  getmaxyx(stdscr, h, w);
  int ht = (h - 4) / GRID_SPLIT;

  WINDOW* w_intro = newwin(ht * 4, w - 4, 2, 2);
  WINDOW* w_ans = newwin(ht, w - 4, (ht * 4) + 2, 2);
  refresh();

  // top windows
  box(w_intro, 0, 0);

  mvwprintw(w_intro, 0, 1, "Intro");
  mvwprintw(w_intro,
            2,
            4,
            "This is the guided JohnOS installer. to continute or move back "
            "use the \"Next\" and \"Back\" buttons");
  wrefresh(w_intro);

  // bottom choices
  box(w_ans, 0, 0);
  wrefresh(w_ans);

  keypad(w_ans, true);

  std::vector<std::string> choices = {"Exit", "Next"};
  int choice;
  int highlighted = 0;

  size_t choices_size = choices.size();

  while (true) {
    for (int i = 0; i < choices_size; i++) {
      if (i == highlighted) {
        wattron(w_ans, A_REVERSE);
      }
      mvwprintw(w_ans, i + 1, 1, choices.at(i).c_str());
      wattroff(w_ans, A_REVERSE);
    }
    choice = wgetch(w_ans);
    switch (choice) {
      case KEY_UP:
        highlighted--;
        if (highlighted == -1) {
          highlighted = choices_size - 1;
        }
        break;
      case KEY_DOWN:
        highlighted++;
        if (highlighted == static_cast<int>(choices_size)) {
          highlighted = 0;
        }
        break;
      default:
        break;
    }
    if (choice == 10) {
      break;
    }
  }

  mvwprintw(w_intro, 8, 4, choices[highlighted].c_str());
  wrefresh(w_intro);

  getch();
  endwin();
  return 0;
}
