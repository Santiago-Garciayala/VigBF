#ifndef MISC_H
#define MISC_H

#include <array>
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
} // namespace misc

#endif
