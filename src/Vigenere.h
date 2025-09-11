#ifndef VIGENERE_H
#define VIGENERE_H

#include <cctype>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class Vigenere {

private:
  string text;
  string text_only_alpha;
  unordered_map<int, char> non_alpha_removed;
  vector<int> non_alpha_removed_keys;

  void removeNonAlpha();
  string addNonAlpha(string in);

public:
  Vigenere(string text);
  static string staticDecode(string ciphertext, string key);
  static string staticEncode(string plaintext, string key);
  string decode(string key);
  string encode(string key);
  string decodeRaw(string key);
  string encodeRaw(string key);
  string getText();
  void setText(string text);
  string getTextOnlyAlpha();
};

#endif
