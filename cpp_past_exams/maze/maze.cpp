#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include "maze.h"

using namespace std;

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* helper function for internal use only which gets the dimensions of a maze */
bool get_maze_dimensions(const char *filename, int &height, int &width) {
  char line[512];
  
  ifstream input(filename);

  height = width = 0;

  input.getline(line,512);  
  while (input) {
    if ( (int) strlen(line) > width)
      width = strlen(line);
    height++;
    input.getline(line,512);  
  }

  if (height > 0)
    return true;
  return false;
}

/* pre-supplied function to load a maze from a file*/
char **load_maze(const char *filename, int &height, int &width) {

  bool success = get_maze_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width);
  
  ifstream input(filename);

  char line[512];

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
  }
  
  return m;
}

/* pre-supplied function to print a maze */
void print_maze(char **m, int height, int width) {
  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(4) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(4) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << m[r][c];
    cout << endl;
  }
}

bool find_marker(char ch, char** maze, int height, int width,
		 int& row, int& col){

  for(int h = 0; h < height; h++){
    for(int w = 0; w < width; w++){
      if(maze[h][w] == ch){
	row = h;
	col = w;
	return true;
      }
    }
  }
  //If not found
  row = -1; col = -1;
  return false;
}

bool valid_solution(char* path, char** maze, int height, int width){

  //Find the start of the maze
  int row = 0; int col = 0;
  find_marker('>', maze, height, width, row, col);
  int cur_pos[2] = { row, col };

  //Determine where the exit is
  int exit_row; int exit_col;
  find_marker('X', maze, height, width, exit_row, exit_col);

  //Try each path in turn
  int path_len = strlen(path);  
  for(int p = 0; p < path_len; p++){

    int dir1; int dir2;
    bool result = get_direction_index(path[p], dir1, dir2);

    if(!result) { cerr << "Error encoutered" << endl; return false; }

    int dir[2] = {dir1, dir2};

    if(try_move(dir, cur_pos, maze, height, width)){
      cur_pos[0] = cur_pos[0] + dir[0];
      cur_pos[1] = cur_pos[1] + dir[1];

      if(cur_pos[0] == exit_row && cur_pos[1] == exit_col){
	return true;
      }

    }
    else{
      return false;
    }
  }
  return false;
}

bool try_move(int dir[], int cur_pos[], char** maze, int height, int width){
  char barriers[5] = { '-', '+', '|', '#', 'D'};
  int test_pos[2];

  int test_row = cur_pos[0] + dir[0];
  int test_col = cur_pos[1] + dir[1];

  //Check if within bounds of maze
  if(test_row < 0 || test_row > height - 1) { return false; }
  if(test_col < 0 || test_col > width - 1) { return false; }
  
  for(int i = 0; i < 5; i++){
    if(maze[test_row][test_col] == barriers[i] && within_bounds()){
      return false;
    }
  }
  return true;
}

bool get_direction_index(char dir, int& dir1, int& dir2){

  int all_dir[4][2] = {
    {-1, 0}, //N
    { 1, 0}, //S
    { 0, 1}, //E
    {0, -1}  //W
  };
  
  char dir_str[NUM_DIRECTIONS] = { 'N', 'S', 'E', 'W' };  

  for(int i = 0; i < NUM_DIRECTIONS; i++){
    if(dir_str[i] == dir){

      dir1 = all_dir[i][0];
      dir2 = all_dir[i][1];      
      return true;
    }
  }
  return false;
}

char* find_path(char** maze, int height, int width, char start, char end){

  char solution[512];
  solution[0] = '\0';
  
  int start_row; int start_col;
  int end_row; int end_col;
  find_marker(start, maze, height, width, start_row, start_col);
  find_marker(end, maze, height, width, end_row, end_col);
  
  if(find_path_rec(maze, start_row, start_col,
		   end_row, end_col, solution,
		   height, width)){
    cout << "FOUND SOLUTION! " << endl;
    
    remove_deadends(maze, height, width);
    print_maze(maze, height, width);

  }
  else{
    strcpy(solution, "No solution found");
  }

  cout << solution << endl;
  return solution;  
}

void remove_deadends(char** maze, int height, int width){
  
  int row; int col;  
  bool success = find_marker('D', maze, height, width, row, col);

  while(success){
    maze[row][col] = ' ';        
    success = find_marker('D', maze, height, width, row, col);
  }
}

bool find_path_rec(char** maze, int cur_row, int cur_col,
		   int exit_row, int exit_col, char* solution,
		   int height, int width){

  if(cur_row == exit_row && cur_col == exit_col){
    solution[strlen(solution) + 1] = '\0';
    return true;
  }

  char dir_str[NUM_DIRECTIONS] = { 'N', 'S', 'E', 'W' };  

  for(int i = 0; i < NUM_DIRECTIONS; i++){
    
    int row; int col;
    get_direction_index(dir_str[i], row, col);
  
    int dir[2] = {row, col};
    int cur_pos[2] = {cur_row, cur_col};
    int sol_length = 0;
    
    if(try_move(dir, cur_pos, maze, height, width)){

      //Update current position
      cur_row = cur_row + dir[0];
      cur_col = cur_col + dir[1];

      //Mark map
      maze[cur_row][cur_col] = '#';

      //Add to solution string
      sol_length = strlen(solution);
      solution[sol_length] = dir_str[i];
      solution[sol_length + 1] = '\0';

      //Go to the next one
      if(find_path_rec(maze, cur_row, cur_col,
		       exit_row, exit_col, solution, height, width)){
	return true;
      }

      //Revert and try again
      sol_length = strlen(solution);
      solution[sol_length - 1] = '\0';
      i = 0;
    }
  }

  //If tried all directions
  maze[cur_row][cur_col] = 'D';
  return false;
}
