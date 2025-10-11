#include "./Vigenere.h"
#include "Attacker.h"
#include "misc.h"
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
void perform_attacks(string input, queue<int> attack_queue);
void perform_attacks(vector<string> inputs, queue<int> attack_queue);

const char *SHORT_OPTS = "ed:f:o:m:p:r:t:ThHvs:";
const struct option LONG_OPTS[] = {{"encode", no_argument, nullptr, 'e'},
                                   {"decode", no_argument, nullptr, 'd'},
                                   {"input", required_argument, nullptr, 'i'},
                                   {"file", required_argument, nullptr, 'f'},
                                   {"output", required_argument, nullptr, 'o'},
                                   {"mode", required_argument, nullptr, 'm'},
                                   {"period", required_argument, nullptr, 'l'},
                                   {"range", required_argument, nullptr, 'r'},
                                   {"threads", required_argument, nullptr, 't'},
                                   {"help", optional_argument, nullptr, 'h'},
                                   {"Handhold", no_argument, nullptr, 'H'},
                                   {"verbose", no_argument, nullptr, 'v'},
                                   {"serial", optional_argument, nullptr, 's'},
                                   {0, 0, 0, 0}};
const size_t DEFAULT_THREADS = 1;

int main(int argc, char *argv[]) {
  char c;
  string out = "";
  string outfile = "";
  vector<string> inputs;
  vector<int> periods;
  vector<pair<int, int>> ranges;
  queue<int> attack_queue;
  size_t threads = DEFAULT_THREADS;
  bool verbose = false;

  while ((c = getopt_long(argc, argv, SHORT_OPTS, LONG_OPTS, nullptr)) != -1) {
    switch (c) {
    case 'e':
      attack_queue.push(attacks::ENCODE);
      break;
    case 'd':
      attack_queue.push(attacks::DECODE);
      break;
    case 'i':
      // insert code here
      inputs.push_back(optarg);
      break;
    case 'f':
      inputs.push_back(misc::getTextFromFile(optarg));
      break;
    case 'm':
      uint8_t attack_num;
      try {
        attack_num = stoi(optarg);
      } catch (...) {
        cerr << "Invalid input for attack type: " << optarg << endl;
        break;
      }
      if (attack_num >= attacks::ENCODE && attack_num < attacks::ATTACKS_LAST) {
        inputs.push_back(optarg);
      } else {
        cerr << "Invalid attacks number: " << optarg
             << ". Valid attack numbers: " << attacks::ENCODE << "-"
             << attacks::ATTACKS_LAST - 1 << endl;
      }
      break;
    case 'p':
      uint8_t period;
      try {
        period = stoi(optarg);
      } catch (...) {
        cerr << "Invalid input for period: " << optarg << endl;
        break;
      }
      periods.push_back(period);

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

      } catch (...) {
        cerr << "ERROR: Range should be inputted as X-Y, where X and Y are "
                "whole numbers. Input read: "
             << optarg << endl;
      }

      pair<int, int> r(low, high);
      ranges.push_back(r);

      break;
    }
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
      std::string encoded_text = v.encodeNoAlpha(key);
      // cout << "encoded text: " << encoded_text << std::endl;
      int period = a.get_period_kasiski(encoded_text);
      // period = 6;
      cout << "period: " << period << std::endl;
      cout << "fitness: " << a.fitness(v.getTextOnlyAlpha()) << std::endl;
      cout << "fitness of encoded: " << a.fitness(v.encodeNoAlpha(key))
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
      exit(EXIT_SUCCESS);
      break;
    default:
      cerr << "Invalid option: " << char(optopt) << ". Use -h for help.\n";
      exit(EXIT_SUCCESS);
      break;
    }
  }

  if (inputs.size() == 0) {
    cerr << "ERROR: No input provided. Use -i for direct input, -f for input "
            "from file or -h for help."
         << endl;
  }

  if (attack_queue.size() > 0) {
    perform_attacks(inputs, attack_queue);
  }

  if (outfile == "") {
    cout << out << "\n" << endl;
  } else {
    misc::writeToFile(out, outfile);
  }

  return 0;
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
void perform_attacks(string input, queue<int> attack_queue) {}

void perform_attacks(vector<string> inputs, queue<int> attack_queue) {}
