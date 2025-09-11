#include "Attacker.h"
#include <array>
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
    print_probabilities();
  }
}

void Attacker::load_probabilities() {
  std::ifstream file(PROBABILITIES_FILE, std::ios::binary);

  // probabilities.fill(0.0);
  // TODO: FIX THIS, IT DOESNT WORK
  file.read(reinterpret_cast<char *>(probabilities.data()), sizeof(double));
  /*
  if (!file) {
    throw std::runtime_error("Error reading probability data from file");
  }
  */
  is_probabilities_loaded = true;
}

double Attacker::fitness(const std::string &text) { return 0; }
} // namespace attacks
