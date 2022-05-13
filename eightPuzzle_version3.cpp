/*
---------------------------------------------
VERSION 3 ADDS A PRIORITY QUEUE TO EXPAND ON
THE NODES THAT HAVE THE BEST HEURISTIC VALUE
---------------------------------------------
*/
#include <vector>
#include <string>
#include <queue>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <cmath>
#include <set>
#include <stack>

using namespace std;
int HEUR = 1;
int EXPANDED_NODES = 0;
int MAX_SIZE_QUEUE = 0;

struct Node {
	//blank tile's location
	pair<int,int> blank_location;

	//board layout (current state)
	vector<vector<int> > state;

	//pointer to parent of node
	Node* parent;

	//heuristics
	int f_n;
	int g_n;
	int h_n;
};

struct CompareHeuristic {
	bool operator()(Node* l, Node* r) {
		return l->f_n > r->f_n;
	}
};

Node* GeneralSearch(Node* initialstate);
bool GoalTest(vector<vector<int>> curr_state);
priority_queue<Node*,vector<Node*>,CompareHeuristic> QueueingFunction(Node* node, priority_queue<Node*,vector<Node*>,CompareHeuristic> nodes, set<vector<vector<int>>> visited);
vector<Node*> Expand(Node* node);
Node* makeNode(Node* node, int blank_x, int blank_y);
void pickHeuristic(Node*& node);
int MisplacedTile(vector<vector<int>> curr_state);
int ManhattanDistance(vector<vector<int>> curr_state);

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
	cout << "1) Uniform Cost Search" << endl;
	cout << "2) A* with Misplaced Tile" << endl;
	cout << "3) A* with Manhattan Distance" << endl;
	cin >> HEUR;
	cout << endl;

	//initialize initial state for search
	Node* initialstate = new Node;
	initialstate->state = input_board_layout;
	initialstate->blank_location = blank;
	initialstate->g_n = 0;
	initialstate->parent = nullptr;
	//heuristics
	pickHeuristic(initialstate);
	//calculate f_n
	initialstate->f_n = initialstate->g_n + initialstate->h_n;

	//run General Search Algorithm from initial state
	Node* result = GeneralSearch(initialstate);

	//Print out solution step by step
	//use a stack to correct the reverse traversal from solution node
	cout << "--------SOLUTION TRACE-----------" << endl;
	Node* tracenode = result;
	stack<Node*> tracestack;
	while (tracenode->parent != nullptr) {
		tracestack.push(tracenode);
		tracenode = tracenode->parent;
	}
	while(!tracestack.empty()) {
		Node* curr_node = tracestack.top();
		tracestack.pop();
		for (int i = 0; i < curr_node->state.size(); i++) {
			for (int j = 0; j < curr_node->state[i].size(); j++) {
				cout << curr_node->state[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl;
	} 
	cout << "depth of solution: " << result->g_n << endl;
	cout << "number of nodes expanded: " << EXPANDED_NODES << endl;
	cout << "Maximum Size of Queue: " << MAX_SIZE_QUEUE << endl;

}

//General Search Algorithm 
Node* GeneralSearch(Node* initialstate) {
	//make queue and add initial state to queue
	priority_queue<Node*,vector<Node*>,CompareHeuristic> nodes;
	set<vector<vector<int>>> visited;
	nodes.push(initialstate);
	while(!nodes.empty()) {
		Node* node = nodes.top();
		cout << node->g_n << endl;
		nodes.pop();
		visited.insert(node->state);
		if (GoalTest(node->state)) return node;
		nodes = QueueingFunction(node,nodes,visited);
		if (nodes.size() > MAX_SIZE_QUEUE) MAX_SIZE_QUEUE = nodes.size();
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

priority_queue<Node*,vector<Node*>,CompareHeuristic> QueueingFunction(Node* node, priority_queue<Node*,vector<Node*>,CompareHeuristic> nodes, set<vector<vector<int>>> visited) {
	//expand the current node
	vector<Node*> children = Expand(node);

	//push the resulting nodes form the expansion to the queue
	for (int i = 0; i < children.size(); i++) {
		if (visited.find(children[i]->state) == visited.end()) {
			nodes.push(children[i]);
		} else {cout << "what up" << endl;}
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
	//increment expanded nodes
	EXPANDED_NODES++;
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

	//heuristics calculation
	newNode->g_n = node->g_n + 1;
	pickHeuristic(newNode);
	newNode->f_n = newNode->g_n + newNode->h_n;
	return newNode;
}

void pickHeuristic(Node*& node) {
	//choose heuristic based on the user's input at the beginning of the program
	//1) Uniform Cost Search
	//2) Misplace Tiles Heuristic
	//3) Manhattan Distance Heuristic
	switch(HEUR) {
		case 1:
			node->h_n = 0;
			break;
		case 2:
			node->h_n = MisplacedTile(node->state);
			break;
		case 3:
			node->h_n = ManhattanDistance(node->state);
			break;
	}
}

int MisplacedTile(vector<vector<int>> curr_state) {
	//goal state for comparison
	vector<vector<int>> goal_state = {
		{1, 2, 3},
		{4, 5, 6},
		{7, 8, 0}
	};
	int count = 0;
	//compare current state to the goal state, tile by tile, counting misplaced tiles
	for (int i = 0; i < goal_state.size(); i++) {
		for (int j = 0; j < goal_state.size(); j++) {
			if (goal_state[i][j] != curr_state[i][j] && curr_state[i][j] != 0) count++;
		}
	}
	return count;
}

int ManhattanDistance(vector<vector<int>> curr_state) {
	//goal state for comparison
	vector<vector<int>> goal_state = {
		{1, 2, 3},
		{4, 5, 6},
		{7, 8, 0}
	};
	//This map is used to store the goal coordinates of each tile
	//The key is the tile's number
	//The value is pair (coordinates in the 2D Vector)
	int tile = 1;
	unordered_map<int,pair<int,int>> placement;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if(tile != 9) {
				placement[tile] = {i,j};
				tile++;
			}
		}
	}
	int dist = 0;
	//compare current state to goal state and calculate distance of each tile to its goal location
	for (int i = 0; i < goal_state.size(); i++) {
		for (int j = 0; j < goal_state.size(); j++) {
			if (curr_state[i][j] != 0) {
				dist +=	abs(placement[curr_state[i][j]].first - i) + abs(placement[curr_state[i][j]].second - j);
			} 
		}
	}
	return dist;
}
