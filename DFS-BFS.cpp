/*
*									Analysis of Algorithms II Homework#1
* Student Name: Bora Oner
* Student ID: 150170301
* Date: 6.04.2021
*
* to_compile: g++ sourceCode.cpp -o hw1
* to_run:  ./hw1 DFS TWO TWO FOUR outputFileName		-->  output file name must not have the .txt part
* example: ./hw1 DFS TWO TWO FOUR TWO_TWO_FOUR_solution -->  will create TWO_TWO_FOUR_solution.txt file
*/

#include <iostream>	//cout
#include <vector>	//vector data structure and vector member functions
#include <fstream>	//file read write
#include <string>	//string type and string operations
#include <ctime>	//clock(),
#include <math.h>	//pow() function is used
#include <stdlib.h>	//exit()


using namespace std;

class Node {	//Node class holds the information of the matrix it inherits and some other attributes aboutthe node

public:

	vector<int> matrix;							//each node's matrix is represented as 1D vector at each index corresponds to a distinct character
	vector<Node*>* next_layer;					//a vector that holds all the childs of the node
	int member_layer;							//node's current layer number

	Node(int, int);								//Node class constructer
	~Node();									//Node class destructor
	void print_martix(vector<char>);			//prints the solution to terminal
};


Node::Node(int char_vec_length, int layer) {	// initialize the node

	vector<int> matrix_s(char_vec_length);		//vector is initialized
	matrix = matrix_s;							//vector is assigned
	member_layer = layer;
	next_layer = NULL;							//next_layer is NULL
}

Node::~Node() {
	matrix.clear();
	delete next_layer;
}

void Node::print_martix(vector<char> arr) {	//prints the solution in terminal

	cout << "Solution: ";
	for (int i = 0; i < matrix.size(); i++) {

		cout << (char)(arr[i] - 32) << ":";
		if (i != matrix.size() - 1) {
			cout << matrix[i] << ", ";
		}
		else {
			cout << matrix[i] << endl;
		}
	}
}


class Queue {	//implementing FIFO, top(), push(),clear(),
private:
	vector<Node*>* queueu_head;
	int front_index;
	int last_index;
	int maximum_size;
public:
	Queue() { queueu_head = NULL; front_index = 0; last_index = 0; maximum_size = 0; };
	Queue(int);
	~Queue();
	void push(vector<Node*>);
	vector<Node*> front();
	void pop();
	bool empty();
};

Queue::Queue(int size) {		//queue constructor

	queueu_head = new vector<Node*>[size];
	last_index = 0;
	front_index = 0;
	maximum_size = 0;
}

Queue::~Queue() {	//queue destructor

	delete[] queueu_head;
	queueu_head = NULL;
}

void Queue::push(vector<Node*> layer) {	//pushes a element to the back of the queue
	queueu_head[last_index] = layer;
	last_index++;
	maximum_size = last_index - front_index;

}

vector<Node*> Queue::front() {			//returns the first element of the queue
	return queueu_head[front_index];
}

void Queue::pop() {						//pop the first element in the queueu
	front_index++;
}


bool Queue::empty() {					//returns true if queue is empty

	if (front_index == last_index) {
		return true;
	}
	else {
		return false;
	}
}

class Tree {							//Tree class that represents the search tree constructed acording to the input 
private:

	Node* root;							//root of the Tree
	Node* solution_node;				//solution node --> used in printing at the end of the program
	string operand1;					//first operand
	string operand2;					//second operand
	string result;						//result
	string file_name;
	vector<char> distinc_set;			//distinct set of characters
	int length_const;					//length of the distinc set
	bool solution;						// True if solution is found --- False if solution is not found
	Queue* BFS_queue;					// BFS is implemented using a Queue data structure so Tree has its own Queue
	vector<int> op1_places;				//first operand's characters' place in the matrices
	vector<int> op2_places;				//second operand's characters' places in the matrices
	vector<int> res_places;				//result's characacters places in the matrices

public:

	int node_count;						//tree's node count
	int visited_count;					//visited node count during the BFS or DFS search algorithms
	Tree(string, string, string, vector<char>, string);	//Tree constructer
	void dfs(Node*);					//depth first search function
	void bfs();							//breadth first search function
	bool check_constraints(Node*);		//constraint checker-> takes the node itself and tests it according to the constraints 
	void delete_tree(Node* temp);		//deletes the whole tree constructed in the create_tree function
	~Tree();							//Tree destructor
	void create_tree(Node*, int);		//creates the tree in a recusive manner
	void write_function(Node*);			//write the solution matrix to a file
	Node* get_root() { return root; }	//getter function for the TREE
	Node* get_solution() { return solution_node; }	//getter for the solution node --> used in printing the mapping of the characters at the end of the program
};

Tree::Tree(string op1, string op2, string out, vector<char> arr, string filen) {	//Tree constructor takes operands and distinc character set as argument


	node_count = 0;		//node created is 0 initially
	visited_count = 0;	//no visited nodes yet
	solution = false;	//solution is not found
	operand1 = op1;		//first operand is assigned
	operand2 = op2;		//second operand is assigned
	result = out;		//result is assigned
	file_name = filen;
	distinc_set = arr;	//distinct set is assigned
	length_const = distinc_set.size();	//the length of the is stored 
	root = NULL;		//root is set to NULL
	solution_node = NULL;
	for (int i = 0; i < 3; i++) {		//in this loop the places of the characters of the each strings are stored into op1_places, op2_places and res_places in order
		string str;
		if (i == 0) {
			str = operand1;
		}
		else if (i == 1) {
			str = operand2;
		}
		else {
			str = result;
		}

		for (int j = 0; j < str.size(); j++) {

			for (int k = 0; k < distinc_set.size(); k++) {

				if (distinc_set[k] == str[j] + 32) {
					if (i == 0) {
						op1_places.push_back(k);	//the positions of the first operands in the matrix is pushed into the op1_places
						break;
					}
					else if (i == 1) {
						op2_places.push_back(k);	//the positions of the second operands in the matrix is pushed into the op2_places
						break;
					}
					else {
						res_places.push_back(k);	//the positions of the result operands in the matrix is pushed into the res_places
						break;
					}
				}
			}
		}
	}
}


Tree::~Tree() {
	delete_tree(root);
	root->next_layer->clear();
	root->matrix.clear();
	delete root;
	root = NULL;
	delete BFS_queue;
}

void Tree::delete_tree(Node* temp) {


	for (int i = 0; i < temp->next_layer[0].size(); i++) {

		Node* to_delete = temp->next_layer[0][i];
		if (to_delete->next_layer == NULL) {
			delete to_delete;
		}
		else {
			delete_tree(to_delete);
			to_delete->next_layer->clear();
			delete to_delete;
		}
	}


}

void Tree::create_tree(Node* the_node, int layer_number) {	//creates the search tree recursively, takes the node and the layer_number as asrguments

	if (root == NULL) {										//if the Tree is empty
		root = new Node(length_const, 0);					// new Node class instance is created
		for (int i = 0; i < length_const; i++) {			//
			root->matrix[i] = 0;							//the root matrix is just 0s 
		}
		node_count++;										//node count++
		create_tree(root, layer_number);					//recursive call for the childs of the root aka 1st layer of the TREE
	}
	else {													//if root is not NULL
		if (layer_number < length_const) {					//the layer number must equal to the number of distinct characters in the question so, the stopping condition is the layer_number
			the_node->next_layer = new vector<Node*>;		//node's next layer vector is initailized 

			for (int m = 0; m < 10; m++) {					//for 0-9 each child's matrix's layer_number index's corresponding character is assigned 

				bool already_assigned = false;				//flag for checking the 'to be assigned number' is already assigned
				for (int i = 0; i < layer_number; i++) {	//each letter must have a distinct number
					if (the_node->matrix[i] == m) {			//if the number is already assigned to a different character the node must not be constructed so continue the loop
						already_assigned = true;
					}
				}
				if (already_assigned) {						//skip the current formation because the number is already assigned
					continue;
				}

				Node* nn = new Node(length_const, layer_number + 1);	//a new Node is initialized
				the_node->next_layer->push_back(nn);				//new constructed node is assigned as the child of the the_node 
				node_count++;										//node count is increased

				nn->matrix = the_node->matrix;						//new node's matrix is the copy of the father node
				nn->matrix[layer_number] = m;						//but the current layers' corresponding character is assigned
				create_tree(nn, layer_number + 1);					//recursively call
			}
		}
	}
}


bool Tree::check_constraints(Node* trial_node) {					//function takes the node and that checks for the constraints 

	vector<int> check_matrix = trial_node->matrix;					//the matrix that will be analysed during the function
	bool flag = true;												//the flag variable is used until the last layer of the tree
	if (trial_node->member_layer >= op1_places[0] + 1) {				//check for the first operand's first character

		if (check_matrix[op1_places[0]] == 0) {						//first layer contains the first operand's most significant digit so can not be 0.
			flag = false;
		}
	}
	if (trial_node->member_layer >= op2_places[0] + 1) {				//check for the second operands' first character

		if (check_matrix[op2_places[0]] == 0) {						//if the second operand's first character is zero return false
			flag = false;
		}
	}

	if (trial_node->member_layer >= res_places[0] + 1) {			//cheking the result's first character

		if (check_matrix[res_places[0]] == 0) {						//
			flag = false;
		}
	}

	if (trial_node->member_layer != distinc_set.size()) {			//if the comparison Node is not a solution--> meaning it is not in the last layer,
		return flag;												//look for the FLAG varible this will allow the searching algorithm not expanding towards un related SOLUTIONS
	}
	else {															//if the node is in the SOLUTION layer

		if ((check_matrix[res_places[0]] == 0) || check_matrix[op2_places[0]] == 0 || check_matrix[op1_places[0]] == 0) {	//for safety check the first letters again
			return false;
		}

		//Implementation of the sumation rules
		vector<int> carry(res_places.size());						//# number of carry => length of the result CARRY[0] = 0 for implementing for loop
		for (int i = 0; i < result.size(); i++) {					//loop goes for the size of the result

			if (i + 1 == result.size()) {								//looking for the the most significant digit summation it is really important to isolate this step since;

				if (result.size() > operand1.size()) {				//the result size is bigger than the both operands than looking for the carry is sufficient because there most be a carry
					if (carry[i] != check_matrix[res_places[0]]) {	//check for if the carry
						return false;								//return false
					}
				}
				else {												//if the operands and result are the same digit size

					if ((carry[i] + check_matrix[op1_places[0]] + check_matrix[op2_places[0]]) != (check_matrix[res_places[0]])) {	//sum the digits
						return false;								//if not return false
					}
				}
			}
			else {													//all digits other than the most significant digit is a procedure where taking a carry value and the summation of the digits

				int op1_digit = check_matrix[op1_places[op1_places.size() - i - 1]];	//holds the digit of the operand 1
				int op2_digit;															//holds the digit of the operand 2
				if (op2_places.size() - 1 < i) {
					op2_digit = 0;														//if there is a size differnce between two operand like 4 digit + 3 digit numbers the corresponding digit of the second operand is 0
				}
				else {
					op2_digit = check_matrix[op2_places[op2_places.size() - i - 1]];	//otherwise the digit is the corresponding matrix index
				}
				int res_digit = check_matrix[res_places[res_places.size() - i - 1]];	//result digit is taken also

				if ((carry[i] + op1_digit + op2_digit) % 10 == res_digit) {				//detecting whether there must be a carry in the summation

					if ((carry[i] + op1_digit + op2_digit) >= 10) {						//if there is a carry
						carry[i + 1] = 1;												//push the carry into the carry vector
					}
					else {																//otherwise no carry
						carry[i + 1] = 0;
					}
				}
				else {																	//if the summation doesnt hold --> return false
					return false;
				}
			}
		}
		write_function(trial_node);
		solution_node = trial_node;
		return true;

	}
}

void Tree::dfs(Node* trave_node) {										//depth first search implementation with recursion using the system Stack
	
	if (trave_node) {													//depth condition
		visited_count++;

		for (int i = 0; i < trave_node->next_layer[0].size(); i++) {	//traverse all the way down as its children bracnhes go

			Node* child = trave_node->next_layer[0][i];					//the child of the current parent node

			if (!solution && child->next_layer == NULL) {				//if the solution is not yet found and the child is in the last layer of the tree
				solution = check_constraints(child);					//check for a solution, if found the solution parameter will become True and all the recursive...
			}															//...calls will be locked that no new expansions can be made
			else {														//if internal layer nodes
				if (!solution && check_constraints(child)) {			//if the solution is not found check the constraints in order no to expand from its children
					dfs(child);											//recursive call 
				}
				else {
					continue;											//if the consraints of the node matrix is false, then continue meaning do not expand
				}
			}
		}
	}
}

void Tree::bfs() {
	BFS_queue = new Queue(node_count);
	Node* travers = root;											//start with root
	BFS_queue->push(*(travers->next_layer));							//roots's next layer vector is pushed to the queue
	visited_count++;
	while (!solution && !BFS_queue->empty()) {						//during each iteration the layers stored in the queue will be unpacked as in level by level iteration

		vector<Node*> layer_vec = BFS_queue->front();				//first layer that is first on the queue is taken
		BFS_queue->pop();											//the layer that will be used to reach the childs is popped off from the queue 

		for (int i = 0; i < layer_vec.size(); i++) {				//for each child in the layer

			travers = layer_vec[i];									//the child in the layer 
			visited_count++;
			if (!solution && travers->member_layer == distinc_set.size()) {	//if the child is the leaf node check for solution
				solution = check_constraints(travers);
			}

			if (travers->next_layer != NULL && check_constraints(travers)) {	//if violates the constraints do not expands on that node and  if the child is not in the last layer, push it's next_layer information to the queue
				BFS_queue->push(*(travers->next_layer));							//the 'next_layer' variable that holds the childs of the travers node is pushed to the Queue
			}
		}
	}
}



void Tree::write_function(Node* solution_node) {

	ofstream file; //output writing file pointer
	//string output_filename(operand1 + " " + operand2 + " " + result + " solution.txt");
	string output_filename(file_name + ".txt");
	const char* str1 = output_filename.c_str();
	file.open(str1); //name of the txt file

	for (int i = 0; i <= distinc_set.size(); i++) {

		if (i == 0) {
			file << '\t';
			for (int i = 0; i < 10; i++) {

				if (i == 9) {
					file << i;
					file << "\n";

				}
				else {
					file << i;
					file << "\t";

				}
			}
		}
		else {

			file << distinc_set[i - 1];
			file << '\t';

			for (int j = 0; j < 10; j++) {

				if (solution_node->matrix[i - 1] == j) {
					file << "1";
				}
				else {
					file << ".";
				}

				if (j == 9) {
					if (i == distinc_set.size()) {
						//donothing
					}
					else {
						file << "\n";
					}
				}
				else {
					file << "\t";
				}
			}
		}
	}
	file.close();
}

int main(int argc, char* argv[])
{

	string input1(argv[2]);			//string variables created
	string input2(argv[3]);			//
	string output(argv[4]);			//
	string filen(argv[5]);
	vector<char> distinct_letters;	//distinc letter set
	distinct_letters.push_back((char)input1[0] + 32);	//initially push the first character
	for (int i = 0; i < 3; i++) {						//in this loop all distinct letters will be found and pushed accordingly
		string str;
		if (i == 0) {
			str = input1;
		}
		else if (i == 1) {
			str = input2;
		}
		else {
			str = output;
		}
		for (int j = 0; j < str.size(); j++) {
			bool found = false;
			for (int k = 0; k < distinct_letters.size(); k++) {

				if ((char)distinct_letters[k] != (char)str[j] + 32) {
					continue;
				}
				else {
					found = true;
				}
			}
			if (!found) {
				distinct_letters.push_back((char)str[j] + 32);
			}
		}
	}

	clock_t start_time = clock();						//start time of the program

	Tree my_tree = Tree(input1, input2, output, distinct_letters, filen);	//Tree class instance is created
	my_tree.create_tree(my_tree.get_root(), 0);			//the constructor of the Tree class is called
	string search_type(argv[1]);						//the search type taken from the input
	if (search_type == "DFS") {							//if search_Type  is Depth first search
		my_tree.dfs(my_tree.get_root());				//call the dfs of the Tree
	}
	else if (search_type == "BFS") {						//if the search type is Breadth First Search 
		my_tree.bfs();									//call the bfs method of the tree class
	}
	else {												//invalid search algorith so terminate the program
		cout << "wrong input" << endl;
		exit(0);
	}

	clock_t end_time = clock();
	double running_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;		//finishing time
	cout << "Algorithm: " << search_type << endl;
	cout << "Number of the visited nodes: " << my_tree.visited_count << endl;
	cout << "Maximum number of nodes kept in the memory: " << my_tree.node_count << endl;
	cout << "Running time: " << running_time << "seconds" << endl;
	if (my_tree.get_solution()) {
		my_tree.get_solution()->print_martix(distinct_letters);
	}
	else {
		cout << "No solution can be found" << endl;
	}
	return 0;
}
