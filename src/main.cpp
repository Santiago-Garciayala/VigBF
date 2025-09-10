#include "./Vigenere.cpp"
#include <array>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>

extern "C" {
#include <getopt.h>
#include <unistd.h>
}

using namespace std;

void handholdMode();
void known_key_handhold();
void unknown_key_handhold();
bool getYesNo(string display, const array<char, 2> chars = {'y', 'n'});
string getTextFromFile();

const char *SHORT_OPTS = "ed:f:m:l:r:t:hHv";
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

  hasKey = getYesNo("Do you know the key to encode/decode your text? (y/n): ");

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

  textInFile = getYesNo("Is your text in a file? (y/n): ");

  if (textInFile) {
    text = getTextFromFile();
  } else {
    cout << "Enter your text to encode/decode: ";
    getline(cin, text);
  }

  keyInFile = getYesNo("Is your key in a file (y/n): ");

  if (keyInFile) {
    key = getTextFromFile();
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

  encode = getYesNo("Do you wish to encode or decode? (e/d): ", {'e', 'd'});

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

void unknown_key_handhold() {}

bool getYesNo(string display, const array<char, 2> chars) {
  char c;

  cout << display;
  cin >> c;
  c = tolower(c);

  // clear kb buffer
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  while (c != chars[0] && c != chars[1]) {
    cout << "Invalid input. Enter '" << chars[0] << "' or '" << chars[1] << "'."
         << endl;
    cout << display;
    cin >> c;
    c = tolower(c);

    // clear kb buffer
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
  }

  return c == chars[0];
}

string getTextFromFile() {
  ifstream file;
  string filename, text;

  cout << "Enter the name of the file: ";
  getline(cin, filename);

  file.open(filename);

  while (!file.is_open() && filename != "q") {
    cout << filename
         << " could not be opened. Enter a different file name or 'q' to "
            "quit."
         << endl;
    cout << "Enter the name of the file: ";
    getline(cin, filename);
    file.open(filename);
  }

  if (filename == "q") {
    exit(EXIT_SUCCESS);
  }

  string line;
  while (getline(file, line)) {
    text += line + "\n";
  }
  file.close();

  return text;
}
