#include <algorithm>
#include <climits>
#include <iostream>
#include <string> // atoi
#include <time.h>
#include <fstream>
#include <vector>
#include <regex>
#include <climits>

using namespace std;


struct Course { // Structure to hold course information
	int courseNumber;
	string courseTitle;
	vector<string> coursePrerequisites;

	Course() {
		courseNumber = INT_MAX; // INT_MAX for debugging
		courseTitle = "";
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
		unsigned int key; // Hash key
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

public:
	HashTable();
	HashTable(unsigned int size);
	~HashTable();
	bool IsEmpty();
	void InsertItem(Course course, string item);
	void RemoveItem(Course course);
	Course SearchHash(Course course);
	void PrintHash();
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
	nodes.resize(tableSize);
}

/**
 * Destructor for the HashTable class.
 *
 * Time: O(n)
 * Space: O(1)
 */
HashTable::~HashTable() {
	// FIXME: Destructor
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
void HashTable::InsertItem(Course course, string item) {
	unsigned key = Hash(course.courseNumber);
	Node* node = &(nodes.at(key));

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
	unsigned int key = Hash(course.courseNumber);
	Node* node = &(nodes[key]);


	if (node->course == course) { // Check if the first node matches the course
		nodes[key] = *(node->next); // Remove the first node by updating the pointer
		delete node;
		return;
	}

	while (node->next != nullptr) { // Search for the course to remove
		if (node->next->course == course) {
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
Course HashTable::SearchHash(int aKey) {
	unsigned int key = Hash(aKey);
	Node* node = &(nodes[key]);

	while (node != nullptr) {
		if (node->key != UINT_MAX && node->course.courseNumber == aKey) {
			return &(node->course);
		}
		node = node->next;
	}
	return nullptr; // Course not found
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

	while (getline(file, line)) { // Read each line from the file
		istringstream iss(line);
		string token;
		int i = 0;

		while (getline(iss, token, ',')) { // Parse the line by commas, token holds the values
			i++ // Increase the line count by 1

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
		Insert(course); // FIXME: Insert for Hash Table
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
	for (const Node node : nodes) { // Traverse the nodes if they exist (collision)
		Node* current = &node;
		while (current) {
			tempCourses.push_back(current->course);
			current = current->next;
		}
	}

	// Sort the extracted courses by course number using a lambda function
	sort(tempCourses.begin(), tempCourses.end(), [](const Course& a, const Course& b)) {
		return std::stoi(a.courseNumber) < std::stoi(b.courseNumber);
	});

	nodes.clear(); // Clear it now that we have a sorted vector to copy into it
	for (const Course& course : tempCourses) { // Reinsert sorted courses into the hash table
		InsertItem(course);
	}
}


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