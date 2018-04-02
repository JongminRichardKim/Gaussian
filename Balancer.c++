#include <iostream>
#include <sstream>
#include <algorithm>
#include "Matrix.h"
#include <math.h>
using namespace std;

struct Element {
	string symbol;
	int row;
	Element *next;
	Element(string s, int r, Element *n){
		symbol = s;
		row = r;
		next = n;
	}
};

int hash(string symbol) {
	int sum = 0;
	for(int i = 0; i < symbol.length(); ++i) {
		sum += (int)symbol[i];
	}
	return sum;
}

bool isUpper(char character){
	return (int)character >= 65 && (int)character <= 90; 
}

bool isLower(char character){
	return (int)character >= 97 && (int)character <= 122; 
}

int main (){
	cout << "= Chemical Equation Balancer =" << endl;
	string s = "";

	Element* found_element[213] = {NULL};
	vector<vector<float>*> matrix;
	vector<float> augment;
	int num_columns = 0;
	int next_row = 0;


	while(s != "Exit" && s != "exit"){
		cout << "Enter an equation"
			"\nEnter \"Exit\" to close the program\n";

		getline(cin, s);
		// remove all of the spaces
		s.erase(remove(s.begin(), s.end(), ' '), s.end());

		std::stringstream ss(s);
		string item;
		string before;
		string after;
		int i = 0;
		while (getline(ss, item, '>')) {
			if(i == 0) before = item;
			else if (i == 1) after = item;
			++i;
		}
		if(s != "Exit" && s != "exit"){
			if(i == 2) {
				// Each row represents an element
				// Each column represents a coefficient
				std::stringstream ss2(before);
				string item2;
				vector<string> before_items;
				vector<string> after_items;

				while (getline(ss2, item2, '+')) {
					before_items.push_back(item2);
					int col_index = num_columns++;
					addColumn(matrix);
					
					int k = 0;
					int length = item2.length();
					
					while(k < length) {
						string symbol = "";
						int subscript = 1;
						
						symbol = item2[k];
						++k;
						
						if(isLower(item2[k])) {
							symbol += item2[k];
							++k;
						}
						
						int h = k;
						while(h < length && !isUpper(item2[h])) {
							// If it is at any point NaN, this equation is invalid
							++h;
						}

						if((h - k) != 0) {
							string sub_string = item2.substr(k, h - k);
							std::stringstream parse3(sub_string);
							parse3 >> subscript;
						}
						k = h;
						
						int hash_num = hash(symbol);
						Element *e;
						if(found_element[hash_num] == NULL) {
							e = new Element(symbol, next_row, NULL);
							found_element[hash_num] = e;
							++next_row;
							vector<float>* new_row = new vector<float>(num_columns);
							matrix.push_back(new_row);
							augment.push_back(0);
						} else {
							e = found_element[hash_num];
							Element *prev;
							while(e != NULL && e->symbol != symbol) {
								prev = e;
								e = e->next;
							}
							if(e == NULL) {
								e = new Element(symbol, next_row, NULL);
								prev->next = e;
								++next_row;
								vector<float>* new_row = new vector<float>(num_columns);
								matrix.push_back(new_row);
								augment.push_back(0);
							}
						}
						(*matrix[e->row])[col_index] += subscript;
					}
				}

				std::stringstream ss3(after);
				string item3;
				
				while (getline(ss3, item3, '+')) {
					after_items.push_back(item3);
					int col_index = num_columns++;
					addColumn(matrix);
					
					int k = 0;
					int length = item3.length();
					
					while(k < length) {
						string symbol = "";
						int subscript = 1;
						symbol= item3[k];
						++k;
						if(isLower(item3[k])) {
							symbol += item3[k];
							++k;
						}
						
						int h = k;
						while(h < length && !isUpper(item3[h])) {
							++h;
						}
						if((h - k) != 0) {
							string sub_string = item3.substr(k, h - k);
							std::stringstream parse3(sub_string);
							parse3 >> subscript;
						}
						k = h;
						subscript *= -1;
						int hash_num = hash(symbol);
						Element *e;
						if(found_element[hash_num] == NULL) {
							e = new Element(symbol, next_row, NULL);
							found_element[hash_num] = e;
							++next_row;
							vector<float>* new_row = new vector<float>(num_columns);
							matrix.push_back(new_row);
							augment.push_back(0);
						} else {
							e = found_element[hash_num];

							Element *prev;
							while(e != NULL & e->symbol != symbol) {
								prev = e;
								e = e->next;
							}
							if(e == NULL) {
								e = new Element(symbol, next_row, NULL);
								prev->next = e;
								++next_row;
								vector<float>* new_row = new vector<float>(num_columns);
								matrix.push_back(new_row);
								augment.push_back(0);
							}
						}
						(*matrix[e->row])[col_index] += subscript;
					}
				}

				int solutions = numSolutions(matrix, augment, num_columns);
				vector<float> coefficients;
				float min = 1.0f;

				if(solutions != 0){
					for(int j = 0; j < before_items.size(); ++j){
						bool found = false;
						for(int k = 0; k < matrix.size() && !found; ++k) {
							bool leading_coefficient = true;
							for(int l = 0; l < j && leading_coefficient; ++l) {
								if((*matrix[k])[l] != 0.0f) leading_coefficient = false;
							}
							if(leading_coefficient && (*matrix[k])[j] == 1) {
								found = true;
								coefficients.push_back(augment[k]);
								float rem = augment[k] - floor(augment[k]);
								if(fabs(rem) > 0.0001f && rem < min) min = rem;
							}
						}
					}

					for(int j = 0; j < after_items.size(); ++j){
						bool found = false;
						for(int k = 0; k < matrix.size() && !found; ++k) {
							bool leading_coefficient = true;
							for(int l = 0; l < (j + before_items.size()) && leading_coefficient; ++l) {
								if((*matrix[k])[l] != 0.0f) leading_coefficient = false;
							}
							if(leading_coefficient && (*matrix[k])[j+ before_items.size()] == 1) {
								found = true;
								coefficients.push_back(augment[k]);
								float rem = augment[k] - floor(augment[k]);
								if(fabs(rem) > 0.0001f && rem < min) min = rem;
							}
						}
					}
					while(min != 1.0f){
						float divisor = min;
						float temp = 1.0f;
						for(int k = 0; k < coefficients.size(); ++k)  {
							coefficients[k] /= divisor;
							float rem = coefficients[k] - floor(coefficients[k]);
							if(fabs(rem) > 0.0001f && rem < temp) temp = rem;
						}
						min = temp;
					}
					cout << "	";
					for(int k = 0; k < before_items.size(); ++k)  {
						cout << coefficients[k] << before_items[k];
						if(k!= before_items.size()-1) cout << " + ";
					}
					cout << " > ";
					for(int k = 0; k < after_items.size(); ++k)  {
						cout << coefficients[k + before_items.size()] << after_items[k];
						if(k!= after_items.size()-1) cout << " + ";
					}

					cout << endl;
				} else {
					cout << "Invalid Equation" << endl;
				}

				// remember to delete everything in the matrix
				matrix.clear();
				augment.clear();
				num_columns = 0;
				next_row = 0;
				for(int j = 0; j < 213; ++j) {
					if(found_element != NULL) {
						delete found_element[j];
						found_element[j] = NULL;
					}
				}
				
				//*/
			} else {
				cout << "Invalid Equation" << endl;
			}
		}
	}

	// remember to delete everything in the matrix
	return 0;
}
