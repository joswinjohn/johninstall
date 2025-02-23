#pragma once

#include "answer_box.hpp"
#include "config.hpp"
#include "sysread.hpp"
#include "window.hpp"

class window_selector
{
public:
  answer_box* ans;

  static int h;
  static int w;

  static window* active;

  void start_window()
  {
    window w_start(h * 3, w, 2, 2);
    w_start.box();
    w_start.print(2,
                4,
                "This is the guided JohnOS installer. to continute or move "
                "back " "use the \"Next\" and \"Back\" buttons");
    active = &w_start;

    if (ans->selection({"Next", "Exit"}) == 1) {
      window::close();
      return;
    }
    part_1();
  }

  void part_1()
  {
    window win(h * 3, w, 2, 2);
    win.box();
    win.print(2, 4, R"(Select your EFI partition)");
    active = &win;

    // Get partitions from /proc/partitions
    std::vector<std::string> parts = reader::get_parts();
    parts.emplace_back("Back");
    int part_choice = ans->selection(parts);

    // If part choice is "Back"
    if (part_choice == parts.size() - 1) {
      ::refresh();
      start_window();
    }
    config::conf["disk-config"]["efi-part"] = parts[part_choice];
    part_2();
  }

  void part_2()
  {
    window win(h * 3, w, 2, 2);
    win.box();
    win.print(2, 4, R"(Select your root partition)");
    active = &win;

    // Get partitions from /proc/partitions
    std::vector<std::string> parts = reader::get_parts();
    parts.emplace_back("Back");
    int part_choice = ans->selection(parts);

    // If part choice is "Back"
    if (part_choice == parts.size() - 1) {
      part_1();
    }
    config::conf["disk-config"]["root-part"] = parts[part_choice];
    part_3();
  }

  void part_3()
  {
    window win(h * 3, w, 2, 2);
    win.box();
    win.print(2, 4, R"(Select your swap partition)");
    active = &win;

    // Get partitions from /proc/partitions
    std::vector<std::string> parts = reader::get_parts();
    parts.emplace_back("No Swap");
    parts.emplace_back("Back");
    int part_choice = ans->selection(parts);

    // If part choice is "Back"
    if (part_choice == parts.size() - 1) {
      part_3();
    } else if (part_choice == parts.size() - 2) {
      config::conf["disk-config"]["swap-part"] = "";
      // part_4();
    } else {
      config::conf["disk-config"]["swap-part"] = parts[part_choice];
      // part_4();
    }
  }
};
