#include <iostream> // I/O operations
#include <fstream> //file operations
#include <string> //string data type

using namespace std;


class nn { //node class where each node stores a player
private:

	string name;    //player's name
	string team;    //player's team
	int rebound;    //player's total rebound
	int assist;     //playe's  total assist
	int point;      //player's total point

	string color;   //node's color where either "RED" or "BLACK"
public:
	nn* left;       //pointer where points node's left child
	nn* right;      //pointer where points node's right child
	nn* parent;     //pointer where points node's parent

	nn(string name, string team, int rb, int ast, int pt); //constructor function for the node(player)

	string get_data() { return this->name; };   //getter for the name attribute of the node 
	string get_color() { return this->color; }  //getter for the node's color

	int get_points() { return point; }          //getter for the player's points
	int get_rebound() { return rebound; }       //getter for the player's rebound
	int get_assist() { return assist; }         //getter for the player's assist

	void set_color(string c) { this->color = c; }          //sets the color (used in SIBLING=="RED" case in the red black tree) 
	void update_data(nn* np);                              //updtaes the points, assists and rebounds of the player
	void toggle_color() { if (color == "RED") { color = "BLACK"; } else { color = "RED"; } } //toggles the color of the node( used in the SIBLING=="BLACK" case)
};

nn::nn(string name, string team, int rb, int ast, int pt) { //constructor body
	this->name = name;  //name is assigned
	this->team = team;  //team is assigned
	this->rebound = rb; //rebounds are assigned
	this->assist = ast; //assist is assigned
	this->point = pt;   //points are assigned

	this->left = NULL;  //pointers ar set to NULL
	this->right = NULL; //set to NULL
	this->parent = NULL;//parent is set to NULL
	color = "RED";		//initial color is red --> while insertion
}

void nn::update_data(nn* np) { //updates the total points, rebounds and assits of the player
	this->name = np->name;
	this->team = np->team;
	this->rebound += np->rebound; //incremented
	this->assist += np->assist;   //incremented
	this->point += np->point;     //incremented
}

struct max_palyer {     //data struct where used in storing the name and max_assist,max_points or max_rebounds player in hte season

	int max;            //the data value
	string name;        //name of the player

}typedef mp;

class red_black_tree {  //RED BLACK TREE class

public:
	nn* root;           //the root node of the tree
	mp* max_assists;    //max_assist player  is stored as attribute in tree
	mp* max_rebounds;   //max_rebounds player is stored in the tree
	mp* max_points;     //max_points player is stored
	red_black_tree() { root = NULL; max_assists = new mp(); max_rebounds = new mp(); max_points = new mp(); } //constructor of the Red Black tree, root is NULL and the three mp* are alocated
	~red_black_tree();			//destructor for the rbt class
	void clear_tree(nn*);		//clears the whole tree
	void insert_nn(nn*, nn*);    //inserts new node to the tree as if BST, then regulates the tree to satisfy the  red black tree constraints
	void print_tree(nn*, int, string,int);  //prints the whole tree recursively
};

red_black_tree::~red_black_tree() { //destructor for the tree to free all allocated space

	clear_tree(root);   //delete all nodes in the tre

	root->left = NULL;  //clear root 
	root->right = NULL;
	root = NULL;

	//delete max players attributes
	delete max_assists;
	max_assists = NULL;
	delete max_rebounds;
	max_rebounds = NULL;
	delete max_points;
	max_points = NULL;
}

void red_black_tree::clear_tree(nn* temp) { //clear the tree

	if (temp) {   //if not NULL

		clear_tree(temp->left); //go as far as left 
		clear_tree(temp->right);//go as far as right 

		//temp is guarantee a leaf node
		temp->parent = NULL;    //the link to the parent is NULL 
		delete temp;            //delete the node itself
		temp = NULL;            //point it to the NULL
	}
}


void red_black_tree::insert_nn(nn* parent, nn* newnode) {   //inserts a new node after that the red black tree construction is made by recoloring, LL LR RR RL rotations 

	bool flag_inserted = false; // flag that if true the new node is inserted like BST tree
	if (root == NULL) { //if tree is empty
		root = newnode; // EMPTY TREE ROOT->COLOR => BLACK
		root->set_color("BLACK");
		max_points->max = newnode->get_points(); //the root node player is set to the max_everything so that after recursive calls, a reference player is obtained
		max_points->name = newnode->get_data();

		max_rebounds->max = newnode->get_rebound();
		max_rebounds->name = newnode->get_data();

		max_assists->max = newnode->get_assist();
		max_assists->name = newnode->get_data();

	}
	else {      //if root is not NULL

		if (parent != NULL && newnode != NULL) {    //this is the recursive insertion part so parent must no t be NULL. newnode!=NULL is for the red black tree re-insertion and the N�L values might have been inserted so prevents that
			if (parent->get_data() > newnode->get_data()) { //if parent's name is higher than the newnode go left direction
				if (parent->left == NULL) { //leaf is reached so insert
					parent->left = newnode;
					newnode->parent = parent;   //newnode parent pointer is assigned to the parent node
					flag_inserted = true;       //the newnode is inserted so flag --> True
				}
				else {
					insert_nn(parent->left, newnode);   //if not inserted call recursiveley again
				}
			}
			else if (parent->get_data() < newnode->get_data()) {    //same operations but direction is right if the node's name is bigger than the parent node
				if (parent->right == NULL) {
					parent->right = newnode;
					newnode->parent = parent;       //newnode parent pointer is assigned to the parent node
					flag_inserted = true;           //FLAG--> True
				}
				else {
					insert_nn(parent->right, newnode);  //�f not call recursily the insert function
				}
			}
			else {
				parent->update_data(newnode);       //if the name of the parent and the new node is same, add the points, assist, rebounds
			}

			if (parent->get_assist() > max_assists->max) { //the parent node's assist value and the newly inserted node is compared 
				max_assists->max = parent->get_assist();
				max_assists->name = parent->get_data();
			}

			if (parent->get_rebound() > max_rebounds->max) {//the parent node's rebound value and the newly inserted node is compared 
				max_rebounds->max = parent->get_rebound();
				max_rebounds->name = parent->get_data();
			}

			if (parent->get_points() > max_points->max) {//the parent node's point value and the newly inserted node is compared 
				max_points->max = parent->get_points();
				max_points->name = parent->get_data();
			}
		}

		if (flag_inserted) {        //the node is inserted as BST now the Red BLACK TREE construction is made

			nn* nn_res = newnode;   //the newnode and the parent node is copied because the operations are in loop so the parent and newnode is going to change in iterations
			nn* parent_res = parent;

			while (true) {

				if (nn_res->parent->get_color() == "BLACK" || newnode->get_color() == "BLACK") { //the parent node is black-> break. Newnode black comparison is where after the rotation the newly inserted node mightbe black so no need to go in to loop
					break; //break the inf loop
				}
				else {  //if the parent color is RED check the siblign

					nn* parent_sibling = NULL;  //the sibling is set to NULL to scope of the inner if else statements
					if (parent_res->parent->left == parent_res) {   //find which node is the sibling of the parent
						parent_sibling = parent_res->parent->right; //the parent is the left node so its sibling is the right node of the parents->parent
					}
					else {
						parent_sibling = parent_res->parent->left;  //the sibling is the left node of the parent->parent
					}

					if ((parent_sibling != NULL && parent_sibling->get_color() == "BLACK") || parent_sibling == NULL) { //sibling is "BLACK" or NULL THEREFORE ROTATION MUST BE DONE

						if (parent_res == parent_res->parent->left) { //L --> the parents is at the left of the parent->parent

							if (nn_res == parent_res->left) { //LL --> LEFT LEFT case 

								//since the LL case the "parent" is going to replace the grandfather  and the parent's left side (newnode) is not changing
								nn* temp = parent_res->right;			//the right subtree is stored in temp 
								parent_res->parent->left = NULL;		//the parent's link to its parent is broken
								parent_res->right = parent_res->parent;	//parent'right child must be the grandparent

								if (parent_res->parent == root) {		//if grandfather is the root 

									root = parent_res;					//root will be replaced by the parent node 
									parent_res->parent = NULL;			//parent's parent will be NULL since its the root node
									parent_res->right->parent = parent_res;		//former grandfather's parent pointer is assigned to parent(root) 

									root->set_color("BLACK");			//root must be BLACK everytime
									root->left->set_color("RED");		//the childs are to be red
									root->right->set_color("RED");		//childs are red
								}
								else {									//if the grandgfather is not the root node 

									if (parent_res->parent == parent_res->parent->parent->left) { //finding the which pointer is that grandfather's parent points to the grandfather 
										parent_res->parent->parent->left = parent_res;	//grandfathers's parent's left node points to the parent node
									}
									else {
										parent_res->parent->parent->right = parent_res;	//grandfather's right pointer points the parent node
									}

									parent_res->parent = parent_res->right->parent;		//parent's parent pointer points to its new parent
									parent_res->right->parent = parent_res;				//former grandfather's parent pointer points to parent 

									parent_res->set_color("BLACK");			//recoloring of the parent node
									parent_res->left->set_color("RED");		//childs assigned to be red
									parent_res->right->set_color("RED");	//childs assigned to red color
								}

								insert_nn(root, temp);		//re-insert the broken node to the red black tree 
								break;						//break the loop
							}
							else { //Left Right case where the newnode is going to be the parent

								nn* nn_left = nn_res->left;	//left sub tree must be stored

								parent_res->parent->left = nn_res;	//grandfather's left is connected to the new node
								nn_res->parent = parent_res->parent;//newnode's parent pointer is the grandfather

								parent_res->parent = nn_res;		//the parent node which will be a left child. so parent's parent pointer is the newnode
								parent_res->right = NULL;			//its's right node is assigned to NULL because it was the newnode

								nn_res->left = parent_res;			//new node's left child is the former(parent)

								insert_nn(root, nn_left);			//insert the broken left node to the tree again

								//same operations with LL case because 
								nn* temp = nn_res->right;			//the right subtree is stored in temp		
								nn_res->parent->left = NULL;		//the parent's link to its parent is broken
								nn_res->right = nn_res->parent;		//parent'right child must be the grandparent

								if (nn_res->parent == root) {		//if grandfather is the root 

									root = nn_res;					//root will be replaced by the parent node
									nn_res->parent = NULL;			//parent's parent will be NULL since its the root node
									nn_res->right->parent = nn_res; //former grandfather's parent pointer is assigned to parent(root)

									root->set_color("BLACK");		//root must be BLACK everytime
									root->left->set_color("RED");	//the childs are to be red
									root->right->set_color("RED");	//the childs are to be red
								}
								else {

									if (nn_res->parent == nn_res->parent->parent->left) { //finding the which pointer is that grandfather's parent points to the grandfather
										nn_res->parent->parent->left = nn_res;			//grandfathers's parent's left node points to the parent node
									}
									else {
										nn_res->parent->parent->right = nn_res;	//grandfather's right pointer points the parent node

									}

									nn_res->parent = nn_res->right->parent;	//parent's parent pointer points to its new parent
									nn_res->right->parent = nn_res;			//former grandfather's parent pointer points to parent 

									nn_res->set_color("BLACK");			//recoloring of the parent node
									nn_res->left->set_color("RED");		//the childs are to be red
									nn_res->right->set_color("RED");	//the childs are to be red
								}


								insert_nn(root, temp);
								break;

							}
						}
						else { //Parent is the Right child of the grandfather 

							if (nn_res == parent_res->left) {			//Right Left case --> grandfather-parent is right child + newnode - parent left child

								nn* nn_right = nn_res->right;			//the left subtree is stored

								parent_res->parent->right = nn_res;		//the grandfathers right pointer is pointing the newnode
								nn_res->parent = parent_res->parent;	//newnodes parent pointer is assigned to the grandfather

								parent_res->parent = nn_res;			//parent nodes's parent pointer is the newnode
								parent_res->left = NULL;				//parent's left pointer is set to NULL because there was newnode

								nn_res->right = parent_res;				//newnode's right child is the parent node

								insert_nn(root, nn_right);				//insert this nn_right node to the tree

								nn* temp = nn_res->left;				//the left subtree is stored in temp	
								nn_res->parent->right = NULL;			//the parent's link to its parent is broken
								nn_res->left = nn_res->parent;			//parent'left child must be the grandparent

								if (nn_res->parent == root) {			//if grandfather is the root

									root = nn_res;						//root will be replaced by the parent node
									nn_res->parent = NULL;				//parent's parent will be NULL since its the root node
									nn_res->left->parent = nn_res;		//former grandfather's parent pointer is assigned to parent(root)

									root->set_color("BLACK");			//root must be BLACK everytime
									root->left->set_color("RED");		//childs assigned to be red
									root->right->set_color("RED");		//childs assigned to be red


								}
								else {

									if (nn_res->parent == nn_res->parent->parent->left) {	//finding the which pointer is that grandfather's parent points to the grandfather
										nn_res->parent->parent->left = nn_res;				//grandfathers's parent's left node points to the parent node
									}
									else {
										nn_res->parent->parent->right = nn_res;				//grandfather's right pointer points the parent node
									}

									nn_res->parent = nn_res->left->parent;		//parent's parent pointer points to its new parent
									nn_res->left->parent = nn_res;				//former grandfather's parent pointer points to parent

									nn_res->set_color("BLACK");					//recoloring of the parent node
									nn_res->left->set_color("RED");				//childs assigned to be red
									nn_res->right->set_color("RED");			//childs assigned to be red
								}


								insert_nn(root, temp);
								break;


							}
							else { //Right right case --> grandfather- parent - newnode is aligned all right 

								nn* temp = parent_res->left;			//the left subtree is stored in temp 
								parent_res->parent->right = NULL;		//the parent's link to its parent is broken
								parent_res->left = parent_res->parent;	//parent'left child must be the grandparent

								if (parent_res->parent == root) {		//if grandfather is the root

									root = parent_res;					//root will be replaced by the parent node
									parent_res->parent = NULL;			//parent's parent will be NULL since its the root node
									parent_res->left->parent = parent_res;	//former grandfather's parent pointer is assigned to parent(root)

									root->set_color("BLACK");			//root must be BLACK everytime
									root->left->set_color("RED");		//childs assigned to be red
									root->right->set_color("RED");		//childs assigned to be red

								}
								else {

									if (parent_res->parent == parent_res->parent->parent->left) {	//finding the which pointer is that grandfather's parent points to the grandfather 
										parent_res->parent->parent->left = parent_res;				//grandfathers's parent's left node points to the parent node
									}
									else {
										parent_res->parent->parent->right = parent_res;				//grandfather's right pointer points the parent node
									}

									parent_res->parent = parent_res->left->parent;		//parent's parent pointer points to its new parent
									parent_res->left->parent = parent_res;				//former grandfather's parent pointer points to parent 

									parent_res->set_color("BLACK");						//recoloring of the parent node
									parent_res->left->set_color("RED");					//childs assigned to be red
									parent_res->right->set_color("RED");				//childs assigned to be red

								}

								insert_nn(root, temp);
								break;
							}

						}
					}
					else {  // the sibling color is "RED" so change color of parent and sibling to "BLACK" and toggle the color of the grandfather of the node and go on with While loop.

						//change color of parent and sibling to black

						parent_sibling->set_color("BLACK");
						parent_res->set_color("BLACK");

						//toggle color of the grandfather

						nn_res = parent_res->parent;   //the grandfather is the child node
						if (nn_res == root) { //if the grandfather is the root break the loop, no further iterations
							break;
						}
						else {                 //if not root the same process where the checking of the sinlings are repeated in inf loop 
							nn_res->toggle_color(); //the grandfather color is toggled
							parent_res = nn_res->parent;  //the parent of grandfather is assigned also
						}

						//recheck so do not break from the inf loop
					}
				}
			}
		}
	}
}

void red_black_tree::print_tree(nn* parent, int depth, string season,int first) {   //print the red black tree in the season format and pre-treversal
	if (parent) {

		if (parent == root) {   //before printing the root node the season summary is printed
			cout << "End of the " << season << " Season" << endl;
			cout << "Max Points: " << max_points->max << " - Player Name: " << max_points->name << endl;
			cout << "Max Assists: " << max_assists->max << " - Player Name: " << max_assists->name << endl;
			cout << "Max Rebs: " << max_rebounds->max << " - Player Name: " << max_rebounds->name << endl;

		}
		if (first == 1) {

			for (int i = 0; i < depth; i++) {   //the depth value is the level of the node and print "-" as the depth of the node
				cout << "-";
			}
			cout << "(" << parent->get_color() << ") " << parent->get_data() << endl;     //printing the node 
			print_tree(parent->left, ++depth, season, first);       //the depth is pre incremented so that at the recursive call the level is updated
			print_tree(parent->right, depth, season, first);        //the depth is incremented in the left subroutine so no need to increment again
		}
	}
}

int main(int argc, char* argv[])
{

	if (argc != 2) {
		return 0;
	}

	char* filename = argv[1];  //the filename is input at start of the program

	ifstream file_pointer;  //input file stream 
	file_pointer.open(filename);    //open the file


	string season_name; //store for season name
	string name;        //the name of the player
	string team;        //the team name
	int rebound;        //the rebound value
	int assist;         //the assist value
	int point;          //the point value

	string first_line;  //garbage container for the firsline and  "," 
	getline(file_pointer, first_line);   //the first line is taken

	string season_end = "INIT"; //the season_end is a flag like varible it is used for detec the season end

	red_black_tree rbt = red_black_tree();  //a red black tree is created
	int counter = 0;	//for priting the first season only after that only max players will be printed

	while (!file_pointer.eof()) {   //while end of file

		season_end = season_name;   //the season end is stores value of the season the one iteration before 

		getline(file_pointer, season_name, ',');  //season name is taken
		getline(file_pointer, name, ',');       //the name is taken 
		getline(file_pointer, team, ',');       //the team name is taken
		file_pointer >> rebound;                 //the rebound value
		getline(file_pointer, first_line, ',');   //get rid of the comma
		file_pointer >> assist;                   //take the assist value
		getline(file_pointer, first_line, ',');   //get rid of the ","
		file_pointer >> point;                    //the point is assigned
		getline(file_pointer, first_line, '\n');  //the new line is taken

		if (season_end != season_name || file_pointer.eof()) {        //if the season is changed the print the season 
			rbt.print_tree(rbt.root, 0, season_end, counter);    //print the season
			counter++;	//if counter == 1 the first season tree will be printed and after counter++ only the season statistics are going to be printed
		}

		nn* nplayer = new nn(name, team, rebound, assist, point); //create a new player
		rbt.insert_nn(rbt.root, nplayer);                       //insert the player to the tree
	}

	file_pointer.close();   //close the file pointer

	return 0;
}
