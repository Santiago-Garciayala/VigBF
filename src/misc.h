#ifndef MISC_H
#define MISC_H

#include <array>
#include <string>

namespace misc {

bool getYesNo(std::string display,
              const std::array<char, 2> chars = {'y', 'n'});
std::string getTextFromFile();
std::string stringToUpper(const std::string in);
} // namespace misc

#endif
