enum MessageStatus {
  INVALID_HEADER=-1,
  WRONG_RECIPIENT=-2,
  INVALID_MESSAGE_DIGEST=-3,
  INVALID_HEADER_DIGEST=-4,
  VALID_EMAIL=1
};

/* pre-supplied helper function */
/* generates the SHA1 hash of input string text into output parameter digest */
void text_to_SHA1_digest(const char *text, char *digest);

//Question 1
int leading_zeros(char* digest);

//Question 2
bool file_to_SHA1_digest(const char* filename, char* digest);

//Question 3
bool make_header(const char* recipient, const char* filename, char* digest);
void prepare_header(const char* recipient, const char* filename,
		     char* digest, int counter, char* header);

bool convert_to_string(int number, char* str);

//Question 4
MessageStatus check_header(const char* email_address, char* header,
		  const char* filename);

bool verify_email(const char* token, const char* email_address);
bool verify_message_digest(const char* token, const char* filename);

