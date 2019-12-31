#define NUM_DIRECTIONS 4

/* You are pre-supplied with the functions below. Add your own 
   function prototypes to the end of this file. */

/* helper functions to allocate and deallocate dynamic 2D arrays */
char **allocate_2D_array(int rows, int columns);
void deallocate_2D_array(char **m, int rows);

/* pre-supplied function to load a maze from a file */
char **load_maze(const char *filename, int &height, int &width);

/* pre-supplied function to print a maze */
void print_maze(char **m, int height, int width);

//Question 1
bool find_marker(char ch, char** maze, int height, int width,
		 int& row, int& col);

//Question 2
bool valid_solution(char* path, char** maze, int height, int width);
bool try_move(int dir[], int cur_pos[], char** maze, int height, int width);
bool get_direction_index(char dir, int& dir1, int& dir2);


//Question 3
char* find_path(char** maze, int height, int width, char start, char end);
bool find_path_rec(char** maze, int cur_row, int cur_col,
		   int exit_row, int exit_col, char* solution,
		   int height, int width);

void remove_deadends(char** maze, int height, int width);
