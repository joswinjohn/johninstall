#include <fstream>
#include <string>
#include <vector>

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
          size_t last = line.find_last_of(' ');
          ret.push_back(line.substr(last + 1));
        }
      }
    }

    return ret;
  }
};
