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
    return (std::pair(h, w));
  }
};
