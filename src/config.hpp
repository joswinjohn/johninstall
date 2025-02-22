#include <filesystem>
#include <fstream>
#include <string>

#include <nlohmann/json.hpp>
#include <sys/stat.h>

class config
{
public:
  static nlohmann::json conf;

  static bool check_file(const std::string& name)
  {
    struct stat buffer {};
    return (stat(name.c_str(), &buffer) == 0);
  }

  static nlohmann::json read_conf()
  {
    if (check_file("config/conf.json")) {
      std::ifstream ifs("config/conf.json");
      return nlohmann::json::parse(ifs);
    }

    // test
    nlohmann::json ex1 = nlohmann::json::parse(R"(
      {
        "pi": 3.141,
        "script": "no"
      }
    )");

    std::filesystem::create_directory("config");
    std::ofstream ofs("config/conf.json");
    ofs << ex1;
    return ex1;
  }
};
