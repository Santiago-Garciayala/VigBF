#include <algorithm>
#include <cctype>
#include <stdexcept>
#include <string>

using namespace std;

class Vigenere {
private:
  string text;
  string text_only_alpha;

  static string removeNonAlpha(string in) {
    string out;
    out.reserve(in.length());

    for (char c : in) {
      if (isalpha(c))
        out += c;
    }

    return out;
  }

public:
  Vigenere(string text) {
    this->text = text;
    text_only_alpha = removeNonAlpha(text);
  }

  /**
   * Decodes ciphertext with the key provided. Validates that all input is
   * alphabetic and converts it to lowercase for uniformity. A version of this
   * function is available for when performance is preferred over input
   * validation: Vigenere::decodeUnchecked(). The option -u makes the program
   * use this function instead.
   */
  static string staticDecode(string ciphertext, string key) {
    string plaintext;
    char input_case = islower(key[0]) ? 'a' : 'A';

    plaintext.reserve(ciphertext.length());

    // convert to lowercase
    /*
    transform(ciphertext.begin(), ciphertext.end(), ciphertext.begin(),
              ::tolower);
    transform(key.begin(), key.end(), key.begin(), ::tolower);
    */

    // main loop
    for (int i = 0; i < ciphertext.length(); ++i) {
      char c = (ciphertext[i] - key[i % key.length()] + 26) % 26;
      c += 'a';
      plaintext.push_back(c);
    }

    return plaintext;
  }

  /**
   * Encodes text with the key provided. Validates that all input is
   * alphabetic and converts it to lowercase for uniformity. A version of this
   * function is available for when performance is preferred over input
   * validation: Vigenere::encodeUnchecked(). The option -u makes the program
   * use this function instead.
   */
  static string staticEncode(string plaintext, string key) {
    string ciphertext;
    char input_case = islower(key[0]) ? 'a' : 'A';

    ciphertext.reserve(plaintext.length());

    // main loop
    for (int i = 0; i < plaintext.length(); ++i) {
      char c = ((plaintext[i] - 'a') + (key[i % key.length()] - 'a')) % 26;
      c += 'a';
      ciphertext.push_back(c);
    }

    return ciphertext;
  }
};
