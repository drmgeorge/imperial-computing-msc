#include <iostream>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <cctype>
#include <fstream>

using namespace std;

#include "dictionary.h"
#include "doublets.h"

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* Looks up a given word in the dictionary of approved words. 
   Returns true if the word is in the dictionary.
   Otherwise returns false. */

bool dictionary_search(const char *word) {
  static Dictionary dictionary("words.txt");
  return dictionary.search(word);
}

/* add your function definitions here */

bool valid_step(const char* current_word, const char* next_word){

  //Rule 2

  //Words must be the same length
  int cw_len = strlen(current_word);
  int nw_len = strlen(next_word);

  if(cw_len != nw_len) { return false; }

  //Only one letter much be changed
  int num_changed = 0;
  for(int ch = 0; ch < cw_len; ch++){
    if(current_word[ch] != next_word[ch]){
      num_changed++;
    }
  }

  if(num_changed > 1) { return false; }

  //Rule 4 - letter must be in the dictionary
  if(!dictionary_search(next_word)) { return false; }

  return true;
  
}

bool display_chain(const char* chain[], ostream& outs){

  int word = 1;
  int num_words = 0;

  for(int i = 0; chain[i] !=NULL; i++){
    num_words++;
  }

  //Print first word
  outs << chain[0] << endl;

  //Print next words except last
  for(word; word < num_words - 1; word++){

    int length = strlen(chain[word]);
    char* word_lower = new char[length];
    
    strcpy(word_lower, chain[word]);
    convert_to_lower(word_lower, length);
    
    outs << word_lower << endl;
  }

  //Print last word
  outs << chain[num_words - 1] << endl;
}

void convert_to_lower(char* word, int length){
  
  for(int ch = 0; ch < length; ch++){
    word[ch] = tolower(word[ch]);
  }
}

bool valid_chain(const char* chain[]){
  
  int num_words = 0;
  for(int i = 0; chain[i] !=NULL; i++){
    num_words++;
  }

  //Chain has to be more than 1 in length
  if(num_words <= 1) { return false; }
  
  //No repeated words in the chain
  for(int i = 0; chain[i]; i++){
    for(int j = 0; chain[j]; j++){
      if(strcmp(chain[i], chain[j]) == 0) {
	return false;
      }
    }
  }
  
  //Each step in the chain is valid
  for(int word = 0; word < num_words -1; word++){
    if(!valid_step(chain[word], chain[word+1])){
      return false;
    }
  }
  
  //If all well
  return true;
}


bool find_chain(const char* start_word, const char* target_word,
	        const char* answer_chain[], int max_steps, int pos,
		bool first_run){

  if(pos == 0){
    insert_into_chain(start_word, answer_chain, 0);
    pos++;
  }

  //BASE CASES:

  //1) max_steps < 0
  if(max_steps < 0){
    return false;
  }

  //2) word not in dictionary
  if(!dictionary_search(start_word)){
    return false;
  }

  //3) not a valid chain
  if(valid_chain(answer_chain)){
    return false;
  }

  if(strcmp(start_word, target_word) == 0){
    return true;
  }

  char alpha[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
  
  int word_len = strlen(start_word);
  char test_word[256];
  strcpy(test_word, start_word);

  for(int ch = 0; ch < word_len; ch++){
    for(int al = 0; al < 26; al++){
      
      test_word[ch] = alpha[al];	

      //Insert into answer chain
      char* word_to_add = new char[word_len];
      strcpy(word_to_add, test_word);

       insert_into_chain(word_to_add, answer_chain, pos);

      //print_chain(answer_chain);
      
      if(find_chain(test_word, target_word, answer_chain,
		    max_steps-1, pos+1)){
	return true;
      }
    }//End of for loop for alphabet
    answer_chain[pos] = NULL;
    strcpy(test_word, start_word);
  }

  //Exhausted all possibilities
  return false;
}

void print_chain(const char* answer_chain[]){
  for(int i = 0; answer_chain[i]; i++){
    cout << answer_chain[i] << "->";
  }
  cout << endl;

}


bool insert_into_chain(const char* test_word, const char* answer_chain[], int pos){

  if(pos == 0){
    answer_chain[pos] = test_word;
    answer_chain[pos + 1] = NULL;    
    return true;
  }
  
  for(int i = 0; answer_chain[i]; i++){
    if(strcmp(answer_chain[i], test_word) == 0){
      return false;
    }
  }

  answer_chain[pos] = test_word;
  answer_chain[pos + 1] = NULL;          
  return true;
}
