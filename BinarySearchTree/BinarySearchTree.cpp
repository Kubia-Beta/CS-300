//============================================================================
// Name        : BinarySearchTree.cpp
// Author      : Connor Sculthorpe
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Bid handling system, BST style
//============================================================================

#include <iostream>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
	string bidId; // unique identifier
	string title;
	string fund;
	double amount;
	Bid() {
		amount = 0.0;
	}
};

// Internal structure for tree node
struct Node {
	Bid bid;
	Node *left;
	Node *right;

	// default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}

	// initialize with a bid
	Node(Bid aBid) :
			Node() {
		bid = aBid;
	}
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
	Node* root;

	void addNode(Node* node, Bid bid);
	void inOrder(Node* node);
	Node* removeNode(Node* node, string bidId);

public:
	BinarySearchTree();
	virtual ~BinarySearchTree();
	void InOrder();
	void Insert(Bid bid);
	void Remove(string bidId);
	Bid Search(string bidId);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
	// FixMe (1): initialize housekeeping variables
	//root is equal to nullptr
	root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
	// recurse from root deleting every node
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() { // This is a dangerous function naming scheme, consider distinct function names
	// FixMe (2): In order root
	// call inOrder fuction and pass root 
	inOrder(root);
}

/**
 * Traverse the tree in post-order
 */
void BinarySearchTree::PostOrder() {
	// FixMe (3): Post order root
	// postOrder root
}

/**
 * Traverse the tree in pre-order
 */
void BinarySearchTree::PreOrder() {
	// FixMe (4): Pre order root
	// preOrder root
}

/**
 * Insert a bid
 */
void BinarySearchTree::Insert(Bid bid) {
	// FIXME (5) Implement inserting a bid into the tree
	if (root == nullptr) {
		root = new Node(bid);
	}
	else {
		this->addNode(root, bid);
	}
	// if root equarl to null ptr
	  // root is equal to new node bid
	// else
	  // add Node root and bid
}

/**
 * Remove a bid
 */
void BinarySearchTree::Remove(string bidId) {
	// FIXME (6) Implement removing a bid from the tree
	// remove node root bidID
}

/**
 * Search for a bid
 */
Bid BinarySearchTree::Search(string bidId) {
	// FIXME (7) Implement searching the tree for a bid
	// set current node equal to root
	Node* currNode = root;
	bool found = false;
	const int matchFlag = 0; // Zero is a match

	while (true) {
		int comparisonResult = currNode->bid.bidId.compare(bidId);
		if (comparisonResult == matchFlag) { // Found the bid
			return currNode->bid; // Return the currently held bid that matched
		}
		else if (comparisonResult < matchFlag) { // The check is less than what was checked against
			currNode = currNode->left; // Shift loop left
		}
		else { // current > bidId checked against
			currNode = currNode->right; // Shift loop right
		} // Continue looping
	}
	// keep looping downwards until bottom reached or matching bidId found
		// if match found, return current bid

		// if bid is smaller than current node then traverse left
		// else larger so traverse right
	Bid bid;
	return bid;
}

/**
 * Add a bid to some node (recursive)
 *
 * @param node Current node in tree
 * @param bid Bid to be added
 */
void BinarySearchTree::addNode(Node* currNode, Bid bid) {
	// FIXME (8) Implement inserting a bid into the tree
	const int matchFlag = 0; // Zero is a match
	
	if (currNode->bid.bidId.compare(bid.bidId) < matchFlag) { // Current node's bidID compared against bidID to be added
		if (currNode->left == nullptr) { // Left subtree, if the string contained here is longer than or has a greater character value and left is null
			currNode->left = new Node(bid); // Construct a new node and make it the left pointer of the current node
		}
		else { // Not null left
			addNode(currNode->left, bid); // Recurse leftward
		}
	}
	else { // Right subtree
		if (currNode->right == nullptr) { // Greater and right is null
			currNode->right = new Node(bid); // Node to be inserted becomes the right
		}
		else { // Not null right
			addNode(currNode->right, bid); // Recurse rightward
		}
	}
	// if node is larger then add to left
		// if no left node
			// this node becomes left
		// else recurse down the left node
	// else
		// if no right node
			// this node becomes right
		//else
			// recurse down the left node
}
void BinarySearchTree::inOrder(Node* node) {
	  // FixMe (9): Pre order root
	if (node == nullptr) {

	}

	  //if node is not equal to null ptr
	  //InOrder not left
	  //output bidID, title, amount, fund
	  //InOder right
}
void BinarySearchTree::postOrder(Node* node) {
	  // FixMe (10): Pre order root
	  //if node is not equal to null ptr
	  //postOrder left
	  //postOrder right
	  //output bidID, title, amount, fund

}

void BinarySearchTree::preOrder(Node* node) {
	  // FixMe (11): Pre order root
	  //if node is not equal to null ptr
	  //output bidID, title, amount, fund
	  //postOrder left
	  //postOrder right      
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
	cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
			<< bid.fund << endl;
	return;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadBids(string csvPath, BinarySearchTree* bst) {
	cout << "Loading CSV file " << csvPath << endl;

	// initialize the CSV Parser using the given path
	csv::Parser file = csv::Parser(csvPath);

	// read and display header row - optional
	vector<string> header = file.getHeader();
	for (auto const& c : header) {
		cout << c << " | ";
	}
	cout << "" << endl;

	try {
		// loop to read rows of a CSV file
		for (unsigned int i = 0; i < file.rowCount(); i++) {

			// Create a data structure and add to the collection of bids
			Bid bid;
			bid.bidId = file[i][1];
			bid.title = file[i][0];
			bid.fund = file[i][8];
			bid.amount = strToDouble(file[i][4], '$');

			//cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

			// push this bid to the end
			bst->Insert(bid);
		}
	} catch (csv::Error &e) {
		std::cerr << e.what() << std::endl;
	}
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
	str.erase(remove(str.begin(), str.end(), ch), str.end());
	return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

	// process command line arguments
	string csvPath, bidKey;
	switch (argc) {
	case 2:
		csvPath = argv[1];
		bidKey = "98109";
		break;
	case 3:
		csvPath = argv[1];
		bidKey = argv[2];
		break;
	default:
		csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
		bidKey = "98109";
	}

	// Define a timer variable
	clock_t ticks;

	// Define a binary search tree to hold all bids
	BinarySearchTree* bst;
	bst = new BinarySearchTree();
	Bid bid;

	int choice = 0;
	while (choice != 9) {
		cout << "Menu:" << endl;
		cout << "  1. Load Bids" << endl;
		cout << "  2. Display All Bids" << endl;
		cout << "  3. Find Bid" << endl;
		cout << "  4. Remove Bid" << endl;
		cout << "  9. Exit" << endl;
		cout << "Enter choice: ";
		cin >> choice;

		switch (choice) {

		case 1:
			
			// Initialize a timer variable before loading bids
			ticks = clock();

			// Complete the method call to load the bids
			loadBids(csvPath, bst);

			//cout << bst->Size() << " bids read" << endl;

			// Calculate elapsed time and display result
			ticks = clock() - ticks; // current clock ticks minus starting clock ticks
			cout << "time: " << ticks << " clock ticks" << endl;
			cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
			break;

		case 2:
			bst->InOrder();
			break;

		case 3:
			ticks = clock();

			bid = bst->Search(bidKey);

			ticks = clock() - ticks; // current clock ticks minus starting clock ticks

			if (!bid.bidId.empty()) {
				displayBid(bid);
			} else {
				cout << "Bid Id " << bidKey << " not found." << endl;
			}

			cout << "time: " << ticks << " clock ticks" << endl;
			cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

			break;

		case 4:
			bst->Remove(bidKey);
			break;
		}
	}

	cout << "Good bye." << endl;

	return 0;
}
