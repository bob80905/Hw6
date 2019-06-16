#ifndef _BOARD_H_
#define _BOARD_H_


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

		vector<int> occx(rc.size(), 0);
		rowocc = occx;

		vector<int> occy(cc.size(), 0);
		colocc = occy;

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

							for(unsigned int j = 0; j<s; j++){
								ret = ret && !g[x+j][y-1] && !g[x+j][y+1];
							}
							if(y+s < g[0].size()){
								ret = ret && !g[x+s][y-1] && !g[x+s][y] && !g[x+s][y+1]; 
							}

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


	//p10 takes 28.9s and finds 3754 solutions.
	//takes 3.12s with -O3 compiler optimization
	void solve(int x, int y){ //begin handling same ship duplicates (also for each ship record the last valid placed coordinate)
		//print();
		if (ships_off_board.size() == 0 && verify_constraints()){
			sols.push_back(ships_on_board);
			return;
		}
		unsigned int i = 0; 
		unsigned int j = 0;
		if(ships_on_board.size() > 0 && ships_off_board.size() > 0){
			if(ships_off_board.front().size() == ships_on_board.back().size()){
				i = x;
				j = y;
			}
		}
		


		for(i; i<g.size(); i++){
			for(j; j<g[0].size(); j++){
				bool o = true;

				if(insert(ships_off_board.front(), i, j, o)){
					ships_off_board.erase(ships_off_board.begin());
					if(space_around_each_ship()){
						solve(i, j);
					}
					del(ships_on_board.back());
				}
								
				
				if(ships_off_board.front().size() != 1){
					if(insert(ships_off_board.front(), i, j, !o) ){ //ensure no duplicate solutions 
					//with submarines 

						ships_off_board.erase(ships_off_board.begin());
						if(space_around_each_ship()){
							solve(i, j);
						}
						del(ships_on_board.back());
						
					}
				}	
			}
			j = 0;
			
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

			vector<int> rtemp = rowocc;
			vector<int> ctemp = colocc;

			for(int i = x; i<x+s.size(); i++){
				rtemp[i]+=1;
				if(rtemp[i]>r_const[i]){
					return false;
				}
			}

			if(ctemp[y] + s.size() > c_const[y]){
				return false;
			}


			for(int i = x; i < x + s.size(); i++){
				g[i][y] = true;
			}
			rowocc = rtemp;
			colocc[y]+=s.size();
			ships_on_board.push_back(s);
			return true;

		}

		else{
			for (int i = y; i < y + s.size();  i++){
				//prevent writing over an existing ship
				if(g[x][i]){ //if there's no space, do not insert, immediately return false
					return false;
				}	
			}


			vector<int> rtemp = rowocc;
			vector<int> ctemp = colocc;

			for(int i = y; i<y+s.size(); i++){
				ctemp[i]+=1;
				if(ctemp[i]>c_const[i]){
					return false;
				}
			}

			if(rowocc[x] + s.size() > r_const[x]){
				return false;
			}

			for(int i = y; i<y+s.size(); i++){
				g[x][i] = true;
			}
			colocc = ctemp;
			rowocc[x]+=s.size();
			ships_on_board.push_back(s);
			return true;
		}
		
	}


	bool del(Ship& s){
		bool ret;
		for(list<Ship>::iterator i = ships_on_board.begin(); i!=ships_on_board.end(); i++){
			if(*i == s){
				if(s.ori){ //delete horrizontal ship
					for(unsigned int j = s.y; j< s.y+s.size(); j++) {
						g[s.x][j] = false;
						colocc[j]-=1;
					}
					rowocc[s.x]-=s.size();
				}
				if(!s.ori){
					for(unsigned int j = s.x; j< s.x+s.size(); j++){
							g[j][s.y] = false;
							rowocc[j]-=1;
						}
						colocc[s.y]-=s.size();
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
		setzero(rowocc);
		setzero(colocc);
		//clear board boolean representation
		vector<bool> yaxis(g[0].size(), false);
		vector<vector<bool> > xaxis(g.size(), yaxis);

		g = xaxis;
	}

	void setzero(vector<int>& a){
		for(unsigned int i = 0; i<a.size(); i++){
			a[i] = 0;
		}
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
		if(sols.size() > 0){
			for(list<Ship>::iterator j = sols[0].begin(); j!=sols[0].end(); j++){
		 		insert(*j, j->x, j->y, j->ori);
			}
			print();
		}
		cout<<"Found "<<sols.size()<<" solutions."<<endl;
	}

	//just print using g

	void print(){
		if(g.size() == 0){
			cout<<"Empty grid"<<endl;
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
		for(unsigned int i = 0; i<g.size(); i++){
			cout<<"|";
			for(unsigned int j = 0; j<g[0].size(); j++){
				if(g[i][j] == false){
					cout<<" ";
				}
				else{
					cout<<"X";
				}
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

	int zero(vector<int> & a){
		for(unsigned int i = 0; i<a.size(); i++){
			if(a[i] != 0){
				return false;
			}
		}
		return true;
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

		if(zero(rtemp) && zero(ctemp)){
			return true;
		}
		return false;
	}

	private:
	vector<int> c_const;
	vector<int> r_const; //contraints for all cols/rows
	vector<int> rowocc;
	vector<int> colocc; //keeps number of ship parts on each column/row
	vector<vector<bool> > g;
	list<Ship> ships_off_board;
	list<Ship> ships_on_board;
	vector<list<Ship> > sols;

};

#endif