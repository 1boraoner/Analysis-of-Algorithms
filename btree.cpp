/*
  Btree insertion and deletinon operations
*/

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

class node {

private:
    int x;      //x input
    int y;      //y input
    char z;     //z input

public:
    void* right;    //right pointer of the node
    node(int, int, char);   //node constructor
    int get_data(char);     //get the data according to the choice of sort given by the user
    void print();           //print the node according to the requirements
};

node::node(int x, int y, char z) {  //constructor of the node
    this->x = x;
    this->y = y;
    this->z = z;
    right = NULL;
}

int node::get_data(char sort_choice) {  //returs the data 

    if (sort_choice == 'x') {
        return this->x;
    }
    else if (sort_choice == 'y') {
        return this->y;

    }
    else {
        int alter_char = (int)this->z;
        return alter_char;
    }
}

void node::print() {        //prints the node
    cout << "(" << x << "," << y << "," << z << ")";
}

class btree_node {      //a btree node class
public:
    vector<node*> bnode;        //vector of Nodes type of elements
    bool exceed;                //True if full else False
    int min_cap;                //minimum capacity of a node according to the formula t-1               
    int max_cap;                //maximum capacity of a node accroding to the formula 2t-1
    char key_type;              //key type determines the sort choice so if x is selected by the user x data will be returned by node
    btree_node* left;           //left pointer points to the left most  child of a btree node 
    btree_node* up;             //up pointer holds the parent of the node

    btree_node(int, int, char); //constructor
    void print_row();           //print the btree node
    int get_median(int);
    int get_max_cap() { return max_cap; }      //getter for max_cap attribute
};

btree_node::btree_node(int a, int b, char key) {    //constructor
    exceed = false;
    min_cap = a;
    max_cap = b;
    key_type = key;
    left = NULL;
    up = NULL;
}

void btree_node::print_row() {  //printing the btree node

    vector<node*>::iterator it;
    for (it = bnode.begin(); it < bnode.end(); it++) {
        (*(it))->print();
    }
    cout << endl;
}

int btree_node::get_median(int n) {

    if (n % 2 == 0) {
        return (n / 2) - 1;
    }
    else {
        return (n) / 2;
    }
}

class btree {

private:
    int node_count;
    int dt;
    char sort_choice;
    btree_node* root;
public:

    btree(int, int, char);
    void insert_nn(btree_node*, node*, bool);
    void delete_n(btree_node*, int);
    void clear_tree();
    void print_tree(btree_node*);
    void delete_tree(btree_node*);
    btree_node* get_root() { return root; }
};

btree::btree(int count, int degg, char choice) {  //constructor of b-tree

    root = NULL;
    node_count = count;
    dt = degg;
    sort_choice = choice;
}

void btree::insert_nn(btree_node* node_row, node* newnode, bool bottom_reached) {        //insert new node to B-tree


    if (root == NULL) {

        root = new btree_node(dt - 1, 2 * dt - 1, sort_choice);
        root->bnode.push_back(newnode);

    }
    else {

        if (bottom_reached == false) { //search for the right place

            if (node_row != NULL) {
                bool end_key = true;
                for (int i = 0; i < (int)node_row->bnode.size(); i++) {

                    if (node_row->bnode[i]->get_data(sort_choice) > newnode->get_data(sort_choice)) {
                        if (i == 0) {
                            end_key = false;
                            insert_nn(node_row->left, newnode, bottom_reached);
                            break;
                        }
                        else {
                            end_key = false;
                            insert_nn((btree_node*)node_row->bnode[i - 1]->right, newnode, bottom_reached);
                            break;
                        }
                    }
                }

                if (end_key == true) {
                    insert_nn((btree_node*)node_row->bnode[node_row->bnode.size() - 1]->right, newnode, bottom_reached);
                }
            }


            if (node_row != NULL && node_row->left == NULL) {        //leaf node is reached

                bottom_reached = true;            //bootom is reached

                if ((int)node_row->bnode.size() != 2 * dt - 1) {    //node has not maximum key number
                    bool inserted = false;
                    vector<node*>::iterator it;        //iterator over the node
                    for (it = node_row->bnode.begin(); it < node_row->bnode.end(); it++) {

                        if (newnode->get_data(sort_choice) < (*(it))->get_data(sort_choice)) {        //insert into the suitable place
                            inserted = true;
                            node_row->bnode.insert(it, newnode);        //insert the node
                            break;                                        //break out the loop
                        }
                    }
                    if (!inserted) {                //if the key is is the biggest among the node
                        node_row->bnode.push_back(newnode);            //push the key to the last key
                    }
                }
                else {

                    if (node_row == root) {        //root node is full so split

                        btree_node* left_child = root;                                                    //left child 
                        btree_node* right_child = new btree_node(dt - 1, 2 * dt - 1, sort_choice);        //right child

                        root = new btree_node(dt - 1, 2 * dt - 1, sort_choice);        //new root
                        root->up = NULL;    //CAUTION

                        int median_index = left_child->get_median(left_child->bnode.size());        //get the median element index
                        root->bnode.push_back(left_child->bnode[median_index]);                        //assign first element to root

                        root->bnode[0]->right = right_child;    //make the connection
                        root->left = left_child;                //make the left child connection

                        right_child->up = root;        //up pointers
                        left_child->up = root;        //up pointers


                        for (int i = median_index + 1; i < (int)left_child->bnode.size(); i++) {        //fill the right child with the according keys
                            right_child->bnode.push_back(left_child->bnode[i]);
                        }

                        int interval = (int)left_child->bnode.size() - median_index;                    //partition around the median
                        for (int i = 0; i < interval; i++) {
                            left_child->bnode.pop_back();                                                //pop the right cild keys
                        }


                        if (newnode->get_data(sort_choice) > root->bnode[0]->get_data(sort_choice)) {    //newnode must be on the right child
                            bool inserted = false;
                            vector<node*>::iterator it;        //iterator over the node
                            for (it = right_child->bnode.begin(); it < right_child->bnode.end(); it++) {

                                if (newnode->get_data(sort_choice) < (*(it))->get_data(sort_choice)) {        //insert into the suitable place
                                    inserted = true;
                                    right_child->bnode.insert(it, newnode);        //insert the node
                                    break;                                        //break out the loop
                                }
                            }
                            if (!inserted) {                //if the key is is the biggest among the node
                                right_child->bnode.push_back(newnode);            //push the key to the last key
                            }
                        }
                        else {        //newkey must be on the left child
                            bool inserted = false;
                            vector<node*>::iterator it;                                                        //iterator over the node
                            for (it = left_child->bnode.begin(); it < left_child->bnode.end(); it++) {

                                if (newnode->get_data(sort_choice) < (*(it))->get_data(sort_choice)) {        //insert into the suitable place
                                    inserted = true;
                                    left_child->bnode.insert(it, newnode);        //insert the node
                                    break;                                        //break out the loop
                                }
                            }
                            if (!inserted) {                //if the key is is the biggest among the node
                                left_child->bnode.push_back(newnode);            //push the key to the last key
                            }
                        }

                    }
                    else {            //leaf node is full so split

                        btree_node* newrow = new btree_node(dt - 1, 2 * dt - 1, sort_choice);
                        newrow->up = node_row->up;

                        int median_index = node_row->get_median(node_row->bnode.size());
                        node* the_node_upward = node_row->bnode[median_index];
                        newrow->left = (btree_node*)the_node_upward->right;

                        if (the_node_upward->right != NULL) {
                            newrow->left->up = newrow;
                        }
                        the_node_upward->right = newrow;

                        for (int i = median_index + 1; i < (int)node_row->bnode.size(); i++) {
                            newrow->bnode.push_back(node_row->bnode[i]);
                        }

                        int interval = (int)node_row->bnode.size() - median_index;
                        for (int i = 0; i < interval; i++) {
                            node_row->bnode.pop_back();
                        }


                        if (newnode->get_data(sort_choice) > the_node_upward->get_data(sort_choice)) {    //newnode must be on the right child
                            bool inserted = false;
                            vector<node*>::iterator it;        //iterator over the node
                            for (it = newrow->bnode.begin(); it < newrow->bnode.end(); it++) {

                                if (newnode->get_data(sort_choice) < (*(it))->get_data(sort_choice)) {        //insert into the suitable place
                                    inserted = true;
                                    newrow->bnode.insert(it, newnode);        //insert the node
                                    break;                                        //break out the loop
                                }
                            }
                            if (!inserted) {                //if the key is is the biggest among the node
                                newrow->bnode.push_back(newnode);            //push the key to the last key
                            }
                        }
                        else {        //newkey must be on the left child
                            bool inserted = false;
                            vector<node*>::iterator it;                                                        //iterator over the node
                            for (it = node_row->bnode.begin(); it < node_row->bnode.end(); it++) {

                                if (newnode->get_data(sort_choice) < (*(it))->get_data(sort_choice)) {        //insert into the suitable place
                                    inserted = true;
                                    node_row->bnode.insert(it, newnode);        //insert the node
                                    break;                                        //break out the loop
                                }

                            }
                            if (!inserted) {                //if the key is is the biggest among the node
                                node_row->bnode.push_back(newnode);            //push the key to the last key
                            }
                        }

                        insert_nn(node_row->up, the_node_upward, bottom_reached);
                    }
                }
            }
        }

        else {        //internal node and root node's key deletion takes place

            if ((int)node_row->bnode.size() != 2 * dt - 1) {    //node has not maximum key number
                bool inserted = false;
                vector<node*>::iterator it;        //iterator over the node
                for (it = node_row->bnode.begin(); it < node_row->bnode.end(); it++) {

                    if (newnode->get_data(sort_choice) < (*(it))->get_data(sort_choice)) {        //insert into the suitable place
                        inserted = true;
                        node_row->bnode.insert(it, newnode);        //insert the node
                        break;                                        //break out the loop
                    }

                }
                if (!inserted) {                //if the key is is the biggest among the node
                    node_row->bnode.push_back(newnode);            //push the key to the last key
                }
            }
            else {    //if the node is full 

                if (node_row == root) {    //if the root is reached

                    btree_node* left_child = root;        //left node
                    btree_node* right_child = new btree_node(dt - 1, 2 * dt - 1, sort_choice);        //right node

                    root = new btree_node(dt - 1, 2 * dt - 1, sort_choice);        //new root
                    root->up = NULL;        //for caution

                    int median_index = left_child->get_median(left_child->bnode.size());        //median of the root
                    root->bnode.push_back(left_child->bnode[median_index]);    //assign first element to root

                    right_child->left = (btree_node*)root->bnode[0]->right;    //IMPORTANT

                    root->bnode[0]->right = right_child;        //connections
                    root->left = left_child;                    //connections

                    right_child->up = root;                        //upward
                    left_child->up = root;                        //upward

                    for (int i = median_index + 1; i < (int)left_child->bnode.size(); i++) {
                        right_child->bnode.push_back(left_child->bnode[i]);
                    }

                    int interval = left_child->bnode.size() - median_index;
                    for (int i = 0; i < interval; i++) {
                        left_child->bnode.pop_back();
                    }

                    if (newnode->get_data(sort_choice) > root->bnode[0]->get_data(sort_choice)) {    //newnode must be on the right child
                        bool inserted = false;
                        vector<node*>::iterator it;        //iterator over the node
                        for (it = right_child->bnode.begin(); it < right_child->bnode.end(); it++) {

                            if (newnode->get_data(sort_choice) < (*(it))->get_data(sort_choice)) {        //insert into the suitable place
                                inserted = true;
                                right_child->bnode.insert(it, newnode);        //insert the node
                                break;                                        //break out the loop
                            }
                        }
                        if (!inserted) {                //if the key is is the biggest among the node
                            right_child->bnode.push_back(newnode);            //push the key to the last key
                        }
                    }
                    else {        //newkey must be on the left child
                        bool inserted = false;
                        vector<node*>::iterator it;                                                        //iterator over the node
                        for (it = left_child->bnode.begin(); it < left_child->bnode.end(); it++) {

                            if (newnode->get_data(sort_choice) < (*(it))->get_data(sort_choice)) {        //insert into the suitable place
                                inserted = true;
                                left_child->bnode.insert(it, newnode);        //insert the node
                                break;                                        //break out the loop
                            }
                        }
                        if (!inserted) {                //if the key is is the biggest among the node
                            left_child->bnode.push_back(newnode);            //push the key to the last key
                        }
                    }

                    for (int i = 0; i < (int)left_child->bnode.size(); i++) {

                        btree_node* child_node = (btree_node*)left_child->bnode[i]->right;
                        child_node->up = left_child;
                    }

                    right_child->left->up = right_child;
                    for (int i = 0; i < (int)right_child->bnode.size(); i++) {
                        btree_node* child_node = (btree_node*)right_child->bnode[i]->right;
                        child_node->up = right_child;
                    }

                }
                else {        //an internal node is reached

                    btree_node* row2 = new btree_node(dt - 1, 2 * dt - 1, sort_choice);        //new node

                    //fill the rows
                    int median_index = node_row->get_median(node_row->bnode.size());        //get the median
                    node* the_node_upward = node_row->bnode[median_index];                    //median key

                    row2->left = (btree_node*)the_node_upward->right;                        //connection
                    row2->left->up = row2;                                                    //connect

                    for (int i = median_index + 1; i < (int)node_row->bnode.size(); i++) {
                        row2->bnode.push_back(node_row->bnode[i]);
                    }

                    int interval = node_row->bnode.size() - median_index;

                    for (int i = 0; i < interval; i++) {
                        node_row->bnode.pop_back();
                    }

                    if (newnode->get_data(sort_choice) > the_node_upward->get_data(sort_choice)) {    //newnode must be on the right child

                        bool inserted = false;
                        vector<node*>::iterator it;        //iterator over the node
                        for (it = row2->bnode.begin(); it < row2->bnode.end(); it++) {

                            if (newnode->get_data(sort_choice) < (*(it))->get_data(sort_choice)) {        //insert into the suitable place
                                inserted = true;
                                row2->bnode.insert(it, newnode);        //insert the node
                                break;                                        //break out the loop
                            }
                        }
                        if (!inserted) {                //if the key is is the biggest among the node
                            row2->bnode.push_back(newnode);            //push the key to the last key
                        }
                    }
                    else {        //newkey must be on the left child
                        bool inserted = false;
                        vector<node*>::iterator it;                                                        //iterator over the node
                        for (it = node_row->bnode.begin(); it < node_row->bnode.end(); it++) {

                            if (newnode->get_data(sort_choice) < (*(it))->get_data(sort_choice)) {        //insert into the suitable place
                                inserted = true;
                                node_row->bnode.insert(it, newnode);        //insert the node
                                break;                                        //break out the loop
                            }
                        }
                        if (!inserted) {                //if the key is is the biggest among the node
                            node_row->bnode.push_back(newnode);            //push the key to the last key
                        }
                    }

                    //up connection for both rows and nodes

                    for (int i = 0; i < (int)node_row->bnode.size(); i++) {
                        btree_node* child_node = (btree_node*)node_row->bnode[i]->right;
                        child_node->up = node_row;
                    }

                    row2->left->up = row2;
                    for (int i = 0; i < (int)row2->bnode.size(); i++) {
                        btree_node* child_node = (btree_node*)row2->bnode[i]->right;
                        child_node->up = row2;
                    }
                    if (the_node_upward->right != NULL) {

                        the_node_upward->right = row2;
                    }

                    insert_nn(node_row->up, the_node_upward, bottom_reached);
                }
            }
        }
    }
}


void btree::print_tree(btree_node* traverse) {

    if (traverse != NULL) {

        for (int i = 0; i < (int)traverse->bnode.size(); i++) {
            if (i == 0) {
                traverse->print_row();
                print_tree(traverse->left);
                print_tree((btree_node*)traverse->bnode[i]->right);

            }
            else {
                print_tree((btree_node*)traverse->bnode[i]->right);
            }

        }
    }
}

void btree::delete_n(btree_node* node_row, int key) {    //delete a particular node of B-tree

    bool found = false;
    if (node_row != NULL) {

        int index = 0;        //place of the key in the node
        bool last_pointer = true;    //for last pointer
        for (int i = 0; i < (int)node_row->bnode.size(); i++) {

            if (node_row->bnode[i]->get_data(sort_choice) > key) {
                if (i == 0) {
                    last_pointer = false;
                    delete_n(node_row->left, key);
                    break;
                }
                else {
                    last_pointer = false;
                    delete_n((btree_node*)node_row->bnode[i - 1]->right, key);
                    break;
                }
            }
            else if (node_row->bnode[i]->get_data(sort_choice) == key) {
                last_pointer = false;
                found = true;
            }
            index++;
        }
        if (last_pointer == true) {
            delete_n((btree_node*)node_row->bnode[node_row->bnode.size() - 1]->right, key);
        }


        if (found) {

            if (node_row->left == NULL) {    //leaf node deletion or root with node childs

                if (node_row == root || (int)node_row->bnode.size() - 1 >= node_row->min_cap) {        //check if the minimum deletion violiation

                    int del_index = index - 1;
                    node_row->bnode.erase(node_row->bnode.begin() + del_index);
                }
                else {

                    //if min size is not satisfied look first left then right
                    //find the father key index
                    //bool edge_leaf = false;
                    bool left_child = false;
                    int cur_index = 0;    //node's parent key index
                    for (int i = 0; i < (int)node_row->up->bnode.size(); i++) {

                        if (i == 0) {
                            if (node_row->up->left == node_row) {
                                left_child = true;
                                cur_index = 0;
                                break;
                            }
                        }

                        if (node_row->up->bnode[i]->right == node_row) {
                            break;
                        }

                        cur_index++;    //curr_index of the parent->bnode[cur_index-1], 
                    }

                    if (left_child) {        //left most parent key

                        //check right sibling
                        //if not sufficient number of keys merge in the left node

                        btree_node* right_sibling = (btree_node*)node_row->up->bnode[cur_index]->right;
                        if ((int)right_sibling->bnode.size() - 1 >= right_sibling->min_cap) {        //a node can be changed from the left sibling to the parent

                            int del_index = index - 1;            //delete the key
                            node_row->bnode.erase(node_row->bnode.begin() + del_index);        //delete the key

                            node* left_most = right_sibling->bnode[0];        //the right most key in the node
                            right_sibling->bnode.erase(right_sibling->bnode.begin());                                                //pop the key from the left sibling it must be deleted

                            left_most->right = right_sibling;                                                //right pointer points to the current node_row 

                            node_row->up->bnode.insert(node_row->up->bnode.begin(), left_most);    //insert the head key into the parent node

                            node* downgraded_node = node_row->up->bnode[cur_index + 1];                            //hold the parent node and delete it
                            downgraded_node->right = NULL;                                                //make the right pointer NULL since it will be a leaf node key

                            node_row->up->bnode.erase(node_row->up->bnode.begin() + cur_index + 1);                //delete the key from the parent node

                            node_row->bnode.insert(node_row->bnode.end(), downgraded_node);        //add the key into the leaf node

                        }
                        else {        //right sibling is not sufficient to borrow a key

                            int to_delete = index - 1;
                            node_row->bnode.erase(node_row->bnode.begin() + to_delete);        //delete the key

                            btree_node* parent = node_row->up;

                            node_row->bnode.push_back(parent->bnode[0]);    //store the parent's first key into the left child
                            parent->bnode[0]->right = NULL;

                            parent->bnode.erase(parent->bnode.begin());        //delete the parent's first key

                            if (parent != root && node_row != root && (int)parent->bnode.size() == 0) {
                                node_row->up = parent->up;
                                int grandfath = 0;
                                bool left = false;
                                for (int i = 0; i < (int)parent->up->bnode.size(); i++) {

                                    if (i == 0) {
                                        if (parent->up->left == parent) {
                                            left = true;
                                            break;
                                        }
                                    }
                                    if (parent->up->bnode[i]->right == parent) {
                                        break;
                                    }
                                    grandfath++;    //curr_index of the parent->bnode[cur_index-1],
                                }

                                if (left) {
                                    parent->up->left = node_row;
                                }
                                else {

                                    parent->up->bnode[grandfath]->right = node_row;
                                }

                            }

                            for (int i = 0; i < (int)right_sibling->bnode.size(); i++) {
                                node_row->bnode.push_back(right_sibling->bnode[i]);
                            }

                            if (node_row->up == root && (int)root->bnode.size() == 0) {        //if the root has 1 key and the 2 child has the minimum key the root is the left child
                                root = node_row;
                                node_row->up = NULL;
                            }
                            delete right_sibling;        //delete the right child
                        }
                    }
                    else if ((int)node_row->up->bnode.size() == 1 || cur_index == (int)node_row->up->bnode.size() - 1) {        //rightmost parent key

                        //left siblinge bak
                        //merge if not sufficient

                        btree_node* left_sibling;        //left sibling
                        if (cur_index == 0) {
                            left_sibling = (btree_node*)node_row->up->left;
                        }
                        else {
                            left_sibling = (btree_node*)node_row->up->bnode[cur_index - 1]->right;
                        }

                        if ((int)left_sibling->bnode.size() - 1 >= left_sibling->min_cap) {        //a node can be changed from the left sibling to the parent

                            int del_index = index - 1;            //delete the key
                            node_row->bnode.erase(node_row->bnode.begin() + del_index);        //delete the key

                            node* downgraded_node = node_row->up->bnode[cur_index];                            //hold the parent node and delete it
                            downgraded_node->right = NULL;                                                //make the right pointer NULL since it will be a leaf node key
                            node_row->up->bnode.erase(node_row->up->bnode.begin() + cur_index);
                            node_row->bnode.insert(node_row->bnode.begin(), downgraded_node);        //add the key into the leaf node

                            node* right_most = left_sibling->bnode[left_sibling->bnode.size() - 1];        //the right most key in the node
                            right_most->right = node_row;
                            left_sibling->bnode.pop_back();
                            node_row->up->bnode.insert(node_row->up->bnode.end(), right_most);    //insert the head key into the parent node

                        }
                        else {        //merge

                            int to_delete = index - 1;
                            node_row->bnode.erase(node_row->bnode.begin() + to_delete);

                            btree_node* parent = node_row->up;
                            left_sibling->bnode.push_back(parent->bnode[cur_index]);
                            left_sibling->bnode[left_sibling->bnode.size() - 1]->right = NULL;
                            parent->bnode.pop_back();

                            if (parent != root && left_sibling != root && (int)parent->bnode.size() == 0) {
                                left_sibling->up = parent->up;
                                int grandfath = 0;
                                bool left = false;
                                for (int i = 0; i < (int)parent->up->bnode.size(); i++) {

                                    if (i == 0) {
                                        if (parent->up->left == parent) {
                                            left = true;
                                            break;
                                        }
                                    }

                                    if (parent->up->bnode[i]->right == parent) {
                                        break;
                                    }
                                    grandfath++;    //curr_index of the parent->bnode[cur_index-1],
                                }

                                if (left) {
                                    parent->up->left = left_sibling;
                                }
                                else {
                                    parent->up->bnode[grandfath]->right = left_sibling;
                                }

                            }

                            for (int i = 0; i < (int)node_row->bnode.size(); i++) {
                                left_sibling->bnode.push_back(node_row->bnode[i]);
                            }

                            if (node_row->up == root && (int)root->bnode.size() == 0) {
                                root = left_sibling;
                                left_sibling->up = NULL;
                            }
                            delete node_row;
                        }

                    }
                    else {        //internal parents keys

                        btree_node* left_sibling;        //left sibling
                        if (cur_index - 1 == 0) {
                            left_sibling = (btree_node*)node_row->up->left;
                        }
                        else {
                            left_sibling = (btree_node*)node_row->up->bnode[cur_index - 1]->right;
                        }
                        btree_node* right_sibling = (btree_node*)node_row->up->bnode[cur_index + 1]->right;    //right sibling

                        if ((int)left_sibling->bnode.size() - 1 >= left_sibling->min_cap) {        //a node can be changed from the left sibling to the parent

                            int del_index = index - 1;            //delete the key
                            node_row->bnode.erase(node_row->bnode.begin() + del_index);        //delete the key

                            node* right_most = left_sibling->bnode[left_sibling->bnode.size() - 1];        //the right most key in the node
                            left_sibling->bnode.pop_back();                                                //pop the key from the left sibling it must be deleted
                            right_most->right = node_row;                                                //right pointer points to the current node_row 

                            node* downgraded_node = node_row->up->bnode[cur_index];                            //hold the parent node and delete it
                            downgraded_node->right = NULL;                                                //make the right pointer NULL since it will be a leaf node key
                            node_row->bnode.insert(node_row->bnode.begin(), downgraded_node);        //add the key into the leaf node
                            node_row->up->bnode.erase(node_row->up->bnode.begin() + cur_index);                //delete the key from the parent node

                            node_row->up->bnode.insert(node_row->up->bnode.begin() + cur_index, right_most);    //insert the head key into the parent node

                        }
                        else if ((int)right_sibling->bnode.size() - 1 >= right_sibling->min_cap) {    //look right sibling

                            int del_index = index - 1;            //delete the key
                            node_row->bnode.erase(node_row->bnode.begin() + del_index);        //delete the key

                            node* left_most = right_sibling->bnode[0];        //the right most key in the node
                            right_sibling->bnode.erase(right_sibling->bnode.begin());                                                //pop the key from the left sibling it must be deleted
                            left_most->right = right_sibling;                                                //right pointer points to the current node_row 

                            node* downgraded_node = node_row->up->bnode[cur_index + 1];                            //hold the parent node and delete it
                            downgraded_node->right = NULL;
                            node_row->up->bnode.erase(node_row->up->bnode.begin() + cur_index + 1);                //delete the key from the parent node

                            node_row->bnode.insert(node_row->bnode.end(), downgraded_node);        //add the key into the leaf node

                            node_row->up->bnode.insert(node_row->up->bnode.begin() + cur_index + 1, left_most);    //insert the head key into the parent node

                        }
                        else {

                            btree_node* left_sibling;        //left sibling
                            if (cur_index - 1 == 0) {
                                left_sibling = (btree_node*)node_row->up->left;
                            }
                            else {
                                left_sibling = (btree_node*)node_row->up->bnode[cur_index - 1]->right;
                            }

                            int to_delete = index - 1;
                            node_row->bnode.erase(node_row->bnode.begin() + to_delete);

                            btree_node* parent = node_row->up;

                            parent->bnode[cur_index]->right = NULL;
                            left_sibling->bnode.push_back(parent->bnode[cur_index]);

                            parent->bnode.erase(parent->bnode.begin() + cur_index);

                            for (int i = 0; i < (int)node_row->bnode.size(); i++) {
                                left_sibling->bnode.push_back(node_row->bnode[i]);
                            }
                            delete node_row;
                        }

                    }
                }

            }
            else {    //root or internal node key deletion

                if (index - 1 == 0) {

                    btree_node* left_children = node_row->left;
                    btree_node* right_children = (btree_node*)node_row->bnode[index - 1]->right;

                    int left_size = left_children->bnode.size();
                    int right_size = right_children->bnode.size();

                    //check left children first
                    if (left_size - 1 >= left_children->min_cap) {

                        btree_node* temp = (btree_node*)left_children->bnode[left_children->bnode.size() - 1]->right;
                        node* inserted_key = left_children->bnode[left_children->bnode.size() - 1];
                        if (temp == NULL) {
                            left_children->bnode.pop_back();

                        }

                        inserted_key->right = right_children;
                        node_row->bnode.erase(node_row->bnode.begin());
                        node_row->bnode.insert(node_row->bnode.begin(), inserted_key);

                    }
                    else if (right_size - 1 >= right_children->min_cap) {        //check right child

                        btree_node* temp = (btree_node*)right_children->left;
                        node* inserted_key = right_children->bnode[0];
                        if (temp == NULL) {
                            right_children->bnode.erase(right_children->bnode.begin());        //delete the key;

                        }

                        inserted_key->right = right_children;
                        node_row->bnode.erase(node_row->bnode.begin());
                        node_row->bnode.insert(node_row->bnode.begin(), inserted_key);
                    }
                    else {

                        //delete the key from the node
                        //merge right of the key and left of the key

                        for (int i = 0; i < (int)right_children->bnode.size(); i++) {
                            left_children->bnode.push_back(right_children->bnode[i]);
                        }
                        int del_index = index - 1;
                        node_row->bnode.erase(node_row->bnode.begin() + del_index);

                        if (node_row == root && (int)root->bnode.size() == 0) { //the root must be the left child
                            root = left_children;
                            root->up = NULL;
                        }

                        delete right_children;
                    }
                }
                else {

                    btree_node* left_children = (btree_node*)node_row->bnode[index - 2]->right;
                    if (node_row->bnode[index - 1]->right == NULL) {
                        node_row->bnode.erase(node_row->bnode.begin() + index - 1);
                        return;
                    }
                    btree_node* right_children = (btree_node*)node_row->bnode[index - 1]->right;    //??

                    int left_size = left_children->bnode.size();
                    int right_size = right_children->bnode.size();

                    if (left_size - 1 >= left_children->min_cap) {

                        btree_node* temp = (btree_node*)left_children->bnode[left_children->bnode.size() - 1]->right;
                        node* inserted_key = left_children->bnode[left_children->bnode.size() - 1];
                        if (temp == NULL) {
                            left_children->bnode.pop_back();
                        }

                        inserted_key->right = right_children;
                        int del_index = index - 1;                                        //the key index
                        node_row->bnode.erase(node_row->bnode.begin() + del_index);        //delete the key

                        node_row->bnode.insert(node_row->bnode.begin() + del_index, inserted_key);

                    }
                    else if (right_size - 1 >= right_children->min_cap) {

                        btree_node* temp = (btree_node*)right_children->left;

                        node* inserted_key = right_children->bnode[0];
                        if (temp == NULL) {
                            right_children->bnode.erase(right_children->bnode.begin());        //delete the key;

                        }

                        inserted_key->right = right_children;
                        int del_index = index - 1;                                        //the key index
                        node_row->bnode.erase(node_row->bnode.begin() + del_index);        //delete the key;
                        node_row->bnode.insert(node_row->bnode.begin() + del_index, inserted_key);
                    }
                    else {

                        for (int i = 0; i < (int)right_children->bnode.size(); i++) {
                            left_children->bnode.push_back(right_children->bnode[i]);
                        }
                        int del_index = index - 1;
                        node_row->bnode.erase(node_row->bnode.begin() + del_index);
                        delete right_children;

                    }
                }
            }
        }
    }
}


int main() {

    int node_count;
    int degree;
    char sort_choice;

    cin >> node_count;
    cin >> degree;
    cin >> sort_choice;
    btree my_btree = btree(node_count, degree, sort_choice);

    int x;
    int y;
    char z;
    string garbage;

    int insert_count = 0;
    while (true) {        //read data create new node;

        if (cin.eof()) {
            break;
        }
        if (insert_count < node_count) {
            cin >> x;
            cin >> y;
            cin >> z;
            node* newnode = new node(x, y, z);

            my_btree.insert_nn(my_btree.get_root(), newnode, false);
        }
        else {
            if (sort_choice == 'z') {
                char to_delete;
                cin >> to_delete;

                my_btree.delete_n(my_btree.get_root(), (int)to_delete);

            }
            else {
                int to_delete;
                cin >> to_delete;
                my_btree.delete_n(my_btree.get_root(), to_delete);
            }
        }
        insert_count++;
    }

    my_btree.print_tree(my_btree.get_root());
    return 0;
}
