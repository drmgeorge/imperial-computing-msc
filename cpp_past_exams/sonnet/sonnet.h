#include <fstream>

// RESET is a constant used by function rhyming_letter(...)
#define RESET NULL
#define NUM_SCHEMES 3
#define SONNET_LEN 15

/* get_word(...) retrieves a word from the input string input_line
   based on its word number. If the word number is valid, the function
   places an uppercase version of the word in the output parameter
   output_word, and the function returns true. Otherwise the function
   returns false. */

bool get_word(const char *input_line, int number, char *output_word);

/* char rhyming_letter(const char *ending) generates the rhyme scheme
   letter (starting with 'a') that corresponds to a given line ending
   (specified as the parameter). The function remembers its state
   between calls using an internal lookup table, such that subsequents
   calls with different endings will generate new letters.  The state
   can be reset (e.g. to start issuing rhyme scheme letters for a new
   poem) by calling rhyming_letter(RESET). */

char rhyming_letter(const char *ending);

//Question 1
int count_words(const char* line);

//Question 2
void find_phonetic_ending(char* word, char* phonetic_ending);
void extract_line(ifstream& infile, char* line);
int get_phonetic_vowel_pos(int num_words, char* line);
bool starts_with_vowel(char* phoneme);
void set_phonetic_ending(char* phonetic_ending, char* line, int num_phonemes);

//Question 3
bool find_rhyme_scheme(char* filename, char* scheme);

//Question 4
char* identify_sonnet(char* filename);
