#include "./Vigenere.cpp"
#include <iostream>
#include <string>

extern "C" {
#include <getopt.h>
#include <unistd.h>
}

using namespace std;

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
  string ciphertext = nullptr;
  string plaintext = nullptr;

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
      // insert code here

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

  cout << out << "\n";

  return 0;
}
