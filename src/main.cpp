#include <utility>
#include <vector>

#include <ncurses.h>

#include "answer_box.hpp"
#include "curse.hpp"
#include "windows.hpp"

#define GRID_SPLIT_H 5
#define GRID_SPLIT_W 1

#define exit() \
  endwin(); \
  return 0;

int windows::h;
int windows::w;
window* windows::active = nullptr;

int main()
{
  initscr();
  curs_set(0);

  std::pair<int, int> size = curse::get_size();
  windows::h = (size.first - 4) / GRID_SPLIT_H;
  windows::w = (size.second - 4) / GRID_SPLIT_W;

  windows::start_window();
  answer_box w_ans;

  if (w_ans.selection({"Next", "Exit"}) == 1) {
    exit();
  }
  windows::partition_window_1();

  if (w_ans.selection({"Next", "Back"}) == 1) {
    exit();
  }

  getch();
  exit();
}
