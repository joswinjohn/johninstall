#include <string>
#include <vector>

#include "window.hpp"

#define KEY_RETURN 10

class answer_box : window
{
public:
  answer_box(int h, int w)
      : window((h * 2) + 2, w, (h * 3) + 2, 2)
  {
    box();
    keypad(/*v=*/true);
  }

  int selection(std::vector<std::string> choices)
  {
    int key;
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
      if (key == KEY_RETURN) {
        break;
      }
    }

    return highlighted;
  }
};
