#ifndef ATTACKER_H
#define ATTACKER_H

#include "Vigenere.h"
#include <array>
#include <cstdint>
#include <map>
#include <queue>
#include <stdint.h>
#include <string>
#include <vector>

namespace attacks {
const static std::string ALPHABET_U = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const static std::string ALPHABET_L = "abcdefghijklmnopqrstuvwxyz";
const static std::string PROBABILITIES_FILE = "resources/probabilities.bin";
const static std::string DICTIONARY_FILE =
    "resources/dictionaries/dict_only_alpha";
const static size_t ALPHABET_LEN = 26;
const static size_t PROBABILITIES_SIZE =
    ALPHABET_LEN * ALPHABET_LEN * ALPHABET_LEN * ALPHABET_LEN;
const static short FITNESS_UNFIT = -15;
const static double FITNESS_THRESHOLD = -9.8;
const static uint8_t MAX_PERIOD = 32;
const static double IOC_THRESHOLD = 1.6;
const static double COSANGLE_GOOD = 0.8;
const static std::pair<std::string, std::string> NOT_FOUND = {"", "NOT FOUND"};
enum Attacks {
  BF_ATTACK,
  DICT_ATTACK,
  CRIB_ATTACK,
  VARIATIONAL_ATTACK,
  STATS_ATTACK,
  // if adding new attacks it must be added before ATTACKS_LAST
  ATTACKS_LAST
};

extern std::array<double, PROBABILITIES_SIZE> probabilities;
extern std::vector<std::string> dict_words;
extern bool is_probabilities_loaded;
extern bool is_dict_loaded;

class Attacker {
private:
  void load_probabilities();
  void load_dict();
  std::string get_key_from_num(int num, bool upper = false);
  double cosangle(vector<double> x, vector<double> y);
  static bool result_found(std::pair<std::string, std::string> in);

public:
  Attacker();
  double static fitness(const std::string &text);
  double static index_of_coincidence(const std::string &text);
  int static get_period(const std::string &text);
  int static get_period_kasiski(const std::string &text);
  std::pair<std::string, std::string>
  brute_force_single_thread(Vigenere &v, int period = 1, uint16_t limit = 8);
  std::pair<std::string, std::string> dictionary_attack(Vigenere &v);
  std::pair<std::string, std::string> crib_attack(Vigenere &v,
                                                  const std::string &crib);
  std::pair<std::string, std::string> variational_attack(Vigenere &v,
                                                         int period);
  std::pair<std::string, std::string> stats_attack(Vigenere &v, int period);
  static pair<string, string>
  perform_attacks(string input, queue<int> attack_queue, uint8_t period,
                  pair<uint8_t, uint8_t> range, std::string crib = "");
  static pair<string, string>
  perform_attacks(vector<string> inputs, queue<int> attack_queue,
                  uint8_t period, pair<uint8_t, uint8_t> range,
                  std::string crib = "", bool all = false);
};
} // namespace attacks

#endif
