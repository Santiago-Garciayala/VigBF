#ifndef VIGENERE_H
#define VIGENERE_H

#include <cctype>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

// TODO: change most strings to references so we stop cloning and wasting memory
// all the time
class Vigenere {

private:
  string text;
  string processed_text;
  unordered_map<int, char> non_alpha_removed;
  vector<int> non_alpha_removed_keys;
  vector<int> upper_letters;

  void removeNonAlpha();
  string addNonAlpha(string &in);
  void check_uppers();
  string add_uppers(string &in);
  void process_in();
  string process_out(string &in);

public:
  Vigenere(string text);
  static string staticDecode(string ciphertext, string key);
  static string staticEncode(string plaintext, string key);
  string decode(string key);
  string encode(string key);
  string decode_processed(string key);
  string encode_processed(string key);
  string decode_raw(string key);
  string encode_raw(string key);
  string getText();
  void setText(string text);
  string get_processed_text();
  string &get_processed_text_ref();
};

#endif
