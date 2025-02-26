#pragma once

#include <chrono>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

class logger
{
public:
  static std::ofstream ofs;

  static std::string get_time()
  {
    std::time_t t =
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm ltime;
    localtime_r(&t, &ltime);
    std::stringstream stream;
    stream << std::put_time(&ltime, "%H:%M:%S");
    return stream.str();
  }

  static void stream_write(const std::string& out)
  {
    ofs << "(" + get_time() + ") " + out + '\n';
  }

  static void info(const std::string& out) { stream_write("[INFO] " + out); }

  static void warning(const std::string& out) { stream_write("[WARN] " + out); }

  static void err(const std::string& out) { stream_write("[ERROR] " + out); }

  ~logger() { ofs.close(); }
};
