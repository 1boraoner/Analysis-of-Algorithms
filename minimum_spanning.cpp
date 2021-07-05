/*								
*******************************	Homework #2 Question 1 ***********************************
* Student Name: Bora Oner
* Student Id: 150170301
* 
* Date: 17.04.2021
*/

#include <iostream>	//cin,cout
#include <vector>	//vector
#include <fstream>	//file operations
#include <utility>  //pair 
#include <map>		//map
#include <string>	//string funcitons

using namespace std;

class Graph {	//Graph class representing the graph in the txt file

public:

	map<string, vector<pair<string, int>>> nodes;			//the nodes and the connections in the graph
	vector<pair<string, pair<string, int>>> edges_str;		//the original file's inputs (for checking the output format)

	Graph() { };	//default constructor
	Graph(map<string, vector<pair<string, int>>>, vector<pair<string, pair<string, int>>>);	//constructor for graph class
	~Graph();		//destructor for graph class
};


Graph::Graph(map<string, vector<pair<string, int>>> graph_nodes, vector<pair<string, pair<string, int>>> edge_reps) {

	nodes = graph_nodes;
	edges_str = edge_reps;
}

Graph::~Graph() {
	nodes.clear();
	edges_str.clear();
}


class MST {	//minimum spanning tree class

private:

	Graph parent_graph;									//the graph that the MST is the subgraph of
	vector<pair<string,pair<string,int>>> mst_output;	//the mst_output that will be used in the output
	int total_cost;										//total_cost of hte tree
	vector<pair<string,pair<string, int>>> poss_edges;	//possible edges that can be added, and they are in sorted fashion (it is used like a priority queue)
public:
	MST(Graph graph) { parent_graph = graph; total_cost = 0; };	//constructor
	void minimum_spanning_tree();								//minimum spanning tree construction - Prims algorithm is used
	bool check_for_constraints(vector<string>);					//checks if the minimum edge added a cycle occurs.
	void edge_queue_filler(vector<string>, int);				//poss_edges vector is filled with ascending order(insertion sort is applied)
	void final_mst();											//outputting the final generated minimum spanning tree 
};

void MST::minimum_spanning_tree() {								//construcs a minimum spanning tree from the parent_graph

	int n_edges = parent_graph.nodes.size();					//number of edges defines the numbr of iterations to make the mst

	vector<string> verteces_picked;								//the verteces that are included in the tree

	int hipp_exists = 1;		//if hipp is not a node in the graph
	if (parent_graph.nodes.count("Hipp") == 1) {
		hipp_exists = 2;		//if hipp is on the graph
	}
	
	for (int i = 0; i < n_edges- hipp_exists; i++) {			//in each iteration a new node is added to mst, (in i==0, if hipp exists 2 nodes are added else only church is added)

		if (i == 0) {											//church is added firstly and then the hipp is added

			verteces_picked.push_back("GP");					//starting from gp

			vector<pair<string, int>> churches;					//pq for churches
			int index_hipp = -1;									//the index of the hipp in the gp edges vector
			for (int j = 0; j < (int)parent_graph.nodes["GP"].size(); j++) {	//iterating over the gp-x edges

				if (!parent_graph.nodes["GP"][j].first.find('C')) {				//if a church is found in the neighbours
					
					if (churches.size() == 0) {									//add the first found churcch to churhces vector
						churches.push_back(parent_graph.nodes["GP"][j]);
					}
					else {

						if (parent_graph.nodes["GP"][j].second < churches[0].second) {	
							churches.insert(churches.begin(), parent_graph.nodes["GP"][j]);	//insert the new found church according to the edge weight order
						}
					}
				}

				if (!parent_graph.nodes["GP"][j].first.find("Hipp")) {	//if hipp is found
					index_hipp = j;	//store the hipp index
				}
			}
			//add the minimum cost hurch to tree

			verteces_picked.push_back(churches[0].first);	//church name is aded to the verteces set
			total_cost += churches[0].second;	//church cost
			mst_output.push_back({verteces_picked[0], churches[0]});

			//add hippodrome if exists
			if (index_hipp != -1) {
				verteces_picked.push_back("Hipp");
				total_cost += parent_graph.nodes["GP"][index_hipp].second;
				mst_output.push_back({ verteces_picked[0], parent_graph.nodes["GP"][index_hipp] });

			}


			poss_edges.push_back({ "GP",parent_graph.nodes[verteces_picked[0]][0] });	//to initialize the poss_edges, the first edge og "GP" is pushed
			edge_queue_filler(verteces_picked,0);			//fill all edges of verteces_picked to the poss_edges vector

		}
		else {


			//find the minimum edge
			//check if cycle
			//not cycle then add vertices

			while (check_for_constraints(verteces_picked)) {	//guarantees non cyclic path
				poss_edges.erase(poss_edges.begin());	//pop if the minimum weight edge creates a cycle
			}

			verteces_picked.push_back(poss_edges[0].second.first);	//push the vertex
			total_cost += poss_edges[0].second.second;				//add the total_cost
			mst_output.push_back({poss_edges[0]});					//push the minimum edge

			poss_edges.erase(poss_edges.begin());					//pop the added edge


			edge_queue_filler(verteces_picked, verteces_picked.size()-1);	//add the edges of the newly added vertex

		}
	}
}

bool MST::check_for_constraints(vector<string> vertex_picked) {		//return true if cycle is created with the minimum cost edge and other constraints

	string pivot_ele = poss_edges[0].second.first;				//minimum cost edge
	bool visited = false;										//visited flag is false if no cycle is created
	for (int i = 0; i < (int)vertex_picked.size(); i++) {		//iterate over the verteces added

		if (pivot_ele == vertex_picked[i]) {					//if the edge contains the verteces in the vertex list then a cycle is created so 
			visited = true;										//make the flag true			
			break;												
		}

	}
	//hipp - bas connections check
	if (poss_edges[0].first == "Hipp" && !poss_edges[0].second.first.find("Bas")) {	//also check for Hipp and bas connection
		visited = true;
	}
	//hp - hp connections check
	if (!poss_edges[0].first.find("Hp") && !poss_edges[0].second.first.find("Hp")) {//check for hp - hp being no directly connected
		visited = true;
	}

	return visited;
}



void MST::edge_queue_filler(vector<string> visited_nodes,int index) {	//fill poss_edges vector 

	for (unsigned int i = index; i < visited_nodes.size(); i++) {		//parameter index is the start position to adding

		for (unsigned int j = 0; j < parent_graph.nodes[visited_nodes[i]].size(); j++) {	//iterate over the all edges of the vertex	
			if (index == 0 && i == 0 && j==0) {
				continue;
			}
			for (unsigned int k = 0; k < poss_edges.size(); k++) {	//iterate over the poss_edges vector to sort it (insertion sort is applied)

				if (parent_graph.nodes[visited_nodes[i]][j].second < poss_edges[k].second.second) {	//if the new edge has less weight then insert

					poss_edges.insert(poss_edges.begin() + k, { visited_nodes[i],parent_graph.nodes[visited_nodes[i]][j] });
					break;
				}
				if (k + 1 == poss_edges.size()) {	//if the new edge has to biggest weigth add the end
					poss_edges.push_back({ visited_nodes[i] ,parent_graph.nodes[visited_nodes[i]][j] });
					break;
				}
			}
		}
	}
}

void MST::final_mst() {	//insertion sort for the desired output structure

	vector<pair<string, pair<string, int>>> fin_const;	//final constructed mst
	fin_const.push_back(mst_output[0]);	//initialize

	for (unsigned int i = 1; i < mst_output.size(); i++) {	//iterate over the mst_output elements 

		for (unsigned int k = 0; k < fin_const.size(); k++) { //insertion sort is applied 

			if (mst_output[i].second.second < fin_const[k].second.second) {

				fin_const.insert(fin_const.begin() + k, mst_output[i]);
				break;
			}
			if (k + 1 == fin_const.size()) {
				fin_const.push_back(mst_output[i]);
				break;
			}
		}
	}

	for (unsigned int i = 0; i < fin_const.size(); i++) {	//outputting the resulting mst
		bool inverse = false;
		for (unsigned int k = 0; k < parent_graph.edges_str.size(); k++) {	//check if the edge is like the input format

			if (fin_const[i].first == parent_graph.edges_str[k].second.first && fin_const[i].second.first == parent_graph.edges_str[k].first) { //if the edge is inverse repsented 
				cout << fin_const[i].second.first << " " << fin_const[i].first << " " << fin_const[i].second.second << endl;
				inverse = true;
			}
		}
		if (!inverse) {
			cout << fin_const[i].first << " " << fin_const[i].second.first << " " << fin_const[i].second.second << endl;
		}

	}
	std::cout << total_cost << endl;	//total cost of the mst

}

int main() {

	string file_name;
	cin >> file_name;		//input file name
	ifstream fp(file_name);	//file pointer

	string node1;	//holds the node1
	string node2;	//holds the node 2
	int weight;		//holds the weight of the edge
	string newline;	//holds the newline char
	map<string, vector<pair<string, int>>> graph_nodes;
	vector<pair<string, pair<string, int>>> edges_def;

	while (!fp.eof()) {
		
		getline(fp, node1, ',');	//get node1
		getline(fp, node2, ',');	//get node2
		fp >> weight;				//get weight of the edge
		getline(fp, newline, '\n');	//newline character

		pair<string, int> connection(node2,weight);
		pair<string, int> conn_mirror(node1, weight);

		graph_nodes[node1].push_back(connection);		//the edge definition from the perspective of first node1
		graph_nodes[node2].push_back(conn_mirror);		//the nodes places are changed to generate the two direction edge
		
		edges_def.push_back({node1,{node2,weight}});	//holds the format of the input file (used in checking the output format)
	}

	Graph city_plan(graph_nodes,edges_def);		//Graph object
	MST minim_span(city_plan);					//minimum spanning tree construction
	minim_span.minimum_spanning_tree();			//generate the minimum spanning tree
	minim_span.final_mst();						//printing the minimum spanning tree generated
	return 0;
}