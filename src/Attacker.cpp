#include "Attacker.h"
#include <array>
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
    print_probabilities();
  }
}

void Attacker::load_probabilities() {
  std::ifstream file(PROBABILITIES_FILE, std::ios::binary);

  // TODO: FIX THIS, IT DOESNT WORK
  file.read(reinterpret_cast<char *>(probabilities.data()),
            PROBABILITIES_SIZE * sizeof(double));

  if (!file) {
    std::cerr << "Error reading probabilities file: " << strerror(errno)
              << std::endl;
    std::cerr << "Bytes read: " << file.gcount() << std::endl;
  }
  is_probabilities_loaded = true;
}

double Attacker::fitness(const std::string &text) { return 0; }
} // namespace attacks
