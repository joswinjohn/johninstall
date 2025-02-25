#include <ctime>
#include <fstream>

class logger
{
public:
  static std::ofstream ofs;

  static void stream_write(const std::string& out)
  {
    time_t now;
    time(&now);
    std::string dt = ctime(&now);
    ofs << "(" + dt + ") " + out + '\n';
  }

  static void info(const std::string& out) { stream_write("[INFO] " + out); }

  static void warning(const std::string& out) { stream_write("[WARN] " + out); }

  static void err(const std::string& out) { stream_write("[ERROR] " + out); }

  ~logger() { ofs.close(); }
};
