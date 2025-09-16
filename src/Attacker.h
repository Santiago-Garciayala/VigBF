#ifndef ATTACKER_H
#define ATTACKER_H

#include <array>
#include <string>

namespace attacks {
const static std::string ALPHABET_U = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const static std::string ALPHABET_L = "abcdefghijklmnopqrstuvwxyz";
const static std::string PROBABILITIES_FILE = "resources/probabilities.bin";
const static int PROBABILITIES_SIZE = 26 * 26 * 26 * 26;
extern std::array<double, PROBABILITIES_SIZE> probabilities;
extern bool is_probabilities_loaded;

class Attacker {
private:
  void load_probabilities();

public:
  Attacker();
  double fitness(const std::string &text);
};
} // namespace attacks

#endif
