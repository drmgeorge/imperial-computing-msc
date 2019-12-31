#include <iostream>
#include <cstring>
#include <cctype>
#include <cassert>
#include <map>
#include <string>

using namespace std;

#include "sonnet.h"

/* PRE-SUPPLIED HELPER FUNCTIONS START HERE */

/* NOTE: It is much more important to understand the interface to and
   the "black-box" operation of these functions (in terms of inputs and
   outputs) than it is to understand the details of their inner working. */

/* get_word(...) retrieves a word from the input string input_line
   based on its word number. If the word number is valid, the function
   places an uppercase version of the word in the output parameter
   output_word, and the function returns true. Otherwise the function
   returns false. */

bool get_word(const char *input_line, int word_number, char *output_word) {
  char *output_start = output_word;
  int words = 0;

  if (word_number < 1) {
    *output_word = '\0';
    return false;
  }
  
  do {
    while (*input_line && !isalnum(*input_line))
      input_line++;

    if (*input_line == '\0')
      break;

    output_word = output_start;
    while (*input_line && (isalnum(*input_line) || *input_line=='\'')) {
      *output_word = toupper(*input_line);
      output_word++;
      input_line++;
    }
    *output_word = '\0';

    if (++words == word_number)
      return true;

  } while (*input_line);

  *output_start = '\0';
  return false;
}

/* char rhyming_letter(const char *ending) generates the rhyme scheme
   letter (starting with 'a') that corresponds to a given line ending
   (specified as the parameter). The function remembers its state
   between calls using an internal lookup table, such that subsequents
   calls with different endings will generate new letters.  The state
   can be reset (e.g. to start issuing rhyme scheme letters for a new
   poem) by calling rhyming_letter(RESET). */

char rhyming_letter(const char *ending) {

  // the next rhyming letter to be issued (persists between calls)
  static char next = 'a';
  // the table which maps endings to letters (persists between calls)
  static map<string, char> lookup;

  // providing a way to reset the table between poems
  if (ending == RESET) {
    lookup.clear();
    next = 'a';
    return '\0';
  }

  string end(ending);

  // if the ending doesn't exist, add it, and issue a new letter
  if (lookup.count(end) == 0) {
    lookup[end]=next;
    assert(next <= 'z');
    return next++;
  }

  // otherwise return the letter corresponding to the existing ending
  return lookup[end];
}

/* START WRITING YOUR FUNCTION BODIES HERE */

int count_words(const char* line){

  char word[512];
  int num_words = 1;
  
  bool success = get_word(line, num_words, word);

  if(!success) { return 0; }
  
  while(success){
    num_words++;    
    success = get_word(line, num_words, word);    
  }

  return num_words - 1;
    
}

void find_phonetic_ending(char* word, char* phonetic_ending){


  phonetic_ending[0] = '\0';
  ifstream infile;
  ofstream outfile;
  bool word_found = false;
  char dict_word[512];
  
  infile.open("dictionary.txt");

  if(infile.fail()){
    cerr << "Error opening file" << endl;
    return;
  }

  infile >> dict_word;

  while(!infile.fail() && !word_found){
    infile >> dict_word;

    if(strcmp(word, dict_word) == 0) {
      word_found = true;

      //Extract the rest of the line
      char line[512];
      extract_line(infile, line);

      //Get number of words
      int num_words = count_words(line);

      //Start from the end word
      int phoneme_pos = get_phonetic_vowel_pos(num_words, line);

      if(phoneme_pos > 0){
	for(int ph = phoneme_pos; ph < num_words + 1; ph++){
	  char ph_part[512];
	  get_word(line, ph, ph_part);
	  strcat(phonetic_ending, ph_part);
	}
      }
    }
  }

  infile.close();
}

void extract_line(ifstream& infile, char* line){
  
  char ch;
  int ch_index = 0;
  
  infile.get(ch);    
  line[ch_index] = ch;
  ch_index++;
  
  while(ch != '\n'){
    infile.get(ch);    
    line[ch_index] = ch;
    ch_index++;
  }
  
  //terminate string
  line[ch_index] = '\0';
}


int get_phonetic_vowel_pos(int num_words, char* line){
  for(num_words; num_words > 0; num_words--){
    char phoneme[512];
    get_word(line, num_words, phoneme);

    if(starts_with_vowel(phoneme)){
      return num_words;
    }
  }
  return 0;
}

bool starts_with_vowel(char* phoneme){

  char vowels[6] = "AEIOU";
  int str_len = strlen(phoneme);

  for(int v = 0; v < 5; v++){
    if(phoneme[0] == vowels[v]){
      return true;
    }
  }

  return false;
}

bool find_rhyme_scheme(char* filename, char* scheme){

  ifstream infile;
  ofstream outfile;
  string line;
  int line_num = 0;
  
  infile.open(filename);
  rhyming_letter(RESET);  

  if(infile.fail()){
    return false;
  }

  getline(infile, line);
  
  while(!infile.fail()){

    int last_word_pos = count_words(line.c_str());
    char last_word[512];
    char phonetic_ending[512];  
    char scheme_letter;

    bool success = get_word(line.c_str(), last_word_pos, last_word); 
    if(!success) { cout << "get_word failed "; return false; }

    find_phonetic_ending(last_word, phonetic_ending);
    scheme_letter = rhyming_letter(phonetic_ending);
    scheme[line_num] = scheme_letter;
    
    getline(infile, line);
    line_num++;    

  }
   return true;
}

char* identify_sonnet(char* filename){

  char shakesperian[SONNET_LEN] = "ababcdcdefefgg";
  char petrarchan[SONNET_LEN] = "abbaabbacdcdcd";
  char spenserian[SONNET_LEN] = "ababbcbccdcdee";

  char* all_schemes[NUM_SCHEMES] = { shakesperian, petrarchan, spenserian };
  char* scheme_poet[NUM_SCHEMES] = { "Shakesperian", "Petrarchan", "Spenserian" };
  ifstream infile;
  char scheme[512];  

  bool success = find_rhyme_scheme(filename, scheme);

  if(success){

    for(int i = 0; i < NUM_SCHEMES; i++){
      int is_sonnet = strcmp(scheme, all_schemes[i]);

      if(is_sonnet == 0) {
	return scheme_poet[i];
      }
    }
  }

  cerr << "Sonnet could not be identified" << endl;
  return nullptr;

}
