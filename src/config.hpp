#pragma once

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>

#include <nlohmann/json.hpp>
#include <nlohmann/json_fwd.hpp>
#include <sys/stat.h>

nlohmann::json base_conf = nlohmann::json::parse(R"(
{
  "language": "English",
  "bootloader": "grub",
  "disk-config": {
    "efi-part": "",
    "root-part": "",
    "swap-part": ""
  },
  "hostname": "johnos",
  "packages": [],
  "username": "",
  "password": "",
  "timezone": "UTC"
})");

class config
{
public:
  static std::string config_loc;
  static nlohmann::json conf;

  static std::string get_config_loc()
  {
    if (std::getenv("XDG_CONFIG_HOME") != nullptr) {
      const std::string xdg_config_home = std::getenv("XDG_CONFIG_HOME");
      if (!xdg_config_home.empty() && xdg_config_home[0] != '\0') {
        return xdg_config_home + "johninstall/";
      }
    }
    if (std::getenv("HOME") != nullptr) {
      const std::string home = std::getenv("HOME");
      if (!home.empty()) {
        return home + "/.config/johninstall/";
      }
    }
    return "/tmp/johninstall";
  }

  static bool check_file(const std::string& name)
  {
    struct stat buffer {};
    return (stat(name.c_str(), &buffer) == 0);
  }

  static nlohmann::json read_conf()
  {
    if (check_file(config_loc + "conf.json")) {
      std::ifstream ifs(config_loc + "conf.json");
      try {
        return nlohmann::json::parse(ifs);
      } catch (...) {
        // send json parse error to logger
      }
    }

    // initialize config if it doesn't exist
    std::filesystem::create_directory(config_loc);
    std::ofstream ofs(config_loc + "conf.json");
    ofs << std::setw(4) << base_conf;
    return base_conf;
  }

  static void write_conf()
  {
    std::ofstream ofs(config_loc + "conf.json");
    ofs << std::setw(4) << conf;
  }
};
