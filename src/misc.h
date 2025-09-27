#ifndef MISC_H
#define MISC_H

#include <array>
#include <string>

namespace misc {

bool getYesNo(std::string display,
              const std::array<char, 2> chars = {'y', 'n'});
std::string getTextFromFile(std::string filename = "");
std::string stringToUpper(const std::string in);
std::string stringToLower(const std::string in);
int getInt(std::string display,
           std::string invalid_txt = "Input is not a number. Try again: ");
} // namespace misc

#endif
