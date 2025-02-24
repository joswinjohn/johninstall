#include <array>
#include <string>
#include <vector>

#include <ncurses.h>

#include "window.hpp"

#define KEY_RETURN 10
#define BUF_SIZE 128

class answer_box : window
{
public:
  answer_box(int h, int w)
      : window((h * 2) + 2, w, (h * 3) + 2, 2)
  {
    box();
    keypad(/*v=*/true);
  }

  void redraw_box()
  {
    werase(window::win);
    window::box();
  }

  WINDOW* get_window() { return window::win; }

  int selection(const std::vector<std::string>& choices)
  {
    int key = 0;
    int highlighted = 0;

    int choices_size = static_cast<int>(choices.size());

    while (true) {
      for (int i = 0; i < choices_size; i++) {
        if (i == highlighted) {
          reverse_on();
        }
        print(i + 1, 1, choices.at(i));
        reverse_off();
      }
      key = wget();
      switch (key) {
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
      // placed outside of switch statement to break loop
      if (key == KEY_RETURN) {
        break;
      }
    }

    return highlighted;
  }

  std::vector<std::string> selection_input(
      const std::vector<std::string>& choices)
  {
    int key = 0;
    int highlighted = 0;

    const int choices_size = static_cast<int>(choices.size());
    std::vector<std::string> rets(choices_size, "       ");

    while (true) {
      // fields
      for (int i = 0; i < choices_size - 2; i++) {
        print(i + 1, 1, choices.at(i));
        if (i == highlighted) {
          reverse_on();
        }
        print(i + 1, static_cast<int>(choices.at(i).length()) + 1, rets.at(i));
        reverse_off();
      }

      // "Next" and "Back" buttons
      if (choices_size - 2 == highlighted) {
        reverse_on();
      }
      print(choices_size - 1, 1, "Next");
      reverse_off();
      if (choices_size - 1 == highlighted) {
        reverse_on();
      }
      print(choices_size, 1, "Back");
      reverse_off();

      // get inputs
      key = wget();
      switch (key) {
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
      if (key == KEY_RETURN) {
        // If the "Back" option is hit return empty vector
        if (highlighted == choices_size - 1) {
          return {};
        }
        // If the "Next" option is hit return rets vector
        if (highlighted == choices_size - 2) {
          return rets;
        }
        // Otherwise listen for input
        char buffer[BUF_SIZE];
        curs_set(1);
        mvwgetstr(window::win,
                  highlighted + 1,
                  choices.at(highlighted).length() + 2,
                  buffer);
        rets.emplace_back(buffer);
        curs_set(1);
      }
    }
  }
};
