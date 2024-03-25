//============================================================================
// Name        : LinkedList.cpp
// Author      : Your Name
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Lab 3-3 Lists and Searching
//============================================================================

#include <algorithm>
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

//============================================================================
// Linked-List class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a linked-list.
 */
class LinkedList {

private:
	//Internal structure for list entries, housekeeping variables
	struct Node {
		Bid bid;
		Node *next;

		// default constructor
		Node() {
			next = nullptr;
		}

		// initialize with a bid
		Node(Bid aBid) {
			bid = aBid;
			next = nullptr;
		}
	};

	Node* head;
	Node* tail;
	int size = 0;

public:
	LinkedList();
	virtual ~LinkedList();
	void Append(Bid bid);
	void Prepend(Bid bid);
	void PrintList();
	void Remove(string bidId);
	Bid Search(string bidId);
	int Size();
};

/**
 * Default constructor
 */
LinkedList::LinkedList() {
	//set head and tail equal to null
	head = nullptr;
	tail = nullptr;
	size = 0;
}

/**
 * Destructor
 */
LinkedList::~LinkedList() {
	// start at the head
	Node* current = head;
	Node* temp;

	while (current != nullptr) { // loop over each node
		temp = current; // hang on to current node
		current = current->next; // make current the next node
		delete temp; // delete temp clutter
	}
}

/**
 * Append a new bid to the end of the list
 */
void LinkedList::Append(Bid bid) {
	Node* newNode = new Node(bid); // Create a new node with the given bid
	if (head == nullptr) { // If there is nothing at the head so the list is empty
		head = newNode;
		tail = newNode;
	} else { // If there is something at the head
			tail->next = newNode; // Make the current tail node point to the new node
			tail = newNode; // Make the tail the new node
	}
	size++; // Increase the size count
}

/**
 * Prepend a new bid to the start of the list
 */
void LinkedList::Prepend(Bid bid) {
	Node* newNode = new Node(bid); // Create a new node with the given bid

	if (head != nullptr) { // If the list is not empty
		newNode->next = head; // New node points to current head as its next node
	}

	head = newNode;
	size++;
}

/**
 * Simple output of all bids in the list
 */
void LinkedList::PrintList() {
	Node* current = head; // Start at the head

	while (current != nullptr) { // While loop over each node looking for a match
			cout << current->bid.bidId << ": " << current->bid.title << " | " << current->bid.amount << " | "
				<< current->bid.fund << endl; // Output the current bidID, title, amount, and fund
			current = current->next; // Set current equal to next
		}
}

/**
 * Remove a specified bid
 *
 * @param bidId The bid id to remove from the list
 */
void LinkedList::Remove(string bidId) {
	Node* current = head; // Start from the head node
	Node* previous = nullptr; // Set null until we obtain a previous node

	while (current != nullptr) { // Loop over each node in the list
		if (current->bid.bidId == bidId) { // If the current node bidID matches the given bidID
			if (previous == nullptr) { // Special case if matching node is the head
				head = current->next; // Make the head point to the next node in the list
			}
			else {
				previous->next = current->next; // Make the previous node point to the next node
			}

			delete current; // Now free up memory held by temp
			size--; // Decrease the size of the list
			return;
		}
		previous = current;
		current = current->next; // curret node is equal to next node
	}

	/* TODO: Evaluate the following comments and try matching their structure, rather than use mine which is derived from it.
	*  It's structure has the same base logic and will work the same as mine.
	*  This means it is a good learning opportunity to replicate more exactly.
	*/ 
	// special case if matching node is the head
		// make head point to the next node in the list
		//decrease size count
		//return

	// start at the head
	// while loop over each node looking for a match
		// if the next node bidID is equal to the current bidID
			// hold onto the next node temporarily
		 // make current node point beyond the next node
		 
		 // decrease size count // Good idea, but notice that we decrease the size count every time, so there is no need to do it seperately
		 //return

	//} // Why was this not commented out when distributed?

}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid LinkedList::Search(string bidId) {
	Node* current = head;

	while (current != nullptr) { // Loop over each node in the list
		if (current->bid.bidId == bidId) { // If the current node bidID matches the given bidID
			return current->bid; // Return the current bid information of the current node
		}
		current = current->next; // Move to the next node
	}
	return Bid(); // Return an empty bid if the bidID is not found
}

/**
 * Returns the current size (number of elements) in the list
 */
int LinkedList::Size() {
	return size;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
	cout << bid.bidId << ": " << bid.title << " | " << bid.amount
		 << " | " << bid.fund << endl;
	return;
}

/**
 * Prompt user for bid information
 *
 * @return Bid struct containing the bid info
 */
Bid getBid() {
	Bid bid;

	cout << "Enter Id: ";
	cin.ignore();
	getline(cin, bid.bidId);

	cout << "Enter title: ";
	getline(cin, bid.title);

	cout << "Enter fund: ";
	cin >> bid.fund;

	cout << "Enter amount: ";
	cin.ignore();
	string strAmount;
	getline(cin, strAmount);
	bid.amount = strToDouble(strAmount, '$');

	return bid;
}

/**
 * Load a CSV file containing bids into a LinkedList
 *
 * @return a LinkedList containing all the bids read
 */
void loadBids(string csvPath, LinkedList *list) {
	cout << "Loading CSV file " << csvPath << endl;

	// initialize the CSV Parser
	csv::Parser file = csv::Parser(csvPath);

	try {
		// loop to read rows of a CSV file
		for (int i = 0; i < file.rowCount(); i++) {

			// initialize a bid using data from current row (i)
			Bid bid;
			bid.bidId = file[i][1];
			bid.title = file[i][0];
			bid.fund = file[i][8];
			bid.amount = strToDouble(file[i][4], '$');

			//cout << bid.bidId << ": " << bid.title << " | " << bid.fund << " | " << bid.amount << endl;

			// add this bid to the end
			list->Append(bid);
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
 *
 * @param arg[1] path to CSV file to load from (optional)
 * @param arg[2] the bid Id to use when searching the list (optional)
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
		bidKey = "98233"; // 98109 is the default bid key, 98233 is the last in eBid_Monthly_Sales_Dec_2016.csv when unsorted
	}

	clock_t ticks;

	LinkedList bidList;

	Bid bid;

	int choice = 0;
	while (choice != 9) {
		cout << "Menu:" << endl;
		cout << "  1. Enter a Bid" << endl;
		cout << "  2. Load Bids" << endl;
		cout << "  3. Display All Bids" << endl;
		cout << "  4. Find Bid" << endl;
		cout << "  5. Remove Bid" << endl;
		cout << "  9. Exit" << endl;
		cout << "Enter choice: ";
		cin >> choice;
		cout << endl; // By not passing endl, cout will have a line buffering issue.
		clearInputBuffer();

		switch (choice) {
		case 1:
			bid = getBid();
			bidList.Append(bid);
			displayBid(bid);

			break;

		case 2:
			ticks = clock();

			loadBids(csvPath, &bidList);

			cout << bidList.Size() << " bids read" << endl;

			ticks = clock() - ticks; // current clock ticks minus starting clock ticks
			cout << "time: " << ticks << " milliseconds" << endl;
			cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

			break;

		case 3:
			bidList.PrintList();

			break;

		case 4:
			ticks = clock();

			bid = bidList.Search(bidKey);

			ticks = clock() - ticks; // current clock ticks minus starting clock ticks

			if (!bid.bidId.empty()) {
				displayBid(bid);
			} else {
				cout << "Bid Id " << bidKey << " not found." << endl;
			}

			cout << "time: " << ticks << " clock ticks" << endl;
			cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

			break;

		case 5:
			bidList.Remove(bidKey);

			break;
		}
	}

	cout << "Good bye." << endl;

	return 0;
}
