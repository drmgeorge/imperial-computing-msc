#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <vector>
#include <map>

using namespace std;

#include "tube.h"

/* You are pre-supplied with the functions below. Add your own 
   function definitions to the end of this file. */

/* internal helper function which allocates a dynamic 2D array */
char **allocate_2D_array(int rows, int columns) {
  char **m = new char *[rows];
  assert(m);
  for (int r=0; r<rows; r++) {
    m[r] = new char[columns];
    assert(m[r]);
  }
  return m;
}

/* internal helper function which deallocates a dynamic 2D array */
void deallocate_2D_array(char **m, int rows) {
  for (int r=0; r<rows; r++)
    delete [] m[r];
  delete [] m;
}

/* internal helper function which gets the dimensions of a map */
bool get_map_dimensions(const char *filename, int &height, int &width) {
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

/* pre-supplied function to load a tube map from a file*/
char **load_map(const char *filename, int &height, int &width) {

  bool success = get_map_dimensions(filename, height, width);
  
  if (!success)
    return NULL;

  char **m = allocate_2D_array(height, width);
  
  ifstream input(filename);

  char line[512];
  char space[] = " ";

  for (int r = 0; r<height; r++) {
    input.getline(line, 512);
    strcpy(m[r], line);
    while ( (int) strlen(m[r]) < width )
      strcat(m[r], space);
  }
  
  return m;
}

/* pre-supplied function to print the tube map */
void print_map(char **m, int height, int width) {
  cout << setw(2) << " " << " ";
  for (int c=0; c<width; c++)
    if (c && (c % 10) == 0) 
      cout << c/10;
    else
      cout << " ";
  cout << endl;

  cout << setw(2) << " " << " ";
  for (int c=0; c<width; c++)
    cout << (c % 10);
  cout << endl;

  for (int r=0; r<height; r++) {
    cout << setw(2) << r << " ";    
    for (int c=0; c<width; c++) 
      cout << m[r][c];
    cout << endl;
  }
}

/* pre-supplied helper function to report the errors encountered in Question 3 */
const char *error_description(int code) {
  switch(code) {
  case ERROR_START_STATION_INVALID: 
    return "Start station invalid";
  case ERROR_ROUTE_ENDPOINT_IS_NOT_STATION:
    return "Route endpoint is not a station";
  case ERROR_LINE_HOPPING_BETWEEN_STATIONS:
    return "Line hopping between stations not possible";
  case ERROR_BACKTRACKING_BETWEEN_STATIONS:
    return "Backtracking along line between stations not possible";
  case ERROR_INVALID_DIRECTION:
    return "Invalid direction";
  case ERROR_OFF_TRACK:
    return "Route goes off track";
  case ERROR_OUT_OF_BOUNDS:
    return "Route goes off map";
  }
  return "Unknown error";
}

/* presupplied helper function for converting string to direction enum */
Direction string_to_direction(const char *token) {
  const char *strings[] = {"N", "S", "W", "E", "NE", "NW", "SE", "SW"};
  for (int n=0; n<8; n++) {
    if (!strcmp(token, strings[n])) 
      return (Direction) n;
  }
  return INVALID_DIRECTION;
}

bool get_symbol_position(char** map, int height, int width,
			 char target, int& r, int& c){

  for(int h = 0; h < height; h++){
    for(int w = 0; w < width; w++){
      if(map[h][w] == target){
	r = h; c = w;
	return true;
      }
    }
  }
  r = c = -1;
  return false;
}
char get_symbol_for_station_or_line(char* name){

  char symbol;
  char* files[2] = { "stations.txt", "lines.txt" };

  for(int i = 0; i < 2; i++){
    bool result = search_file(files[i], name, symbol);

    if(result) {
      return symbol;
    }
  }
  //If symbol not found
  return SYMBOL_ERROR;
}

bool search_file(char* filename, char* name, char& symbol){
  
  ifstream infile;
  char line[512];

  infile.open(filename);

  if(infile.fail()){
    cerr << "Error opening " << filename << endl;
    symbol = SYMBOL_ERROR;
    return false;
  }

  infile.getline(line, 512);
    
  while(!infile.fail()){
    infile.getline(line, 512);

    if(strcmp(name, line + 2) == 0){
      symbol = line[0];
      return true;
    }
  }

  infile.close();
  return false;
}

int validate_route(char** map, int height, int width, char* start_station,
		  char* route, char* destination){

  int coords[][8] = {
    {0, -1}, //N
    {0, 1},  //S
    {-1, 0}, //W
    {1, 0},  //E
    {1, -1}, //NE
    {-1, -1}, //NW
    {1, 1},   //SE
    {-1, 1}  //SW
  };
  
  //Validate start station
  char symbol;
  if(!search_file("stations.txt", start_station, symbol)){
    return ERROR_START_STATION_INVALID;
  }

  //Validate directions string
  vector<int> converted;
  if(!route_valid(route, converted)){
    return ERROR_INVALID_DIRECTION;
  }

  //Get starting station
  int cur_r, cur_c;
  get_symbol_position(map, height, width, symbol, cur_r, cur_c);

  //Keep track of prev and current directions to determine if backtracking
  Direction cur_dir, prev_dir;
  cur_dir = prev_dir = INVALID_DIRECTION;
  
  //Movement
  int num_stations = 0;
  for(int direction : converted){

    char next_symbol;
    cur_dir = (Direction)direction;
    int x = coords[direction][0];
    int y = coords[direction][1];

    //Do the moving
    cur_c += x;
    cur_r += y;

    //Ensure within map bounds
    if(cur_r > MAP_HEIGHT || cur_r < 0) { return ERROR_OUT_OF_BOUNDS; }    
    if(cur_c > MAP_WIDTH || cur_c < 0) { return ERROR_OUT_OF_BOUNDS; }
    
    //Check on a track or station
    if(map[cur_r][cur_c] == ' ') { return ERROR_OFF_TRACK; }

    next_symbol = map[cur_r][cur_c];

    //Count total number of stations encountered (not inc departure station)
    if(at_station(next_symbol)) { num_stations++; }    

    //If symbol changes, then should either entered or left station
    if(next_symbol != symbol){
      if(!at_station(symbol) && !at_station(next_symbol)){
	return ERROR_LINE_HOPPING_BETWEEN_STATIONS;
      }
    }
    symbol = next_symbol;

    //Check if backtracking
    if(cur_dir != INVALID_DIRECTION && prev_dir != INVALID_DIRECTION){    
      if(backtracking(cur_dir, prev_dir)){
	return ERROR_BACKTRACKING_BETWEEN_STATIONS;
      }
    }
    prev_dir = cur_dir;    
  }

  //Check endpoint is a station
  if(!at_station(map[cur_r][cur_c], destination)){
    return ERROR_ROUTE_ENDPOINT_IS_NOT_STATION;
  }
  else{
    //Return number of station changes
    return num_stations - 1;
  }
}

bool route_valid(char* route, vector<int>& converted){

  char direction[256];
  int route_len = strlen(route);
  int count = 0;

  for(int ch = 0; ch < route_len + 1; ch++){

    //Construct string
    direction[count] = route[ch];
    count++;

    if(route[ch] == ',' || route[ch] == '\0'){
      
      //Send string for validation
      if(route[ch] == ',') { direction[count - 1] = '\0'; }
      
      if(string_to_direction(direction) == INVALID_DIRECTION){
	return false;
      }
      else{
	//Add to an integer array of directions
	converted.push_back(string_to_direction(direction));
	
	//Restart string
	direction[0] = '\0';
	count = 0;
      }
    }
  }
  return true;
}
 
bool at_station(char symbol, char* destination){

   ifstream infile;
   char line[512];

   infile.open("stations.txt");

   if(infile.fail()){
     cerr << "Error opening stations.txt" << endl;
   }

   while(!infile.fail()){
     infile.getline(line, 512);

     if(line[0] == symbol){
       if(destination != nullptr){
	 strcpy(destination,line + 2);
       }
       return true;
     }
   }
   return false;
 }

bool backtracking(Direction cur_dir, Direction prev_dir){

  map<Direction, Direction> opposites = {
    {N, S},
    {S, N},
    {E, W},
    {W, E},    
    {NE, SW},
    {SW, NE},
    {SE, NW},
    {NW, SE},    
  };

  if(opposites[cur_dir] == prev_dir){
    return true;
  }

  return false;
}
