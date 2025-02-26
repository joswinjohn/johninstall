#pragma once

#include <array>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#define PROC_PART_MINOR_OFFSET 12
#define BUFFER_SIZE 1024

class reader
{
public:
  static std::vector<std::string> get_parts()
  {
    std::vector<std::string> ret;
    std::ifstream stream("/proc/partitions");
    std::string line;
    std::getline(stream, line);
    if (stream.is_open()) {
      while (std::getline(stream, line)) {
        if (line.size() != 0) {
          if (line.substr(PROC_PART_MINOR_OFFSET, 1) != "0") {
            size_t last = line.find_last_of(' ');
            ret.push_back("/dev/" + line.substr(last + 1));
          }
        }
      }
    }

    return ret;
  }

  static std::string exec(const char* cmd)
  {
    std::array<char, BUFFER_SIZE> buffer {};
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
      throw std::runtime_error("popen() failed!");
    }
    while (!feof(pipe.get())) {
      if (fgets(buffer.data(), BUFFER_SIZE, pipe.get()) != nullptr) {
        result += buffer.data();
      }
    }
    return result;
  }

  static std::vector<std::string> get_times()
  {
    std::string times = exec(
        "awk '/^Z/ { print $2 }; /^L/ { print $3 }' "
        "/usr/share/zoneinfo/tzdata.zi");

    std::vector<std::string> ret {};
    std::stringstream ss(times);

    for (std::string line; std::getline(ss, line, '\n');) {
      ret.push_back(line);
    }

    return ret;
  }
};
