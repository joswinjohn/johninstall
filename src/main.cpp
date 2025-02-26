#include <fstream>
#include <string>
#include <utility>
#include <vector>

#include <ncurses.h>

#include "config.hpp"
#include "curse.hpp"
#include "logger.hpp"
#include "selector.hpp"
#include "window.hpp"

#define GRID_SPLIT_H 5
#define GRID_SPLIT_W 1

#define exit() \
  endwin(); \
  return 0;

int window_selector::h;
int window_selector::w;
window* window_selector::active = nullptr;
std::vector<window> window_selector::windows;

nlohmann::json config::conf;
std::string config::config_loc;

std::ofstream logger::ofs;

int main()
{
  // logger
  if (std::filesystem::is_directory(config::get_config_loc())) {
    logger::ofs = std::ofstream(config::get_config_loc() + "out.log");
    logger::ofs << "Initialized Logs" << '\n';
    logger::ofs.close();
  }
  // start ncurses screen
  initscr();
  cbreak();
  curs_set(0);
  noecho();

  // initialize statics
  config::config_loc = config::get_config_loc();
  config::conf = config::read_conf();

  std::pair<int, int> size = curse::get_size();
  window_selector::h = (size.first - 4) / GRID_SPLIT_H;
  window_selector::w = (size.second - 4) / GRID_SPLIT_W;

  // create answer_box
  answer_box w_ans(window_selector::h, window_selector::w);

  // initialize selector
  window_selector selector {};
  selector.ans = &w_ans;

  // start dialog windows
  selector.start_window();

  // last window should contain write to conf.json
  config::write_conf();

  exit();
}
