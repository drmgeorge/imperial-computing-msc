#ifndef PLAYFAIR_H
#define PLAYFAIR_H

#define GRID_SIZE 6

//Question 1
void prepare(char* input, char* output);

//Question 2
void grid(char* codeword, char square[][GRID_SIZE]);
bool is_in_alphanum(char* alphanum, char letter);
void create_string(char* codeword, char* str);

//Question 3
void bigram(char square[][GRID_SIZE], char inchar1, char inchar2,
	    char& outchar1, char& outchar2);
int find_row(char square[][GRID_SIZE], char ch);
int find_col(char square[][GRID_SIZE], char ch);


//Question 4
void encode(char square[][GRID_SIZE], char* prepared, char* encoded);

//Bonus

void decode(char square[][GRID_SIZE], char* encoded, char* decoded);
	    
void print_square(char square[][GRID_SIZE]);

#endif //PLAYFAIR_H

