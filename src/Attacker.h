#ifndef ATTACKER_H
#define ATTACKER_H

#include "Vigenere.h"
#include <array>
#include <map>
#include <stdint.h>
#include <string>

namespace attacks {
const static std::string ALPHABET_U = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const static std::string ALPHABET_L = "abcdefghijklmnopqrstuvwxyz";
const static std::string PROBABILITIES_FILE = "resources/probabilities.bin";
const static short ALPHABET_LEN = 26;
const static int PROBABILITIES_SIZE =
    ALPHABET_LEN * ALPHABET_LEN * ALPHABET_LEN * ALPHABET_LEN;
const static short FITNESS_UNFIT = -15;
extern std::array<double, PROBABILITIES_SIZE> probabilities;
extern bool is_probabilities_loaded;

class Attacker {
private:
  void load_probabilities();
  std::string get_key_from_num(int num);

public:
  Attacker();
  double static fitness(const std::string &text);
  double static index_of_coincidence(const std::string &text);
  int static get_period(const std::string &text);
  std::pair<std::string, std::string>
  brute_force_single_thread(Vigenere &v, int period = 1, uint16_t limit = 8);
};
} // namespace attacks

#endif
