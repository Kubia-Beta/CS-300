#include <algorithm>
#include <climits>
#include <iostream>
//#include <string> // atoi
#include <time.h>
#include <fstream>

using namespace std;


struct Course { // Structure to hold course information
	int courseNumber;
	string courseTitle;
	vector<string> coursePrerequisites;
	Course() {
		courseNumber = INT_MAX; // INT_MAX for debugging
	}
}

//FIXME: lacks detailed collision management strategies
//Incorporating clear processes for dealing with hash key collisions, through methods such as chaining or linear probing
class HashTable {
private:
	const unsigned int DEFAULT_SIZE = 97; // Largest 2 digit prime as a default constant

	struct Node {
		Course course
			unsigned int key
			Node* next; // Double link?
		Node* prev;

		Node() { // Default constructor
			key = UINT_MAX; // 0xFFFFFFFF
			next = nullptr;
			prev = nullptr;
		}

		Node(Course aCourse) : Node() { // Call the default constructor when calling the constructor
			course = aCourse;
		}

		Node(Course Course, unsigned int aKey) : Node(Course Course) { // Continue calling previous constructors
			key = aKey
		}
	}

	vector<Node> nodes;
	unsigned int tableSize = DEFAULT_SIZE;
	unsigned int Hash(int aKey);

public:
	HashTable();
	HashTable(unsigned int size);
	~HashTable();
	bool IsEmpty() const;
	void InsertItem(Course course, string item);
	void RemoveItem(Course course);
	Course SearchHash(Course course);
	void PrintHash();
}

HashTable::HashTable() {
	nodes.resize(tableSize);
}

HashTable::HashTable(unsigned int size) {
	this->tableSize = size
		nodes.resize(tableSize)
}

HashTable::~HashTable() {
	// Node vector will deallocate itself when called here
}

int HashTable::Hash(int aKey) {
	return aKey % tableSize
}

bool HashTable::IsEmpty() {
	for (i < tableSize) { // Loop through the whole table
		int sum = table.size();
	}
	if (the sum == 0) {
		return true;
	else {
		return false;
	}
	}

	void HashTable::InsertItem(Course course, string item) {
		unsigned key = hash(course.courseNumber);
		Node* node = &(nodes.at(key));
		if (nullptr) {
			newNode = new Node(course, key)
		}
		else if (unused and found) {
			node->key, course equal to name of variable pointed to
				next-> = nullptr
		}
		else {
			while (we are not at last node) {
				node = node->next // Make the next node the new current
			}
			node->next = new Node(course, key) // Add on the new node to the end
		}
	}

	void HashTable::RemoveItem(Course course) { // Or would you pass courseNumber?
		key = hash(course.courseNumber)
			nodes.erase(nodes.begin() + key) // This structure worked in the hash table from before, double check though
	}

	Course HashTable::SearchHash(int aKey) {
		Course course
			Node* node as address of node inside bucket
			key = aKey

			while (node != nullptr) {
				if (node->key != UINT_MAX && node->course.courseNumber == courseNumber) {
					return node->course;
				}
				node = node->next
			}
	}

	void HashTable::PrintHash() {
		for (size of nodes) {
			while (current node is not null) {
				if (current is not MAX flag) {
					cout << course information from key
				}
				current = current->next
			}
		}
	}


	// Code based on ony of my own public repositories, Hold_to_Craft
	// https://github.com/Kubia-Beta/Hold_to_Craft
	void verifyFile(string & filename) {
		string inLine = "" // Now unused comparison string? Keep for now for version control?
			string line = ""
			ifstream infile(filename)
			regex fileFormatting("C.*\d+,\s*[A-Za-z].*") // Catches C, then any characeters, ends in a digit, comma, any letter
			try {
			if (!infile) { // could not open, failbit
				cerr << "error on file opening, make sure the program hass access to the directory" // Prints an error to the console
					throw runtime_error("File Access: Please verify the input file exists and is not protected")
			}
			else if (file.good == false) { // Other file operating issue
				throw runtime_error("Unknown File Access error")
			}
			while (getline(infile, line)) { // While reading line by line
				int i = 1
					if (line.find("CSC" || "Intro" || "100") != string::npos) { // Do these common contents exist
						if (line.substr(line.find("CSC" || "Intro" || "100") + 1).empty()) { // If it does exist, does anything exist after it
							cerr << "Error in file formatting" // If not, we have a major problem
								throw runtime_error("File Format: Error in file formatting")
						}
					}
					else if (regex_match(line, fileFormatting) == false) { // File does not have the correct ordering of structure
						cerr << "Error in file structure at line: " + i // show expected format in error?
							throw runtime_error("Internal File Structure: error at line " + i)
					}
			} // To get to this line, it must now be verified
			infile.close() // We are done verifying, close the file
				return
		}
		catch () {
			infile.close()
				cerr << "Eror in file verification. Please check the console log."
				return
		}
	}

	// Open the file, read the data, parse each line
	void openAndReadFile(string file, Vector<Course> courses) {
		string filename = file
			string line = "";
		verifyFile(filename)

			ifstream infile(filename)

			while (getline(infile, line)) {
				istringstream iss(line)
					string token
					vector<string> tokens

					while (getline(iss, token, ',')) {
						pushback(token)
					}

				for (token size) {
					coursePrerequisites.pushback(tokens[i])
				}
				Course course(number, title, prerequisites)
					courses.pushback(course)
			}

		infile.close()
	}