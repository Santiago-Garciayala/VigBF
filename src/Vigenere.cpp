#include "Vigenere.h"
#include "misc.h"
#include <cctype>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

void Vigenere::removeNonAlpha() {
  this->processed_text.reserve(this->text.length());

  for (int i = 0; i < this->text.length(); ++i) {
    if (isalpha(this->text[i]))
      this->processed_text += this->text[i];
    else {
      non_alpha_removed[i] = this->text[i];
      non_alpha_removed_keys.push_back(i);
    }
  }
}

string Vigenere::addNonAlpha(string &in) {
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

void Vigenere::check_uppers() {
  for (int i = 0; i < this->processed_text.length(); ++i) {
    if (isupper(this->processed_text[i]))
      upper_letters.push_back(i);
  }
  this->processed_text = misc::stringToLower(this->processed_text);
}

string Vigenere::add_uppers(string &in) {
  string out = in;

  if (upper_letters.size() == 0)
    return out;

  for (int i = 0, j = 0; i < out.length(); ++i) {
    if (i == upper_letters[j]) {
      out[i] = toupper(out[i]);
      ++j;
    }
  }
  return out;
}

void Vigenere::process_in() {
  removeNonAlpha();
  check_uppers();
}

string Vigenere::process_out(string &in) {
  string out = in;
  out = add_uppers(out);
  out = addNonAlpha(out);
  return out;
}

Vigenere::Vigenere(string text) {
  this->text = text;
  process_in();
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

  for (int i = 0; i < plaintext.length(); ++i) {
    char c = ((plaintext[i] - 'a') + (key[i % key.length()] - 'a')) % 26;
    c += input_case;
    ciphertext.push_back(c);
  }

  return ciphertext;
}

string Vigenere::decode(string key) {
  string decoded = staticDecode(this->processed_text, key);
  decoded = process_out(decoded);
  return decoded;
}

string Vigenere::encode(string key) {
  string encoded = staticEncode(this->processed_text, key);
  encoded = process_out(encoded);
  return encoded;
}

string Vigenere::decode_processed(string key) {
  string decoded = staticDecode(this->processed_text, key);
  return decoded;
}

string Vigenere::encode_processed(string key) {
  string encoded = staticEncode(this->processed_text, key);
  return encoded;
}

string Vigenere::decode_raw(string key) {
  return staticDecode(this->processed_text, key);
}

string Vigenere::encode_raw(string key) {
  return staticEncode(this->processed_text, key);
}

string Vigenere::getText() { return this->text; }

void Vigenere::setText(string text) {
  non_alpha_removed.clear();
  non_alpha_removed_keys.clear();
  upper_letters.clear();

  this->text = text;
  process_in();
}

string Vigenere::get_processed_text() { return this->processed_text; }
string &Vigenere::get_processed_text_ref() { return this->processed_text; }
