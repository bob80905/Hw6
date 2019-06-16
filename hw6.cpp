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
typedef std::chrono::high_resolution_clock Clock;
using namespace std;

class Ship{

public: 
	int x = 0;
	int y = 0;
	int size_ = 0;
	bool ori = true;

	Ship(int s){
		size_ = s;
		ori = true;	
		x = 0;
		y = 0;
	}

	void rotate(bool o){
		ori = o;
	}

	int size(){
		return size_;
	}	

	bool operator==(Ship& s2){
		if (x == s2.x and y == s2.y and size_ == s2.size() and ori == s2.ori){
			return true;
		}
		return false;
	}

};

bool read(Ship& s1, Ship& s2){
	if(s1.y < s2.y || (s1.y == s2.y && s1.x < s2.x)){
		return true;
	}
	return false;
}

class grid{
public:
	grid(int x, int y, vector<int> rc, vector<int> cc, vector<int> sizes){
		vector<bool> yaxis(y, false);
		vector<vector<bool> > xaxis(x, yaxis);
		g = xaxis;
		r_const = rc;
		c_const = cc;
		sort(sizes.rbegin(), sizes.rend());
		for(unsigned int i = 0; i<sizes.size(); i++){
			Ship temp = Ship(sizes[i]);
			ships_off_board.push_back(temp);
		}
	}

	//This function investigates the grid g, which contains booleans that represent the current 
	//positions of all boards on the ship. 
	bool space_around_each_ship(){
		bool ret = true;

		if(g.size() <= 2 && g[0].size() <= 2 && ships_on_board.size() >=2){
			return false; 
		}


		for(list<Ship>::iterator i = ships_on_board.begin(); i!= ships_on_board.end(); i++){
			int s =  i->size();
			int x =  i->x;
			int y =  i->y;
			if(i->ori){ //if the ships is oriented horizontally. 
				if(x == 0){ // if we're on the top row
					if(y == 0){//if we're on the top left
						if(s == 1){
							ret = ret && !g[1][0] && !g[1][1] && !g[0][1];
						}
						else{
							ret = ret && !g[1][0] && !g[1][1];
							for(unsigned int j = 0; j<s; j++){
								ret = ret && !g[x+1][y+j];
							}

							if(y + s < g[0].size()){
								ret = ret && !g[0][y+s] && !g[1][y+s];
							}
						}
					}

					else if(y == g[0].size()-1){ //on the top right
						//remember, we assume the ship is oriented horizontally, so s must be 1.
						ret = ret && !g[0][g[0].size()-2] && !g[1][g[0].size()-2] && !g[1][g[0].size()-1];
					}

					else{ //top middle
						if(s == 1){
							ret = ret && !g[x][y-1] && !g[x][y+1] && !g[x+1][y-1] && !g[x+1][y] && !g[x+1][y+1];
						}
						else{
							ret = ret && !g[x][y-1] && !g[x+1][y-1] && !g[x+1][y];
							for(unsigned int j = 0; j<s; j++){
								ret = ret && !g[x+1][y+j];
							}
							if(y+s < g[0].size()){
								ret = ret && !g[0][y+s] && !g[1][y+s]; 
							}
						}
					}
				}

				else if (x == g.size()-1){ // if we're on the bottom row
					if(y == 0){ //bottom left
						if(s == 1){
							ret = ret && !g[x-1][0] && !g[x-1][1] && !g[x][1];
						}
						else{
							ret = ret && !g[x-1][0] && !g[x-1][1];
							for(unsigned int j = 0; j<s; j++){
								ret = ret && !g[x-1][y+j];
							}

							if(y + s < g[0].size()){
								ret = ret && !g[x-1][y+s] && !g[x][y+s];
							}
						}
					}

					else if(y == g[0].size()-1){//bottom right
						ret = ret && !g[x][y-1] && !g[x-1][y-1] && !g[x-1][y];
					}
					else{ //bottom middle
						if(s == 1) {
							ret = ret && !g[x][y-1] && !g[x][y+1] && !g[x-1][y-1] && !g[x-1][y] && !g[x-1][y+1];
						}
						else{
							ret = ret && !g[x][y-1] && !g[x-1][y-1] && !g[x-1][y];
							for(unsigned int j = 0; j<s; j++){
								ret = ret && !g[x-1][y+j];
							}
							if(y+s < g[0].size()){
								ret = ret && !g[x-1][y+s] && !g[x][y+s]; 
							}
						}
					}
				}

				else{ //we are on a middle row
					if(y == 0){ //middle left
						if(s == 1){
							ret = ret && !g[x-1][0] && !g[x-1][1] && !g[x][1] && !g[x+1][1] && !g[x+1][0];
						}
						else{
							ret = ret && !g[x-1][0] && !g[x+1][0];
							for(unsigned int j = 0; j<s; j++){
								ret = ret && !g[x-1][y+j] && !g[x+1][y+j];
							}

							if(y + s < g[0].size()){ //if we dont end on the right edge:
								ret = ret && !g[x-1][y+s] && !g[x][y+s] && !g[x+1][y+s];
							}
							else{
								ret = ret && !g[x-1][y+s-1] && !g[x+1][y+s-1];
							}
						}
					}

					else if(y == g[0].size()-1){//middle right, must be s == 1
						ret = ret && !g[x][y-1] && !g[x-1][y-1] && !g[x-1][y] && !g[x+1][y-1] && !g[x+1][y];
					}
					else{ //middle middle
						if(s == 1) {
							ret = ret && !g[x][y-1] && !g[x][y+1] && !g[x-1][y-1] && !g[x-1][y] && !g[x-1][y+1]
							&& !g[x+1][y-1] && !g[x+1][y] && !g[x+1][y+1];
						}
						else{
							ret = ret && !g[x][y-1] && !g[x-1][y-1] && !g[x-1][y] && !g[x+1][y] && !g[x+1][y-1];
							for(unsigned int j = 0; j<s; j++){
								ret = ret && !g[x-1][y+j] && !g[x+1][y+j];
							}
							if(y+s < g[0].size()-1){
								ret = ret && !g[x-1][y+s] && !g[x][y+s] && !g[x+1][y+s]; 
							}

							ret = ret && !g[x-1][y+s-1] && !g[x+1][y+s-1];
						}
					}	
				}
			}



			else{ //the ship is oriented vertically. 
				if(x == 0){ // if we're on the top row

					if(y == 0){//if we're on the top left
						if(s == 1){
							ret = ret && !g[1][0] && !g[1][1] && !g[0][1];
						}
						else{
							ret = ret && !g[0][1] && !g[1][1];
							for(unsigned int j = 0; j<s; j++){
								ret = ret && !g[x+j][y+1];
							}

							if(x + s < g.size()){
								ret = ret && !g[x+s][0] && !g[x+s][1] && !g[x+s-1][1];
							}
						}
					}

					else if(y == g[0].size()-1){ //on the top right
						if(s == 1){
							ret = ret && !g[x][y-1] && !g[x+1][y-1] && !g[x+1][y];
						}
						else{
							ret = ret && !g[x][y-1] && !g[x+1][y-1];
							for(unsigned int j = 0; j<s; j++){
								ret = ret && !g[x+j][y-1];
							}

							if(x + s < g.size()){
								ret = ret && !g[x+s][g[0].size()-1] && !g[x+s][g[0].size()-2];
							}

						}
					}

					else{ //top middle
						if(s == 1){
							ret = ret && !g[x][y-1] && !g[x][y+1] && !g[x+1][y-1] && !g[x+1][y] && !g[x+1][y+1];
						}
						else{

							ret = ret && !g[x][y-1] && !g[x][y+1];
							for(unsigned int j = 0; j<s; j++){
								ret = ret && !g[x+j][y-1] && !g[x+j][y+1];
							}
							if(y+s < g[0].size()){
								ret = ret && !g[x+s][y-1] && !g[x+s][y] && !g[x+s][y+1]; 
							}

							ret = ret && !g[1][y+s-1];
						}
					}
				}

				else if (x == g.size()-1){ // if we're on the bottom row
					if(y == 0){ //bottom left
						if(s == 1){
							ret = ret && !g[x-1][0] && !g[x-1][1] && !g[x][1];
						}
					}

					else if(y == g[0].size()-1){//bottom right
						if(s == 1){
							ret = ret && !g[x-1][g[0].size()-1] && !g[x-1][g[0].size()-2] && !g[x][g[0].size()-2];
						}					}
					else{ //bottom middle
						if(s == 1) {
							ret = ret && !g[x][y-1] && !g[x][y+1] && !g[x-1][y-1] && !g[x-1][y] && !g[x-1][y+1];
						}
					}
				}

				else{ //we are on a middle row
					if(y == 0){ //middle left
						if(s == 1){
							ret = ret && !g[x-1][0] && !g[x-1][1] && !g[x][1] && !g[x+1][1] && !g[x+1][0];
						}
						else{
							ret = ret && !g[x-1][0] && !g[x-1][1];
							for(unsigned int j = 0; j<s; j++){
								ret = ret && !g[x+j][y+1];
							}

							if(x + s < g.size()){ //if we dont end on the left edge:
								ret = ret && !g[x+s][y] && !g[x+s][y+1];
							}
						}
					}

					else if(y == g[0].size()-1){ //middle right
						if(s == 1){
							ret  = ret && !g[x][y-1] && !g[x-1][y-1] && !g[x-1][y] && !g[x+1][y-1] && !g[x+1][y];
						}
						else{
							ret = ret && !g[x-1][y-1] &&  !g[x-1][y];
							for(unsigned int j = 0; j<s; j++){
								ret = ret && !g[x+j][y-1];
							}

							if(x+s < g.size()){
								ret = ret && !g[x+s][y] && !g[x+s][y-1];
							}
						}
					}
					else{ //middle middle
						if(s == 1) {
							ret = ret && !g[x][y-1] && !g[x][y+1] && !g[x-1][y-1] && !g[x-1][y] && !g[x-1][y+1]
							&& !g[x+1][y-1] && !g[x+1][y] && !g[x+1][y+1];
						}
						else{
							ret = ret && !g[x-1][y-1] && !g[x-1][y] && !g[x+1][y+1];
							for(unsigned int j = 0; j<s; j++){
								ret = ret && !g[x+j][y-1] && !g[x+j][y+1];
							}
							if(x+s < g.size()-1){
								ret = ret && !g[x+s][y-1] && !g[x+s][y] && !g[x+s][y+1]; 
							}
						}
					}	
				}
			}
		}
			
		return ret;
	}

	void solve(){
		if (ships_off_board.size() == 0 && verify_constraints()){
			sols.push_back(ships_on_board);
			return;
		}

		for(unsigned int i = 0; i<g.size(); i++){
			for(unsigned int j = 0; j<g[0].size(); j++){
				bool o = true;

				if(insert(ships_off_board.front(), i, j, o)){
					ships_off_board.erase(ships_off_board.begin());
					if(space_around_each_ship()){
						solve();
					}
					del(ships_on_board.back());
				}
								
				
				if(ships_off_board.front().size() != 1){
					if(insert(ships_off_board.front(), i, j, !o) ){ //ensure no duplicate solutions 
					//with submarines 

						ships_off_board.erase(ships_off_board.begin());
						if(space_around_each_ship()){
							solve();
						}
						del(ships_on_board.back());
						
					}
				}
				
				
			}
			
		}
	}


	//x represents a row, y a column
	//consider adding functionality such that insert returns false for optimality
	//when the insert will not have space around the ship inserted.

	bool insert(Ship& s, int x, int y, bool h){ //if h is true, then the ship should be inserted horrizontally
		s.rotate(h);
		//if the ship will go off an edge when being inserted, return false
		if((x + s.size() > g.size() && !h) || (y + s.size() > g[0].size() && h) ){return false;}
		s.x = x;
		s.y = y;

		if(!h){ //ship is vertical in orientation
			//first check if there is space to place the ship vertically.
			for (int i = x; i < x + s.size();  i++){
				if(g[i][y]){ //if there's no space, do not insert, immediately return false
					return false;
				}	
			}


			for(int i = x; i < x + s.size(); i++){
				g[i][y] = true;
			}
			ships_on_board.push_back(s);
			return true;

		}

		else if(h){
			for (int i = y; i < y + s.size() && i < g[0].size();  i++){
				//prevent writing over an existing ship
				if(g[x][i]){ //if there's no space, do not insert, immediately return false
					return false;
				}	
			}

			for(int i = y; i<y+s.size(); i++){
				g[x][i] = true;
			}
			ships_on_board.push_back(s);
			return true;
		}
		
	}

	bool del(Ship& s){
		bool ret;
		for(list<Ship>::iterator i = ships_on_board.begin(); i!=ships_on_board.end(); i++){
			if(*i == s){
				if(s.ori){
					for(unsigned int j = s.y; j< s.y+s.size(); j++) {
						g[s.x][j] = false;
					}
				}
				if(!s.ori){
					for(unsigned int j = s.x; j< s.x+s.size(); j++){
							g[j][s.y] = false;
						}
				}

				ships_off_board.push_front(*i);

				ships_on_board.erase(i);
				return true;
			}
		}
		return false;
	}

	//clears the grid, ships on board vector, and ships off board vector.
	void clear_board(){
		if(g.size() == 0){
			return;
		}
		if(g[0].size() == 0){
			return;
		}
		ships_off_board.clear();
		ships_on_board.clear();
		//clear board boolean representation
		vector<bool> yaxis(g.size(), false);
		vector<vector<bool> > xaxis(g[0].size(), yaxis);
		g = xaxis;
	}

	void print_sols(){
		clear_board();
		for(unsigned int i = 0; i<sols.size(); i++){
			for(list<Ship>::iterator j = sols[i].begin(); j!=sols[i].end(); j++){
				insert(*j, j->x, j->y, j->ori);
			}
			print();
			clear_board();
		}
		cout<<"Found "<<sols.size()<<" solutions."<<endl;
	}

	void print(){
		ships_on_board.sort(read);
		if(g.size() == 0){
			cout<<"Empty grid"<<endl;
			return;
		}

		if(ships_on_board.size() == 0){
			cout<<" ";
			for(unsigned int i = 0; i<g[0].size(); i++){
				cout<<c_const[i];
			}
			cout<<endl;
			cout<<" ";
			for(unsigned int i = 0; i<g[0].size(); i++){
				cout<<"-";
			}
			cout<<endl;
			for(unsigned int i = 0; i<g.size(); i++){
				cout<<"|";
				for(unsigned int j = 0; j<g[0].size(); j++){
					cout<<" ";
				}
				cout<<"|"<<r_const[i]<<endl;
			}
			cout<<" "; 	
			for(unsigned int i = 0; i<g[0].size(); i++){
				cout<<"-";
			}
			cout<<endl;
			return;
		}


		cout<<" ";
		for(unsigned int i = 0; i<g[0].size(); i++){
			cout<<c_const[i];
		}
		cout<<endl;
		cout<<" ";
		for(unsigned int i = 0; i<g[0].size(); i++){
			cout<<"-";
		}

		cout<<endl;
		for(unsigned int i = 0; i<g.size(); i++){ //which row
			cout<<"|";
			for(unsigned int j = 0; j<g[0].size(); j++){ //which col
				string out = " ";
				for(list<Ship>::iterator k = ships_on_board.begin(); k!=ships_on_board.end();k++){//if any ship is on the current coord (i,j)
					if(k->ori && k->x == i && k->y <= j &&
					 j < k->y + k->size()){
						//if this ships is horizontal, and is on the same row as one of the ships,
						//and the current column coordinate we're on is within the size of the starting point of the ship:
						out = "X";
						break;
					}
					else if(!k->ori && j == k->y &&
					 k->x <= i && i < k->x + k->size()){
						out = "X";
						break;
					}
				}
				cout<<out;
				if(j == g[0].size()-1){
						cout<<"|"<<r_const[i];
						cout<<endl;
				}
			}
		}
		cout<<" ";
		for(unsigned int i = 0; i<g[0].size(); i++) {
			cout<<"-";
		}
		cout<<endl;
	}

	int sum(vector<int> & a){
		int s = 0;
		for(unsigned int i = 0; i<a.size(); i++){
			s+=a[i];
		}
		return s;
	}

	bool verify_constraints(){
		if(g.size() == 0){
			return false;
		}

		vector<int> rtemp = r_const;
		vector<int> ctemp = c_const;

		for(unsigned int i = 0; i<g.size(); i++){
			for(unsigned int j = 0; j<g[0].size(); j++){
				if(g[i][j]){
					rtemp[i]-=1;
					ctemp[j]-=1;
				}
			}
			if(rtemp[i] != 0){
				return false;
			}
		}

		if(sum(rtemp) + sum(ctemp) == 0){
			return true;
		}
		return false;
	}

	bool verify_cols(){
		if(g.size() == 0){
			return false;
		}

		for(unsigned int i = 0; i<g.size(); i++){
			int row_c = 0;
			for(unsigned int j = 0; j<g[0].size(); j++){
				if(g[i][j]){
					row_c++;
				}
			}
			if(row_c != r_const[i]){
				return false;
			}
		}
		return true;
	}

	private:
	vector<int> c_const;
	vector<int> r_const; //contraints for all cols/rows
	vector<vector<bool> > g;
	list<Ship> ships_off_board;
	list<Ship> ships_on_board;
	vector<list<Ship> > sols;

};

int main(){
	string cmd;
	cout<<"Input file name: "<<endl;
	cin>> cmd;
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
	gr.solve();
	gr.print_sols();
}