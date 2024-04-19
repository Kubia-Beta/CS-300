#include <algorithm>
#include <climits>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <time.h>
#include <vector>
#include <regex>
#include <climits>

using namespace std;


struct Course { // Structure to hold course information
	string courseNumber;
	string courseTitle;
	vector<string> coursePrerequisites;

	Course() {
		courseNumber = ""; 
		courseTitle = "";
	}
	Course(string number, string title, vector<string> prerequisites) {
		courseNumber = number;
		courseTitle = title;
		coursePrerequisites = prerequisites;
	}
};

//FIXME: lacks detailed collision management strategies
//Incorporating clear processes for dealing with hash key collisions, through methods such as chaining or linear probing
 /**
  * Time: O(n)
  * Space: O(n)
  */
class HashTable {
private:
	const unsigned int DEFAULT_SIZE = 97; // Largest 2 digit prime as a default constant

	struct Node {
		Course course;
		unsigned int key; // Hash key, obtained by hashing the course number after converting it to an integer through stoi
		Node* next;


		Node() { // Default constructor
			key = UINT_MAX; // 0xFFFFFFFF
			next = nullptr;
		}

		Node(Course aCourse) : Node() { // Call the default constructor when calling the constructor
			course = aCourse;
		}

		Node(Course aCourse, unsigned int aKey) : Node(aCourse) { // Continue calling previous constructors
			key = aKey;
		}
	};

	vector<Node> nodes;
	unsigned int tableSize = DEFAULT_SIZE;
	unsigned int Hash(unsigned int aKey);
	unsigned int Hash(string courseNumber);

public:
	HashTable();
	HashTable(unsigned int size);
	~HashTable();
	bool IsEmpty();
	void InsertItem(Course course);
	void RemoveItem(Course course);
	Course SearchHash(unsigned int aKey);
	void PrintHash();
	void CreateHashFromFile(string file);
	void SortHashByCourseNumber();
};



/**
 * Default constructor for the HashTable class.
 *
 * Time: O(1)
 * Space: O(1)
 */
HashTable::HashTable() {
	nodes.resize(tableSize);
	for (Node& node : nodes) { // Initialize each node in the vector
		node = Node();
	}
}

/**
 * Constructor for the HashTable class.
 *
 * Time: O(n)
 * Space: O(n)
 * @param unsigned int size: The size of the hash table.
 */
HashTable::HashTable(unsigned int size) {
	this->tableSize = size;
	// nodes.resize(tableSize); // Necessary?
}

/**
 * Destructor for the HashTable class.
 *
 * Time: O(n)
 * Space: O(1)
 */
HashTable::~HashTable() {
	for (Node& node : nodes) {
		Node* current = node.next;
		while (current) {
			Node* temp = current;
			current = current->next;
			delete temp;
		}
	}
}

/**
 * Hash function to calculate the index for a given key.
 *
 * Time: O(1)
 * Space: O(1)
 * @param unsigned int aKey: The key to be hashed.
 * @return unsigned int: The hash value.
 */
unsigned int HashTable::Hash(unsigned int aKey) {
	return aKey % tableSize;
}

/**
 * Hash function to calculate the index for a given key.
 *
 * Time: O(1)
 * Space: O(1)
 * @param string courseNumber: The coursenumber value to convert to a key to then hash.
 * @return unsigned int: The hash value.
 */
unsigned int HashTable::Hash(string courseNumber) {
	return stoi(courseNumber) % tableSize;
}

/**
 * Checks if the hash table is empty.
 *
 * Time: O(n)
 * Space: O(1)
 * @return bool: True if the hash table is empty, false otherwise.
 */
bool HashTable::IsEmpty() {
	for (const Node node : nodes) {
		if (node.key != UINT_MAX) {
			return false; // Non-empty slot found
		}
	}
	return true; // All slots are empty
}

/**
 * Inserts a new item into the hash table.
 *
 * Time: O(n)
 * Space: O(1)
 * @param Course course: The course to be inserted.
 */
void HashTable::InsertItem(Course course) {
	unsigned key = Hash(course.courseNumber); // Hash the course number
	Node* node = &(nodes.at(key)); // Get the first node at the hashed index

	if (node->key == UINT_MAX) { // Is empty
		node = new Node(course, key);
	}
	else {
		while (node->next != nullptr) { // Traverse to find the end
			node = node->next;
		}
		node->next = new Node(course, key); // Add a new node to the end
	}
}

/**
 * Removes an item from the hash table.
 *
 * Time: O(n)
 * Space: O(1)
 * @param Course course: The course to be removed.
 */
void HashTable::RemoveItem(Course course) {
	unsigned int key = Hash(course.courseNumber); // Hash the course number by converting it to an integer
	Node* node = &(nodes[key]);


	if (node->course.courseNumber == course.courseNumber) { // Check if the first node matches the course
		nodes[key] = *(node->next); // Remove the first node by updating the pointer
		delete node;
		return;
	}

	while (node->next != nullptr) { // Search for the course to remove
		if (node->next->course.courseNumber == course.courseNumber) {
			Node* temp = node->next;
			node->next = temp->next;
			delete temp;
			return;
		}
		node = node->next;
	}
}

/**
 * Searches for a course in the hash table.
 *
 * Time: O(n)
 * Space: O(1)
 * @param int aKey: The key of the course to be searched.
 * @return Course*: Pointer to the found course, or nullptr if not found.
 */
Course HashTable::SearchHash(unsigned int aKey) {
	unsigned int key = Hash(aKey); // Hash the key, remember a key is just the stoi of a course number
	Node* node = &(nodes[key]); // Get the first node at the hashed index

	while (node != nullptr) { // Traverse the linked list
		if (node->key != UINT_MAX && stoi(node->course.courseNumber) == aKey) { // Check if the course number matches
			return node->course; // Course found
		}
		node = node->next; // Move to the next node
	}
	return; // Course not found
}

/**
 * Prints the contents of the hash table.
 *
 * Time: O(n)
 * Space: O(n)
 */
void HashTable::PrintHash() {
	for (const Node& node : nodes) {
		Node* current = node.next;
		while (current != nullptr) {
			cout << "Key: " << current->key << ", Course: " << current->course.courseNumber << endl;
			current = current->next;
		}
	}
}

/**
  * Opens a file, reads its data, and parses each line and adds it to the Hash Table
  *
  * Time: O(n)
  * Space: O(n)
  * @param string filename to be opened
  */
void HashTable::CreateHashFromFile(string file) {
	string line = ""; // Holds each line read from file
	verifyFile(file);

	// Initialize our variables for holding course information
	vector<string> prerequisites;
	string number;
	string title;

	ifstream infile(file); // Open the file

	while (getline(infile, line)) { // Read each line from the file
		istringstream iss(line);
		string token;
		int i = 0;

		while (getline(iss, token, ',')) { // Parse the line by commas, token holds the values
			i++; // Increase the line count by 1

			if (i > 2) { // Add the prerequisites to the vector after the first two commas
				prerequisites.push_back(token);
			}
			else if (i == 1) { // The first line ex. "CSCI100"
				number = token;
			}
			else if (i == 2) { // The second line ex. "Introduction to Computer Science"
				title = token;
			}
		} // The line has been fully read
		Course course(number, title, prerequisites); // Create a course object with the parsed data
		InsertItem(course); // FIXME: Insert for Hash Table
	} // Continues looping until all lines have been parsed into course in BST



	infile.close(); // Close the file
}


/**
  * Sorts the hash in alphanumeric order, lowest to highest
  *
  * Time: O(N log N)
  * Space: O(n)
  * Reference: https://en.cppreference.com/w/cpp/algorithm/sort
  */
void HashTable::SortHashByCourseNumber() {
	vector<Course> tempCourses; // Create a temp vector to hold the course information while we sort it
	for (const Node& node : nodes) { // Extract all courses from the hash table
		Node* current = node.next; // Start at the first node
		while (current) { // Traverse the linked list
			tempCourses.push_back(current->course); // Add the course to the vector
			current = current->next; // Move to the next node
		}
	}

	// Sort the extracted courses by course number using a lambda function
	sort(tempCourses.begin(), tempCourses.end(), [](const Course& a, const Course& b) {
		return std::stoi(a.courseNumber) < std::stoi(b.courseNumber);
	});

	nodes.clear(); // Clear it now that we have a sorted vector to copy into it
	for (const Course& course : tempCourses) { // Reinsert sorted courses into the hash table
		InsertItem(course);
	}
}


//============================================================================
// Static methods
//============================================================================


 /**
  * Verifies file and data integrity and validity of a course document. Returns specific errors about different file format issues.
  *
  * Time: O(n), where n is the number of lines in the file
  * Space: O(k), where k is the largest line in the file
  * @param string filename to be searched and verified
  *
  * Code based on ony of my own public repositories, Hold_to_Craft
  * https://github.com/Kubia-Beta/Hold_to_Craft
  */
void verifyFile(string& filename) {
	string line = ""; // Comparison string, starts as empty
	ifstream infile(filename) // Open the file
		regex fileFormatting("C.*\d+,\s*[A-Za-z].*") // REGEX: Catches C, then any characeters, then a digit, a comma, and then any letter
		try {
		if (!infile) { // could not open, failbit. We may not have access, or the file may not exist.
			throw runtime_error("File Access: Please verify the input file " + filename // Output what file went wrong with the error
				+ " exists and is not protected.");
		}
		else if (file.good == false) { // Other file operating issue
			throw runtime_error("Unknown File Access error on file " + filename + ".");
		}
		while (getline(infile, line)) { // While reading line by line
			int currLine = 1 // keep track of which line may throw an error for debugging

				if (line.find("CSC" != string::npos) || // Checks for common contents and compares them to "no position", throws error if they do not exist
					(line.find("Intro" != string::npos) || // If all three equal npos, then none were found
						(line.find("100" != string::npos) || { // In short, we check for these three markers for valid formatting line by line
					throw runtime_error("File Format: Error at line: " + currLine // Add the error line in the document to the error
						+ "Class names are of an incorrect format."
						+ " Expected type: CSCI100.");
					// Removed the extra check for being unnecessary algorithmically
				}
			// Since the substrings exist, check if they are in the expected order:
				else if (regex_match(line, fileFormatting) == false) { // Uses a regex to find any class, if we do not find it:
					throw runtime_error("Internal File Structure: error at line " + currLine // Add the error line in the document to the error
						+ ". Class naming convention is incorrect and could not be read."
						+ "Expected type: CSCI100,Introduction to Computer Science");
				}

			++currLine; // Increase the line counter each time while() finishes a loop
		} // To get to this line, the file must now be verified
		infile.close() // We are done verifying, close the file
			return
	}
	catch () { // Something went wrong
		infile.close()
			cerr << "Error in file verification. Please check the console log."
			return
	}
}


void DataStructureChoicePrinter(int& curLoaded, string& file) {
	take choice from main
		take file from main

		if (curLoaded is 1) {
			call vector print
		}
	if (curLoaded is 2) {
		call hash table print
	}
	if (curLoaded is 3) {
		call InOrder()
	}
	return;
}

void DataStructureChoiceFinder(int& curLoaded) {
	take curLoaded from main
		int searchKey = 100

		if (curLoaded is 1) {
			call vector search
		}
	if (curLoaded is 2) {
		call hash table search
	}
	if (curLoaded is 3) {
		call Search(searchKey)

			return;
	}
}

void DataStructureChoiceRemover(int& curLoaded) {
	take curLoaded from main

		if (curLoaded is 1) {
			call vector remove
		}
	if (curLoaded is 2) {
		call hash table remove
	}
	if (curLoaded is 3) {
		call Remove(removeKey)

			return;
	}
}

int main(int argc, char* argv[]) {
	string filename = "courses.txt";
	int choice = 0;
	int curLoaded = 0;
	clock_t ticks;
	
	while (choice != 9) {
		cout << "Menu:" << endl;
		cout << "1. Load Vector" << endl;
		cout << "2. Load Hash Table" << endl;
		cout << "3. Load Tree" << endl;
		cout << "4. Print loaded data" << endl;
		cout << "5. Find selected data" << endl;
		cout << "6. Remove selected data" << endl;
		cout << "9. Exit" << endl;
		cin >> choice;

		switch (choice) {

		case 1: // Load vector
			CreateVectorFromFile(file); // Load the vector
			SortVectorByCourseNumber();
			curLoaded = 1; // Set flag for the vector as the loaded element
			break;

		case 2: // Load Hash Table
			CreateHashFromFile(file);
			SortHashByCourseNumber();
			curLoaded = 2;
			break;

		case 3: // Load Binary Search Tree
			call CreateBSTFromFile(file);
			curLoaded = 3;
			break;

		case 4: // Print loaded data
			DataStructureChoicePrinter(curLoaded);
			break;
			
		case 5: // Find data
			DataStructureChoiceFinder(curLoaded);
			break;
			
		case 6: // Remove data
			DataStructureChoiceRemover(curLoaded);
			break;
		}
	}
	cout << "Good bye." << endl;

	return 0;
}