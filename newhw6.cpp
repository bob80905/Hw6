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
#include <unordered_map>
#include <map>
#include <cstdlib>
#include <cstdio>
#include <chrono>
#include "board.h"
typedef std::chrono::high_resolution_clock Clock;
using namespace std;

/*On the command line, run "./main.out newpx.txt", where x is the puzzle number
Compile with -O3 for super fast results!

Data
For puzzle 9
old version took 14 m 57s under -O3 opt and printing each step to arrive at 14 solutions.  
4.1 s needed to find a single solution, and 13m 25s without printing each step to get 14 solutions.

new took 40.5s under -O3 opt and printing each step to arrive at 152 solutions.
without any printing, it finds 152 sols in .27s
*/


int main(int argc, char* argv[]){
	string cmd = argv[1];
	ifstream in_str(cmd); //input files will contain the number of 
	//rows, endl, cols, endl, rconstraints, endl, cconstraints, endl 
	//the number of ships, endl, all ship sizes. 
	in_str>>cmd; //number of rows read.
	int rows = atoi(cmd.c_str());
	in_str>>cmd; //number of columns read.
	int cols = atoi(cmd.c_str());
	vector<int> r_consts;
	vector<int> c_consts;
	for(unsigned int i = 0; i<rows; i++){
		string temp;
		in_str>>temp;
		r_consts.push_back(atoi(temp.c_str()));
	}

	for(unsigned int i = 0; i<cols; i++){
		string temp;
		in_str>>temp;
		c_consts.push_back(atoi(temp.c_str()));
	}
	in_str>>cmd;
	vector<int> sizes;
	for(unsigned int i = 0; i < atoi(cmd.c_str()); i++){
		string temp;
		in_str>>temp;
		sizes.push_back(atoi(temp.c_str()));
	}


	grid gr(rows, cols, r_consts, c_consts, sizes);

	//now solve the board according to constraints
	gr.solve(0, 0);
	gr.print_sols();
}