#include "Attacker.h"
#include <array>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

namespace attacks {

std::array<double, PROBABILITIES_SIZE> probabilities = {};
bool is_probabilities_loaded = false;

void print_probabilities() {
  for (double num : probabilities) {
    std::cout << num << " ";
  }
}

Attacker::Attacker() {
  if (!is_probabilities_loaded) {
    load_probabilities();
    // print_probabilities();
  }
}

void Attacker::load_probabilities() {
  std::ifstream file(PROBABILITIES_FILE, std::ios::binary);

  file.read(reinterpret_cast<char *>(probabilities.data()),
            PROBABILITIES_SIZE * sizeof(double));

  if (!file) {
    std::cerr << "Error reading probabilities file: " << strerror(errno)
              << std::endl;
    std::cerr << "Bytes read: " << file.gcount() << std::endl;
  }
  is_probabilities_loaded = true;
}

/**
 *Determines how close a body of text is to valid english text.
 *The return value should be around -9.6 for valid english text.
 *Any lower than that should not be considered valid english.
 *NOTE: the string thats passed to this function should only contain
 *alphabetic characters, and they should all be either uppercase or lowercase,
 *never a mix of both. A quick way to achieve this is by plugging the text into
 *a Vigenere object and using its getTextOnlyAlpha() method.
 */
double Attacker::fitness(const std::string &text) {
  double result = 0;
  std::string alphabet;
  alphabet.assign(isupper(text[0]) ? ALPHABET_U : ALPHABET_L);

  for (int i = 0; i < text.size() - 3; ++i) {
    // this gives weird memory error for some reason
    // std::string tetragram = text.substr(i, 4);

    int x = ((int)alphabet.find(text[i]) * 26 * 26 * 26 +
             (int)alphabet.find(text[i + 1]) * 26 * 26 +
             (int)alphabet.find(text[i + 2]) * 26 +
             (int)alphabet.find(text[i + 3]));
    double y = probabilities[x];

    if (y == 0) {
      result += -15;
    } else {
      result += std::log(y);
    }
  }
  result = result / (text.size() - 3);
  return result;
}
} // namespace attacks
