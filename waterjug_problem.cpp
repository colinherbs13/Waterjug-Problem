#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

struct State {
	int aGallons;
	int bGallons;
};
int jugASize;
int jugBSize;
vector<State> history = {};
State initialState;
State currentState;
State goalState;
ifstream inputFile;
ofstream outputFile;


bool checkSuccess(State currState) {
	if (goalState.aGallons == -1) {
		if (currState.bGallons == goalState.bGallons && currState.aGallons <= jugASize) 
			return true;
	}
	if (goalState.bGallons == -1) {
		if (currState.aGallons == goalState.aGallons && currState.bGallons <= jugBSize)
			return true;
	}
	if (currState.aGallons == goalState.aGallons && currState.bGallons == goalState.bGallons)
		return true;

	return false;
}

void getUserInputs() {
	string line;
	int count = 0;
	while (getline(inputFile, line)) {
		string data = line.substr(line.find(": ")).substr(2);
		if (data.length() > 1) {
			string dataA = data.substr(0, data.find(" "));
			string dataB = data.substr(2, data.find(" "));
			if (count == 2) {
				initialState.aGallons = stoi(dataA);
				initialState.bGallons = stoi(dataB);
			}
			if (count == 3) {
				goalState.aGallons = stoi(dataA);
				goalState.bGallons = stoi(dataB);
			}
		}
		if (count == 0) {
			jugASize = stoi(data);
		}
		if (count == 1) {
			jugBSize = stoi(data);
		}
		count += 1;
	}
}

State performAction(int optionChoice) {
	switch (optionChoice) {
		case 0:
			// fill jug A completely
			currentState.aGallons = jugASize;
			break;
		case 1:
			// fill jug B completely
			currentState.bGallons = jugBSize;
			break;
		case 2:
			// pour jug A into jug B
			currentState.bGallons += currentState.aGallons;
			currentState.aGallons = 0;
			if (currentState.bGallons > jugBSize) {
				currentState.aGallons = currentState.bGallons - jugBSize;
				currentState.bGallons = jugBSize;
			}
			break;
		case 3:
			// pour jug B into jug A
			currentState.aGallons += currentState.bGallons;
			currentState.bGallons = 0;
			if (currentState.aGallons > jugASize) {
				currentState.bGallons = currentState.aGallons - jugASize;
				currentState.aGallons = jugASize;
			}
			break;
		case 4:
			// empty A
			currentState.aGallons = 0;
			break;
		case 5: 
			// empty B
			currentState.bGallons = 0;
			break;
		default: 
			// invalid option
			break;
	}
	return State{ currentState.aGallons, currentState.bGallons };
}

void printOperation(int operation) {
	switch (operation) {
	case -1:
		cout << "Initial State." << endl;
		outputFile << "Initial State." << endl;
		break;
	case 0:
		cout << "Operation: Fill jug A completely." << endl;
		outputFile << "Operation: Fill jug A completely." << endl;
		break;
	case 1:
		cout << "Operation: Fill jug B completely." << endl;
		outputFile << "Operation: Fill jug B completely." << endl;
		break;
	case 2:
		cout << "Operation: Pour jug A into jug B until jug A is empty or jug B is full." << endl;
		outputFile << "Operation: Pour jug A into jug B until jug A is empty or jug B is full." << endl;
		break;
	case 3:
		cout << "Operation: Pour jug B into jug A until jug B is empty or jug A is full." << endl;
		outputFile << "Operation: Pour jug B into jug A until jug B is empty or jug A is full." << endl;
		break;
	case 4:
		cout << "Operation: Empty jug A" << endl;
		outputFile << "Operation: Empty jug A" << endl;
		break;
	case 5:
		cout << "Operation: Empty jug B" << endl;
		outputFile << "Operation: Empty jug B" << endl;
		break;
	default: 
		break;
	}
}
bool process(State currState, int operation) {
	// check current state to see if we have met our goal
	if (checkSuccess(currState)) {
		printOperation(operation);
		std::cout << currState.aGallons << " " << currState.bGallons << endl;
		outputFile << currState.aGallons << " " << currState.bGallons << endl;
		return true;
	}
	
	bool isVisited = false;
	for (int i = 0; i < history.size(); i++) {
		if (history.at(i).aGallons == currState.aGallons && history.at(i).bGallons == currState.bGallons) {
			isVisited = true;
		}
	}
	// check if current state has been visited before
	if (!isVisited) {
		printOperation(operation);
		std::cout << currState.aGallons << " " << currState.bGallons << endl;
		outputFile << currState.aGallons << " " << currState.bGallons << endl;
		history.push_back(currState);
		if (process(performAction(0), 0) ||
			process(performAction(1), 1) ||
			process(performAction(2), 2) ||
			process(performAction(3), 3) ||
			process(performAction(4), 4) ||
			process(performAction(5), 5))
			return true;
		else {
			std::cout << "No more valid operations to be taken. Exiting..." << endl;
			outputFile << "No more valid operations to be taken. Exiting..." << endl;
		}
	}
	else {
		currentState = history.back();
		return false;
	}
}

// Valid actions: fill A all the way, fill B all the way, Pour A into B until B is full, pour B into A until A is full, empty A or B
int main(int argc, char **argv) {
	if (argc == 0) {
		cout << "No input file given!" << endl;
	}
	else {
		inputFile.open(argv[1]);
		outputFile.open("./output.txt");
		getUserInputs();
		currentState = initialState;
		process(currentState, -1);
		inputFile.close();
		outputFile.close();
		return 0;
	}
}
