#include "misc.h"
#include <algorithm>
#include <array>
#include <cctype>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>

namespace misc {

bool getYesNo(std::string display, const std::array<char, 2> chars) {

  char c;

  std::cout << display;
  std::cin >> c;
  c = tolower(c);

  // clear kb buffer
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

  while (c != chars[0] && c != chars[1]) {
    std::cout << "Invalid input. Enter '" << chars[0] << "' or '" << chars[1]
              << "'." << std::endl;
    std::cout << display;
    std::cin >> c;
    c = tolower(c);

    // clear kb buffer
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  return c == chars[0];
}
std::string getTextFromFile() {

  std::ifstream file;
  std::string filename, text;

  std::cout << "Enter the name of the file: ";
  getline(std::cin, filename);

  file.open(filename);

  while (!file.is_open() && filename != "q") {
    std::cout << filename
              << " could not be opened. Enter a different file name or 'q' to "
                 "quit."
              << std::endl;
    std::cout << "Enter the name of the file: ";
    getline(std::cin, filename);
    file.open(filename);
  }

  if (filename == "q") {
    exit(EXIT_SUCCESS);
  }

  std::string line;
  while (getline(file, line)) {
    text += line + "\n";
  }
  file.close();

  return text;
}
std::string stringToUpper(const std::string in) {
  std::string out = in;
  std::transform(out.begin(), out.end(), out.begin(), ::toupper);
  return out;
}
} // namespace misc
