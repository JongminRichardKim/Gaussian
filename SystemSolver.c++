#include <iostream>
#include <sstream>
#include <algorithm>
#include "Matrix.h"
using namespace std;

int hash(char letter) {
	return toupper(letter) - 65;
}

bool isLetter(char character){
	return (toupper(character) >= 65 && toupper(character) <= 90); 
}

int main (){
	cout << "= System of Equation Solver =" << endl;
	string s = "";
	int table[26];
	fill_n(table, 26, -1);
	vector<char> variables;

	vector<vector<float>*> matrix;
	vector<float> augment;
	int next_col = 0; // also functions as the number of columns

	while(s != "Exit" && s != "exit"){
		cout << "Enter an equation"
			"\nEnter \"Print\" to print the current matrix"
			"\nEnter \"Exit\" to close the program and print the solution\n";

		getline(cin, s);
		// remove all of the spaces
		s.erase(remove(s.begin(), s.end(), ' '), s.end());

		std::stringstream ss(s);
		string item;
		string before;
		string after;
		int i = 0;
		while (getline(ss, item, '=')) {
			if(i == 0) before = item;
			else if (i == 1) after = item;
			++i;
		}

		if(s != "Exit" && s != "exit" && s != "Print" && s != "print"){
			if(i == 2){
				// For now, after is a single integer,
				// before is an expression with only + or - and no constants
				vector<float>* new_row = new vector<float>(next_col);
				matrix.push_back(new_row);

				float value;

				std::stringstream parse2(after);
				parse2 >> value;
				augment.push_back(value);

				int length = before.length();
				int i = 0; // beginning parts of the substring
				char character;
				float coefficient;
				while(i < length) {
					//The case where the first variable has no coefficient
					if(isLetter(before[i])) {
						character = before[i];
						coefficient = 1.0f;
					} else {
						int j = i+1;
						while(j < length && !isLetter(before[j])){
							++j;
						}
						character = before[j];

						string sub_string = before.substr(i, j-i);
						if(sub_string[0] == '+') {
							sub_string.erase(0, 1);
						}
						if(sub_string[0] == '-' && sub_string.length() == 1) {
							coefficient = -1.0f;
						} else {
							std::stringstream parse3(sub_string);
							parse3 >> coefficient;
						}
						i = j;
					}
					
					if(table[hash(character)] == -1) {
						table[hash(character)] = next_col;
						++next_col;
						addColumn(matrix);
						variables.push_back(character);
					}
					(*matrix[matrix.size()-1])[table[hash(character)]] += coefficient;
					
					++i;
				}
			} else {
				cout << "Invalid Equation or Command" << endl;
			}
		} else {
			if(s == "Print" || s == "print") printMatrix(matrix, augment, next_col);
		}
	}
	
	int solutions = numSolutions(matrix, augment, next_col);
	if(solutions != 0){
		if (solutions > 1) {
			cout << "Many Solutions. One Possible Solution:" << endl;
		} else {
			cout << "One Solution as Follows:" << endl;
		}
		for(int i = 0; i < variables.size(); ++i){
			char var = variables[i];
			bool found = false;
			for(int j = 0; j < matrix.size() && !found; ++j){
				bool leading_coefficient = true;
				for(int k = 0; k < table[hash(var)] && leading_coefficient; ++k) {
					if((*matrix[j])[k] != 0) leading_coefficient = false;
				}
				if(leading_coefficient && (*matrix[j])[table[hash(var)]] == 1){
					cout << " " << var << " = " << augment[j] << endl;
					found = true;
				}
			}
		}

	} else {
		cout << "No solutions; this system of equations is inconsistent." << endl;
	}
	
	return 0;
}
