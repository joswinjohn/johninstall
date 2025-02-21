#include <utility>
#include <vector>

#include <ncurses.h>

#include "curse.hpp"
#include "lib.hpp"
#include "window.hpp"

#define GRID_SPLIT_H 5
#define GRID_SPLIT_W 1

int main()
{
  initscr();
  curs_set(0);

  std::pair<int, int> size = curse::get_size();
  int seg_h = (size.first - 4) / GRID_SPLIT_H;
  int seg_w = (size.second - 4) / GRID_SPLIT_W;

  window win(seg_h * 4, seg_w, 2, 2);
  win.box();
  win.print(2,
            4,
            "This is the guided JohnOS installer. to continute or move back "
            "use the \"Next\" and \"Back\" buttons");

  window w_ans(seg_h, seg_w, (seg_h * 4) + 2, 2);
  w_ans.box();
  w_ans.keypad(true);

  std::vector<std::string> choices = {"Exit", "Next"};
  int choice;
  int highlighted = 0;

  int choices_size = static_cast<int>(choices.size());

  while (true) {
    for (int i = 0; i < choices_size; i++) {
      if (i == highlighted) {
        w_ans.reverse_on();
      }
      w_ans.print(i + 1, 1, choices.at(i));
      w_ans.reverse_off();
    }
    choice = w_ans.wget();
    switch (choice) {
      case KEY_UP:
        highlighted--;
        if (highlighted == -1) {
          highlighted = choices_size - 1;
        }
        break;
      case KEY_DOWN:
        highlighted++;
        if (highlighted == choices_size) {
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

  win.print(8, 4, choices[highlighted]);

  getch();
  endwin();
  return 0;
}
