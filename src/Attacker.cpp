#include "Attacker.h"
#include "Vigenere.h"
#include <array>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>

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

    int x =
        (alphabet.find(text[i]) * ALPHABET_LEN * ALPHABET_LEN * ALPHABET_LEN +
         alphabet.find(text[i + 1]) * ALPHABET_LEN * ALPHABET_LEN +
         alphabet.find(text[i + 2]) * ALPHABET_LEN +
         alphabet.find(text[i + 3]));
    double y = probabilities[x];

    if (y == 0) {
      result += FITNESS_UNFIT;
    } else {
      result += std::log(y);
    }
  }
  result = result / (text.size() - 3);
  return result;
}

double Attacker::index_of_coincidence(const std::string &text) {
  std::array<int, ALPHABET_LEN> counts = {};
  int numer = 0, total = 0;
  std::string alphabet;
  alphabet.assign(isupper(text[0]) ? ALPHABET_U : ALPHABET_L);

  for (auto c : text) {
    counts[alphabet.find(c)] += 1;
  }

  for (int i = 0; i < ALPHABET_LEN; ++i) {
    numer += counts[i] * (counts[i] - 1);
    total += counts[i];
  }

  return static_cast<double>(ALPHABET_LEN) * numer / (total * (total - 1));
}

int Attacker::get_period(const std::string &text) {
  bool found = false;
  int period = 0;

  while (!found) {
    period += 1;
    std::vector<std::string> slices(period, "");
    for (int i = 0; i < text.size(); ++i) {
      slices[i % period] += text[i];
    }
    double sum = 0;
    for (int j = 0; j < period; ++j) {
      sum += index_of_coincidence(slices[j]);
    }
    double ioc = sum / period;
    if (ioc > 1.6)
      found = true;
  }

  return period;
}

// TODO: this doesnt produce the first series of A's after the length increases.
// Eg: Z jumps to AB instead of AA.
std::string Attacker::get_key_from_num(int num) {
  std::string key;
  size_t len = 0;

  // get length of key
  while (num >= pow(ALPHABET_LEN, len)) {
    len += 1;
  }

  for (int i = 1; i <= len; ++i) {
    key.push_back(ALPHABET_U[num % static_cast<int>((pow(ALPHABET_LEN, i))) /
                             pow(ALPHABET_LEN, i - 1)]);
  }

  return key;
}

std::pair<std::string, std::string>
Attacker::brute_force_single_thread(Vigenere &v, int period, uint16_t limit) {
  std::pair<std::string, std::string> solution;
  bool found = false;
  std::string key;
  int key_num = static_cast<int>(pow(ALPHABET_LEN, period - 1));
  double fitns = FITNESS_UNFIT;
  int limit_val = pow(ALPHABET_LEN, limit) + 1;
  short warning_exp =
      period <= 1 ? 4 : period; // if no period specified, give warning when
                                // key length reaches 4.
                                // Otherwise give warning when
                                // key length exceeds period.
  int warning_val = pow(ALPHABET_LEN, warning_exp) + 1;

  key.reserve(period);

  while (!found) {
    key = get_key_from_num(key_num);
    cout << key << std::endl;
    fitns = Attacker::fitness(v.decodeNoAlpha(key));
    cout << "fitness: " << fitns << std::endl;
    if (fitns > -10) {
      found = true;
      solution.first = key;
      solution.second = v.decode(key);
    }
    if (key_num == warning_val) {
      cout << "WARNING: KEY LENGTH EXCEEDED " << warning_exp
           << " IN BRUTE FORCE ATTACK." << std::endl
           << "THE PROGRAM WILL CONTINUE EXECUTING, BUT IT IS VERY "
              "UNLIKELY A SOLUTION WILL BE FOUND."
           << std::endl
           << "YOU CAN USE Ctrl + C TO "
              "QUIT."
           << std::endl;
    }
    if (key_num >= limit_val) {
      solution.first = "";
      solution.second = "NOT FOUND";
      cout << "KEY LENGTH LIMIT REACHED FOR BRUTE FORCE ATTACK. SOLUTION NOT "
              "FOUND."
           << std::endl
           << "KEY LENGTH LIMIT: " << limit << std::endl;
      break;
    }
    ++key_num;
  }

  return solution;
}

} // namespace attacks
