#include <iostream>
#include <cctype>
#include <cstring>
#include "playfair.h"

using namespace std;

void prepare(char* input, char* output){

  int str_len = strlen(input);
  int next = 0;
		
  for(int i = 0; i < str_len; i++){
    if(isalnum(input[i])){
      output[next] = toupper(input[i]);
      next++;
    }
  }

  //Add an X if odd number of characters
  if(next % 2 == 1){
    output[next] = 'X';
    next++;
  }
  
  //Add terminating character
  output[next] = '\0';
}
void grid(char* codeword, char square[][GRID_SIZE]){

  char str[36];
  int str_index = 0;
  create_string(codeword, str);

  //Fill the grid
  for(int row = 0; row < GRID_SIZE; row++){
    for(int col = 0; col < GRID_SIZE; col++){
      if(str[str_index] != '\0'){
	square[row][col] = str[str_index];
	str_index++;
      }
    }
  }
}

void create_string(char* codeword, char* str){

  char alphanum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  
  //Iterate through codeword first

  int word_len = strlen(codeword);
  int alphanum_len = strlen(alphanum);
  int ch_index = 0;
  
  for(int ch = 0; ch < word_len; ch++){
    if(is_in_alphanum(alphanum, codeword[ch])){
      str[ch_index] = codeword[ch];
      ch_index++;
    }
  }

  //Fill in the rest of the str
  for(int ch = 0; ch < alphanum_len; ch++){
    if(isalnum(alphanum[ch])){
      str[ch_index] = alphanum[ch];
      ch_index++;
    }
  }

  str[ch_index] = '\0';
  cout << "str : " << str << endl;
}

bool is_in_alphanum(char* alphanum, char letter){

  int alphanum_len = strlen(alphanum);

  for(int i = 0; i < alphanum_len; i++){

    if(letter == alphanum[i]){
      cout << "letter found " << alphanum[i] << endl;      
      alphanum[i] = '_';
      return true;
    }
    
  }

  return false;
}

void bigram(char square[][GRID_SIZE], char inchar1, char inchar2,
	    char& outchar1, char& outchar2){

  //For outchar 1
  int row = find_row(square, inchar1);
  int col = find_col(square, inchar2);  

  if(row == -1 || col == -1){
    cerr << "Could not find letter in grid" << endl;
    return;
  }
  
  outchar1 = square[row][col];

  //For outchar 2
  row = find_row(square, inchar2);
  col = find_col(square, inchar1);

  if(row == -1 || col == -1){
    cerr << "Could not find letter in grid" << endl;
    return;
  }
  
  outchar2 = square[row][col];

}

int find_row(char square[][GRID_SIZE], char ch){
  for(int row = 0; row < GRID_SIZE; row++){
    for(int col = 0; col < GRID_SIZE; col++){

      if(square[row][col] == ch){
	return row;
      }
    }
  }
  return -1;
}

int find_col(char square[][GRID_SIZE], char ch){
  for(int row = 0; row < GRID_SIZE; row++){
    for(int col = 0; col < GRID_SIZE; col++){

      if(square[row][col] == ch){
	return col;
      }
    }
  }
  return -1;
}

void encode(char square[][GRID_SIZE], char* prepared, char* encoded){

  //Base case
  if(prepared[0] == '\0'){
    encoded[0] = '\0';
    return;
  }

  //Do the encoding
  char outchar1;
  char outchar2;
  
  bigram(square, prepared[0], prepared[1], outchar1, outchar2);

  encoded[0] = outchar1;
  encoded[1] = outchar2;

  
  //Repeat
  encode(square, prepared + 2, encoded + 2);
}

void decode(char square[][GRID_SIZE], char* encoded, char* decoded){
  encode(square, encoded, decoded);
}
