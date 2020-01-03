#include <iostream>
#include <openssl/sha.h>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <fstream>
#include <cctype>
#include <string>
#include "stamp.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

// helper function for internal use only
// transforms raw binary hash value into human-friendly hexademical form
void convert_hash(const unsigned char *str, char *output, int hash_length) {
  char append[16];
  strcpy (output, "");
  for (int n=0; n<hash_length; n++) {
    sprintf(append,"%02x",str[n]);
    strcat(output, append);
  }
}

// pre-supplied helper function
// generates the SHA1 hash of input string text into output parameter digest
// ********************** IMPORTANT **************************
// ---> remember to include -lcrypto in your linking step <---
// ---> so that the definition of the function SHA1 is    <---
// ---> included in your program                          <---
// ***********************************************************
void text_to_SHA1_digest(const char *text, char *digest) {
  unsigned char hash[SHA_DIGEST_LENGTH];
  SHA1( (const unsigned char *) text, strlen(text), hash);
  convert_hash(hash, digest, SHA_DIGEST_LENGTH);
}

/* add your function definitions here */

int leading_zeros(char* digest){
  //Iterate over the cstring looking for leading zeros
  int num_zeros = 0;
  int digest_len = strlen(digest);
  
  for(int ch = 0; ch < digest_len; ch++){

    //Not valid hex digi if nonzero    
    if(isxdigit(digest[ch]) == 0){
      return -1;
    }

    //Look for leading zeros
    if(digest[ch] != '0'){
      return num_zeros;
    }
    else{
      num_zeros++;
    }
  }
  //Got to end of digest string
  return num_zeros;
}


bool file_to_SHA1_digest(const char* filename, char* digest){

  ifstream infile;
  char ch;
  int str_count = 0;

  infile.open(filename);

  if(infile.fail()){
    strcpy(digest, "error");
    return false;
  }

  //Get the size of the file
  infile.seekg(0, infile.end);
  int length = infile.tellg();
  infile.seekg(0, infile.beg);   

  //To handle large file (message3.txt)
  char* undigested = new (nothrow)char[length + 1];
  if(undigested == NULL) {
    strcpy(digest, "error");
    return false;
  }
  
  //Get the first character
  infile.get(ch);

  //Iterate through the rest of the filen
  while(!infile.fail()){
    *(undigested + str_count) = ch;
    str_count++;
    infile.get(ch);
  }

  //Terminate string, convert and close file
  *(undigested + str_count) = '\0';
  text_to_SHA1_digest(undigested, digest);
  infile.close();
  return true;

}

void prepare_header(const char* recipient, const char* filename,
		    char* digest, int counter, char* header){

  char msg_digest[41];
  file_to_SHA1_digest(filename, msg_digest);
  strcpy(header, recipient);
  strcat(header, ":");
  strcat(header, msg_digest);
  strcat(header, ":");

  //Create string from counter
  string counter_str = std::to_string(counter);
  strcat(header, counter_str.c_str());



}

//Question 3
bool make_header(const char* recipient, const char* filename, char* digest){

  int counter = 0;
  char header[512];
  
  //Prepare header
  prepare_header(recipient, filename, digest, counter, header);
  
  //Convert header to SHA1
  text_to_SHA1_digest(header, digest);
  
  while(leading_zeros(digest) != 5){
    
    counter++;
    prepare_header(recipient, filename, digest, counter, header);
    
    //Convert header to SHA1
    text_to_SHA1_digest(header, digest);

    //If 1 million attempts made unsuccessfully, return
    if(counter > 1000000){
      return false;
    }
  }

  return true;
}

MessageStatus check_header(const char* email_address, char* header,
		  const char* filename){

  char* token;
  char header_cpy[512];
  int num_fields = 0;
  strcpy(header_cpy, header);
  token = strtok(header_cpy, ":");

  while(token != NULL){

    num_fields++;

    //Check email address matches
    if(num_fields == 1) {
      if(!verify_email(token, email_address)) { return WRONG_RECIPIENT; }
    }

    //Check message digest matches    
    if(num_fields == 2) {
      if(!verify_message_digest(token, filename)) { return INVALID_MESSAGE_DIGEST; }
    }

    token = strtok(NULL, ":");
  }

  //Check the header has 3 parts (separated by semicolons)
  if(num_fields != 3) { return INVALID_HEADER; }

  //Check the digest of the header is acceptable
  char hdr_digest[41];
  text_to_SHA1_digest(header, hdr_digest);
  if(leading_zeros(hdr_digest) != 5){
    return INVALID_HEADER_DIGEST;
  }

  return VALID_EMAIL;
}

bool verify_email(const char* token, const char* email_address){
  if(strcmp(token, email_address) == 0){
    return true;
  }
  return false;
}

bool verify_message_digest (const char* token, const char* filename){
  char digest[41];
  file_to_SHA1_digest(filename, digest);

  if(strcmp(token, digest) == 0){
    return true;
  }
  return false;
}



