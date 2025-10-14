#include "./Vigenere.h"
#include "Attacker.h"
#include "misc.h"
#include <algorithm>
#include <array>
#include <bits/getopt_core.h>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <string>

extern "C" {
#include <getopt.h>
#include <unistd.h>
}

using namespace std;

void printHelp();
void handholdMode();
void known_key_handhold();
void unknown_key_handhold();
string mode_to_text(uint8_t mode);
void set_mode(uint8_t *mode, uint8_t new_mode);

const char *SHORT_OPTS = "edi:f:k:K:o:m:p:r:c:C:t:ThHvs:";
const struct option LONG_OPTS[] = {
    {"encode", no_argument, nullptr, 'e'},
    {"decode", no_argument, nullptr, 'd'},
    {"input", required_argument, nullptr, 'i'},
    {"file", required_argument, nullptr, 'f'},
    {"key", required_argument, nullptr, 'k'},
    {"key-file", required_argument, nullptr, 'K'},
    {"output", required_argument, nullptr, 'o'},
    {"mode", required_argument, nullptr, 'm'},
    {"period", required_argument, nullptr, 'l'},
    {"range", required_argument, nullptr, 'r'},
    {"crib", required_argument, nullptr, 'c'},
    {"crib-file", required_argument, nullptr, 'C'},
    {"threads", required_argument, nullptr, 't'},
    {"help", optional_argument, nullptr, 'h'},
    {"Handhold", no_argument, nullptr, 'H'},
    {"verbose", no_argument, nullptr, 'v'},
    {"serial", optional_argument, nullptr, 's'},
    {0, 0, 0, 0}};
enum Modes { NOT_SET, ENCODE, DECODE, ATTACK };
const size_t DEFAULT_THREADS = 1;

int main(int argc, char *argv[]) {
  char c;
  uint8_t mode = NOT_SET;
  string out = "";
  string outfile = "";
  string text = "";
  string key = "";
  queue<int> attack_queue;
  uint8_t period = 0;
  pair<uint8_t, uint8_t> range = {0, 0};
  string crib = "";
  size_t threads = DEFAULT_THREADS;
  bool verbose = false;

  while ((c = getopt_long(argc, argv, SHORT_OPTS, LONG_OPTS, nullptr)) != -1) {
    switch (c) {
    case 'e':
      set_mode(&mode, ENCODE);
      break;
    case 'd':
      set_mode(&mode, DECODE);
      break;
    case 'i':
      if (text != "") {
        cout << "WARNING: Input text already specified. Only the first "
                "instance will be used."
             << endl;
        break;
      }
      text = optarg;
      break;
    case 'f':
      if (text != "") {
        cout << "WARNING: Input text already specified. Only the first "
                "instance will be used."
             << endl;
        break;
      }
      text = misc::getTextFromFile(optarg);
      break;
    case 'k':
      if (key != "") {
        cout << "WARNING: Key already specified. Only the first "
                "instance will be used."
             << endl;
        break;
      }
      key = optarg;
      break;
    case 'K':
      if (key != "") {
        cout << "WARNING: Key already specified. Only the first "
                "instance will be used."
             << endl;
        break;
      }
      key = misc::getTextFromFile(optarg);
      break;
    case 'm':
      uint8_t attack_num;

      set_mode(&mode, ATTACK);
      if (mode != ATTACK)
        break;

      try {
        attack_num = stoi(optarg);
      } catch (...) {
        cerr << "Invalid input for attack type: " << optarg << endl;
        break;
      }
      if (attack_num >= attacks::BF_ATTACK &&
          attack_num < attacks::ATTACKS_LAST) {
        attack_queue.push(attack_num);
      } else {
        cerr << "Invalid attacks number: " << optarg
             << ". Valid attack numbers: " << attacks::BF_ATTACK << "-"
             << attacks::ATTACKS_LAST - 1 << endl;
        cerr << "Use -h for help and to see which attack corresponds to what "
                "number."
             << endl;
      }
      break;
    case 'p':
      uint8_t p;
      try {
        p = stoi(optarg);
      } catch (...) {
        cerr << "Invalid input for period: " << optarg << endl;
        break;
      }

      if (p <= 0) {
        cerr << "ERROR: Period cannot be less than or equal to 0. Period "
                "entered: "
             << p << endl;
        break;
      }

      if (period == 0) {
        period = p;
      } else {
        cout << "WARNING: More than one period specified. Only the first one ("
             << period << ") will be used. " << endl;
        cout << "You can specify a range of periods using -r / --range" << endl;
      }

      break;
    case 'r': {
      uint8_t low, high;
      try {
        string optarg_s = optarg;
        vector<string> nums = misc::split(optarg_s, "-");
        if (nums.size() != 2) {
          cerr << "ERROR: Range should be inputted as X-Y. Input read: "
               << optarg << endl;
          break;
        }

        low = stoi(nums[0]);
        high = stoi(nums[1]);

        if (low <= 0 || high <= 0) {
          cerr << "ERROR: Numbers in range cannot be less than or equal to 0. "
               << endl;
          cerr << "Numbers entered: " << low << ", " << high << endl;
          break;
        }

        if (nums[0] > nums[1]) {
          uint8_t temp = low;
          low = high;
          high = temp;
        }

      } catch (...) {
        cerr << "ERROR: Range should be inputted as X-Y, where X and Y are "
                "whole numbers. Input read: "
             << optarg << endl;
      }

      pair<int, int> r(low, high);

      if (range.first == 0 && range.second == 0) {
        range = r;
      } else {
        cout << "WARNING: More than one range specified. Only the first one ("
             << range.first << "-" << range.second << ") will be used. "
             << endl;
      }

      break;
    }
    case 'c':
      if (crib != "") {
        cout << "WARNING: Crib already specified. Only the first crib will be "
                "used."
             << endl;
        cout << "CRIB: \n\n" << crib << endl;
        break;
      }
      crib = optarg;

      break;

    case 'C':
      if (crib != "") {
        cout << "WARNING: Crib already specified. Only the first crib will be "
                "used."
             << endl;
        cout << "CRIB: \n\n" << crib << endl;
        break;
      }
      crib = misc::getTextFromFile(optarg);

      break;
    case 't':
      size_t t;
      try {
        t = stoi(optarg);
      } catch (...) {
        cerr << "Invalid input for threads: " << optarg << endl;
        break;
      }
      threads = t;

      break;
    case 'T': {
      // TEST MODE
      std::string tesxt = misc::getTextFromFile("resources/books/pg22400.txt");
      //      tesxt =
      //      "hellomynameisthefollowingstringofnumbersthatifoundonthestreether"
      //             "eyougo";
      Vigenere v(misc::stringToLower(tesxt));
      attacks::Attacker a;
      std::string key = "migogosz";
      std::string encoded_text = v.encode_processed(key);
      // cout << "encoded text: " << encoded_text << std::endl;
      int period = a.get_period_kasiski(encoded_text);
      // period = 6;
      cout << "period: " << period << std::endl;
      cout << "fitness: " << a.fitness(v.get_processed_text()) << std::endl;
      cout << "fitness of encoded: " << a.fitness(v.encode_processed(key))
           << std::endl;

      Vigenere v2(encoded_text);
      std::string attack_result;
      // attack_result = a.brute_force_single_thread(v2, 2, 3).second;
      // attack_result = a.dictionary_attack(v2).second;
      // attack_result = a.crib_attack(v2, v.getTextOnlyAlpha().substr(4,
      // 4)).first;
      attack_result = a.variational_attack(v2, period).first;
      // attack_result = a.stats_attack(v2, 6).first;
      cout << attack_result << std::endl;

      break;
    }
    case 'h':
      printHelp();
      break;
    case 'H':
      handholdMode();
      exit(EXIT_SUCCESS);
      break;
    case 'v':
      verbose = true;
      break;
    case 's':
      threads = 1;
      break;
    case '?':
      cerr << "Invalid option: " << char(optopt) << ". Use -h for help.\n";
      exit(EXIT_FAILURE);
      break;
    default:
      cerr << "Invalid option: " << char(optopt) << ". Use -h for help.\n";
      exit(EXIT_FAILURE);
      break;
    }
  }

  if (text == "") {
    cerr << "ERROR: No input provided. Use -i for direct input, -f for input "
            "from file or -h for help."
         << endl;
    exit(EXIT_FAILURE);
  }

  // MODE HANDLING
  if (mode == ENCODE) {
    if (key == "") {
      cerr << "ERROR: No key provided while in encode mode. To enter a key use "
              "-k. Use -h for help."
           << endl;
      exit(EXIT_FAILURE);
    }

    Vigenere v(text);
    out = v.encode(key);

  } else if (mode == DECODE) {
    if (key == "") {
      cerr << "ERROR: No key provided while in encode mode. To enter a key use "
              "-k. Use -h for help."
           << endl;
      exit(EXIT_FAILURE);
    }

    Vigenere v(text);
    out = v.encode(key);

  } else if (mode == ATTACK) {
    // check that there are attacks to do
    if (attack_queue.size() <= 0) {
      cerr << "ERROR: No attacks selected when in Attack mode." << endl;
      exit(EXIT_FAILURE);
    }

    // check if there is a crib attack in the queue so we can check if a crib
    // was provided
    vector<int> attacks_q_vec = misc::queue_to_vec(attack_queue);
    if (std::find(attacks_q_vec.begin(), attacks_q_vec.end(),
                  attacks::CRIB_ATTACK) != attacks_q_vec.end()) {
      if (crib == "") {
        cerr << "ERROR: No crib was provided when trying to use a crib attack."
             << endl;
        cerr << "Use -c to provide a crib directly or -C to get the crib from "
                "a file."
             << endl;
        exit(EXIT_FAILURE);
      }
    }

    auto result = attacks::Attacker::perform_attacks(text, attack_queue, period,
                                                     range, crib);
    key = result.first;
    out = result.second;

  } else {
    cerr << "ERROR: No mode set. Use -h for help." << endl;
    exit(EXIT_FAILURE);
  }

  // OUTPUT STAGE
  if (outfile == "") {
    if (mode == ATTACK) {
      cout << "KEY: " << key << endl;
    }
    cout << "RESULT:\n" << out << "\n" << endl;
  } else {
    misc::writeToFile(out, outfile);
    cout << "Wrote output to " << outfile << endl;
    if (mode == ATTACK) {
      cout << "KEY: " << key << endl;
    }
  }

  return EXIT_SUCCESS;
}

void printHelp() {}

void handholdMode() {
  bool hasKey;

  hasKey =
      misc::getYesNo("Do you know the key to encode/decode your text? (y/n): ");

  // simple encode/decode path
  if (hasKey) {
    known_key_handhold();
  } else { // unknown key path
    unknown_key_handhold();
  }
}

void known_key_handhold() {
  string text, key, output;
  bool textInFile, keyInFile, encode;

  textInFile = misc::getYesNo("Is your text in a file? (y/n): ");

  if (textInFile) {
    text = misc::getTextFromFile();
  } else {
    cout << "Enter your text to encode/decode: ";
    getline(cin, text);
  }

  keyInFile = misc::getYesNo("Is your key in a file (y/n): ");

  if (keyInFile) {
    key = misc::getTextFromFile();
  } else {
    cout << "Enter your key: ";
    getline(cin, key);

    string tmp;
    for (char c : key) {
      if (isalpha(c)) {
        tmp += c;
      }
    }
    key = tmp;
  }

  encode =
      misc::getYesNo("Do you wish to encode or decode? (e/d): ", {'e', 'd'});

  if (encode) {
    Vigenere vig(text);
    output = vig.encode(key);
  } else {
    Vigenere vig(text);
    output = vig.decode(key);
  }
  cout << "OUTPUT: " << endl;
  cout << output << endl;
}

void unknown_key_handhold() {
  string text;
  bool textInFile;

  textInFile = misc::getYesNo("Is the text you want to decode in a file?");

  if (textInFile) {
    text = misc::getTextFromFile();
  } else {
    cout << "Enter the text you want to decode: ";
    getline(cin, text);

    string tmp;
    for (char c : text) {
      if (isalpha(c)) {
        tmp += c;
      }
    }
    text = tmp;
  }
}

string mode_to_text(uint8_t mode) {

  switch (mode) {
  case NOT_SET:
    return "NOT SET";
  case ENCODE:
    return "ENCODE";
  case DECODE:
    return "DECODE";
  case ATTACK:
    return "ATTACK";
  }
  return "NOT SET";
}

void set_mode(uint8_t *mode, uint8_t new_mode) {
  if (*mode == NOT_SET) {
    *mode = new_mode;
  } else {
    cerr << "WARNING: Mode set more than once. Only first value will be "
            "used: "
         << mode_to_text(*mode) << endl;
  }
}
