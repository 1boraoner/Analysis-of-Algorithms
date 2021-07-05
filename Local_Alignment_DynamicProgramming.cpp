/*
* ******************* HOMEWORK #3 ****************
*												 *
*	Name Surname : Bora Oner					 *
*	Student Id   : 150170301					 *
*	Date	     : 19.05.2021					 *
*												 *
**************************************************
*/

#include <iostream>	//cout
#include <vector>	//vector data type
#include <string>	//string data type and string operations
#include <fstream>	//file operations
using namespace std;


class Matrix {									//Matrix class for reprsenting the Smith-Waterman Matrix
private:
	string input_string1;						//First string in the matrix that the chars are the rows of hte matr,x
	string input_string2;						//Second string in the matrix that the chars are the columns of the matrix
	vector<vector<int> > matrix;				//Matrix is a vector that is consisting of multiple vector (basically a 2D array)

	int match;									//match score
	int mismatch;								//mismatch penalty
	int gap;									//gap penalty

	int max_score;								//max score in the matrix, is used to ease the identifying the maximum values grid in the matrix 
	vector<pair<int, int> > max_score_coord;	//vector that holds the coordinates of the max_valued points in the matrix
	vector<pair<int,string> > found_allignments;//vector for storing the Scores and cooresponding strings that are result of local alignment
	
public:
	Matrix(string,string);						// Matrix class constructor
	~Matrix();									// Matrix class destructor
	void construct_matrix();					//filling the matrix according to the rules of Smith-Waterman algorithm, a dynamic programming approach
	pair<int, string> traceback_op(pair<int, int>);//traceback operation, takes the row and column information as parameter and moves diagonally until reaching 0
	void print_locals(ofstream&);				//for constructing the output.txt file
};

Matrix::~Matrix() {	//Matrix destructor

	matrix.clear();				//matrix is cleared
	max_score_coord.clear();	//vector container is cleared
	found_allignments.clear();	//vector is cleared

}
Matrix::Matrix(string s1, string s2) {	//Matrix Constructor
	
	input_string1 = s1;				//input string is assigned
	input_string2 = s2;				//input string is assigned

	match = 1;						//match score is defined 
	mismatch = -50;					//mismatch penalty depends on the length of any of the strings (prevents the diagonal alignment problem)
	gap = -100;						//gap penalty

	max_score = 0;					//max score is set 0 initially
	vector<vector<int> > mat(s1.length() + 1);	//matrix will have an additional row and column that are filled with zeros
	matrix = mat;

	for (unsigned int i=0; i < s1.length()+1; i++) {	//the extra row and column is filled with zero in this follwing nested for block the other

		if (i == 0) {	//the first row that is acutally an extra row is filled with zeros
			for (unsigned int j = 0; j < s2.length() + 1; j++) {
				matrix[i].push_back(0);
			}
		}
		else {			//the first column of all rows are zeros
			matrix[i].push_back(0);
		}
	}
}

void Matrix::construct_matrix() {	//filling the matrix and identify the max valued grid
	
	for (unsigned int i = 1; i < matrix.size(); i++) {			//	iterating over the rows (string1 chars)

		for (unsigned int j = 1; j < matrix[0].size(); j++) {	//	iterating over the rows (string2 chars)

			int lateral = matrix[i][j - 1] + gap;				//	lateral point in the matrix of the 
			int vertical = matrix[i - 1][j] + gap;				//	vertical neighbour of the i,j point in the matrix

			int diag = matrix[i-1][j-1];						//	diagonal neighbour of i,j element
			if (input_string1[i - 1] == input_string2[j - 1]) {	//	if the string1 and string2 chars are the same 
				diag += match;									//	diagonal element is added with the match score
			}
			else {		
				diag += mismatch;								//	diagonal element is added with the mismatch score
			}

			int arr[3] = { lateral,vertical,diag};				//	the max value out of the three values must be assigned
			//finding the max out of the possible values
			int max = 0;										//	max is 0 in the worst case
			for (int k = 0; k < 3; k++) {
				if (max < arr[k]) {
					max = arr[k];
				}
			}
			matrix[i].push_back(max);							//	assign the value to the corresponding matrix index

			if (matrix[i][j] > max_score) {						//	check if the matrix's point is the max score
				max_score = matrix[i][j];
			}
		}
	}
}

pair<int,string> Matrix::traceback_op(pair<int,int> location) {	//traceback in the matrix, location is a pair type that has i,j values 

	int acc = 0;		//Score is accumulated in the acc variable
	int traverse = matrix[location.first][location.second];	//the value in the location

	string s;			//string that will be aligned
	while (traverse) {	//while not 0

		acc += traverse;//the matrix grids are added iteratively
		traverse = matrix[--location.first][--location.second];	//traverse point is moved up to diagonally 
		s = input_string1[location.first] + s;					//string is appended from head of the string

	}
	pair<int, string> as = { acc , s };							//pair is constructed
	return as;													//pair is returned
}

void Matrix::print_locals(ofstream &of) {	//invokes tracback operation, writes the alignment to the file that is given as parameter

	//find the max_score i,j
	
	for (unsigned int i = 0; i < matrix.size(); i++) {
		for (unsigned int j = 0; j < matrix[0].size(); j++) {

			if (matrix[i][j] == max_score) {
				max_score_coord.push_back({ i,j });	
			}
		}
	}

	for (unsigned int k = 0; k < max_score_coord.size(); k++) {				//	iterating over 
		pair<int, string> local_align = traceback_op(max_score_coord[k]);	//	the local alignment is found and returned from traceback function
		bool inserted = false;												//	insertion sort flag
		if (k == 0) {														//	the first element is added by default
			found_allignments.push_back(local_align);						//	
		}
		else {																//	insertion sort in order to keep the local alignments in alphabethic order
			for (unsigned int t = 0; t < found_allignments.size(); t++) {	//	iterating over the found_alingments vector
				if (local_align.second == found_allignments[t].second) {	//	if the same str is already in the vector pass that
					inserted = true;										//	
					break;													//	break
				}
				if (local_align.second < found_allignments[t].second) {		//	if the current string is smaller than insert
					found_allignments.insert(found_allignments.begin() + t, local_align);//the Score and the corresponding local alignment is stored
					inserted = true;										//	
					break;													//	break
				}
			}
			if (!inserted) {												//if not inserted, insert at the end
				found_allignments.push_back(local_align);					//push to the end
			}
		}
	}

	// The alignment is written into the output file as the wanted format 

	if (found_allignments[0].first == 0) {	//if no alignment								
		of << input_string1 << " - " << input_string2 << endl;			
		of << "Score: 0" << " Sequence(s):" << endl;
	}
	else {
		for (unsigned int t = 0; t < found_allignments.size(); t++) {
			if (t == 0) {
				of << input_string1 << " - " << input_string2 << endl;
				of << "Score: " << (found_allignments[0].second).length() << " Sequence(s): \"" << found_allignments[t].second;
			}
			else {
				of << "\" \"" << found_allignments[t].second;
			}
		}
		of << "\"" << endl;
	}
}

class Local_Aligner {		//Local Aligner is a class that has the attributes and functions to solve the ordering of all input strings, creating combinations of them etc.

private:

	vector<string> ordered_list;							//ordered list of the input strings
	vector<pair<string, string>> string_pairs;				//ordered pairs of strings that are ordered within the pair also

public:

	Local_Aligner() {};	//destructor
	~Local_Aligner() {};//constructor
	void add_string(string);	//insertion sort to create the oredered_list
	void create_pairs();		//creates the two combination of the ordered list, applies insertion sort to the pairs
	void create_alignments();	//solves the allignment problem for all the string pairs, creates the output.txt 
};


void Local_Aligner::add_string(string str) {	//add string as insertion sort

	if (ordered_list.size() == 0) {				//empty list so first string is added directly
		ordered_list.push_back(str);			
	}
	else {
		bool inserted = false;					//insertion sort flag
		for (unsigned int i=0; i < ordered_list.size(); i++) {

			if (str <= ordered_list[i]) {		//if the string is lower than 
				ordered_list.insert(ordered_list.begin()+i, str);	//insert the string to the index
				inserted = true;
				break;
			}
		}
		if (!inserted) {
			ordered_list.push_back(str);		//if the string is biggest pusht to end
		}
	}
}

void Local_Aligner::create_pairs() {			//create the pairs of the strings in the ordered list

	for (unsigned int i = 0; i < ordered_list.size(); i++) {

		for (unsigned int j = i+1; j <ordered_list.size(); j++) {
			string_pairs.push_back({ ordered_list[i],ordered_list[j] });
		}
	}
}

void Local_Aligner::create_alignments() {		//creates the output.txt file

	//get the pairs, create matrixes
	ofstream fp("output.txt");	//the output file is created
	for (unsigned int i = 0; i < string_pairs.size(); i++) {	//for each pair in the string_pairs create the matrix and find local alignments

		Matrix str_str(string_pairs[i].first,string_pairs[i].second);	//matrix object is created

		str_str.construct_matrix();								//the matrix filling for the created Matrix obj
		str_str.print_locals(fp);								//write the allignments to the file pointer and output.txt 
	}
	fp.close();
}

// points are defined in the Matrix Class Constructor function
// match = +1
// mismatch = -50
// gap penatly = -100

int main()
{
	
	Local_Aligner myloc;					//local aligner object is defined

	ifstream file_pointer("strings.txt");	//input file is defined
	while (!file_pointer.eof()) {			//reading all the inputs
		string str;		
		file_pointer >> str;				//get the string from file
		myloc.add_string(str);				//add to the ordered list
	}

	myloc.create_pairs();					//create pairs from the strings
	myloc.create_alignments();				//

	return 0;
}

