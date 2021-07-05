/* * * * * * * * * * * * * * * * * * * * * * * * * *
 * BLG 336E Analysis of Algorithms II
 * Spring 2021
 * Student Name: Bora Oner
 * Student ID: 150170301
 * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <iostream>
#include <vector>
#include <map>

using namespace std;


bool isBipartite(vector<vector<int> >& graph) {
    
    map<int, int> cmap;     //colour map that the keys are the node indexes and the values are the colours 1:red -1:blue
    cmap[0] = 1;            //the 0th node is coloured as red
    for (unsigned int i = 0; i < graph[0].size(); i++) { //traversing all the neighbouring nodes to colour them
        cmap[graph[0][i]] = -1;     //all the neighbouring nodes are coloured blue
    }

    map<int, int>::iterator it;                         //iterator over the cmap
    for (it = cmap.begin(); it != cmap.end(); it++) {   //for each coloured node in the map

        if (it == cmap.begin()) {   //adjacent nodes of 0th node is already coloured so skipping 0th node
            continue;
        }

        for (unsigned int j = 0; j < graph[it->first].size(); j++) { //for each neighbours of node in the cmap

            int node_c = cmap[graph[it->first][j]];         //identify the colour of the neighbouring node (if not found it returns 0)

            if (node_c == it->second) {                     //if the colours are the same with the neighbour return false
                return false;
            }

            if (node_c == 0) {                              //if the node is uncoloured
                if (it->second == 1) {                      //if the indexed node is red coloured then 
                    cmap[graph[it->first][j]] = -1;         //this node is blue
                }
                else {
                    cmap[graph[it->first][j]] = 1;          //else means that the indexed node is blue so mark this node as red
                }
            }
        }
    }
    return true;        //colourding is done and no two same colours are neighbouring so this is a bipartite graph
}


