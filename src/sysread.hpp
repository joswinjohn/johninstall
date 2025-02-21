#include <fstream>
#include <string>
#include <vector>

class reader
{
  static std::vector<std::string> get_parts()
  {
    std::ifstream stream("/proc/partitions");
  }
};
