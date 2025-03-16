#pragma once

#include <stdexcept>
#include <string>
#include <vector>

#include <ncurses.h>

#include "inputs.hpp"
#include "config.hpp"
#include "sysread.hpp"
#include "window.hpp"
#include "curse.hpp"

#define draw_window() \
  ans->redraw_box(); \
  window win(h * 3, w, 2, 2); \
  win.box();

class window_selector
{
public:
  inputs* ans;

  static int h;
  static int w;

  static window* active;

  static std::vector<window> windows;

  void start_window() {
    draw_window();
    win.print(2,
                4,
                "This is the guided JohnOS installer. to continute or move "
                "back " "use the \"Next\" and \"Back\" buttons");
    active = &win;
    win.print(
        (h * 3) - 4, 4, "Config will be written to: " + config::config_loc);

    if (ans->select_input({"Next", "Exit"}) == 1) {
      curse::stop_curses();
      return;
    }
    part_1();
  }

  void part_1() {
    draw_window();
    win.print(2, 4, R"(Select your EFI partition)");
    active = &win;

    // Get partitions from /proc/partitions
    std::vector<std::string> parts = reader::get_parts();
    parts.emplace_back("Back");
    int part_choice = ans->select_input(parts);

    // If part choice is "Back"
    if (part_choice == parts.size() - 1) {
      win.close();
      start_window();
    } else {
      config::conf["disk-config"]["efi-part"] = parts[part_choice];
      part_2();
    }
  }

  void part_2() {
    draw_window();
    win.print(2, 4, R"(Select your root partition)");
    active = &win;

    // Get partitions from /proc/partitions
    std::vector<std::string> parts = reader::get_parts();
    parts.emplace_back("Back");
    int part_choice = ans->select_input(parts);

    // If part choice is "Back"
    if (part_choice == parts.size() - 1) {
      part_1();
    } else {
      config::conf["disk-config"]["root-part"] = parts[part_choice];
      part_3();
    }
  }

  void part_3() {
    draw_window();
    win.print(2, 4, R"(Select your swap partition)");
    active = &win;

    // Get partitions from /proc/partitions
    std::vector<std::string> parts = reader::get_parts();
    parts.emplace_back("No Swap");
    parts.emplace_back("Back");
    int part_choice = ans->select_input(parts);

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

  void create_user() {
    draw_window();
    win.print(2, 4, R"(Enter a username and password)");
    active = &win;

    // Get username and password inputs
    std::vector<std::string> fields = {
        "Hostname", "Username", "Password", "Next", "Back"};
    std::vector<std::string> inputs = ans->field_input(fields);

    if (inputs.empty()) {
      part_3();
      return;
    }

    config::conf["hostname"] = inputs.at(0);
    config::conf["username"] = inputs.at(1);
    config::conf["password"] = inputs.at(2);
    set_time();
  }

  void set_time() {
    draw_window();
    win.print(2, 4, R"(Select your timezone)");
    active = &win;

    // Get timezones from /usr/share/zoneinfo/tzdata.zi
    std::vector<std::string> timezones = reader::get_times();
    timezones.emplace_back("Back");
    int choice = ans->select_input(timezones);

    // If part choice is "Back"
    if (choice == timezones.size() - 1) {
      part_2();
    } else {
      config::conf["timezone"] = timezones[choice];
    }
  }
};
