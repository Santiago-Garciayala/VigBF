#ifndef MISC_H
#define MISC_H

#include <array>
#include <queue>
#include <string>
#include <vector>

namespace misc {

bool getYesNo(std::string display,
              const std::array<char, 2> chars = {'y', 'n'});
std::string getTextFromFile(std::string filename = "");
void writeToFile(std::string in, std::string filename);
std::string stringToUpper(const std::string in);
std::string stringToLower(const std::string in);
int getInt(std::string display,
           std::string invalid_txt = "Input is not a number. Try again: ");
std::vector<std::string> split(const std::string &s,
                               const std::string &delimiter);
template <typename T> std::vector<T> queue_to_vec(std::queue<T> q) {
  std::vector<T> vec;

  while (!q.empty()) {
    vec.push_back(q.front());
    q.pop();
  }

  return vec;
}
} // namespace misc

#endif
