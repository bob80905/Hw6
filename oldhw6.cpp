/*This program takes in as input a text file, parses it, and treats it as a
board that needs to be solved. The board has constraints that need to be met,
and the program solves the board recursively. The program goes about this naively,
in that it goes through each spot on the board and tests whether it's possible
to place the current ship horizontally or vertically. There are some shortcuts
that this program takes, to improve the run time. For example, never place a ship
where a ship is placed already, don't put a ship anywhere where putting it would
violate the row constraint, etcetera. This program takes as arguments the name of
an input file and the optional argument "find all solutions". The program finds
one solution if this optional argument is not given, otherwise, it finds all and
outputs the number of solutions found. */

#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <iomanip>
#include <cmath>
#include <locale>
#include <list>
using namespace std;
//given the string name of the vessel, returns the size of the vessel
 int return_ship_size(string ship){
  if(ship == "submarine"){
    return 1;
  }
  if(ship == "destroyer"){
    return 2;
  }
  if(ship == "cruiser"){
    return 3;
  }
  if(ship == "battleship"){
    return 4;
  }
  if(ship == "carrier"){
    return 5;
  }
  if(ship == "cargo"){
    return 6;
  }
  if(ship == "tanker"){
    return 7;
  }
}

//sorts the vector of vessels by largest first, smallest last.
void sort_vessel(vector<string>& vessel){
  vector<int> num_vector;
  for(unsigned int i = 0; i<vessel.size();i++){
    if(vessel[i]=="unknown"){
      num_vector.push_back(0);
      continue;
    }
    if(vessel[i]=="submarine"){
      num_vector.push_back(1);
      continue;
    }
    if(vessel[i]=="destroyer"){
      num_vector.push_back(2);
      continue;
    }
    if(vessel[i]=="cruiser"){
      num_vector.push_back(3);
      continue;
    }
    if(vessel[i]=="battleship"){
      num_vector.push_back(4);
      continue;
    }
    if(vessel[i]=="carrier"){
      num_vector.push_back(5);
      continue;
    }
    if(vessel[i]=="cargo"){
      num_vector.push_back(6);
      continue;
    }
    if(vessel[i]=="tanker"){
      num_vector.push_back(7);
      continue;
    }
  } //convert to numbers, sort numbers, then convert back to ships.
  sort(num_vector.rbegin(), num_vector.rend());
  vessel.clear();
  for(unsigned int i = 0; i<num_vector.size();i++){
    if(num_vector[i]==7){
      vessel.push_back("tanker");
      continue;
    }
    if(num_vector[i]==6){
      vessel.push_back("cargo");
      continue;
    }
    if(num_vector[i]==5){
      vessel.push_back("carrier");
      continue;
    }
    if(num_vector[i]==4){
      vessel.push_back("battleship");
      continue;
    }
    if(num_vector[i]==3){
      vessel.push_back("cruiser");
      continue;
    }
    if(num_vector[i]==2){
      vessel.push_back("destroyer");
      continue;
    }
    if(num_vector[i]==1){
      vessel.push_back("submarine");
      continue;
    }
    if(num_vector[i]==0){
      vessel.push_back("unknown");
      continue;
    }
  }
}
//converts a solution to a vector of strings that represent each row of the board.
vector<string> solution_parse(vector<string> solution, unsigned int num_rows, unsigned int num_cols){
  string dummy_string(num_cols, ' ');
  vector<string> return_vec(num_rows, dummy_string);
  for(unsigned int i = 0; i<solution.size();i++){
    if(return_ship_size(solution[i])==1){ //submarine case
      return_vec[atoi(solution[i+1].c_str())][atoi(solution[i+2].c_str())]='o';
      i+=2;
      continue;
    }
    //the horizontal method.
    if(return_ship_size(solution[i])>=2){
      if(solution[i+3]=="horizontal"){
        return_vec[atoi(solution[i+1].c_str())][atoi(solution[i+2].c_str())]='<';
        for(unsigned int j = 0; j<return_ship_size(solution[i])-2;j++){
          return_vec[atoi(solution[i+1].c_str())][atoi(solution[i+2].c_str())+1+j]='X';
        }
        return_vec[atoi(solution[i+1].c_str())][atoi(solution[i+2].c_str())+return_ship_size(solution[i])-1]='>';
      }
      //solution says vertical
      else{
        return_vec[atoi(solution[i+1].c_str())][atoi(solution[i+2].c_str())]='^';
        for(unsigned int j = 0; j<return_ship_size(solution[i])-2;j++){
          return_vec[atoi(solution[i+1].c_str())+j+1][atoi(solution[i+2].c_str())]='X';
        }
        return_vec[atoi(solution[i+1].c_str())+return_ship_size(solution[i])-1][atoi(solution[i+2].c_str())]='v';
      }
      i+=3;
    }
  }
  return return_vec;
}
//returns true if every ship in the given solution has the required space
//surrounding it, returns false otherwise.
bool space_around_each_ship(unsigned int num_rows, unsigned  int num_cols, vector<string>& solution){
  std::vector<string> grid;
  grid = solution_parse(solution, num_rows, num_cols);
  for(unsigned int i = 0; i<num_rows;i++){
    for(unsigned int j = 0; j<num_cols; j++){
      //we found a submarine.
      if(grid[i][j]=='o'){
        //top left
        if(i==0&&j==0){
          if(grid[i][j+1]!=' '||grid[i+1][j]!=' '||grid[i+1][j+1]!=' '){
            return false;
          }
        }
        //on the left edge
        if(i!=0&&i!=num_rows-1&&j==0){
          if(grid[i-1][j]!=' '||grid[i-1][j+1]!=' '||grid[i][j+1]!=' '
          ||grid[i+1][j+1]!=' '||grid[i+1][j]!=' '){
            return false;
          }
        }
        //bottom left
        if(i==num_rows-1&&j==0){
          if(grid[i-1][j]!=' '||grid[i-1][j+1]!=' '||grid[i][j+1]!=' '){
            return false;
          }
        }
        //top edge
        if(i==0&&j!=num_cols-1&&j!=0){
          if(grid[i][j+1]!=' '||grid[i+1][j+1]!=' '||grid[i+1][j]!=' '||
          grid[i+1][j-1]!=' '||grid[i][j-1]!=' ') {
            return false;
          }
        }
        //bottom edge
        if(i==num_rows-1&&j!=num_cols-1&&j!=0){
          if(grid[i-1][j]!=' '||grid[i-1][j+1]!=' '||grid[i][j+1]!=' '||
          grid[i][j-1]!=' '||grid[i-1][j-1]!=' '){
            return false;
          }
        }
        //somewhere in the middle.
        if(i!=num_rows-1&&i!=0&&j!=num_cols-1&&j!=0){
          if(grid[i-1][j]!=' '||grid[i-1][j+1]!=' '||grid[i][j+1]!=' '
          ||grid[i+1][j+1]!=' '||grid[i+1][j]!=' '||grid[i+1][j-1]!=' '
          ||grid[i][j-1]!=' '||grid[i-1][j-1]!=' '){
            return false;
          }
        }
        //top right
        if(i==0&&j==num_cols-1){
          if(grid[i+1][j]!=' '||grid[i+1][j-1]!=' '||grid[i][j-1]!=' '){
            return false;
          }
        }
        //right edge
        if(i!=0&&i!=num_rows-1&&j==num_cols-1){
          if(grid[i+1][j]!=' '||grid[i+1][j-1]!=' '
          ||grid[i][j-1]!=' '||grid[i-1][j-1]!=' '||grid[i-1][j]!=' '){
            return false;
          }
        }
        //bottom right
        if(i==num_rows-1&&j==num_cols-1){
          if(grid[i][j-1]!=' '||grid[i-1][j-1]!=' '||grid[i-1][j]!=' '){
            return false;
          }
        }
      continue;
      }
      //we are looking at a case where grid[i][j] is not ' ' and not 'o'
      if(grid[i][j]!=' '&&grid[i][j]!='o'){
        //the top of a ship has been found.
        if(grid[i][j]=='^'){
          //bottoms never happen
          //top left
          if(i==0&&j==0){
            if(grid[i][j+1]!=' '||grid[i+1][j+1]!=' '){
              return false;
            }
          }
          //left edge
          if(i!=0&&j==0){
            if(grid[i-1][j]!=' '||grid[i-1][j+1]!=' '||grid[i][j+1]!=' '||
            grid[i+1][j+1]!=' '){
              return false;
            }
          }
          //top edge
          if(i==0&&j!=0&&j!=num_cols-1){
            if(grid[i][j+1]!=' '||grid[i][j-1]!=' '||grid[i+1][j+1]!=' '||grid[i+1][j-1]!=' '){
              return false;
            }
          }
          //top right
          if(i==0&&j==num_cols-1){
            if(grid[i][j-1]!=' '||grid[i+1][j-1]!=' '){
              return false;
            }
          }
          //right edge
          if(i!=0&&i!=num_rows-1&&j==num_cols-1){
            if(grid[i-1][j]!=' '||grid[i-1][j-1]!=' '||
            grid[i][j-1]!=' '||grid[i+1][j-1]!=' '){
              return false;
            }
          }
          //somewhere in the middle
          if(i!=0&&i!=num_rows-1&&j!=0&&j!=num_cols-1){
            if(grid[i-1][j]!=' '||grid[i-1][j-1]!=' '||
            grid[i][j-1]!=' '||grid[i+1][j-1]!=' '||grid[i-1][j+1]!=' '||
            grid[i][j+1]!=' '||grid[i+1][j+1]!=' '){
              return false;
            }
          }
        }
        //the bottom of a ship has been found
        if(grid[i][j]=='v'){
          //never on the top row
          //left edge
          if(i!=0&&i!=num_rows-1&&j==0){
            if(grid[i][j+1]!=' '||grid[i+1][j+1]!=' '||grid[i+1][j]!=' '){
              return false;
            }
          }
          //bottom left
          if(i!=0&&i!=num_rows-1&&j==0){
            if(grid[i][j+1]!=' '){
              return false;
            }
          }
          //bottom edge
          if(i==num_rows-1&&j!=0&&j!=num_cols-1){
            if(grid[i][j+1]!=' '||grid[i][j-1]!=' '){
              return false;
            }
          }

          //bottom right
          if(i==num_rows-1&&j==num_cols-1){
            if(grid[i][j-1]!=' '){
              return false;
            }
          }
          //right edge
          if(i!=0&&i!=num_rows-1&&j==num_cols-1){
            if(grid[i][j-1]!=' '||grid[i+1][j-1]!=' '||grid[i+1][j]!=' '){
              return false;
            }
          }
          //somewhere in the middle
          if(i!=0&&i!=num_rows-1&&j!=0&&j!=num_cols-1){
            if(grid[i-1][j-1]!=' '||
            grid[i][j-1]!=' '||grid[i+1][j-1]!=' '||grid[i-1][j+1]!=' '||
            grid[i][j+1]!=' '||grid[i+1][j+1]!=' '||grid[i+1][j]!=' '){
              return false;
            }
          }
        }
        //the left edge of a ship has been found
        if(grid[i][j]=='<'){
          //never on the right edge
          //top left
          if(i==0&&j==0){
            if(grid[i+1][j]!=' '||grid[i+1][j+1]!=' '){
              return false;
            }
          }
          //left edge
          if(i!=0&&i!=num_rows-1&&j==0){
            if(grid[i-1][j]!=' '||grid[i-1][j+1]!=' '||
            grid[i+1][j]!=' '||grid[i+1][j+1]!=' '){
              return false;
            }
          }
          //bottom left
          if(i==num_rows-1&&j==0){
            if(grid[i-1][j]!=' '||grid[i-1][j+1]!=' '){
              return false;
            }
          }
          //top edge
          if(i==0&&j!=0&&j!=num_cols-1){
            if(grid[i][j-1]!=' '||grid[i+1][j-1]!=' '||
            grid[i+1][j]!=' '||grid[i+1][j+1]!=' '){
              return false;
            }
          }
          //bottom edge
          if(i==num_rows-1&&j!=0&&j!=num_cols-1){
            if(grid[i-1][j-1]!=' '||grid[i-1][j]!=' '||
            grid[i-1][j+1]!=' '||grid[i][j-1]!=' '){
              return false;
            }
          }
          //somewhere in the middle
          if(i!=0&&i!=num_rows-1&&j!=0&&j!=num_cols-1){
            if(grid[i-1][j]!=' '||grid[i-1][j-1]!=' '
            ||grid[i][j-1]!=' '||grid[i+1][j-1]!=' '||grid[i+1][j]!=' '){
              return false;
            }
          }
        }
        //the right edge of a ship has been found
        if(grid[i][j]=='>'){
          //never on the left edge
          //top edge
          if(i==0&&j!=0&&j!=num_cols-1){
            if(grid[i][j+1]!=' '||grid[i+1][j+1]!=' '||grid[i+1][j-1]!=' '){
              return false;
            }
          }
          //bottom edge
          if(i==num_rows-1&&j!=0&&j!=num_cols-1){
            if(grid[i][j+1]!=' '||grid[i-1][j+1]!=' '||grid[i-1][j]!=' '){
              return false;
            }
          }
          //top right
          if(i==0&&j==num_cols-1){
            if(grid[i+1][j]!=' '||grid[i+1][j-1]!=' '){
              return false;
            }
          }
          //right edge
          if(i!=0&&i!=num_rows-1&&j==num_cols-1){
            if(grid[i-1][j]!=' '||grid[i-1][j-1]!=' '||
            grid[i+1][j]!=' '||grid[i+1][j-1]!=' '){
              return false;
            }
          }
          //bottom right
          if(i==num_rows-1&&j==num_cols-1){
            if(grid[i-1][j]!=' '||grid[i-1][j-1]!=' '){
              return false;
            }
          }
          //somewhere in the middle
          if(i!=0&&i!=num_rows-1&&j!=0&&j!=num_cols-1){
            if(grid[i-1][j]!=' '||grid[i-1][j-1]!=' '||
            grid[i+1][j]!=' '||grid[i+1][j-1]!=' '||
            grid[i][j+1]!=' '||grid[i+1][j+1]!=' '||grid[i-1][j+1]!=' '){
              return false;
            }
          }
        }
        //the inside of a ship has been found
        if(grid[i][j]=='X'){
          //there can never be an x on the left or right edge while it's horizontal,
          //or an x on the top or bottom edge when it's vertical.
          //let's start with the left and right edges, the vertical cases
          if(i!=0&&i!=num_rows-1&&j==0){
            if(grid[i][j+1]!=' '){
              return false;
            }
          }
          //now right edge
          if(i!=0&&i!=num_rows-1&&j==num_cols-1){
            if(grid[i][j-1]!=' '){
              return false;
            }
          }
          //top edge and bottom edge, horizontal cases
          if(i==0&&j!=0&&j!=num_cols-1){
            if(grid[i+1][j]!=' '){
              return false;
            }
          }
          //bottom edge
          if(i==num_rows-1&&j!=0&&j!=num_cols-1){
            if(grid[i-1][j]!=' '){
              return false;
            }
          }
          //somewhere in the middle for X is quite different
          //if there are any X's right or left of an X, then there should
          //be space perpendicularly. Same case for the second block.
          if(i!=0&&i!=num_rows-1&&j!=0&&j!=num_cols-1){
            if(grid[i][j+1]!=' '&&grid[i][j-1]!=' '){
              if(grid[i+1][j]!=' '||grid[i-1][j]!=' '){
                return false;
              }
            }
            if(grid[i+1][j]!=' '&&grid[i-1][j]!=' '){
              if(grid[i][j+1]!=' '||grid[i][j-1]!=' '){
                return false;
              }
            }
          }
        }
      }
    }
  }
return true;
}
//returns true if a pair of coordinates is found within the vector supplied.
bool pair_contained(vector<int> already_coords, int x, int y){
  if(already_coords.size()==0){
    return false;
  }
  for(unsigned int i = 0;i<already_coords.size()-1;i+=2){
    if(already_coords[i]==x&&already_coords[i+1]==y){
      return true;
    }
  }
  return false;
}
//given the solution, and the row/col constraints, prints the board like the pdf.
void print_board(vector<string> solution, unsigned int num_rows, unsigned int num_cols, int* row_e, int* cols_e){
  cout<<"Solution:"<<endl;
  int max_size = 0;
  for(unsigned int i = 0; i<solution.size();i++){
    if(solution[i]=="submarine"){
      max_size = max(max_size, 9);
    }
    if(solution[i]=="destroyer"){
      max_size = max(max_size, 9);
    }
    if(solution[i]=="cruiser"){
      max_size = max(max_size, 7);
    }
    if(solution[i]=="battleship"){
      max_size = max(max_size, 10);
    }
    if(solution[i]=="carrier"){
      max_size = max(max_size, 7);
    }
    if(solution[i]=="cargo"){
      max_size = max(max_size, 5);
    }
    if(solution[i]=="tanker"){
      max_size = max(max_size, 6);
    }
  }
  //find the longest length of the word in vessel, and say that length plus 2 for the width.
  for(unsigned int i = 0;i<solution.size();i++){
    cout.width(max_size+2);
    cout<<left<<solution[i]<<right<<solution[i+1]<<" "<<solution[i+2]<<" ";
    if(i+3<solution.size()){
      if(solution[i+3]=="horizontal"||solution[i+3]=="vertical"){
        cout<<solution[i+3];
        i+=1;
      }
    }
    cout<<endl;
    i+=2;
  }
  cout<<'+';
  for(unsigned int i = 0; i<num_cols;i++){
    cout<<'-';
  }
  cout<<'+'<<endl;

  //fill in middle of the board with for loop using parser
  vector<string>  grid = solution_parse(solution, num_rows, num_cols);
  for(unsigned int i=0; i<num_rows;i++){
    cout<<'|'<<grid[i]<<'|';
    if(row_e[i]==-1){ //print out the row numbers
      cout<<'?';
    }
    else{
      cout<<row_e[i];
    }
    cout<<endl;
  }

  cout<<'+'; //the bottom of the board
  for(unsigned int i = 0; i<num_cols;i++){
    cout<<'-';
  }
  cout<<'+'<<endl<<' ';
  for(unsigned int i = 0; i<num_cols;i++){
    if(cols_e[i]==-1){ //print out the column numbers
      cout<<"?";
    }
    else{
      cout<<cols_e[i];
    }
  }
  cout<<endl;
}
//returns true if 2 solutions are identical.
bool solutions_are_equal(vector<string> sol1, vector<string> sol2, unsigned int num_rows, unsigned int num_cols){
  //just sort each one, if any are == return false, but make sure to unsort them back.
  vector<string> grid1 = solution_parse(sol1, num_rows, num_cols);
  vector<string> grid2 = solution_parse(sol2, num_rows, num_cols);
  if(grid1==grid2){
    return true;
  }
  return false;
}

//makes the set of vectors in all_solutions all unique
void duplication_remover(vector<vector<string> >& all_solutions, unsigned int num_rows, unsigned int num_cols){
  for(unsigned int i =0;i<all_solutions.size();i++){
    for(unsigned int j = i; j<all_solutions.size();j++){
      if(i==j){
        continue;
      }
      if(solutions_are_equal(all_solutions[i], all_solutions[j], num_rows, num_cols)){
        all_solutions.erase(all_solutions.begin()+j);
        j--;
      }
    }
  }
}

//given a solution vector, returns true if it satisfies all the column constraints.
bool cols_ok( int* cols_e, unsigned int num_rows, unsigned int num_cols, vector<string> solution ){
  vector<int> col_temp(num_cols, 0);
  vector<string> grid = solution_parse(solution, num_rows, num_cols);
  for(unsigned int i = 0; i <num_rows;i++){ //summing up the elements.
    for(unsigned int j = 0; j <num_cols;j++){
      if(grid[i][j]!=' '){
        col_temp[j]+=1;
      }
    }
  }
  //gives truth value that all the elements in each column exactly matches
   //constraints, unless there's an unknown constraint.
  for(unsigned int i = 0; i<num_cols;i++){
    if(col_temp[i]!=cols_e[i]){
      if(cols_e[i]==-1){
        continue;
      }
      return false;
    }
  }
  return true;
}

//given a solution vector of strings, returns true if the row constraints are met.
bool rows_ok( int* row_e, unsigned int num_rows, unsigned int num_cols, vector<string> solution){
  vector<int> row_temp(num_rows, 0);
  vector<string> grid = solution_parse(solution, num_rows, num_cols);
  for(unsigned int i = 0; i <num_rows;i++){
    for(unsigned int j = 0; j <num_cols;j++){ //summing up the elements.
      if(grid[i][j]!=' '){
        row_temp[i]+=1;
      }
    }
  }
  //gives truth value that all the elements in each row exactly matches
   //constraints, unless there's an unknown constraint.
  for(unsigned int i = 0; i<num_rows;i++){
    if(row_temp[i]!=row_e[i]){
      if(row_e[i]==-1){
        continue;
      }
      return false;
    }
  }
  return true;
}
//returns true if the input file's constraints are satisfied in the solution
//false otherwise.
bool constraint_ok(vector<string> constraint, vector<string> solution, unsigned int num_rows, unsigned int num_cols){
  if(constraint.size()==0){
    return true;
  }
  else{
    vector<string> grid = solution_parse(solution, num_rows, num_cols);
    for(unsigned int i = 0; i<constraint.size();i++){
      if(constraint[i+2][0]=='_'){ //constraint for open water is special.
        //open water constraint
        if(grid[atoi(constraint[i].c_str())][atoi(constraint[i+1].c_str())]!=' '){
          return false;
          }
        else{
          i+=3;
          continue;
          }
        }
      if(grid[atoi(constraint[i].c_str())][atoi(constraint[i+1].c_str())]==constraint[i+2][0] ) {
        i+=3;
      }
      else{
        return false;
      }
    }
  }
  return true;
}
//checks that along the vertical length of the ship there are no indicators
//that nothing should be on the board in each of the rows.
//if the row above or below the ship says 0, return true, false otherwise.
bool no_adjacent_row_0s(string vessel, unsigned int var, unsigned int num_rows, int* row_e){
  if(var==0){
    if(row_e[1]==0){
      return false;
    }
    else{
      return true;
    }
  }
  if(var==num_rows-1){
    if(row_e[num_rows-2]==0){
      return false;
    }
    else{
      return true;
    }
  }
  unsigned int size = return_ship_size(vessel);
  for(unsigned int i = var; i<var+size;i++){
    if(row_e[i]==0){
      return false;
      }
    }
  return true;
}
//same as above, but with horizontal ships, check for adjacent col restrains = 0
bool no_adjacent_col_0s(string vessel, unsigned int var, unsigned int num_cols, int* cols_e){
  if(var==0){
    if(cols_e[1]==0){
      return false;
    }
    else{
      return true;
    }
  }
  if(var==num_cols-1){
    if(cols_e[num_cols-2]==0){
      return false;
    }
    else{
      return true;
    }
  }
  unsigned int size = return_ship_size(vessel);
  for(unsigned int i = var; i<var+size;i++){
    if(cols_e[i]==0){
      return false;
      }
    }
  return true;
}
//sums up all the elements in a row, using coordinates that are already occupied.
int sum_row_elements(unsigned int var, vector<int> already_coords){
  int sum = 0;
  if(already_coords.size()==0){
    return sum;
  }
  for(unsigned int i=0; i<already_coords.size()-1;i+=2){
    if(already_coords[i]==var){
      sum+=1;
    }
  }
  return sum;
}
//same as above, counts elements in a column.
int sum_col_elements(unsigned int var, vector<int> already_coords){
  int sum = 0;
  if(already_coords.size()==0){
    return sum;
  }
  for(unsigned int i=0; i<already_coords.size()-1;i+=2){
    if(already_coords[i+1]==var){
      sum+=1;
    }
  }
  return sum;
}
//returns true if there is no longer any place to put a ship on the given row.
//returns false otherwise.
bool no_row_space(vector<string> solution, unsigned int num_rows, unsigned int num_cols, unsigned int i){
  vector<string> grid = solution_parse(solution, num_rows, num_cols);
  int space_count = 0;
  for(unsigned int a = 0; a<num_cols; a++){
    if(grid[i][a]==' '){
      space_count+=1;
    }
    if(space_count==3){
      return false;
    }
    if(a==0){
      if(grid[i][a+1]==' '&&grid[i][a]==' ')
      return false;
    }
    if(a==num_cols-1){
      if(grid[i][a-1]==' '&&grid[i][a]==' ')
      return false;
    }
    if(grid[i][a]!=' '){
      space_count=0;
    }
  }
  return true;
}
//as above, returns true if the column cannot fit another ship, false otherwise.
bool no_col_space(vector<string> solution, unsigned int num_rows, unsigned int num_cols, unsigned int j){
  vector<string> grid = solution_parse(solution, num_rows, num_cols);
  int space_count = 0;
  for(unsigned int a = 0; a<num_rows; a++){
    if(grid[a][j]==' '){
      space_count+=1;
    }
    if(space_count==3){
      return false;
    }
    if(a==0){
      if(grid[a+1][j]==' '&&grid[a][j]==' ')
      return false;
    }
    if(a==num_rows-1){
      if(grid[a-1][j]==' '&&grid[a][j]==' ')
      return false;
    }
    if(grid[a][j]!=' '){
      space_count=0;
    }
  }
  return true;
}

void valid_recursive_solutions(
  vector<string>& vessel, int* row_e, int* cols_e,
  unsigned int num_rows, unsigned int num_cols, vector<string>& solution,
  vector<vector<string> >& all_solutions, vector<int>& already_coords,
  const vector<string>& constraint, unsigned int argc, string& previous_ship,
  vector<vector<string> >& legit_all_sols, unsigned int& a, unsigned int& b){
 
  //print_board(solution, num_rows, num_cols, row_e, cols_e);
    //for finding a single solution, break the call once a solution is found.
    if(argc==2&&all_solutions.size()>=1){
      return;
    }
    //any solution states that are met again should cause the program to stop
    //recursing.
    for(unsigned int i = 0; i<legit_all_sols.size();i++){
      if(solutions_are_equal(legit_all_sols[i], solution, num_rows, num_cols)){
        return;
      }
    }
    //push back the current grid into a vector of all grids encountered.
    legit_all_sols.push_back(solution);
    //resort the ship vessel.
    sort_vessel(vessel);
    //Base case: the grid is a valid solution, since row and column constraints
    //are satisfied, and there is space around each ship, and there are no ships
    //left to place, and any constraints supplied are met.
      if(rows_ok(row_e, num_rows, num_cols,solution)&&
      cols_ok(cols_e, num_rows, num_cols, solution)
      &&space_around_each_ship(num_rows, num_cols, solution)
      &&constraint_ok(constraint, solution, num_rows, num_cols)&&vessel.size()==0){
        all_solutions.push_back(solution);
        return;
      }
      //if there are no vessels left, just stop, there is nothing left to do.
      if(vessel.size()==0){
        return;
      }
      //here is where we progress an unsolved board.
      if(!rows_ok(row_e, num_rows, num_cols,solution)||!
      cols_ok(cols_e, num_rows, num_cols, solution)||vessel.size()!=0){
        string new_vessel = vessel[0];
        vessel.erase(vessel.begin());
        solution.push_back(new_vessel);
        if(new_vessel == "submarine"){ //if the ship we're on is a submarine:
          bool flag = false;
          for(unsigned int i=0; i<num_rows; i++){
            if(row_e[i]!=0){
              for(unsigned int j=0; j<num_cols;j++){
                if(return_ship_size(previous_ship)<=return_ship_size(solution.back())
                &&!flag){ //if the previous ship was smaller than the sihp
                  i=a;//that we're looking for a place to put down now, there's
                  j=b;//no sense in searching where we've already looked.
                  flag = true;//use the last coordinates and start from there.
                }
                //if placing this sub doesn't violate any row/col constraints:
                if((cols_e[j]!=0&&(sum_col_elements(j, already_coords)+1<=cols_e[j])
                ||cols_e[j]==-1)&&(sum_row_elements(i, already_coords)+1<=row_e[i]||
                row_e[i]==-1)) {
                  //if the current coordinates are not held by another ship:
                  if(!pair_contained(already_coords, i, j)) {
                    string this_ship = solution.back();
                    solution.push_back(to_string(i));
                    solution.push_back(to_string(j)); //write down coords to ship.
                    for(unsigned int b = 0; b<legit_all_sols.size();b++){
                      if(solutions_are_equal(legit_all_sols[b], solution,
                         num_rows, num_cols)){ //if any grids are seen again,
                        solution.pop_back();//stop, go back up a level, and
                        solution.pop_back();//continue, we've already been
                        solution.pop_back();//down this path.
                        vessel.push_back("submarine");
                        return;
                      }
                    }
                    if(!space_around_each_ship(num_rows, num_cols, solution)){
                      solution.pop_back();//if there is not space around each
                      solution.pop_back();//ship, continue looking for new coords.
                      continue;
                    }
                    already_coords.push_back(i);//record the coords of the sub,
                    already_coords.push_back(j);//record that this ship IS a sub,
                    previous_ship = this_ship;//and recurse.
                    valid_recursive_solutions(vessel, row_e, cols_e, num_rows, num_cols,
                      solution, all_solutions, already_coords, constraint, argc, previous_ship,
                      legit_all_sols, i, j);
                    solution.pop_back();//destroy this ship's information, and
                    solution.pop_back();//the coords associated with it.
                    already_coords.pop_back();
                    already_coords.pop_back();
                  }

                }
              }
            }
          }
          vessel.push_back(solution.back()); //destroy this ship from the solution
          solution.pop_back();//and put it back into vessels that need to be placed.
        }

        else{  //the vessel ship is not a submarine
          int size = return_ship_size(solution.back());
          string this_ship = solution.back();
          for(unsigned int i = 0; i<num_rows; i++){
            if(row_e[i]!=0) { //branch into horizontal check and vertical check
              for(unsigned int j = 0; j<num_cols;j++){
                if(cols_e[j]!=0) {
                  if(!pair_contained(already_coords, i, j)){ //no ship here.
                    bool r_ok = true;
                    for(unsigned int a = j;a<j+size;a++){
                      if(sum_col_elements(a,already_coords)+1>cols_e[a]&&cols_e[a]!=-1){
                        r_ok = false; //r_ok means no column constraints are
                      }//broken when addint this ship horizontally.
                    }
                    solution.push_back(to_string(i));
                    solution.push_back(to_string(j));
                    solution.push_back("horizontal"); //test horizontal.
                    for(unsigned int k = j;k<j+size; k++){ //horizontal placement at first.
                      already_coords.push_back(i);
                      already_coords.push_back(k); //this ship occupies these coords.
                    }
                     //if horizontal placement is legal
                    if(j+size<=num_cols&&
                      space_around_each_ship(num_rows, num_cols, solution)&&
                      no_adjacent_col_0s(new_vessel, j, num_cols, cols_e)&&
                      (sum_row_elements(i, already_coords)<=row_e[i]||
                      row_e[i]==-1)&&r_ok&&
                      !(no_row_space(solution, num_rows, num_cols, i)&&
                      sum_row_elements(i, already_coords)!=row_e[i]&&row_e[i]!=-1))
                      {


                      for(unsigned int b = 0; b<legit_all_sols.size();b++){
                        if(solutions_are_equal(legit_all_sols[b], solution, num_rows, num_cols)){
                          solution.pop_back();
                          solution.pop_back();//if this grid has been seen,
                          solution.pop_back();//stop looking at this grid.
                          vessel.push_back(this_ship);
                          solution.pop_back();
                          for(unsigned int k = j;k<j+size; k++){
                            already_coords.pop_back();
                            already_coords.pop_back();
                          }
                          return;
                        }
                      }
                      //record this ship's identity, and recurse.
                      previous_ship = this_ship;
                      valid_recursive_solutions(vessel, row_e, cols_e, num_rows,
                        num_cols,solution, all_solutions, already_coords,
                        constraint, argc, previous_ship, legit_all_sols, i,j);
                    }
                    //destroy this ship's orientation.
                    solution.pop_back();
                    for(unsigned int k = i;k<i+size; k++){
                      already_coords.pop_back();//destroy coordinates this ship
                      already_coords.pop_back();//occupies.
                    }

                    bool c_ok = true; //like r_ok, checks that adding vertically
                    for(unsigned int a = i;a<i+size;a++){//does not violate any
                      //row constraints.
                      if(sum_row_elements(a,already_coords)+1>row_e[a]&&row_e[a]!=-1){
                        c_ok = false;
                      }
                    }
                    solution.push_back("vertical");//test vertical placement.
                    for(unsigned int k = i;k<i+size; k++){
                      already_coords.push_back(k); //this ship occupies these
                      already_coords.push_back(j); //coordinates.
                    }

                    if(i+size<=num_rows&& //if vertical placement is legal:
                      space_around_each_ship(num_rows, num_cols, solution)&&
                      no_adjacent_row_0s(new_vessel, i, num_rows, row_e)&&
                      (sum_col_elements(j, already_coords)<=cols_e[j]
                      ||cols_e[i]==-1)&&c_ok&&!(no_col_space(solution, num_rows,
                      num_cols, j)&&sum_col_elements(j, already_coords)!=cols_e[j]
                      &&cols_e[j]!=-1)) {
                        //if this grid has been seen before, stop loooking here.
                      for(unsigned int b = 0; b<legit_all_sols.size();b++){
                        if(solutions_are_equal(legit_all_sols[b], solution, num_rows, num_cols)){
                            solution.pop_back();
                            solution.pop_back();
                            solution.pop_back();
                            vessel.push_back(solution.back());
                            solution.pop_back();
                            for(unsigned int k = j;k<j+size; k++){
                              already_coords.pop_back();
                              already_coords.pop_back();
                            }
                            return;
                          }
                        }
                      //record this ship's identity, and recurse.
                      previous_ship = this_ship;
                      valid_recursive_solutions(vessel, row_e, cols_e, num_rows,
                        num_cols,solution, all_solutions, already_coords,
                        constraint, argc, previous_ship, legit_all_sols, i,j);
                      }//destroy this ship's orientation and coordinates,
                      solution.pop_back();
                      solution.pop_back();
                      solution.pop_back();
                      for(unsigned int k = i;k<i+size; k++){
                        already_coords.pop_back(); //and free these coordinates up.
                        already_coords.pop_back();
                      }
                    }
                  }
                }
              }
            }
            //put this ship back into the ships we need to place down, and
          vessel.push_back(solution.back());//destroy this ship from the solution.
          solution.pop_back();

        }
      }
    }
//this function takes in a vector of ships, a cap, and a range, and increments the
//vector of ships upwards, so submarine becomes destroyer and so on. this is capped
//at max_e, so if max_e is 3, incrementation maxes at cruiser. If cruiser must be
//incremented, it gets set to a submarine and the next ship behind it gets incremented.
void increment_vec(unsigned int max_e, vector<string>& ships, vector<string> all_ships, unsigned int u_size){
  if(ships.back()==all_ships[max_e-1]){
    for(unsigned int i = 0; i<ships.size();i++){
      if(ships[u_size-i-1]!=all_ships[max_e-1]){
        ships[u_size-i-1] = all_ships[return_ship_size(ships[u_size-i-1])];
        break;
      }
      else{
        ships[u_size-i-1] = "submarine";
      }
    }
  }
  else{
    ships.back() = all_ships[return_ship_size(ships.back())];
  }
}
//sums up all the vessel sizes inside a vector of vessels, and returns this size.
int sum_of_vessels(vector<string> a){
  int sum = 0;
  for(unsigned int i = 0; i<a.size();i++){
    sum+=return_ship_size(a[i]);
  }
  return sum;
}
int main(int argc, char* argv[]){
  if (argc>3){//error checking that 2 or 3 arguments were given.
    cerr<<"Error: Expected 2 to 3 arguments:"<<endl;
    cerr<<" Input program exe name, input file, and optional \"find all solutions\"."<<endl;
    exit(1);
  }
  //create input stream to read the file.
  std::ifstream in_str(argv[1]);
  if (!in_str.good()) {
    std::cerr << "Can't open " << argv[1] << " to read.\n";
    exit(1);
  }

  string word;
  vector<string> vessel;
  vector<string> constraint; //constraint vector
  vector<string> unknown; //unknown vector holds number of unknowns read.
  vector<string> all_ships; //vector of variety of ships to help with unknowns.
  all_ships.push_back("submarine");
  all_ships.push_back("destroyer");
  all_ships.push_back("cruiser");
  all_ships.push_back("battleship");
  all_ships.push_back("carrier");
  all_ships.push_back("cargo");
  all_ships.push_back("tanker");
  vector<vector<string > > legit_all_sols; //all grids seen.
  unsigned int y = 0;
  unsigned int z = 0;
  bool q = false; //is true when a question mark is read.

  //Parsing file.
  in_str>>word;
  in_str>>word;
  unsigned int num_rows = atoi(word.c_str()); //reading in number of rows/columns.
  in_str>>word;
  unsigned int num_cols = atoi(word.c_str());
  in_str>>word;
  int row_e[num_rows];
  int cols_e[num_cols];
  int sum_of_elements=0;
  for(unsigned int i = 0; i<num_rows;i++){ //reading row constraints, -1 if ? is read.
    in_str>>word;
    if(word=="?"){
      row_e[i]=-1;
      q=true;
    }
    else{
    row_e[i] = atoi(word.c_str());
    sum_of_elements+=atoi(word.c_str());
    }
  }

  in_str>>word;
  for(unsigned int i = 0; i<num_cols;i++){ //reading column constraints, -1 if ?.
    in_str>>word;
    if(word =="?"){
      cols_e[i]=-1;
      q=true;
    }
    else{
    cols_e[i] = atoi(word.c_str());
    }
  }

  while(in_str>>word){
    if(word=="constraint"){ //
      while(in_str>>word){
        constraint.push_back(word);
      } //constraints are always last, so break when stream ends.
      break;
    }
    if(word=="unknown"){ //if unknown is read, add to the unknown vector.
      unknown.push_back("unknown");
      continue;
    }
    vessel.push_back(word);
  }

  vector<string> solution;
  vector<vector <string> >all_solutions;
  sort_vessel(vessel); //sorting vessels by largest first reduces computation time.
  vector<int> already_coords; //all coordinates that have been occupied.
  int max_e = 0;//maximum row or column constraint encountered.
  for(unsigned int i = 0; i<num_rows; i++){
    if(row_e[i]>max_e){
      max_e = row_e[i];
    }
    if(row_e[i]==-1){
      max_e = 7;
    }
  }
  for(unsigned int i = 0; i<num_cols; i++){
    if(cols_e[i]>max_e){
      max_e = cols_e[i];
    }
    if(cols_e[i]==-1){
      max_e = 7;
    }
  }
  string previous_ship = "nah";//there is no ship before the first ship, so...

vector<string> known_ships(vessel.begin(), vessel.end()); //records ships started with.

  if(argc==3){
    if(unknown.size()>0){ //all solutions inputted, and unknowns in the file.
      for(unsigned int i = 0; i<unknown.size();i++){
        vessel.push_back("submarine"); //input submarines for each unknown.
      }
      vector<string> temp_ves = vessel;
      for(unsigned int i = 0; i<pow(7,unknown.size());i++){
        if(sum_of_vessels(vessel)==sum_of_elements||q) //only recurse if number of ships
        //match the row/column constraints, but always recurse if a ? is found.
          valid_recursive_solutions(vessel, row_e, cols_e, num_rows, num_cols, solution,
             all_solutions, already_coords, constraint, argc, previous_ship, legit_all_sols, y, z );
        legit_all_sols.clear();//delete all solutions found for each recursion,
        vessel = temp_ves;  //too many solutions, inefficient.
        increment_vec(max_e, vessel, all_ships, unknown.size());//return ships to its
        temp_ves = vessel;//previous state, and increment, and try again.
      }

      if(all_solutions.size()==0){ //no solutions found, print no solutions.
        cout<<"No solutions.";
        return 0;
      }
      duplication_remover(all_solutions, num_rows, num_cols); //removes any duplicates.
      for(unsigned int i = 0; i<all_solutions.size();i++){ //prints all solutions found.
        print_board(all_solutions[i], num_rows, num_cols, row_e, cols_e);
      }
      cout<<"Found "<<all_solutions.size()<<" solution(s) "<<endl; //cout number found
      return 0;
    }
    //no unknowns were read.
    valid_recursive_solutions(vessel, row_e, cols_e, num_rows, num_cols, solution,
       all_solutions, already_coords, constraint, argc, previous_ship, legit_all_sols, y ,z);
       duplication_remover(all_solutions, num_rows, num_cols);
    if(all_solutions.size()==0){
      cout<<"No solutions.";
      return 0;
    }
    for(unsigned int i = 0; i<all_solutions.size();i++){
      print_board(all_solutions[i], num_rows, num_cols, row_e, cols_e);
    }
    cout<<"Found "<<all_solutions.size()<<" solution(s) "<<endl;
  }
  //only one solution needed
  else{
    if(unknown.size()>0){ //if any unknowns were read.
      for(unsigned int i = 0; i<unknown.size();i++){
        vessel.push_back("submarine");
      } //default start with submarines, increment from there.
      vector<string> temp_ves = vessel;
      for(unsigned int i = 0; i<pow(7,unknown.size());i++){
        if(sum_of_vessels(vessel)==sum_of_elements||q)
          valid_recursive_solutions(vessel, row_e, cols_e, num_rows, num_cols, solution,
             all_solutions, already_coords, constraint, argc, previous_ship, legit_all_sols, y, z );
        legit_all_sols.clear();
        vessel = temp_ves;
        increment_vec(max_e, vessel, all_ships, unknown.size());
        temp_ves = vessel;
        if(all_solutions.size()!=0){ //break if a solution was found.
          break;
        }
      }
      //No solutions still? print no solutions.
      if(all_solutions.size()==0){
        cout<<"No solutions.";
        return 0;
      }//print the single solution.
      print_board(all_solutions[0],num_rows,num_cols,row_e,cols_e);
      return 0;
    }
    //no unknowns were read, simply recurse once.
    valid_recursive_solutions(vessel, row_e, cols_e, num_rows, num_cols, solution,
       all_solutions, already_coords, constraint, argc, previous_ship, legit_all_sols,y ,z);
    if(all_solutions.size()==0){
      cout<<"No solutions.";
      return 0;
    }
    print_board(all_solutions[0],num_rows,num_cols,row_e,cols_e);
  }
return 0;
}
//check for symmetry and only look at the second halves's final ships position, the final ship must be flipped to the first solution.
//make sure literally every solution is unique?

//do unknowns individually.
//store all the arguments of the recursion in a data base, if the same arguments are seen, return.
//look at corner dimensions.
//for each placement, if within the ship any constraints are come across, don't recurse if the constraint isn't met.
//if q, raise 7 to power, else, raise max_e to power, limited by 7.
//also, record each set, sorted, that was tested for unknowns, and make sure each input set is completely unique.
