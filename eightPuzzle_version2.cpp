/*
---------------------------------------------
VERSION 2 OF BUILDS ON VERSION 1 BY ADDING THE
QUEUEING-FUCNTION AND EXPAND FUNCTIONS AND 
IMPLEMENTS UNIFORM COST SEARCH
---------------------------------------------
*/
#include <vector>
#include <string>
#include <queue>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <cmath>

using namespace std;
int HEUR = 1;
int EXPANDED_NODES = 0;
struct Node {
	//blank tile's location
	pair<int,int> blank_location;

	//board layout (current state)
	vector<vector<int> > state;

	//pointer to parent of node
	Node* parent;

	//heuristics
	double f_n;
	double g_n;
	double h_n;
};

Node* GeneralSearch(Node* initialstate);
bool GoalTest(vector<vector<int>> curr_state);
queue<Node*> QueueingFunction(Node* node, queue<Node*> nodes);
vector<Node*> Expand(Node* node);
Node* makeNode(Node* node, int blank_x, int blank_y);

int main() {
	vector<vector<int>> input_board_layout;
	cout << "Enter your puzzle input as 3 lines of 3 space-separated integers (use 0 to indicate blank tile):" << endl;
	//intialize row inputs from user
	for (int row = 0; row < 3; row++) {
		cout << "Enter row of 3 numbers:" << endl;
		string curr_row;
		getline(cin,curr_row);
		istringstream iss_curr_row(curr_row);
		vector<int> row_vec;
		for(int i = 0; i < 3; i++) {
			int curr_tile;
			iss_curr_row >> curr_tile;
			row_vec.push_back(curr_tile);
		}
		input_board_layout.push_back(row_vec);
	}
	pair<int,int> blank;
	for (int i = 0; i < input_board_layout.size(); i++) {
		for (int j = 0; j < input_board_layout[i].size(); j++) {
			cout << input_board_layout[i][j] << " ";
			if (input_board_layout[i][j] == 0) blank = {i,j};
		}
		cout << endl;
	}
	//found out which heuristic is being used
	int heuristic;
	cout << "Enter your preferred heuristic by typing the corresponding number in the list and pressing enter:" << endl;
	cout << "1: Uniform Cost Search" << endl;
	cout << "2: A* with Misplaced Tile" << endl;
	cout << "3: A* with Manhattan Distance" << endl;
	cin >> HEUR;

	//initialize initial state for search
	Node* initialstate = new Node;
	initialstate->state = input_board_layout;
	initialstate->blank_location = blank;
	initialstate->g_n = 0;
	//heuristics (uniform cost search for now...)
	initialstate->h_n = 0;
	//calculate f_n
	initialstate->f_n = initialstate->g_n + initialstate->h_n;
	Node* result = GeneralSearch(initialstate);
	cout << "depth of result: " << result->g_n << endl;
	for (int i = 0; i < result->state.size(); i++) {
		for (int j = 0; j < result->state[i].size(); j++) {
			cout << result->state[i][j] << " ";
		}
		cout << endl;
	}
	cout << EXPANDED_NODES << endl;
}

//General Search Algorithm 
Node* GeneralSearch(Node* initialstate) {
	//make queue and add initial state to queue
	queue<Node*> nodes;
	nodes.push(initialstate);
	while(!nodes.empty()) {
		Node* node = nodes.front();
		nodes.pop();
		for (int i = 0; i < node->state.size(); i++) {
			for (int j = 0; j < node->state[i].size(); j++) {
				cout << node->state[i][j] << " ";
			}
			cout << endl;
		}	
		if (GoalTest(node->state)) return node;
		nodes = QueueingFunction(node,nodes);
	}

	//if fails, output "FAILURE" and return initial state node
	cout << "FAILURE" << endl;
	return initialstate;
}

//Compares a state to the goal state to see if a solution is found
bool GoalTest(vector<vector<int>> curr_state) {
	//create goal board layout
	vector<vector<int>> goal_state = {
		{1, 2, 3},
		{4, 5, 6},
		{7, 8, 0}
	};

	//compare current state to the goal state, tile by tile
	for (int i = 0; i < goal_state.size(); i++) {
		for (int j = 0; j < goal_state.size(); j++) {
			if (goal_state[i][j] != curr_state[i][j]) return false;
		}
	}

	return true;
}

queue<Node*> QueueingFunction(Node* node, queue<Node*> nodes) {
	//expand the current node
	vector<Node*> children = Expand(node);

	//increment expanded nodes
	EXPANDED_NODES++;

	//push the resulting nodes form the expansion to the queue
	for (int i = 0; i < children.size(); i++) {
		nodes.push(children[i]);
	}
	return nodes;
}

vector<Node*> Expand(Node* node) {
	vector<Node*> children;
	//if conditions for valid moves of the blank tile
	if (node->blank_location.first + 1 < 3) children.push_back(makeNode(node,node->blank_location.first+1,node->blank_location.second));
	if (node->blank_location.first - 1 >= 0) children.push_back(makeNode(node,node->blank_location.first-1,node->blank_location.second));
	if (node->blank_location.second + 1 < 3) children.push_back(makeNode(node,node->blank_location.first,node->blank_location.second+1));
	if (node->blank_location.second - 1 >= 0) children.push_back(makeNode(node,node->blank_location.first,node->blank_location.second-1));
	return children;
}

Node* makeNode(Node* node, int blank_x, int blank_y) {
	//initialize new node
	Node* newNode = new Node;

	//set parent of new node to current node
	newNode->parent = node;

	//enter new coordinates for the blank tile
	newNode->blank_location.first = blank_x;
	newNode->blank_location.second = blank_y;

	//correct state of new node to reflect blank tile move
	newNode->state = node->state;
	newNode->state[node->blank_location.first][node->blank_location.second]=node->state[blank_x][blank_y];
	newNode->state[blank_x][blank_y] = 0;

	//heuristics
	newNode->g_n = node->g_n + 1;
	cout << newNode->g_n << endl;
	if (HEUR == 1) newNode->h_n = 0;
	//else if (HEUR == 2) newNode->h_n = MisplacedTile(newNode->state);
	//else if (HEUR == 3) newNode-> h_n = ManhattanDistance(newNode->state);

	newNode->f_n = newNode->g_n + newNode->h_n;
	return newNode;
}