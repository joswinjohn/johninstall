#pragma once

#include <string>
#include <vector>

#include <ncurses.h>

#include "answer_box.hpp"
#include "config.hpp"
#include "sysread.hpp"
#include "window.hpp"

#define draw_window() \
  ans->redraw_box(); \
  window win(h * 3, w, 2, 2); \
  win.box();

class window_selector
{
public:
  answer_box* ans;

  static int h;
  static int w;

  static window* active;

  static std::vector<window> windows;

  void start_window()
  {
    draw_window();
    win.print(2,
                4,
                "This is the guided JohnOS installer. to continute or move "
                "back " "use the \"Next\" and \"Back\" buttons");
    active = &win;

    if (ans->selection({"Next", "Exit"}) == 1) {
      endwin();
      return;
    }
    part_1();
  }

  void part_1()
  {
    draw_window();
    win.print(2, 4, R"(Select your EFI partition)");
    active = &win;

    // Get partitions from /proc/partitions
    std::vector<std::string> parts = reader::get_parts();
    parts.emplace_back("Back");
    int part_choice = ans->selection(parts);

    // If part choice is "Back"
    if (part_choice == parts.size() - 1) {
      win.close();
      start_window();
    } else {
      config::conf["disk-config"]["efi-part"] = parts[part_choice];
      part_2();
    }
  }

  void part_2()
  {
    draw_window();
    win.print(2, 4, R"(Select your root partition)");
    active = &win;

    // Get partitions from /proc/partitions
    std::vector<std::string> parts = reader::get_parts();
    parts.emplace_back("Back");
    int part_choice = ans->selection(parts);

    // If part choice is "Back"
    if (part_choice == parts.size() - 1) {
      part_1();
    } else {
      config::conf["disk-config"]["root-part"] = parts[part_choice];
      part_3();
    }
  }

  void part_3()
  {
    draw_window();
    win.print(2, 4, R"(Select your swap partition)");
    active = &win;

    // Get partitions from /proc/partitions
    std::vector<std::string> parts = reader::get_parts();
    parts.emplace_back("No Swap");
    parts.emplace_back("Back");
    int part_choice = ans->selection(parts);

    // If part choice is "Back"
    if (part_choice == parts.size() - 1) {
      part_2();
    } else if (part_choice == parts.size() - 2) {
      config::conf["disk-config"]["swap-part"] = "";
      create_user();
    } else {
      config::conf["disk-config"]["swap-part"] = parts[part_choice];
      create_user();
    }
  }

  void create_user()
  {
    draw_window();
    win.print(2, 4, R"(Select your username)");
    active = &win;

    // Get username and password inputs
    std::vector<std::string> fields = {"Username", "Password", "Next", "Back"};
    std::vector<std::string> inputs = ans->selection_input(fields);
    config::conf["username"] = inputs.at(0);
    config::conf["password"] = inputs.at(1);

    if (fields.empty()) {
      part_3();
    }

    // last window should contain write to conf.json
    config::write_conf();
  }
};
