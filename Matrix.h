#include <vector>
using namespace std;

class Matrix {
	private: 
		vector<vector<float>*> mat;
		int numColumns;
	public:
		Matrix() : mat(){
			numColumns = 0;
		}

		vector<float> &operator [](int x) {
			return (*mat[x]);
		}

		void push_back(vector<float>* new_row) {
			mat.push_back(new_row);
		}

		// Adds a new column to the matrix
		int add_column(){
			int rows = num_rows();
			Matrix &thisMatrix = (*this);

			for(int i = 0; i < rows; ++i){
				thisMatrix[i].push_back(0);
			}
			return numColumns++; // return first since we want to return the index, not the number of columns
		}

		int num_rows(){
			return mat.size();
		}

		int num_columns(){
			return numColumns;
		}

		// Prints out the contents of the matrix
		void print_matrix(){
			int rows = num_rows();
			int columns = num_columns();
			Matrix &thisMatrix = (*this);

			if(rows == 0) {
				cout << "	Empty Matrix!" << endl;
			} else {
				for(int i = 0; i < rows; ++i) {
					cout << "[";
					for(int j = 0; j < columns; ++j){
						cout << " " << thisMatrix[i][j] << " ";
					}
					cout << " ]" << endl;
				}
			}
		}
};

class AugmentedMatrix : public Matrix {
	private:
		vector<float> augment;
	public:
		AugmentedMatrix() : Matrix(), augment() {};

		float get_augment(int x) {
			return augment[x];
		}

		void push_back (vector<float>* new_row){
			Matrix::push_back(new_row);
			augment.push_back(0);
		}

		void push_back(vector<float>* new_row, float augValue) {
			Matrix::push_back(new_row);
			augment.push_back(augValue);
		}

		// Prints out the contents of the matrix, including the augment
		void print_matrix() {
			int rows = num_rows();
			int columns = num_columns();
			AugmentedMatrix &thisMatrix = (*this);

			if(rows == 0) {
				cout << "	Empty Matrix!" << endl;
			} else {
				for(int i = 0; i < rows; ++i) {
					cout << "[";
					for(int j = 0; j < columns; ++j){
						cout << " " << thisMatrix[i][j] << " ";
					}
					cout << "| " << augment[i] << " ]" << endl;
				}
			}
		}

		// Row-reduces the matrix
		void gaussian_elimination(){
			int n_rows = num_rows();
			int n_col = num_columns();
			AugmentedMatrix &thisMatrix = (*this);

			if(n_rows == 0) {
				cout << "	Empty Matrix!" << endl;
			} else {
				for(int i = 0; i < n_col; ++i) {
					// i is the column that we currently want the leading coefficient for
					bool found = false;
					int row = -1;
					
					// Look for the first row with a leading coefficient at column i
					for(int j = 0; j < n_rows && !found; ++j) { 
						bool coeff_found = false;
						int coeff_column = -1;

						for(int k = 0; k <= i && !coeff_found; ++k) {
							if(thisMatrix[j][k] != 0.0) {
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
						float dividend = thisMatrix[row][i];
						augment[row] /= dividend;
						for(int j = i; j < n_col; ++j) {
							thisMatrix[row][j] /= dividend;
						}

						// subtract that row from every other row with a nonzero ith column
						for(int j = 0; j < n_rows; ++j) {
							if(j != row && thisMatrix[j][i] != 0.0) {
								float factor = thisMatrix[j][i];
								
								augment[j] -= factor * augment[row];
								for(int k = 0; k < n_col; ++k) {
									thisMatrix[j][k] -= factor * thisMatrix[row][k];
								}
							}
						}
					}
				}
			}
		}

		int num_solutions(){
			// Make sure that the matrix is row-reduced
			gaussian_elimination();

			int rows = num_rows();
			int cols = num_columns();
			AugmentedMatrix &thisMatrix = (*this);

			// Iterate through the matrix
			// If there is a zero row with a non-zero augment, no solutions
			// Keep count of non-zero rows
			int num_non_zero = 0;
			int bit_vector = 0;

			for(int j = 0; j < rows; ++j) {
				bool zero_row = true;
				int leading_coefficient = -1;

				// Go through the current row and find the column with the leading coefficient
				for(int i = 0; i < cols && zero_row; ++i) {
					if(thisMatrix[j][i] != 0) {
						leading_coefficient = i;
						zero_row = false;
					}
				}
				// If it is a zero row lacking any coefficients...
				if(zero_row) {
					// And it has a non-zero augment, there are no solutions
					if(get_augment(j) != 0) {
						return 0;
					}
					// If it has a zero-augment, continue
				// If it is a non-zero row
				} else {
					// Since it is row-reduced, only one row should correspond with a leading coefficient
					bit_vector |= (1 << leading_coefficient);
					++num_non_zero;
				}
			}

			if(num_non_zero == cols) {
				return 1;
			} else {
				// Add a new row that sets a currently "free" variable to 1;
				for(int i = 0; i < cols; ++i) {
					if(((bit_vector >> i) & 1) == 0) {
						vector<float>* new_row = new vector<float>(cols);
						thisMatrix.push_back(new_row, 1);
						thisMatrix[num_rows()-1][i] = 1;
					}
				}
				// Apply guassian elimination to the new matrix
				gaussian_elimination();
				return 2;
			}
		}
};
