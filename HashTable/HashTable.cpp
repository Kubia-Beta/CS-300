//============================================================================
// Name        : HashTable.cpp
// Author      : John Watson
// Co-Author   : Connor Sculthorpe
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Bid parsing in C++, Hash Table style
//============================================================================

#include <algorithm>
#include <climits>
#include <iostream>
#include <string> // atoi
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

const unsigned int DEFAULT_SIZE = 179;

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

//============================================================================
// Hash Table class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a hash table with chaining.
 */
class HashTable {

private:
	// Define structures to hold bids
	struct Node {
		Bid bid;
		unsigned int key;
		Node* next;

		// default constructor
		Node() {
			key = UINT_MAX; // 0xFFFFFFFF
			next = nullptr;
		}

		// initialize with a bid
		Node(Bid aBid) : Node() { // Calls the default constructor along with the new structure, minimalist design
			bid = aBid;
		}

		// initialize with a bid and a key
		Node(Bid aBid, unsigned int aKey) : Node(aBid) { // Calls the bid constructor along with the new structure
			key = aKey;
		}
	};

	vector<Node> nodes;

	unsigned int tableSize = DEFAULT_SIZE;

	unsigned int hash(int key);

public:
	HashTable();
	HashTable(unsigned int size);
	virtual ~HashTable();
	void Insert(Bid bid);
	void PrintAll();
	void Remove(string bidId);
	Bid Search(string bidId);
};

/**
 * Default constructor
 */
HashTable::HashTable() {
	nodes.resize(tableSize); // Initalize node structure by resizing tableSize
}

/**
 * Constructor for specifying size of the table
 * Use to improve efficiency of hashing algorithm
 * by reducing collisions without wasting memory.
 */
HashTable::HashTable(unsigned int size) {
	this->tableSize = size;// invoke local tableSize to size with this->
	nodes.resize(tableSize);// resize nodes size
}


/**
 * Destructor
 */
HashTable::~HashTable() {
	nodes.erase(nodes.begin());
}

/**
 * Calculate the hash value of a given key.
 * Note that key is specifically defined as
 * unsigned int to prevent undefined results
 * of a negative list index.
 *
 * @param key The key to hash
 * @return The calculated hash
 */
unsigned int HashTable::hash(int key) {
	return key % tableSize;
}

/**
 * Insert a bid
 *
 * @param bid The bid to insert
 */
void HashTable::Insert(Bid bid) {
	unsigned key = hash(atoi(bid.bidId.c_str())); // Bid ID is converted to string of characters from string object, ascii to integer is then called, the key is now finalized
	// Or put another way, we take the bidId, grab the c string, revert to int, call the hash for a key to have an index to the vector
	Node* node = &(nodes.at(key)); // Gets the address of the node at the key position
	if (node == nullptr) { // if no entry found for the key
		Node* newNode = new Node(bid, key); // assign this node to the key position
		nodes.insert(nodes.begin() + key, (*newNode)); // Insert into the vector from the offset with the address of the node
	}
	else if (node->key == UINT_MAX) { // else if node is not used but found
		node->key = key; // assing old node key to UNIT_MAX, set to key, set old node to bid and old node next to null pointer
		node->bid = bid;
		node->next = nullptr;
	}
	else { // else find the next open node
		while (node->next != nullptr) { // Iterates through, stops when the last node is reached (next is nullptr)
			node = node->next; // Make the next node the "current" node
		}
		node->next = new Node(bid, key); // add new newNode to end
	}
}

/**
 * Print all bids
 */
void HashTable::PrintAll() {
	// FIXME (6): Implement logic to print all bids
	// for node begin to end iterate
	//   if key not equal to UINT_MAx
			// output key, bidID, title, amount and fund
			// node is equal to next iter
			// while node not equal to nullptr
			   // output key, bidID, title, amount and fund
			   // node is equal to next node

}

/**
 * Remove a bid
 *
 * @param bidId The bid id to search for
 */
void HashTable::Remove(string bidId) {
	// FIXME (7): Implement logic to remove a bid
	// set key equal to hash atoi bidID cstring
	// erase node begin and key
}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid HashTable::Search(string bidId) {
	Bid bid;
	unsigned key = hash(atoi(bidId.c_str())); // calculate and create the key for the given bid
	Node* node = &(nodes.at(key));

	if (node != nullptr && node->key != UINT_MAX // if the entry exists AND is not unused
		&& node->bid.bidId.compare(bidId) == 0) { // AND is matching to the searched bidId
		return node->bid; //return node bid
	}
	else if (node == nullptr || node->key == UINT_MAX) { // if no entry found for the key OR it is unused
		return bid;
	}
	else {
		while (node != nullptr) { // while node not equal to nullptr
			if (node->key != UINT_MAX && node->bid.bidId.compare(bidId) == 0) {// if the current node matches, return it
				return node->bid;
			}
			node = node->next; // node is equal to next node
		}
		return bid; // if you fall out of while, return an empty bid
	}
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
void loadBids(string csvPath, HashTable* hashTable) {
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
			hashTable->Insert(bid);
		}
	}
	catch (csv::Error& e) {
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
 * Simple C function to clear the input buffer
 * when we are done reading through cin
 *
 * Credit:  https://stackoverflow.com/a/257091/
 */
void clearInputBuffer() {
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

	// process command line arguments
	string csvPath, searchValue;
	switch (argc) {
	case 2:
		csvPath = argv[1];
		searchValue = "98109";
		break;
	case 3:
		csvPath = argv[1];
		searchValue = argv[2];
		break;
	default:
		csvPath = "eBid_Monthly_Sales_Dec_2016.csv";
		searchValue = "98109";
	}

	// Define a timer variable
	clock_t ticks;

	// Define a hash table to hold all the bids
	HashTable* bidTable;

	Bid bid;
	bidTable = new HashTable();

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
		cout << endl; // By not passing endl, cout will have a line buffering issue.
		clearInputBuffer();

		switch (choice) {

		case 1:

			// Initialize a timer variable before loading bids
			ticks = clock();

			// Complete the method call to load the bids
			loadBids(csvPath, bidTable);

			// Calculate elapsed time and display result
			ticks = clock() - ticks; // current clock ticks minus starting clock ticks
			cout << "time: " << ticks << " clock ticks" << endl;
			cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
			break;

		case 2:
			bidTable->PrintAll();
			break;

		case 3:
			ticks = clock();

			bid = bidTable->Search(searchValue);

			ticks = clock() - ticks; // current clock ticks minus starting clock ticks

			if (!bid.bidId.empty()) {
				displayBid(bid);
			}
			else {
				cout << "Bid Id " << searchValue << " not found." << endl;
			}

			cout << "time: " << ticks << " clock ticks" << endl;
			cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
			break;

		case 4:
			bidTable->Remove(searchValue);
			break;
		}
	}

	cout << "Good bye." << endl;

	return 0;
}
