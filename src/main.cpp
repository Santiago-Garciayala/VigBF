#include "./Vigenere.h"
#include "Attacker.h"
#include "misc.h"
#include <array>
#include <cstdlib>
#include <iostream>
#include <string>

extern "C" {
#include <getopt.h>
#include <unistd.h>
}

using namespace std;

void handholdMode();
void known_key_handhold();
void unknown_key_handhold();

const char *SHORT_OPTS = "ed:f:m:l:r:t:ThHv";
const struct option LONG_OPTS[] = {{"encode", no_argument, nullptr, 'e'},
                                   {"decode", no_argument, nullptr, 'd'},
                                   {"input", required_argument, nullptr, 'i'},
                                   {"file", required_argument, nullptr, 'f'},
                                   {"mode", required_argument, nullptr, 'm'},
                                   {"length", required_argument, nullptr, 'l'},
                                   {"range", required_argument, nullptr, 'r'},
                                   {"threads", required_argument, nullptr, 't'},
                                   {"help", optional_argument, nullptr, 'h'},
                                   {"Handhold", no_argument, nullptr, 'H'},
                                   {"verbose", no_argument, nullptr, 'v'},
                                   {0, 0, 0, 0}};

int main(int argc, char *argv[]) {
  char c;
  string out = "";
  string ciphertext;
  string plaintext;

  while ((c = getopt_long(argc, argv, SHORT_OPTS, LONG_OPTS, nullptr)) != -1) {
    switch (c) {
    case 'e':

      break;
    case 'd':
      // insert code here

      break;
    case 'f':
      // insert code here

      break;
    case 'm':
      // insert code here

      break;
    case 'l':
      // insert code here

      break;
    case 'r':
      // insert code here

      break;
    case 't':
      // insert code here

      break;
    case 'T': {
      // TEST MODE
      std::string tesxt = misc::getTextFromFile("resources/books/pg22400.txt");
      //      tesxt =
      //      "hellomynameisthefollowingstringofnumbersthatifoundonthestreether"
      //             "eyougo";
      Vigenere v(misc::stringToLower(tesxt));
      attacks::Attacker a;
      std::string key = "bogosl";
      std::string encoded_text = v.encodeNoAlpha(key);
      // cout << "encoded text: " << encoded_text << std::endl;
      int period = a.get_period(encoded_text);
      cout << "period: " << period << std::endl;
      cout << "fitness: " << a.fitness(v.getTextOnlyAlpha()) << std::endl;
      cout << "fitness of encoded: " << a.fitness(v.encodeNoAlpha(key));

      Vigenere v2(encoded_text);
      std::string attack_result;
      // attack_result = a.brute_force_single_thread(v2, 2, 3).second;
      // attack_result = a.dictionary_attack(v2).second;
      // attack_result = a.crib_attack(v2, v.getTextOnlyAlpha().substr(4,
      // 4)).first;
      //    attack_result = a.variational_attack(v2, period).second;
      attack_result = a.stats_attack(v2, 6).second;
      cout << attack_result << std::endl;

      break;
    }
    case 'h':
      // insert code here

      break;
    case 'H':
      handholdMode();
      exit(EXIT_SUCCESS);

      break;
    case 'v':
      // insert code here

      break;
    case '?':
      cerr << "Invalid option: " << optopt << ". Use -h for help.\n";
      break;
    default:
      cerr << "Invalid option: " << optopt << ". Use -h for help.\n";
      break;
    }
  }

  cout << out << "\n" << endl;

  return 0;
}

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
