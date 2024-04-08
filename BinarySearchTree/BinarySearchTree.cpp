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
//using std::unique_ptr;

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
	void InOrderHelper(Node* node);
	void PreOrderHelper(Node* node);
	void PostOrderHelper(Node* node);
	Node* removeNode(Node* node, string bidId);

public:
	BinarySearchTree();
	virtual ~BinarySearchTree();
	void BSTDestructorHelper(Node* node);
	void InOrder();
	void PostOrder();
	void PreOrder();
	void Insert(Bid bid);
	void Remove(string bidId);
	Bid Search(string bidId);
	Node* ParentSearch(Node* child); // Created but unused
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
	root = nullptr;
	return;
}

/**
 * Destructor.
 * Iterates through the BST and frees related Node memory with a helper function. (recursive)
 */
BinarySearchTree::~BinarySearchTree() {
	BSTDestructorHelper(root); // Recursive function helper
	root = nullptr; // Called when all Node are deleted
	return;
}

/**
 * Helper function.
 * Deletes the current Binary Search Tree node by node. (recursive)
 *
 * @param Root node in tree
 */
void BinarySearchTree::BSTDestructorHelper(Node* node) {
	// FIXME: Create unit tests to check destructor behavior
	if (node == nullptr) { // Stop us from traversing nothing
		return;
	}

	BSTDestructorHelper(node->left); // Recursively call left until we cannot go left anymore
	BSTDestructorHelper(node->right); // Recursively call right until we canot go right anymore
	string bidId = node->bid.bidId; // Take the node's bidId,
	Remove(bidId); // feed it to remove,
	delete node; // then delete the node.
	node = nullptr;
	return;
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
	this->InOrderHelper(root);
}

/**
 * Traverse the tree in post-order
 */
void BinarySearchTree::PostOrder() {
	this->PostOrderHelper(root);
}

/**
 * Traverse the tree in pre-order
 */
void BinarySearchTree::PreOrder() {
	PreOrderHelper(root);
}

/**
 * Insert a bid
 */
void BinarySearchTree::Insert(Bid bid) {
	if (root == nullptr) {
		root = new Node(bid);
	}
	else {
		this->addNode(root, bid);
	}
	return;
}

 /**
  * Remove a bid
  *
  * @param string Bid to be removed
  */
void BinarySearchTree::Remove(string bidId) {
	this->removeNode(root, bidId);
	cout << "Removed " + bidId << endl;
	return;
}

Node* BinarySearchTree::removeNode(Node* node, string bidId) { 
	if (node == nullptr) {
		return nullptr;
	}
	
	int comparisonResult = node->bid.bidId.compare(bidId); // target bidId compared to searched bidId
	const int matchFlag = 0; // Zero is a match, less is shorter/lower more is longer/higher
	//Node* parent = ParentSearch(node);

	if (comparisonResult < matchFlag) { // target is larger
		node->left = removeNode(node->left, bidId); // Recurse down and replace the removed node
	}
	else if (comparisonResult > matchFlag) { // searched is larger
		node->right = removeNode(node->right, bidId);
	}
	else {
		if (node->left == nullptr && node->right == nullptr) { // leaf case
			delete node;
			node = nullptr; // Keep the pointer safe
			return nullptr;
		}
		else if (node->left != nullptr && node->right == nullptr) { // Right is null case ("Case 4")
			Node* temp = node;
			node = node->left;
			delete temp;
		}
		else if (node->left == nullptr && node->right != nullptr) { // Left is null case ("Case 3")
			Node* temp = node;
			node = node->right;
		}
		else { // Two children case ("Case 1")
			Node* temp = node->right;
			while (temp->left != nullptr) {  // Traverse left
				temp = temp->left;
			}
			node->bid = temp->bid;
			node->right = removeNode(node->right, temp->bid.bidId); // Recursively remove successor since it was copied
		}
	}
	return node;
}

/**
 * Search for a bid
 * 
 * @param string bidId to be searched for
 */
Bid BinarySearchTree::Search(string bidId) {
	Node* currNode = root;
	const int matchFlag = 0; // Zero is a match, less is shorter/lower more is longer/higher

	while (currNode != nullptr) {
		int comparisonResult = currNode->bid.bidId.compare(bidId); // Compare the search bidId against the currently held bidId
		if (comparisonResult == matchFlag) { // Found the bid
			return currNode->bid; // Return the currently held bid that matched
		}
		else if (comparisonResult < matchFlag) { // The check is less than what was checked against
			currNode = currNode->left; // Shift loop left
		}
		else { // current > bidId checked against
			currNode = currNode->right; // Shift loop right
		} // Continue looping
	} // Current is nullptr, match is not found

	Bid bid;
	return bid;
}

/**
 * Search for a parent. FIXME: Not working on most cases OR create pointer parent
 *
 * @param Node* node to be searched for.
 */
Node* BinarySearchTree::ParentSearch(Node* child) {
	// Currently unused.
	Node* currNode = root;
	Node* parentNode = root;
	const int matchFlag = 0;

	while (currNode != nullptr) {
		int comparisonResult = currNode->bid.bidId.compare(parentNode->bid.bidId); // Compare the search bidId against the currently held parent bidId
		if (currNode == child) { // Found the child
			return parentNode; // Return the currently held parent that points to the child
		}
		else if (comparisonResult > matchFlag) { // The check is less than what was checked against
			parentNode = currNode; // Move the parent
			currNode = currNode->left; // Shift loop left
		}
		else { // current < bidId checked against
			parentNode = currNode; // Move the parent
			currNode = currNode->right; // Shift loop right
		} // Continue looping
	}
	cout << "\nParent() failed to find child" << endl; // Child not found, something deeply wrong has happened
	return nullptr;
}

/**
 * Add a bid to some node (recursive)
 *
 * @param node Current node in tree
 * @param bid Bid to be added
 */
void BinarySearchTree::addNode(Node* node, Bid bid) {
	
	const int matchFlag = 0; // Zero is a match
	int comparisonResult = node->bid.bidId.compare(bid.bidId);
	
	if (comparisonResult < matchFlag) { // Current node's bidID compared against bidID to be added
		if (node->left == nullptr) { // Left subtree, if the string contained here is longer than or has a greater character value and left is null
			node->left = new Node(bid); // Construct a new node and make it the left pointer of the current node
		}
		else { // Not null left
			addNode(node->left, bid); // Recurse leftward
		}
	}
	else { // Right subtree
		if (node->right == nullptr) { // Greater and right is null
			node->right = new Node(bid); // Node to be inserted becomes the right
		}
		else { // Not null right
			addNode(node->right, bid); // Recurse rightward
		}
	}
}

/**
 * InOrderHelper
 *
 * @param node Current node in tree
 */
void BinarySearchTree::InOrderHelper(Node* node) {
	if (node != nullptr) {
		InOrderHelper(node->right);
		cout << node->bid.bidId << ": " << node->bid.title << " | " << node->bid.amount << " | "
			<< node->bid.fund << endl;
		InOrderHelper(node->left);
	}
}

/**
 * postOrder
 *
 * @param node Current node in tree
 */
void BinarySearchTree::PostOrderHelper(Node* node) {
	  // FixMe (10): Pre order root
	  //if node is not equal to null ptr
	  //postOrder left
	  //postOrder right
	  //output bidID, title, amount, fund

}

/**
 * preOrder
 *
 * @param node Current node in tree
 */
void BinarySearchTree::PreOrderHelper(Node* node) {
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
		bidKey = "98009";
		break;
	case 3:
		csvPath = argv[1];
		bidKey = argv[2];
		break;
	default:
		csvPath = "eBid_Monthly_Sales.csv"; // _Dec_2016
		bidKey = "98009";
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
