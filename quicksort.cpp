//***************README*****************
//	In this assignment the data read from the sales.txt file are stored in a datastructure called sale struct and an dynamic array is dynamically allocated space to hold these into an array
//	The sorting algorithm used is Quick Sort Algorithm however, not only the Country names are sorted alphabetically but also in each country the total profits of the orders are sorted as decreasing manner
//	The computations for all input sizes 10 - 1M are calculated in my personal system.
*/
#include<iostream> //swap, cout
#include<fstream> //file operations
#include<string> //string operations , stof, stoi fucntions
#include <vector> //vector data structure
#include <ctime> //clock_t, clock(), CLOCKS_PER_SEC
#include <stdlib.h>

using namespace std;

struct sale {	//data structure where a line of txt file is in one data

	string country; //holds the country name
	string item_type;	//holds the item_type
	string order_id;	//holds the order_id
	double units_sold;	//units_sold 
	double total_profit;//total_profit
}typedef sale;

int sort(sale** arr, int start, int end) { //sorts the subarrrays with the given start and end indexes, returns the pivot index after swap

	string pivot = arr[end]->country; 	//pivot is defined before hand
	int lf = -1;					  	//least final, holds the smallest last element index
	int hs = 0;							//higher start, holds the bigger element thatn pivot start

	for (int i = 0; i < end - start; i++) { //loops trhough the given indexes start and end 

		if (i == 0) {					//the start index index checked whether its is bigger or smaller than pivot, hs and lf is initilized
			if (arr[start + i]->country <= pivot) { //checking if the pivot string has before alphaabetically

				if (pivot == arr[start + i]->country) { //if the coutnry names are same with the first element

					double first_ele = arr[start]->total_profit;	//holds the total profit value of first element
					double pivot_prof = arr[end]->total_profit;		//hodls the total profit of te pivot element

					if (first_ele < pivot_prof) {					//fhecking if the pivot has bigger total_profit
						swap(arr[start], arr[end]);					//always keep pivot the smallest among the same country names because after loop pivot will be the last element...+
					}												//..+ in the sequence so that the order in the total profit be decreasing.
				}
				lf = start;		//since the first element is smaller than pivot lf points the 0th index
				hs = lf + 1;	//hs holds the next element of the lf
			}
			else {
				hs = start;		//since the first element is bigger than pivot hs is assigned to the first index
				lf = -1;		//lf is kept -1 
			}
		}
		else {
			if (pivot >= arr[start + i]->country) {	//check pivot and other elements in the indexes start+1 -> end-1

				if (lf == -1) { //first smaller element is encountrered case 

					lf = start + i;	// lf is assigned to the index and then swapped in the following statements
				}

				if (pivot == arr[start + i]->country) {						//if the country names are same with the element
					double pivot_profit = arr[end]->total_profit;			// pivot_total_profit
					double element_profit = arr[start + i]->total_profit;	//element_profit

					if (element_profit < pivot_profit) {	//if the pivot has bigger profit change the pivot value
						swap(arr[start + i], arr[end]);		//swap the pivot and keep pivot with the smallest profit amond the same countries 
					}
				}

				swap(arr[hs], arr[start + i]); //swap the hs with the current index point so the smaller elements are on the left side of the array
				lf = hs;	
				hs += 1;
			}
		}
	}
	if (lf == -1) {						// if the pivot is the smallest case 
		swap(arr[end], arr[start]);		//swap the pivot to the first element
		return start;					//return the index of the first index
	}

	swap(arr[end], arr[hs]);			//swap the hs index and the pivot so the partitioning is acghieved in the array
	return hs;							//return the index of the pivot
}


void quick_sort(sale** arr , int start, int end) { //recursive calls for the partitioning the array 

	if (start < end) {	// if the end and the start indexes are feasible

		int	p_index = sort(arr, start, end);	//gets the pivot index so partition around that index
		quick_sort(arr, start, p_index - 1);	//the left sub array around the partition index 
		quick_sort(arr, p_index + 1, end);	//the right sub array
	}

}

void write_file(sale** myvec,int size) { //writes the content of the sales into the txt file

	ofstream file; //output writing file pointer
	file.open("sorted.txt"); //name of the txt file

	file<<"Country"<<"\t"<<"Item Type"<<"\t"<<"Order ID"<<"\t"<<"Units Sold"<<"\t"<<"Total Profit"<<"\n";

	for (int i = 0; i < size; i++) {
		file << myvec[i]->country << "\t" << myvec[i]->item_type << "\t" << myvec[i]->order_id << "\t" << myvec[i]->units_sold << "\t" << myvec[i]->total_profit << "\t" << endl;
	}
	file.close();

}
void delete_arr(sale** arr,int size) {	//frees the alloated memory so that no memory leakage is observed
	
	for (int i = 0; i < size; i++) {	//delete the inner pointers
		delete arr[i];
		arr[i] = NULL;
	}
	delete[] arr;						//delete the dynamic array itself
	arr = NULL;
}


int main(int argc, char* argv[]) { 

	if (argc != 2) {
	//	exit(1);
		cout<<"Enter N value"<<endl;
		return 0;
	}


	ifstream file;
	file.open("sales.txt");

	if (!file) {
		cerr << "File cannot be opened!";
		return 0;
		//exit(1);
	}

	int N = atoi(argv[1]); //you should read value of N from command line
	string line;

	sale** dyn_arr = new sale*[N]; //array size = (N) size container

	getline(file, line); //this is the header line

	for (int i = 0; i < N; i++) {
		dyn_arr[i] = new sale;	//new sale data type pointer is memory allocated
	
		getline(file, line, '\t'); //country (string)
		dyn_arr[i]->country = line;	//sale[0] holds the country names

		getline(file, line, '\t'); //item type (string)
		dyn_arr[i]->item_type =line;	//sale[1] holds the item type
		
		getline(file, line, '\t'); //order id (string)
		dyn_arr[i]->order_id = line;	//sale[2] holds the order id

		file >> dyn_arr[i]->units_sold;	//sale[3] holds the units sold

		file>>dyn_arr[i]->total_profit;	//sale[4] holds the total_profit

		getline(file, line, '\n'); //this is for reading the \n character into dummy variable.
	}

	file.close();
	
	clock_t start_time = clock();  //start time assigned

	quick_sort(dyn_arr, 0, N -1);  //quick sort for names and totoal profits

	clock_t end_time = clock();		//end time assigned
	//start_time = clock() - start_time;
	clock_t elapsed_time = end_time - start_time; //time difference(in terms of clocks)
	cout << "Total time elapsed is = " << ((float)elapsed_time)/ CLOCKS_PER_SEC << endl; //elapsed time is divided by CLOCKS_PER_SEC becuase clock() returns time in terms of clocks

	write_file(dyn_arr,N); 	//writing to the file 

	delete_arr(dyn_arr,N);	//freeing the memory
	return 0;
}
