/***********************************************************
2021 Spring - BLG 336E-Analysis of Algorithms II
Final Project
Question on Greedy Algorithms
Modified Dijkstra Algorithms for Maximum Capacity Path
Submitted: 15.06.2021 
**********************************************************/

/***********************************************************
STUDENT INFORMATION
Full Name : Bora Oner
Student ID: 150170301
**********************************************************/

// Some of the libraries you may need have already been included.
// If you need additional libraries, feel free to add them
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>

// Do not change this definition
#define INT_MAX 1000

using namespace std;


void Modified_Dijkstra(Graph* graph) {

	vector <int> status(graph->node_count);	//status vector holding the status of each node
	vector <int> wt(graph->node_count);		// wt vector that holds the minimum edge weight in the path between source and wt[i]'th node
	vector <int> dad(graph->node_count);	//dad vector that holds the parent information of the nodes

	//initializing the status, wt and dad vectors

	for (int i = 0; i < graph->node_count; i++) {
		status[i] = 0;
		wt[i] = 0;
		dad[i] = 0;
	}

	//prepering the source node

	status[0] = 2;		//initially visited and finished
	wt[0] = INT_MAX;

	// visiting the neighbouring nodes of the source node
	
	int ones_count = 0;	// used to keep track if there are any nodes that are in the "fringe" status

	for (int i = 0; i < graph->node_count; i++) {

		if (graph->adjacency_matrix[0][i] != 0) {

			status[i] = 1;	// all neighbours are in fringe status
			wt[i] = graph->adjacency_matrix[0][i]; //the edge between them is the minimum path 
			dad[i] = 0;		//their parent node is the source node
			
			ones_count++;	//ones_count incremented because a nodes status assgined as "fringe" 

		}
	}
	
	while (ones_count) {	// loop ends when ones_count -> 0

		//finding the max wt_value

		int max_wt = 1;		// discarding the source node
		int max_val = 0;	// holds the maximum value in the wt[i]  
		for (int i = 1; i < graph->node_count; i++) {

			if (status[i] != 2 && wt[i] != 0 && wt[i] > max_val) { // if the wt[i] is bigger than temporal max
				max_wt = i;				//assign the index value as the maximum wt
				max_val = wt[i];		// change the max 
			}
		}

		status[max_wt] = 2;	//assigning the node as finished


		for (int i = 0; i < graph->node_count; i++) {	//visiting each neighbour of the node and upğdating their wt, status and dad values

			if (graph->adjacency_matrix[max_wt][i] != 0) {

				if (status[i] == 0) { // not yet visited node 

					status[i] = 1;
					wt[i] = min(wt[max_wt], graph->adjacency_matrix[max_wt][i]);
					dad[i] = max_wt;

					ones_count++;

				}
				else if (status[i] == 1) {	//if the node is fringe status then its wt value could be updated

					int cond_min = min(wt[max_wt], graph->adjacency_matrix[max_wt][i]);	// minimum of the parent and the edge between its children
					
					if (wt[i] < cond_min) {	//if the child nodes' maximum_min is less then the 
						wt[i] = cond_min;	//assigning the maximum_minimum edge val
						dad[i] = max_wt;	//parent node is the max_wt 

						ones_count++;
					}
				}
			}
		}
		ones_count--;		//decrementing ones count since the node mx_wt is status =2 after operations

	}
  return;
}

int main(int argc, char **argv){
	Graph* graph = new Graph();
	graph->read_file(argv[1]);
	Modified_Dijkstra(graph);
	
	return 0;	
}
