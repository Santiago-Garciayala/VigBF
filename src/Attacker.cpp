#include "Attacker.h"
#include "Vigenere.h"
#include "misc.h"
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <experimental/random>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>

namespace attacks {

std::array<double, PROBABILITIES_SIZE> probabilities = {};
std::vector<std::string> dict_words = {};
bool is_probabilities_loaded = false;
bool is_dict_loaded = false;

void print_probabilities() {
  for (double num : probabilities) {
    std::cout << num << " ";
  }
  cout << std::endl;
}
void print_dict() {
  for (auto word : dict_words) {
    std::cout << word << " ";
  }
  cout << std::endl;
}

Attacker::Attacker() {
  if (!is_probabilities_loaded) {
    load_probabilities();
    // print_probabilities();
  }
}

void Attacker::load_probabilities() {
  std::ifstream file(PROBABILITIES_FILE, std::ios::binary);

  if (!file) {
    std::cerr << "Error reading probabilities file: " << strerror(errno)
              << std::endl;
    std::cerr << "Bytes read: " << file.gcount() << std::endl;
    return;
  }

  file.read(reinterpret_cast<char *>(probabilities.data()),
            PROBABILITIES_SIZE * sizeof(double));

  is_probabilities_loaded = true;
}

void Attacker::load_dict() {
  std::ifstream file(DICTIONARY_FILE);

  if (!file) {
    std::cerr << "Error reading dictionary file: " << strerror(errno)
              << std::endl;
    std::cerr << "Bytes read: " << file.gcount() << std::endl;
    return;
  }

  std::string line;
  while (getline(file, line)) {
    if (line.size() > 0) {
      dict_words.push_back(line);
    }
  }

  is_dict_loaded = true;
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
  const std::string &alphabet = isupper(text[0]) ? ALPHABET_U : ALPHABET_L;

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
  const std::string &alphabet = isupper(text[0]) ? ALPHABET_U : ALPHABET_L;

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
std::string Attacker::get_key_from_num(int num, bool upper) {
  std::string key;
  size_t len = 0;
  const std::string &alphabet = upper ? ALPHABET_U : ALPHABET_L;

  // get length of key
  while (num >= pow(ALPHABET_LEN, len)) {
    len += 1;
  }

  for (int i = 1; i <= len; ++i) {
    key.push_back(alphabet[num % static_cast<int>((pow(ALPHABET_LEN, i))) /
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
    if (fitns > FITNESS_THRESHOLD) {
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
      solution = NOT_FOUND;
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

std::pair<std::string, std::string> Attacker::dictionary_attack(Vigenere &v) {
  std::pair<std::string, std::string> solution = NOT_FOUND;
  std::string key;
  double fitns = FITNESS_UNFIT;

  if (!is_dict_loaded) {
    load_dict();
    // print_dict();
  }

  for (auto key : dict_words) {
    fitns = Attacker::fitness(v.decodeNoAlpha(key));
    /*
    cout << key << std::endl;
    cout << "fitness: " << fitns << std::endl;
    if (key == "aani") {
      cout << v.decodeNoAlpha(key) << std::endl;
    }
    */
    if (fitns > FITNESS_THRESHOLD) {
      solution.first = key;
      solution.second = v.decode(key);
      break;
    }
  }

  return solution;
}

std::pair<std::string, std::string>
Attacker::crib_attack(Vigenere &v, const std::string &crib) {
  std::pair<std::string, std::string> solution;
  std::string chunk;
  std::string decrypted_chunk;
  std::string decrypted_txt;
  int chunk_choice;
  double fitns;

  chunk.reserve(crib.length());
  decrypted_chunk.reserve(crib.length());

  for (int i = 0; i < v.getTextOnlyAlpha().length() - crib.length(); ++i) {
    chunk = v.getTextOnlyAlpha().substr(i, crib.length());
    decrypted_chunk = Vigenere::staticDecode(chunk, crib);
    std::cout << i + 1 << ": " << decrypted_chunk << "\t";
  }
  cout << std::endl;

  chunk_choice = misc::getInt(
      "Enter the number of the chunk that looks most like a key: ");
  --chunk_choice;

  while (chunk_choice < 0 ||
         chunk_choice >= v.getTextOnlyAlpha().length() - crib.length()) {
    std::cout << "Invalid choice. Enter a number that is listed on the screen."
              << std::endl;
    chunk_choice = misc::getInt(
        "Enter the number of the chunk that looks most like a key: ");
    --chunk_choice;
  }

  chunk =
      v.getTextOnlyAlpha().substr(chunk_choice, chunk_choice + crib.length());
  decrypted_chunk = Vigenere::staticDecode(chunk, crib);

  decrypted_txt = v.decodeNoAlpha(decrypted_chunk);
  fitns = Attacker::fitness(decrypted_txt);

  // cout << "fitness: " << fitns << std::endl;

  if (fitns > FITNESS_THRESHOLD) {
    solution.first = decrypted_chunk;
    solution.second = v.decode(decrypted_chunk);
    return solution;
  }

  return NOT_FOUND;
}

std::pair<std::string, std::string> Attacker::variational_attack(Vigenere &v,
                                                                 int period) {
  std::pair<std::string, std::string> solution = NOT_FOUND;
  const std::string &alphabet =
      isupper(v.getTextOnlyAlpha()[0]) ? ALPHABET_U : ALPHABET_L;
  std::string key;
  std::string new_key;
  std::string attempt;
  double fitns = FITNESS_UNFIT;

  key.resize(period, alphabet[0]);
  attempt.reserve(v.getTextOnlyAlpha().length());

  while (fitns < FITNESS_THRESHOLD) {
    new_key = key;
    int x = std::experimental::randint(0, period);
    for (int i = 0; i < ALPHABET_LEN; ++i) {
      new_key[x] = alphabet[i];
      attempt = v.decodeNoAlpha(new_key);
      double new_fit = Attacker::fitness(attempt);
      if (new_fit > fitns) {
        key = new_key;
        fitns = new_fit;
        std::cout << key << " ";
      }
    }
  }

  solution.first = key;
  solution.second = v.decode(key);

  return solution;
}

} // namespace attacks
