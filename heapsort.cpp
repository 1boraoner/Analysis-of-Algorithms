#include <iostream>     //i/o operations
#include <fstream>      //file operations
#include <algorithm>    //swap function
#include <vector>       //vector data structure is used
#include <cstdlib>      //rand(), RAND_MAX
#include <cmath>        //pow(), sqrt() 
#include <string>       //getline(), atof(), atoi()
#include <ctime>        //clock_t, time(), srand(), CLOCKS_PER_CYCLE

using namespace std;

struct Taxi {   //Taxi struct holds the data of Taxis with its's number and the distance to the referance point
	int taxi_number;    //Taxi identifier number -> the number indicates the position in the txt file
	double distance;    //distance
}typedef Taxi;

// a newly added Taxi object starts from the bottom of the heap and goes upwards as far as it can; ==> it has to be smaller than its parent in the heap 
void minify(vector<Taxi>&pqvec,int index) { // x ==> heap index number,Therefore adding-> index-1 in array index, 

	int parent_index = index / 2;   //the parent index of any node in the heap

	if (parent_index - 1  >= 0) {   // parent's index value in the vector must >= 0 

		if (pqvec[index - 1].distance < pqvec[parent_index - 1].distance) { //if the current ndoe is smaller than its parent, swap them

			swap(pqvec[index - 1], pqvec[parent_index - 1]);        //swap the parent and child so that the lower node is above
			minify(pqvec, parent_index);                          //call recursivly for the parent node since the actual value is now in the parent's index
		}
	}
}

//preserves the min_heap property --> the parent looks for its children if there any of its children smaller than him, swaps itself with the lowest child
void fix_minheap(vector<Taxi>&pqvec,int parent) { 

	int next_node = parent;             //next_node initially itself for default case
	int left_child = parent * 2;        //the left child, heap index
	int right_child = parent * 2 + 1;   //the right child, heap index

	if (right_child <= pqvec.size() || left_child <= pqvec.size()) {    //TO be sure that the parent has any children left or right with in the bounds of the heap array

		if (right_child <= pqvec.size() && (pqvec[left_child - 1].distance > pqvec[right_child - 1].distance)) { //if there exists a right child; look for the comparison between childs
			next_node = right_child; // Right children is smaller than Left children so next node is Right Child
		}
		else {
			next_node = left_child; //Left Child is smaller than the right child so next node is left
		}

		if (pqvec[parent - 1].distance > pqvec[next_node - 1].distance) { //if parent is bigger than the smallest of its children
			swap(pqvec[parent - 1], pqvec[next_node - 1]);                //swap the parent and the child so the lower priority is at the above
			fix_minheap(pqvec, next_node);                              //call recurisvly the index at the next node becuase --> the node where the previous parent is now
		}
	}
}


Taxi extract_min(vector<Taxi>& pqvec) {   //returns the smallest priority item from the PQ

	Taxi min_dist_taxi = pqvec[0];        //the first index holds the minimum of the heap
	pqvec[0] = pqvec[pqvec.size() - 1];       //before the smallest value is extracted the last element in the heap is put on the first index to prevent the shift of all elements in the vector
	pqvec.erase(pqvec.begin() + pqvec.size() - 1); //the last elements is erased from the vector because last element is now in the first index and it will travel the tree starting from 0th index
	fix_minheap(pqvec, 1);                //the function preserves the min heap property of the three by moving the smaller elemnts to above while large values of roots are going to deep
	return min_dist_taxi;               //returns the minimum priority element
}

void key_update(vector<Taxi>& pqvec, int taxi_pos) {  //function updates the priority of a given taxi 
	pqvec[taxi_pos].distance -= 0.01;         //decrease by 0.01
	if (pqvec[taxi_pos].distance <= 0) {      //since the distance can't be negative value
		pqvec[taxi_pos].distance = 0;         //assign it 0
	}       
	minify(pqvec, taxi_pos+1); //after the decrement operation the element can become smaller than its' parent, so minify is invoked on the index
}


int main(int argc, char* argv[]) 
{
	
	if (argc != 3) {
		cout<<"Not Enough Arguments"<<endl;
		return(0);
	}

	int m = atoi(argv[1]);    // the number of operations
	double p = atof(argv[2]); // probabilty value

	ifstream fp;               //file pointer
	fp.open("locations.txt");  //file that holds the dataset is opened
	
	vector<Taxi> priority_queue;    //vector data structure where it stores "Taxi" data type
	double lon = 0;                 //the longitude value initialized   
	double lat = 0;                 //the latitude value is initialized

	srand(time(NULL));              //seed is changing so every run of the program will be different

	string first_line;              //the first headlins are removed from hte data txt
	getline(fp, first_line);        
	
	int counter = 1;                //coutner for the Taxi Number
	
	int nupdate = 0;                //number of update operations
	int naddition = 0;              //number of addition operations 
	int nextract = 0;               //number of extract operations
	
	clock_t start_time = clock();  //start time assigned
	for (int i = 1; i <= m; i++) {  //m operations so 1-m loop

		if(i % 100 == 0){   //in each 100 operations a Taxi is called (Extacted from the heap)
			
			if(priority_queue.size()>0){ // If no element in the PQ --> there is no taxi to call
				nextract++;
				Taxi nearest_taxi = extract_min(priority_queue);
				cout <<"The Nearest Taxi = Taxi #" << nearest_taxi.taxi_number << " is called and with the smallest distance of " << nearest_taxi.distance  <<endl;
			}else{
				cout<<"No Available Taxi In Order To Call"<<endl;
			}
		}
		else { //if not extract; addition of new Taxi to priority queue  OR update the key value of a randomly selected Taxi

			double rand_op = (double) rand() / RAND_MAX;    // a random value is generated between 0 - 1 
			if (rand_op > p || i==1) {  //for first iteration, the heap is empty so  go into addition opeartion in the first iter

				naddition++;

				fp >> lon; //longitute is read 
				fp >> lat;  //latitue is read 
				double dist = sqrt(pow((33.40819 - lon), 2) + pow((39.19001 - lat), 2));    //the distance of the referance point and the Taxi point is calculated

				Taxi ntaxi = { counter,dist };  //data is stroed as Taxi struct varible 
				priority_queue.push_back(ntaxi);    //the data is added to the end of the vector
				minify(priority_queue, priority_queue.size());  //the last added element in the vector is goes up to the heap 

				counter++;	//taxi number counter
			}
			else {
				if(priority_queue.size()>0){
					nupdate++;
					int rand_index = rand() % priority_queue.size(); //random index between (0-vec.size()-1)
					key_update(priority_queue, rand_index);         // update the key of the randomly selected tax index
				}else{            //incase of no element to decrease
					cout << "No Taxi In Order to Decrease It's Dıstance" << endl;
				}
			}
		}
	}

	clock_t end_time = clock();
	clock_t elapsed_time = end_time - start_time; //time difference(in terms of clocks)
	cout << "Total time elapsed is = " << (float)elapsed_time * 1000 / CLOCKS_PER_SEC <<" milliseconds " << endl; //elapsed time is divided by CLOCKS_PER_SEC becuase clock() returns time in terms of clocks
	cout << "Total update operation = " << nupdate << "  Total addition operation  = " << naddition<<endl;
	cout << "Total extract operation = " << nextract << endl;

	return 0;
}
