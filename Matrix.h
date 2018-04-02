#include <vector>
using namespace std;

// Adds a new column to the matrix
void addColumn(vector<vector<float>*>& matrix){
	int size = matrix.size();
	for(int i = 0; i < size; ++i){
		matrix[i]->push_back(0);
	}
}

// Prints out the contents of the matrix
void printMatrix(vector<vector<float>*>& matrix, vector<float>& augment, int num_col){
	int size = matrix.size(); // num rows
	if(size == 0) {
		cout << "	Empty Matrix!" << endl;
	} else {
		for(int i = 0; i < size; ++i) {
			cout << "[";
			for(int j = 0; j < num_col; ++j){
				cout << " " << (*matrix[i])[j] << " ";
			}
			cout << "| " << augment[i] << " ]" << endl;
		}
	}
}

// Row-reduces the matrix
void gaussianElimination(vector<vector<float>*>& matrix, vector<float>& augment, int num_col){
	int num_rows = matrix.size();
	if(num_rows == 0) {
		cout << "	Empty Matrix!" << endl;
	} else {
		for(int i = 0; i < num_col; ++i) {
			// i is the column that we currently want the leading coefficient for
			bool found = false;
			int row = -1;
			
			// Look for the first row with a leading coefficient at column i
			for(int j = 0; j < num_rows && !found; ++j) { 
				bool coeff_found = false;
				int coeff_column = -1;

				for(int k = 0; k <= i && !coeff_found; ++k) {
					if((*matrix[j])[k] != 0.0) {
						coeff_found = true;
						coeff_column = k;
					}
				}

				if(coeff_found && coeff_column == i) {
					found = true;
					row = j;
				}
			}

			if(found) {
				// reduce the row such that the ith column is 1
				float dividend = (*matrix[row])[i];
				augment[row] /= dividend;
				for(int j = i; j < num_col; ++j) {
					(*matrix[row])[j] /= dividend;
				}

				// subtract that row from every other row with a nonzero ith column
				for(int j = 0; j < num_rows; ++j) {
					if(j != row && (*matrix[j])[i] != 0.0) {
						float factor = (*matrix[j])[i];
						
						augment[j] -= factor * augment[row];
						for(int k = 0; k < num_col; ++k) {
							(*matrix[j])[k] -= factor * (*matrix[row])[k];
						}
					}
				}
			}
		}
	}
}

int numSolutions(vector<vector<float>*>& matrix, vector<float>& augment, int num_col){
	// Make sure that the matrix is row-reduced
	gaussianElimination(matrix, augment, num_col);

	// Iterate through the matrix
	// If there is a zero row with a non-zero augment, no solutions
	// Keep count of non-zero rows
	int num_rows = matrix.size();
	int num_non_zero = 0;
	int bit_vector = 0;

	for(int j = 0; j < num_rows; ++j) {
		bool zero_row = true;
		int leading_coefficient = -1;
		for(int i = 0; i < num_col && zero_row; ++i) {
			if((*matrix[j])[i] != 0) {
				leading_coefficient = i;
				zero_row = false;
			}
		}
		// If it is a zero row
		if(zero_row) {
			if(augment[j] != 0) {
				return 0;
			}
		// If it is a non-zero row
		} else {
			// Since it is row-reduced, only one row should correspond with a leading coefficient
			bit_vector |= (1 << leading_coefficient);
			++num_non_zero;
		}
	}
	
	if(num_non_zero == num_col) {
		return 1;
	} else {
		// Add a new row that sets a currently "free" variable to 1;
		for(int i = 0; i < num_col; ++i) {
			if(((bit_vector >> i) & 1) == 0) {
				vector<float>* new_row = new vector<float>(num_col);
				matrix.push_back(new_row);
				(*matrix[matrix.size()-1])[i] = 1;
				augment.push_back(1);
			}
		}
		gaussianElimination(matrix, augment, num_col);
		return 2;
	}
}