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
void verifyFile(string &filename);

struct Course { // Structure to hold course information
	string courseNumber;
	string courseTitle;
	vector<string> coursePrerequisites;

	Course() {
		courseNumber = "";
		courseTitle = "";
	}
	Course(string number, string title) {
		courseNumber = number;
		courseTitle = title;
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
		unsigned int key; // Hash key, obtained by hashing the course number after converting it to an integer
		Node* next;


		Node() { // Default constructor
			key = UINT_MAX; // 0xFFFFFFFF
			next = nullptr;
		}

		Node(Course aCourse) : Node() { // Call the default constructor when calling the constructor
			course = aCourse;
		}

		Node(Course aCourse, int aKey) : Node(aCourse) { // Continue calling previous constructors
			key = aKey;
		}

		Node(Course aCourse, int aKey, Node* aNext) : Node(aCourse, aKey) { // Continue calling previous constructors
			next = aNext;
		}
	};

	vector<Node> nodes; // Vector to hold the hash table
	unsigned int tableSize = DEFAULT_SIZE;
	unsigned int Hash(string courseNumber);
	int getSize() const { return nodes.size(); } // debug function

public:
	HashTable();
	HashTable(unsigned int size);
	~HashTable();
	bool IsEmpty();
	void InsertItem(Course course);
	void RemoveItem(Course course);
	void SearchHash(string courseNumber);
	void PrintHash();
	void PrintHashInOrder();
	void CreateHashFromFile(string &file);
};



/**
 * Default constructor for the HashTable class.
 *
 * Time: O(1)
 * Space: O(1)
 */
HashTable::HashTable() {
	nodes.resize(tableSize, Node()); // Resize the vector to the default size and initialize each node
	tableSize = nodes.size(); // Set the table size to the size of the vector
	for (Node& node : nodes) { // Initialize each node in the vector
		node = Node();
	}
}

/**
 * Constructor for the HashTable class.
 *
 * Time: O(n)
 * Space: O(n)
 * @param int size: The size of the hash table.
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
 * @param string courseNumber: The coursenumber value to convert then hash.
 * @return int: The hash value.
 */
unsigned int HashTable::Hash(string courseNumber) {
	unsigned int hash = 0;
	for (char ch : courseNumber) {
		hash += static_cast<unsigned int>(ch); // Convert the character to an integer and add it to the hash
	}
	hash %= tableSize; // Modulo the hash to fit within the table size
	return hash;
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
	//cout << "insert step 1" << endl;
	if (key > nodes.size()) { // Check if the key fits within the vector
		nodes.resize(key + 1); // Resize vector
		//cout << "insert step 1.5" << endl;
	}
	//cout << "insert step 2" << endl;
	Node* node = &(nodes.at(key)); // Get the first node at the hashed index
	//cout << "insert step 3" << endl;
	if (node->key == UINT_MAX) { // Is empty
		*node = Node(course, key); // Add the course to the empty slot
		//cout << "\nCourse added to the hash table: " << course.courseNumber
			//<< ", Title: " << course.courseTitle << ", Key: " << key
			//<< ", Hash: " << Hash(course.courseNumber) << endl;
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
	int key = Hash(course.courseNumber); // Hash the course number by converting it to an integer
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
	// nodes.erase(nodes.begin() + key); // erase node begin and key
	return;
}

/**
 * Searches for a course in the hash table.
 *
 * Time: O(n)
 * Space: O(1)
 * @param int aKey: The key of the course to be searched.
 * @return Course*: Pointer to the found course, or nullptr if not found.
 */
void HashTable::SearchHash(string courseNumber) {
	unsigned int aKey = Hash(courseNumber); // Hash the course number
	Course aCourse; // Create a null course to return if the course is not found
	Node* node = &(nodes[aKey]); // Get the first node at the hashed index

	while (node != nullptr) { // Traverse the linked list
		if (node->key != UINT_MAX && courseNumber == node->course.courseNumber) { // Check if the course number matches
			cout << "\nCourse found.\n" << "Course title: " << node->course.courseTitle
				<< ", Course number: " << node->course.courseNumber << ", Course prerequisites: ";
			for (auto i : node->course.coursePrerequisites) {
				cout << i << ',';
			}
			cout << endl;
			return; // Course found
		}
		node = node->next; // Move to the next node
	}
	cout << "Course " << courseNumber <<" not found." << endl;
	return; // Course not found
}

/**
 * Prints the contents of the hash table.
 *
 * Time: O(n)
 * Space: O(n)
 */
void HashTable::PrintHash() {
	cout << "\nHash Table:" << endl;
	for (unsigned int i = 0; i < nodes.size(); ++i) {
		Node* current = &(nodes.at(i));
		while (current != nullptr) {
			if (current->key != UINT_MAX) {
				cout << "Course title: " << current->course.courseTitle
					<< ", Course number: " << current->course.courseNumber << ", Key: "
					<< current->key << ", Course prerequisites: ";
				for (auto i : current->course.coursePrerequisites) {
					cout << i << ',';
				}
				cout << endl;
			}
			current = current->next;
		}
	}
	return;
}

/**
 * Prints the contents of the hash table in order.
 * Reference: https://en.cppreference.com/w/cpp/algorithm/sort
 *
 * Time: O(n log n)
 * Space: O(n)
 */
void HashTable::PrintHashInOrder() {
	vector<Course> tempCourses; // Create a temp vector to hold the course information while we sort it
	for (const Node& node : nodes) { // Extract all courses from the hash table
		if (node.key != UINT_MAX) { // Check if the node is not empty
			tempCourses.push_back(node.course); // Add the course to the vector
		}
		Node* current = node.next; // Start at the first node
		while (current) { // Traverse the linked list
			tempCourses.push_back(current->course); // Add the course to the vector
			current = current->next; // Move to the next node
		}
	}

	// Sort the vector by course title alphanumerically
	sort(tempCourses.begin(), tempCourses.end(), [](const Course& a, const Course& b) {
		return a.courseTitle < b.courseTitle;
	});

	cout << "\nHash Table In Order:" << endl;
	// Print the vector
	for (const Course& course : tempCourses) { // Print each course in the vector
		cout << "Course title: " << course.courseTitle
			 << ", Course number: " << course.courseNumber << ", Course prerequisites: ";
		for (const string& prerequisite : course.coursePrerequisites) { // Print each prerequisite
			cout << prerequisite << ',';
		}
		cout << endl;
	}
}

/**
  * Opens a file, reads its data, and parses each line and adds it to the Hash Table
  *
  * Time: O(n)
  * Space: O(n)
  * @param string filename to be opened
  */
void HashTable::CreateHashFromFile(string& file) {
	string line = ""; // Holds each line read from file
	//cout << "create hash step 1" << endl;
	verifyFile(file); // Verify the file is in the correct format
	//cout << "create hash step 2" << endl;

	// Initialize our variables for holding course information
	vector<string> prerequisites;
	string number;
	string title;

	ifstream infile(file); // Open the file
	//cout << "create hash step 3" << endl;
	while (getline(infile, line)) { // Read each line from the file
		int csvCount = 0; // how many substrings we have already made into an object part
		stringstream ss(line); // create a string stream from the line

		// grab the input stream which is the current line from the file and break it into parts for a Course
		// inner loop is to work on each line (comlpexity m, m<51)
		// break the line into parts :
		while (ss.good()) { // while the stream is good
			string substr; // create a substring to hold the current part of the line
			getline(ss, substr, ','); // get the next part of the line, separated by commas
			if (csvCount == 0) { // the first loop trigger
				// Start by removing Byte Order Mark from the stream if it exists to clean the stream: https://stackoverflow.com/q/2223882
				if (line.size() >= 3 && line[0] == '\xEF' && line[1] == '\xBB' && line[2] == '\xBF') { // Byte Order Mark (BOM) marker parsing
					substr = substr.substr(3); // Remove the BOM from the line
				}
				number = substr; // assign our courseNumber
				//cout << "create hash step 3.1, number added " << number << endl;
			}
			else if (csvCount == 1) { // the second loop trigger
				title = substr; // assign our courseTitle
				//cout << "create hash step 3.2, title added " << title << endl;
			}
			else if (csvCount < 50) { // for all items after the first two comma separated values
				prerequisites.push_back(substr); // Add to the string vector for prerequisites
				//cout << "create hash step 3.3, prerequisites added " << substr << endl;
			}
			else { // error handling, exit loop, we are over 50 prerequisites in this loop
				ss.str(""); // clear the stringstream to exit the loop early, something is wrong
			}
			csvCount++; // increase the count of substrings we have made into an object part
		} // The line has been fully read
		//cout << "create hash step 3.4, end of line" << endl;
		Course course(number, title, prerequisites); // Create a course object with the parsed data
		//cout << "create hash step 3.41, course created" << endl;
		InsertItem(course); // FIXME: Send to function that discerns the data structure, not just hash table
		//cout << "create hash step 3.42, course inserted" << endl;
		prerequisites.clear(); // Clear the prerequisites vector for the next course
	}
	PrintHash(); // Print the hash table
	infile.close(); // Close the file
}


//============================================================================
// Menu methods
//============================================================================


class Menu {
private:
	unique_ptr<HashTable> courseTable; // unique pointer to a hash table
	// other data structures...

	void DataStructureReleaser(int& curLoaded);
	void DataStructureChoicePrinter(int& curLoaded);
	void DataStructureChoiceFinder(int& curLoaded);
	void DataStructureChoiceRemover(int& curLoaded);

public:
	void CreateCourseTable() {
		courseTable = std::make_unique<HashTable>(); // create a new CourseTable
	}

	void DeleteCourseTable() {
		courseTable.reset();  // automatically deletes the CourseTable
	}

	void InputMenu(string &filename);
	// similar methods for other data structures...
};

void Menu::InputMenu(string &filename) {
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
			DataStructureReleaser(curLoaded);
			//CreateVectorFromFile(file); // Load the vector
			//SortVectorByCourseNumber();
			curLoaded = 1; // Set flag for the vector as the loaded element
			break;

		case 2: // Load Hash Table
			DataStructureReleaser(curLoaded);
			HashTable* courseTable;
			courseTable = new HashTable();
			courseTable->CreateHashFromFile(filename);
			courseTable->PrintHashInOrder();
			courseTable->SearchHash("CSCI300");
			courseTable->SearchHash("CSCI900");
			curLoaded = 2;
			break;

		case 3: // Load Binary Search Tree
			DataStructureReleaser(curLoaded);
			//call CreateBSTFromFile(file);
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
	return;
}

void Menu::DataStructureChoicePrinter(int& curLoaded) {
	//take choice from main
	//	take file from main

	//	if (curLoaded is 1) {
	//		call vector print
	//	}
	//if (curLoaded is 2) {
	//	call hash table print
	//}
	//if (curLoaded is 3) {
	//	call InOrder()
	return;
}

void Menu::DataStructureChoiceFinder(int& curLoaded) {
	//take curLoaded from main
	//	int searchKey = 100

	//	if (curLoaded is 1) {
	//		call vector search
	//	}
	//if (curLoaded is 2) {
	//	call hash table search
	//}
	//if (curLoaded is 3) {
	//	call Search(searchKey)

	//		return;
	//}
}

void Menu::DataStructureChoiceRemover(int& curLoaded) {
	//take curLoaded from main

	//	if (curLoaded is 1) {
	//		call vector remove
	//	}
	//if (curLoaded is 2) {
	//	call hash table remove
	//}
	//if (curLoaded is 3) {
	//	call Remove(removeKey)

	//		return;
	//}
}

void Menu::DataStructureReleaser(int& curLoaded) {
	if (curLoaded == 1) {
		//call vector release
	}
	if (curLoaded == 2) {
		//call hash table release
	}
	if (curLoaded == 3) {
		//call Release()
	}
	return;
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
void verifyFile(string &filename) {
	string line = ""; // Comparison string, starts as empty
	ifstream infile(filename); // Open the file
	regex fileFormatting("^[cmCM].*\\d+,\\s*[A-Za-z].*$"); // REGEX: Catches C/c/m/M, then any characeters, then a digit, a comma, and then any letter
	try { // Try to open the file
		if (!infile) { // could not open, failbit. We may not have access, or the file may not exist.
			cerr << "Error A.\n";
			throw runtime_error("File Access: Please verify the input file " + filename // Output what file went wrong with the error
			+ " exists and is not protected.");
		}
		else if (infile.good() == false) { // Other file operating issue
			throw runtime_error("Unknown File Access error on file " + filename + ".");
		}
		while (getline(infile, line)) { // While reading line by line
			int currLine = 0; // keep track of which line may throw an error for debugging

			if (line.size() >= 3 && line[0] == '\xEF' && line[1] == '\xBB' && line[2] == '\xBF') {
				line = line.substr(3); // If the line starts with the BOM, erase it
			} // BOM is a byte order mark, a special character that can be added to the start of a file, which we ignore
			else if (line.find("CSC") != string::npos && // Checks for common contents and compares them to "no position", throws error if they do not exist
				line.find("MAT") != string::npos && // If all three equal npos, then none were found
				line.find("100") != string::npos) { // In short, we check for these three markers for valid formatting line by line
				cerr << "Error B.\nLocated at " << currLine;
				throw runtime_error("File Format: Error at line: " + to_string(currLine) // Add the error line in the document to the error
					+ ". Class names are of an incorrect format."
					+ " Expected type: CSCI100.");
					// Removed the extra check for being unnecessary algorithmically
			}
			// Since the substrings exist, check if they are in the expected order:
			else if (regex_match(line, fileFormatting) == false) { // Uses a regex to find any class, if we do not find it:
				cerr << "Error C.\n";
				cerr << "Internal File Structure: error at line " << currLine // Add the error line in the document to the error
				<< ". Class naming convention is incorrect and could not be read. "
				<< "Expected type: CSCI100,Introduction to Computer Science. Got: "
				<< line << endl; // Add the error line in the document to the error
				throw runtime_error("Error C.");
			}
			else if (currLine > 50) {
				// If we have more than 50 prerequisites, we have an error
				cerr << "Error D.\n";
				cerr << "Internal File Structure: error at line " << currLine // Add the error line in the document to the error
				<< ". Too many prerequisites for a single course. "
				<< "Expected 50 or fewer prerequisites. Got: "
				<< line << endl; // Add the error line in the document to the error
				throw runtime_error("Error D.");
			}
			++currLine; // Increase the line counter each time while() finishes a loop
		} // To get to this line, the file must now be verified
		infile.close(); // We are done verifying, close the file
			return;
	}
	catch (const std::exception& error) { // Something went wrong
		infile.close();
		cerr << "Error in file verification. Please check the console log." << endl;
		return;
	}
}


//============================================================================
// Main
//============================================================================

int main(int argc, char* argv[]) {
	string filename = "ABCU_Advising_Program_Input.txt";
	clock_t ticks;

	Menu menu;
	menu.InputMenu(filename);
	return 0;
}