#pragma once

#include <string>
#include <vector>

#include "config.hpp"
#include "window.hpp"

class windows
{
public:
  static int h;
  static int w;

  static window* active;

  static void start_window()
  {
    window w_start(h * 4, w, 2, 2);
    w_start.box();
    w_start.print(2,
                4,
                "This is the guided JohnOS installer. to continute or move "
                "back " "use the \"Next\" and \"Back\" buttons");
    active = &w_start;
  }

  static void partition_window_1()
  {
    window w_partition(h * 4, w, 2, 2);
    w_partition.box();
    w_partition.print(2, 4, R"(Select your EFI partition)");
    active = &w_partition;
  }

  static void partition_window_2()
  {
    window w_partition(h * 4, w, 2, 2);
    w_partition.box();
    w_partition.print(2, 4, R"(Select your next partition)");
    w_partition.print(6, 4, config::conf["script"]);
    active = &w_partition;
  }
};
