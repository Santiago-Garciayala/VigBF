#include "Vigenere.h"
#include <cctype>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

void Vigenere::removeNonAlpha() {
  this->text_only_alpha.reserve(this->text.length());

  for (int i = 0; i < this->text.length(); ++i) {
    if (isalpha(this->text[i]))
      this->text_only_alpha += this->text[i];
    else {
      non_alpha_removed[i] = this->text[i];
      non_alpha_removed_keys.push_back(i);
    }
  }
}

string Vigenere::addNonAlpha(string in) {
  string outstr = in;
  outstr.resize(in.length() + non_alpha_removed.size());

  int offset = non_alpha_removed_keys.size();
  for (int i = outstr.length(); offset > 0; --i) {
    if (non_alpha_removed_keys[offset - 1] == i) {
      outstr[i] = non_alpha_removed[i];
      --offset;
      continue;
    }

    outstr[i] = outstr[i - offset];
  }

  return outstr;
}

Vigenere::Vigenere(string text) {
  this->text = text;
  removeNonAlpha();
}

/**
 * Decodes ciphertext with the key provided. Validates that all input is
 * alphabetic and converts it to lowercase for uniformity. A version of this
 * function is available for when performance is preferred over input
 * validation: Vigenere::decodeUnchecked(). The option -u makes the program
 * use this function instead.
 */
string Vigenere::staticDecode(string ciphertext, string key) {
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
    c += input_case;
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
string Vigenere::staticEncode(string plaintext, string key) {
  string ciphertext;
  char input_case = islower(key[0]) ? 'a' : 'A';

  ciphertext.reserve(plaintext.length());

  // main loop
  for (int i = 0; i < plaintext.length(); ++i) {
    char c = ((plaintext[i] - 'a') + (key[i % key.length()] - 'a')) % 26;
    c += input_case;
    ciphertext.push_back(c);
  }

  return ciphertext;
}

string Vigenere::decode(string key) {
  string decoded = staticDecode(this->text_only_alpha, key);
  decoded = addNonAlpha(decoded);
  return decoded;
}

string Vigenere::encode(string key) {
  string encoded = staticEncode(this->text_only_alpha, key);
  encoded = addNonAlpha(encoded);
  return encoded;
}

string Vigenere::decodeNoAlpha(string key) {
  string decoded = staticDecode(this->text_only_alpha, key);
  return decoded;
}

string Vigenere::encodeNoAlpha(string key) {
  string encoded = staticEncode(this->text_only_alpha, key);
  return encoded;
}

string Vigenere::decodeRaw(string key) {
  return staticDecode(this->text_only_alpha, key);
}

string Vigenere::encodeRaw(string key) {
  return staticEncode(this->text_only_alpha, key);
}

string Vigenere::getText() { return this->text; }

void Vigenere::setText(string text) {
  non_alpha_removed.clear();
  non_alpha_removed_keys.clear();

  this->text = text;
  removeNonAlpha();
}

string Vigenere::getTextOnlyAlpha() { return this->text_only_alpha; }
string &Vigenere::getTextOnlyAlpha_ref() { return this->text_only_alpha; }
