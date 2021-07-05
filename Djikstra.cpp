/*
*******************************	Homework #2 Question 2 ***********************************
 Student Name: Bora Oner
* Student Id: 150170301
*
* Date: 17.04.2021
*/
#include <iostream> //cout, cin
#include <fstream>  //file ops
#include <vector>   //vector data structure
#include <utility>  //pair data structure
#include <string>   //string data type
#include <map>      //map structure
using namespace std;

class vertex {      //node class

private:
    string vertex_name;                         //vertex name
public:
    vector<pair<string, int> > edges;           //vertex neighbors and edge values pair

    vertex() {  };                              //vertex constructure
    vertex(string);                             //constructure with node name
    ~vertex();                                  //destructer of vertex
    void add_connection(pair<string, int>);      //adds the connections of the node to other nodes
    void print_edges();                         //
    string get_name() { return vertex_name; };  //getter function for vertex name
};

vertex::vertex(string name) {                   //inits the vertex name
    vertex_name = name;
}

void vertex::add_connection(pair<string, int> edge) {    //add connection to vertex
    edges.push_back(edge);
}

void vertex::print_edges() {                            //useless for debugging
    for (unsigned int i = 0; i < edges.size(); i++) {   //
        cout << vertex_name << " " << edges[i].first << " " << edges[i].second << " " << endl;  //
    }
}

vertex::~vertex() { //destructer for vertex
    edges.clear();
}

class Graph {       //Graph data type holds the graph, all its vertex, and information for finding the shortest path

private:
    vector<vertex> verteces;                    //node information on the graph
    vector<string > near_by_enemy;              //the ndoes that are dangerous to pass by
    vector<pair<string, int>> resting_spots;    //stores the resting points on the graph
public:
    pair<vector<string>, int> shortest_sol;     //the final solution aka shortest path

    Graph() {};                     //graph default constructer
    ~Graph();                       //destructors 
    void add_vertex(vertex);        //adds new vertex to the graph and fills the near_by_enemy and resting_spots verctors accordingly
    void print_graph();             //useless
    vector<vertex> get_nodes() { return verteces; };    //
    void path_finder();             //finds the shortest path on the graph between Ma <-> Mo
    pair<map<string, vector<string>>, map<string, int>> calculate_shortest(int);
};

void Graph::add_vertex(vertex node) {   //adds a vertex to the graph

    verteces.push_back(node);           //node is pushed
    if (!node.get_name().find('R')) {   //if the name includes resting point
        pair<string, int> rest_loc(node.get_name(), verteces.size() - 1);
        resting_spots.push_back(rest_loc);  //store the resting point
    }
    if (!node.get_name().find('E')) {   //if the node is an enemy spot store it as no go vector
        near_by_enemy.push_back(node.get_name());   //push the node to near_by_enemy
        for (unsigned int i = 0; i < node.edges.size(); i++) {   //all edges of the Enemy spot which has distance less than 5 pushed also
            if (node.edges[i].second < 5) {
                near_by_enemy.push_back(node.edges[i].first);
            }
        }
    }
    else {          //if the node is not an enemy spot store it as no go vector
        for (unsigned int i = 0; i < node.edges.size(); i++) {  //iterate over the edges
            if (!node.edges[i].first.find('E') && node.edges[i].second < 5) {   //if one of the edges is Enemy and the distance is < 5
                near_by_enemy.push_back(node.get_name());                       //push as near by enemy
            }
        }
    }
}

Graph::~Graph() {

    verteces.clear();
    near_by_enemy.clear();
    resting_spots.clear();

}

//for debugging
void Graph::print_graph() {
    for (unsigned int i = 0; i < verteces.size(); i++) {
        verteces[i].print_edges();
    }
}


//applies djikstra algorithm to the graph starting from the given node(int argument is the index in the verteces)
pair<map<string, vector<string>>, map<string, int>> Graph::calculate_shortest(int starting_node_index) { //returns the distances of all nodes and the visited nodes to that vertex

    map<string, int > distances;                //holds the shortest distances from the starting node to all nodes 
    map<string, vector<string>> check_points;   //the nodes that are crossed to reach the target node

    for (unsigned int i = starting_node_index; i < verteces.size(); i++) { //parameter starting node_index
        check_points[verteces[i].get_name()];
        if ((int)i == starting_node_index) {   //parameter starting node_index
            distances[verteces[i].get_name()] = 0;
        }
        else {
            distances[verteces[i].get_name()] = 100000; //big number 
        }
    }
    distances["Mo"] = 100000;                   //"Mo" is assigned
    check_points["Mo"];
    vector<string> visited;                     //

    for (unsigned int i = starting_node_index; i < verteces.size(); i++) { //parameter starting node_index

        bool pass = true;   //if the node is among the near_by_enemy nodes pass that node
        vector<string>::iterator it;
        for (it = near_by_enemy.begin(); it != near_by_enemy.end(); it++) {     //look if the vertex selexted is near the enemy or an enemy spot
            if (*it == verteces[i].get_name()) {
                pass = false;   //the flag is false so the current spot must not extended
                break;
            }
        }

        if (pass) {     //if the node is safe to pass or rest point

            for (unsigned int j = 0; j < verteces[i].edges.size(); j++) {   //iterate over the edges of the selected vertex

                if (verteces[i].edges[j].first.find('E')) { // if the edge is not a Enemy spot
                    int d0 = distances[verteces[i].get_name()]; //get the distance of the current node
                    int d1 = verteces[i].edges[j].second;       //get the edge distance
                    if (d0 + d1 < distances[verteces[i].edges[j].first]) {  //if the summation is less then the current 

                        vector<string> checks = check_points[verteces[i].get_name()];   //check points are updated
                        checks.push_back(verteces[i].get_name());                       //the current node is added to the checks

                        check_points[verteces[i].edges[j].first] = checks;              //update checks
                        distances[verteces[i].edges[j].first] = d0 + verteces[i].edges[j].second;   //update the distance value
                    }

                }
            }
        }
    }

    pair<map<string, vector<string>>, map<string, int>> path_dist(check_points, distances);
    return path_dist;       //return the distance
}

//call on Ma to all
//if solution is good meaning that it contains a resting spot then terminate:
//else:
//get all Ma-R distances
//call djikstra(R) in loop for each R
//find the minimum sum of (Ma-R + R-Mo)

void Graph::path_finder() { //finds the shortest possible path between "Ma" - "Mo"

    pair<map<string, vector<string>>, map<string, int>> Ma_to_all = calculate_shortest(0);  //the shorteste path 
    vector<string> optimal_sol = Ma_to_all.first["Mo"]; //optimal solution extracted from the map

    bool optimal = false;           //optimal flag
    for (unsigned int i = 0; i < optimal_sol.size(); i++) {

        if (!optimal_sol[i].find('R')) {    //check wheter the path that is found as optimal includes any restpoint
            optimal = true;
            break;
        }
    }
    shortest_sol.first = optimal_sol;   //assign the shortest path
    shortest_sol.second = Ma_to_all.second["Mo"];   //
    shortest_sol.first.push_back("Mo");

    if (!optimal) { //look all R's to Mo distances  (extra controls for making sure that the path contains a Resting spot)

        //get the Mo-R separetely

        vector<pair<vector<string>, int>> R_to_all;
        for (unsigned int j = 0; j < resting_spots.size(); j++) {

            pair<map<string, vector<string>>, map<string, int>> spec_djik = calculate_shortest(resting_spots[j].second);
            vector<string> optimal_sol_cont = spec_djik.first["Mo"];

            pair<vector<string>, int> r_solution(optimal_sol_cont, spec_djik.second["Mo"]);

            R_to_all.push_back(r_solution);
        }

        //sum the two vales
        int lowest_distance; //holds the lowest distance index
        int index = 0;
        for (unsigned int k = 0; k < resting_spots.size(); k++) {

            vector<string> before = Ma_to_all.first[resting_spots[k].first];
            for (unsigned int t = 0; t < R_to_all[k].first.size(); t++) {
                before.push_back(R_to_all[k].first[t]);
            }
            before.push_back("Mo");
            R_to_all[k].first = before;
            R_to_all[k].second = R_to_all[k].second + Ma_to_all.second[resting_spots[k].first]; //add distances

            if (k == 0) {
                lowest_distance = R_to_all[0].second;   //assign the first R value for a comparison mechanism
                index = 0;
            }
            else {
                if (R_to_all[k].second < lowest_distance) { //take the lowest distance Resting point from the possible Rs
                    index = k;
                }
            }
        }
        shortest_sol = R_to_all[index];  //shortest path is assigned
    }
}


int main()
{
    string file_name;
    cin >> file_name;
    ifstream file_pointer(file_name);
    
    string changed;     //changed is used to detect the change in the input.txt 
    string from;        //starting node in the edge
    string to;          //end node in the edge
    int edge_value;     //edge weight
    string newline;     //the newline character holder
    vector<pair<string, int> > stack;      //is used to store the edges of a single node, it is cleared for each node

    Graph war_graph;    //graph object

    while (!file_pointer.eof()) {    //reading the txt file

        getline(file_pointer, from, ',');   //get the starting vertex
        getline(file_pointer, to, ',');     //get the end vertex
        file_pointer >> edge_value;         //the edge value between two nodes
        getline(file_pointer, newline, '\n');


        pair<string, int> new_neigh(to, edge_value);    //a new neighbour is stored as pair
        stack.push_back(new_neigh);                     //pushed into to the stack

        if (from != "Ma" && changed != from) {          //the node is changed in the txt file input
            vertex n_vertex(changed);                   //a new node instance is created 

            if (stack.size() == 1) {                    //
                n_vertex.add_connection(stack[0]);      //create the connection
            }
            else {
                for (unsigned int i = 0; i < stack.size() - 1; i++) {   //
                    n_vertex.add_connection(stack[i]);  //create conncetions
                }
            }

            war_graph.add_vertex(n_vertex);             //
            stack.clear();                              //clear the stack for the next node
            stack.push_back(new_neigh);                 //
        }

        changed = from; //changed is assigned in each iteration after the edge additions to detect the change of the node in txt file

        if (file_pointer.eof()) {   //eof is reached
            vertex n_vertex(changed);   //the last node is added 

            if (stack.size() == 1) {
                n_vertex.add_connection(stack[0]);
            }
            else {
                for (unsigned int i = 0; i < stack.size() - 1; i++) {
                    n_vertex.add_connection(stack[i]);
                }
            }


            war_graph.add_vertex(n_vertex);

            stack.clear();
            stack.push_back(new_neigh);

        }
    }

    war_graph.path_finder();    //find the shortest path between Ma - Mo

    for (unsigned int j = 0; j < war_graph.shortest_sol.first.size(); j++) {    //printing the shortest path found 

        cout << war_graph.shortest_sol.first[j] << " ";
        if (j + 1 == war_graph.shortest_sol.first.size()) {  //print the distance at the end         
            cout << war_graph.shortest_sol.second << endl;
        }
    }

    return 0;
}