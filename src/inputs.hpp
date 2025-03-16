#include <array>
#include <cstring>
#include <string>
#include <vector>

#include <ncurses.h>

#include "logger.hpp"
#include "window.hpp"

#define KEY_RETURN 10
#define KEY_ESC 27
#define KEY_BACKSPACE_I 127

#define BUF_SIZE 128
#define MAX_INPUT 20

#define COL_WIDTH 20

class inputs : window
{
public:
  inputs(int h, int w)
      : window((h * 2) + 2, w, (h * 3) + 2, 2)
  {
    box();
    keypad(/*v=*/true);
  }

  void print(int pos_y, int pos_x, const std::string& str)
  {
    window::print(pos_y, pos_x, str);
  }

  void redraw_box()
  {
    werase(window::win);
    window::box();
  }

  WINDOW* get_window() { return window::win; }

  int select_input(const std::vector<std::string>& choices)
  {
    int key = 0;
    int highlighted = 0;

    int choices_size = static_cast<int>(choices.size());

    while (true) {
      int n = 0;
      int cols = w / COL_WIDTH;

      // prev
      for (int i = 0; i < cols; i++) {
        for (int j = 1; j <= h - 2 && n < choices_size; j++) {
          if (n == highlighted) {
            reverse_on();
          }
          print(j, 1 + (i * COL_WIDTH), choices.at(n));
          reverse_off();
          n++;
        }
      }
      key = wgetch(win);
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

  std::vector<std::string> field_input(const std::vector<std::string>& choices)
  {
    int key = 0;
    int highlighted = 0;
    std::string whitespace(MAX_INPUT, ' ');

    const int choices_size = static_cast<int>(choices.size()) - 2;
    std::vector<std::string> rets(choices_size, "");

    while (true) {
      // fields
      for (int i = 0; i < choices_size; i++) {
        print(i + 1, 1, choices.at(i));
        if (i == highlighted) {
          reverse_on();
        }
        print(i + 1,
              static_cast<int>(choices.at(i).length()) + 2,
              rets.at(i) + whitespace.substr(rets.at(i).size()));
        reverse_off();
      }

      // "Next" and "Back" buttons
      if (choices_size == highlighted) {
        reverse_on();
      }
      print(choices_size + 1, 1, "Next");
      reverse_off();
      if (choices_size + 1 == highlighted) {
        reverse_on();
      }
      print(choices_size + 2, 1, "Back");
      reverse_off();

      // get inputs
      key = wgetch(win);
      switch (key) {
        case KEY_UP:
          highlighted--;
          if (highlighted == -1) {
            highlighted = choices_size + 1;
          }
          break;
        case KEY_DOWN:
          highlighted++;
          if (highlighted == choices_size + 2) {
            highlighted = 0;
          }
          break;
        default:
          break;
      }
      if (key == KEY_RETURN) {
        // If the "Back" option is hit return empty vector
        if (highlighted == choices_size + 1) {
          return {};
        }
        // If the "Next" option is hit return rets vector
        if (highlighted == choices_size) {
          if (rets.at(0).empty() || rets.at(1).empty()) {
            print(choices_size + 4, 1, "Input a username and password");
            continue;
          }
          return rets;
        }
        // Otherwise listen for input to write to field
        char buffer[BUF_SIZE];
        for (int i = 0; i < rets.at(highlighted).size(); i++) {
          buffer[i] = rets.at(highlighted).at(i);
        }

        int ch;
        int i = rets.at(highlighted).size();

        move(highlighted + 1, choices.at(highlighted).length() + 2);

        while ((ch = wgetch(win)) != '\n') {
          if (ch == KEY_ESC) {
            break;
          }
          if (ch == KEY_BACKSPACE || ch == KEY_BACKSPACE_I) {
            if (i > 0) {
              buffer[--i] = '\0';
              reverse_on();
              print(highlighted + 1,
                    choices.at(highlighted).length() + 2,
                    buffer + std::string(" "));
              reverse_off();
            }
          } else if (i < sizeof(buffer) - 1) {
            buffer[i++] = ch;
            buffer[i] = '\0';
            reverse_on();
            print(
                highlighted + 1, choices.at(highlighted).length() + 2, buffer);
            reverse_off();
          }
        }

        rets[highlighted] = buffer;
      }
    }
  }
};
