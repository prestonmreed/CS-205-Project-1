/*
---------------------------------------------
VERSION 1 OF THIS PROJECT INCLUDES TAKING IN 
THE INPUT AND DARFTING UP GENERAL SEARCH WITH 
THE GOAL TEST FUNCTION
*CODE DOES NOT WORK YET*
---------------------------------------------
*/
#include <vector>
#include <string>
#include <queue>
#include <iostream>
#include <sstream>

using namespace std;

struct Node {
	//blank tile's location
	pair<int,int> blank_location;

	//board layout (current state)
	vector<vector<int> > state;

	//left,right,up,down
	string move;

	//pointer to parent of node
	Node* parent;
};

//General Search Algorithm 
Node* GeneralSearch(Node* initialstate) {
	//make queue and add initial state to queue
	queue<Node*> nodes;
	nodes.push(initialstate);
	while(!nodes.empty()) {
		Node* node = nodes.front();
		nodes.pop();
		if (GoalTest(node->state)) return node;
		//nodes = QueueingFunction(nodes);
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
		{7, 8, 9}
	};

	//compare current state to the goal state, tile by tile
	for (int i = 0; i < goal_state.size(); i++) {
		for (int j = 0; j < goal_state.size(); j++) {
			if (goal_state[i][j] != curr_state[i][j]) return false;
		}
	}

	return true;
}

//queue<Node*> QueueingFunction(queue<Node*> nodes) {}

//void Expand() {}

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

	for (int i = 0; i < input_board_layout.size(); i++) {
		for (int j = 0; j < input_board_layout[i].size(); j++) {
			cout << input_board_layout[i][j] << " ";
		}
		cout << endl;
	}
	//found out which heuristic is being used
	int heuristic;
	cout << "Enter your preferred heuristic by typing the corresponding number in the list and pressing enter:" << endl;
	cout << "1: Uniform Cost Search" << endl;
	cout << "2: A* with Misplaced Tile" << endl;
	cout << "3: A* with Manhattan Distance" << endl;
	cin >> heuristic;
	cout << heuristic << endl;
}
