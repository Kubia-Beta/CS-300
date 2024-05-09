///////////////////////////
// By:                 ///
// Connor Sculthorpe  ///
// 21 April 2024     ///
///////////////////////

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <regex> // Regular expressions
#include <Windows.h> // Console color
#include <conio.h> // _getch

using namespace std;
void verifyFile(string &filename);

//============================================================================
// Course methods
//============================================================================
// Normally you would keep these in separate files, included with a header. It is included here to keep it to one file.
// Security worries make for strange constraints. ZIP's are an reasonable worry.

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


//============================================================================
// Vector methods
//============================================================================


class CourseVector {
	private:
	vector<Course> courses;

	public:
		void Insert(Course course);
		void Remove(string courseNumber);
		void Search(string courseNumber);
		void Print();
		void SortByCourseNumber();
};

/**
 * Insert a course into the vector
 *
 * Time: O()
 * Space: O()
 * @param Course course to be inserted
 */
void CourseVector::Insert(Course course) {
	courses.push_back(course);
	return;
}


/**
 * Remove a course from the vector
 * Reference: https://www.geeksforgeeks.org/std-find-in-cpp/
 *
 * Time: O()
 * Space: O()
 * @param string courseNumber to be removed
 */
void CourseVector::Remove(string courseNumber) {
	auto it = find_if(courses.begin(), courses.end(), [&](const Course& course) {
		return course.courseNumber == courseNumber;
		});
	if (it != courses.end()) {
		courses.erase(it);
		cout << "Removed " << courseNumber << endl;
	}
	else {
		cout << "Course " << courseNumber << " not found." << endl;
	}
	return;
}


/**
 * Search for a course in the vector
 *
 * Time: O()
 * Space: O()
 * @param string courseNumber to be searched for
 */
void CourseVector::Search(string courseNumber) {
	auto it = find_if(courses.begin(), courses.end(), [&](const Course& course) {
		return course.courseNumber == courseNumber;
		});
	if (it != courses.end()) {
		cout << "Course found." << endl;
		cout << "Course title: " << it->courseTitle
			<< ", Course number: " << it->courseNumber
			<< ", Course prerequisites: ";
		for (auto i : it->coursePrerequisites) {
			cout << i << ','; // Print each prerequisite
		}
		cout << endl;
	}
	else {
		cout << "Course " << courseNumber << " not found." << endl;
	}
}

/**
 * Print the contents of the vector
 *
 * Time: O()
 * Space: O()
 */
void CourseVector::Print() {
	for (Course course : courses) {
		cout << course.courseNumber << ": " << course.courseTitle << " | ";
		for (auto i : course.coursePrerequisites) {
			cout << i << ',';
		}
		cout << endl;
	}
	return;
}

/**
 * Sort the vector by course title alphanumerically
 *
 * Time: O(n log n)
 * Space: O(1)
 */
void CourseVector::SortByCourseNumber() {
	sort(courses.begin(), courses.end(), [](const Course& a, const Course& b) {
		return a.courseTitle < b.courseTitle;
	});
	return;
}


//============================================================================
// Binary Search Tree methods
//============================================================================



class BST {
	struct Node {
		Course course;
		Node* left;
		Node* right;
		int height; // Field to store AVL implementation height association

		Node() { // Default constructor
			left = nullptr;
			right = nullptr;
			height = 1; // Default height is 1
		}

		Node(Course aCourse) : Node() { // Call the default constructor when calling the constructor
			course = aCourse;
		}
	};
private:
	Node* root;
	void InsertItem(Node* node, Course aCourse);
	void InOrderHelper(Node* node);
	void PreOrderHelper(Node* node);
	void PostOrderHelper(Node* node);
	void DeconstructorHelper(Node* node);
	Node* removeNode(Node* node, string courseNumber);
	int TreeSize;
	void vectorPrinter(vector<string> Vector);

public:
	BST();
	virtual ~BST();
	void InOrder();
	void PostOrder();
	void PreOrder();
	void Insert(Course aCourse);
	void Remove(string courseNumber);
	int Size();
	void Search(string courseNumber);
};

/**
 * Adds a node with course to the tree
 *
 * Time: O(log n)
 * Space: O(n)
 * @param Node* node to be referenced from
 * @param Course aCourse to be added
 */
void BST::InsertItem(Node* node, Course aCourse) {
	if (node->course.courseTitle > aCourse.courseTitle) { // If the existing course number is greater than the one to be inserted
		if (node->left == nullptr) { // Left subtree has no lesser node
			node->left = new Node(aCourse); // Construct a new node and make it the left pointer of the current node
			this->TreeSize = this->TreeSize + 1; // Tree size has increased by one node
		}
		else { // Left not nullptr, exists
			InsertItem(node->left, aCourse); // Recurse leftward
		}
	}
	else { // Existing course number is less than the one to be inserted
		if (node->right == nullptr) { // Right subtree has no lesser node
			node->right = new Node(aCourse); // Construct a new node and make it the right pointer of the current node
			this->TreeSize = this->TreeSize + 1; // Tree size has increased by one node
		}
		else { // Right nmot nullptr, exists
			InsertItem(node->right, aCourse); // Recurse rightward
		}
	}
}

/**
 * Passes root to the helper function to recursively traverse left, visit root, traverse right
 *
 * Time: O(n^2), θ(n log n), Ω(1)
 * Space: O(n)
 * @param Node* node to be displayed
 */
void BST::InOrder() {
	cout << "\nBinary Search Tree In Order:" << endl;
	this->InOrderHelper(root);
}

/**
 * Recursively traverse left, visit root, traverse right
 *
 * Time: O(n^2), θ(n log n), Ω(1)
 * Space: O(n)
 * @param Node* node to be displayed
 */
void BST::InOrderHelper(Node* node) {
	if (node != nullptr) {
		InOrderHelper(node->left); // Recurse leftward to print all items before
		cout << node->course.courseNumber << ": " << node->course.courseTitle << " | ";
		vectorPrinter(node->course.coursePrerequisites);
		cout << endl;
		InOrderHelper(node->right); // Now recurse rightward now that we have printed all nodes to the left and the input node
	}
}

/**
 * Passes root to the helper function to recursively visit root, traverse left, traverse right
 * Time: O(n^2), θ(n log n), Ω(1)
 * Space: O(n)
 */
void BST::PreOrder() {
	cout << "\nBinary Search Tree Pre Order:" << endl;
	this->PreOrderHelper(root);
}

/**
 * Recursively visit root, traverse left, traverse right
 *
 * Time: O(n^2), θ(n log n), Ω(1)
 * Space: O(n)
 * @param Node* node to be displayed
 */
void BST::PreOrderHelper(Node* node) { //FIXME: Add code comments to explain the algorithm, then add to all Order type
	if (node != nullptr) {
		cout << node->course.courseNumber << ": " << node->course.courseTitle << " | ";
		vectorPrinter(node->course.coursePrerequisites);
		cout << endl;
		PreOrderHelper(node->left);
		PreOrderHelper(node->right);
	}
}

/**
 * Passes root to the helper function to recursively traverse left, traverse right, visit root
 * Time: O(n^2), θ(n log n)
 * Space: O(n)
 */
void BST::PostOrder() {
	cout << "\nBinary Search Tree Post Order:" << endl;
	this->PostOrderHelper(root);
}

/**
 * Recursively traverse left, traverse right, visit root
 *
 * Time: O(n^2), θ(n log n)
 * Space: O(n)
 * @param Node* node to be displayed
 */
void BST::PostOrderHelper(Node* node) {
	if (node != nullptr) {
		cout << node->course.courseNumber << ": " << node->course.courseTitle << " | ";
		vectorPrinter(node->course.coursePrerequisites);
		cout << endl;
		PostOrderHelper(node->left);
		PostOrderHelper(node->right);
	}
}

/**
 * Default constructor
 * Time: O(1)
 * Space: O(1)
 */
BST::BST() {
	this->root = nullptr;
	this->TreeSize = 0;
}

/**
 * Destructor
 * Time: O(n)
 * Space: O(n), θ(log n)
 */
BST::~BST() {
	DeconstructorHelper(this->root);
}

/**
 * Recursively deletes the nodes of a tree
 *
 * Time: O(n)
 * Space: O(n), θ(log n)
 * @param Node* node to be deleted
 */
void BST::DeconstructorHelper(Node* node) {
	if (node != nullptr) {
		DeconstructorHelper(node->left);
		DeconstructorHelper(node->right);
		delete node;
		node = nullptr;
		this->TreeSize = this->TreeSize - 1;
	}
}

/**
 * Insert a course
 *
 * Time: O(log n)
 * Space: O(n)
 * @param int Course to be inserted
 */
void BST::Insert(Course aCourse) {
	if (root == nullptr) { // Root does not exist
		root = new Node(aCourse); // Make the new node root
		this->TreeSize = this->TreeSize + 1;
	}
	else {
		InsertItem((this->root), aCourse); // Add by passing the root as reference since node was not specified
	}
}

/**
 * Recursively remove a course
 * Passes node information to removeNode(Node* node, int courseNumber).
 *
 * Time: O(n)
 * Space: O(n), θ(log n)
 * @param int courseNumber to be removed
 */
void BST::Remove(string courseNumber) {
	this->removeNode(root, courseNumber); // Calls the helper function to remove the node
	cout << "Removed " + courseNumber << endl; // Outputs a message indicating the successful removal
	this->TreeSize = this->TreeSize - 1; // Decrements the size of the tree
	return;
}


/**
 * Recursively remove a course from the tree
 *
 * Time: O(n)
 * Space: O(n), θ(log n)
 * @param Node* node to reference from
 * @param int Course to be removed
 */
BST::Node* BST::removeNode(Node* node, string courseNumber) {
	if (node == nullptr) { // If we have a blank for a node
		return nullptr; // Then do not try to remove it
	}

	if (node->course.courseNumber < courseNumber) { // If the existing course number is greater than the one to be removed
		node->left = removeNode(node->left, courseNumber); // Recurse down and replace the removed node
	}
	else if (node->course.courseNumber > courseNumber) { // Course for removal is larger, go right
		node->right = removeNode(node->right, courseNumber); // Recurse down and replace the removed node
	}
	else { // Match found
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
			node->course = temp->course;
			node->right = removeNode(node->right, temp->course.courseNumber); // Recursively remove successor since it was copied
		}
	}
	return node;
}

/**
 * Searches a course from the tree
 *
 * Time: O(n)
 * Space: O(n)
 * @param int Course to be searched for
 */
void BST::Search(string courseNumber) {
	Node* currNode = this->root;

	while (currNode != nullptr) {
		// Compare the search courseNumber against the currently held courseNumber
		if (currNode->course.courseNumber == courseNumber) { // Found the course
			cout << "Course found.\n" << "Course title: " << currNode->course.courseTitle
				<< ", Course number: " << currNode->course.courseNumber << ", Course prerequisites: ";
			vectorPrinter(currNode->course.coursePrerequisites);
			return;
			//return currNode->course; // Return the currently held course that matched
		}
		else if (currNode->course.courseNumber < courseNumber) { // The check is less than what was checked against
			currNode = currNode->left; // Shift loop left
		}
		else { // current > courseNumber checked against
			currNode = currNode->right; // Shift loop right
		} // Continue looping
	} // Current is nullptr, match is not found

	cout << "Course " << courseNumber << " not found." << endl;
	return;
}

/**
 * Get the size of the current Binary Search Tree
 * Time: O(1)
 * Space: O(1)
 */
int BST::Size() {
	return this->TreeSize;
}

void BST::vectorPrinter(vector<string> Vector) {
	for (auto i : Vector) {
		cout << i << ',';
	}
}


//============================================================================
// Hash Table methods
//============================================================================

class HashTable {
private:
	const unsigned int DEFAULT_SIZE = 97; // Largest 2 digit prime as a default constant

	struct htNode { // Structure to hold a course and a pointer to the next node
		Course course;
		unsigned int key; // Hash key, obtained by hashing the course number after converting it to an integer
		htNode* next;


		htNode() { // Default constructor
			key = UINT_MAX; // 0xFFFFFFFF
			next = nullptr;
		}

		htNode(Course aCourse) : htNode() { // Call the default constructor when calling the constructor
			course = aCourse;
		}

		htNode(Course aCourse, int aKey) : htNode(aCourse) { // Continue calling previous constructors
			key = aKey;
		}

		htNode(Course aCourse, int aKey, htNode* aNext) : htNode(aCourse, aKey) { // Continue calling previous constructors
			next = aNext;
		}
	};

	vector<htNode> nodes; // Vector to hold the hash table
	unsigned int tableSize = DEFAULT_SIZE;
	unsigned int Hash(string courseNumber);
	int getSize() const { return nodes.size(); } // debug function

public:
	HashTable();
	HashTable(unsigned int size);
	~HashTable();
	bool IsEmpty();
	void InsertItem(Course course);
	void RemoveItem(string courseNumber);
	void SearchHash(string courseNumber);
	void PrintHash();
	void PrintHashInOrder();
};



/**
 * Default constructor for the HashTable class.
 *
 * Time: O(1)
 * Space: O(1)
 */
HashTable::HashTable() {
	nodes.resize(tableSize, htNode()); // Resize the vector to the default size and initialize each node
	tableSize = nodes.size(); // Set the table size to the size of the vector
	for (htNode& node : nodes) { // Initialize each node in the vector
		node = htNode();
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
	for (htNode& node : nodes) {
		htNode* current = node.next;
		while (current) {
			htNode* temp = current;
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
	for (const htNode node : nodes) {
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
	htNode* node = &(nodes.at(key)); // Get the first node at the hashed index
	//cout << "insert step 3" << endl;
	if (node->key == UINT_MAX) { // Is empty
		*node = htNode(course, key); // Add the course to the empty slot
		//cout << "\nCourse added to the hash table: " << course.courseNumber
			//<< ", Title: " << course.courseTitle << ", Key: " << key
			//<< ", Hash: " << Hash(course.courseNumber) << endl;
	}
	else {
		while (node->next != nullptr) { // Traverse to find the end
			node = node->next;
		}
		node->next = new htNode(course, key); // Add a new node to the end
	}
}

/**
 * Removes an item from the hash table.
 *
 * Time: O(n)
 * Space: O(1)
 * @param Course course: The course to be removed.
 */
void HashTable::RemoveItem(string courseNumber) {
	int aKey = Hash(courseNumber); // Hash the course number by converting it to an integer
	htNode* node = &(nodes[aKey]); // Get the first node at the hashed index


	if (node->course.courseNumber == courseNumber) { // Check if the first node matches the course
		if (node->next == nullptr) { // If there is only one node
			nodes[aKey] = htNode(); // Clear the node
			cout << "Course " << courseNumber << " removed." << endl;
			return;
		}
		nodes[aKey] = *(node->next); // Remove the first node by updating the pointer
		delete node;
		nodes.erase(nodes.begin() + aKey); // erase node begin and key
		return;
	}

	while (node->next != nullptr) { // Search for the course to remove
		if (node->next->course.courseNumber == courseNumber) {
			htNode* temp = node->next;
			node->next = temp->next;
			delete temp;
			cout << "Course " << courseNumber << " removed from chain." << endl;
			return;
		}
		node = node->next;
	}
	cout << "Course " << courseNumber << " not found, "
		<< courseNumber << " was not removed." << endl;
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
	htNode* current = &(nodes[aKey]); // Get the first node at the hashed index

	while (current != nullptr) { // Traverse the linked list
		if (current->key != UINT_MAX && courseNumber == current->course.courseNumber) { // Check if the course number matches
			cout << "\nCourse found.\n" << current->course.courseTitle
				<< "  | " << current->course.courseNumber << " | ";
			for (auto i : current->course.coursePrerequisites) {
				cout << i << ',';
			}
			cout << endl;
			return; // Course found
		}
		current = current->next; // Move to the next node
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
	for (unsigned int i = 0; i < nodes.size(); ++i) { // Traverse
		htNode* current = &(nodes.at(i)); // Get the first node at the index
		while (current != nullptr) {
			if (current->key != UINT_MAX) { // Check if the node is not empty
				cout << current->course.courseNumber << ": " << current->course.courseTitle << " | ";
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
	for (const htNode& node : nodes) { // Extract all courses from the hash table
		if (node.key != UINT_MAX) { // Check if the node is not empty
			tempCourses.push_back(node.course); // Add the course to the vector
		}
		htNode* current = node.next; // Start at the first node
		while (current) { // Traverse the linked list
			tempCourses.push_back(current->course); // Add the course to the vector
			current = current->next; // Move to the next node
		}
	}

	// Sort the vector by course title alphanumerically
	sort(tempCourses.begin(), tempCourses.end(), [](const Course& a, const Course& b) {
		return a.courseTitle < b.courseTitle;
	});

	// Print the vector
	for (const Course& course : tempCourses) { // Print each course in the vector
		cout << course.courseNumber << ": " << course.courseTitle << " | ";
		for (const string& prerequisite : course.coursePrerequisites) { // Print each prerequisite
			cout << prerequisite << ',';
		}
		cout << endl;
	}
}


//============================================================================
// Menu methods
//============================================================================


class Menu {
private:
	unique_ptr<CourseVector> courseVector; // unique pointer to a vector
	unique_ptr<BST> courseBST; // unique pointer to a binary search tree
	unique_ptr<HashTable> courseTable; // unique pointer to a hash table

	// Menu functions:
	void DataStructureReleaser(int &curLoaded);
	void DataStructureChoicePrinter(int &curLoaded);
	void DataStructureChoiceFinder(int &curLoaded, string &searchTerm);
	void DataStructureChoiceRemover(int &curLoaded, string &removeTerm);
	void ConsoleColor(int color) { // Obsolete, Reference: https://stackoverflow.com/q/4053837/
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color); // Set the console text color
	}
	void MenuColorDefault() { // Obsolete
		ConsoleColor(7); // Set background color back to white/gray
	}
	void CreateFromFile(string &filename, int &currLoaded);
	void LoadIntoStructure(int &currLoaded, Course &course);
	void RelativeSpeeds(string &filename); // Loads every data structure with the same data and compares the speeds of various operations
	// Note: This function will load all the data multiple times and call every function multiple times, so it will 'spam'

	// Vector functions:
	void CreateVector() {
		courseVector = make_unique<CourseVector>(); // create a new vector
		cout << "Vector created." << endl;
	}
	void DeleteVector() {
		courseVector.reset(); // Deletes the vector
	}
	void PrintVector() {
		courseVector->SortByCourseNumber(); // Sort the vector by course number
		cout << "\nVector:\n"
			<< "Course Number: Course Title | Course Prerequisites, " << endl;
		courseVector->Print();
	}
	void SearchVector(string& courseNumber) {
		courseVector->Search(courseNumber);
	}
	void RemoveVector(string& courseNumber) {
		courseVector->Remove(courseNumber);
	}

	// Binary Search Tree functions:
	void CreateBST() {
		courseBST = make_unique<BST>(); // create a new BST
		cout << "Binary Search Tree created." << endl;
	}
	void DeleteBST() { // 
		courseBST.reset(); // Deletes the BST
	}
	clock_t PrintBST();
	void SearchBST(string &courseNumber) {
		courseBST->Search(courseNumber);
	}
	void RemoveBST(string &courseNumber) {
		courseBST->Remove(courseNumber);
	}

	// Hash table functions:
	void CreateCourseTable() {
		courseTable = make_unique<HashTable>(); // create a new CourseTable
		//CreateFromFile(filename, currLoaded); // load the CourseTable
		cout << "Hash table created." << endl;
	}
	void DeleteCourseTable() {
		courseTable.reset();  // Deletes the CourseTable
	}
	clock_t PrintCourseTable();
	void SearchCourseTable(string courseNumber) {
		courseTable->SearchHash(courseNumber);
	}
	void RemoveCourseTable(string courseNumber) {
		courseTable->RemoveItem(courseNumber);
	}

public:

	void InputMenu(string &filename); // Takes user input 1-9 from cin and calls the appropriate function

};

void Menu::InputMenu(string &filename) {
	int choice = 0;
	int curLoaded = 0;
	string chosenCourseNumber = "CSCI300";
	while (choice != 9) {
		cout << "Menu:" << endl;
		cout << "1. Load Vector" << endl;
		cout << "2. Load Hash Table" << endl;
		cout << "3. Load Tree" << endl;
		cout << "4. Print loaded data" << endl;
		cout << "5. Find selected data" << endl;
		cout << "6. Remove selected data" << endl;
		cout << "7. Display relative data structure speeds" << endl;
		cout << "8. Change input file and search/remove course number" << endl;
		cout << "9. Exit" << endl;
		cin >> choice;

		switch (choice) {

		case 1: // Load vector
			curLoaded = 1; // Set flag for the vector as the loaded element
			CreateFromFile(filename, curLoaded); // Load the vector
			courseVector->SortByCourseNumber(); // Sort the vector by course number
			break;

		case 2: // Load Hash Table
			DataStructureReleaser(curLoaded);
			curLoaded = 2; // Set flag for the hash table as the loaded element
			CreateFromFile(filename, curLoaded); // Load the hash table
			
			break;

		case 3: // Load Binary Search Tree
			DataStructureReleaser(curLoaded);
			curLoaded = 3; // Set flag for the binary search tree as the loaded element
			CreateFromFile(filename, curLoaded); // Load the binary search tree
			
			break;

		case 4: // Print loaded data
			DataStructureChoicePrinter(curLoaded);
			break;

		case 5: // Find data
			DataStructureChoiceFinder(curLoaded, chosenCourseNumber);
			break;

		case 6: // Remove data
			DataStructureChoiceRemover(curLoaded, chosenCourseNumber);
			break;
			//FIXME: Use menu 7/8 to cin chosenCourseNumber
		case 7: // Display relative data structure speeds
			RelativeSpeeds(filename);
			break;
		case 8:
			cout << "Input file name: " << endl;
			cin >> filename;
			cout << "Input search/remove course number: " << endl;
			cin >> chosenCourseNumber;
			break;
		case 0: // Change menu color
			// Note that cin is unformatted, you would need printf to print color
			ConsoleColor(14); // Set font color to tan
			cout << "\nSmall things like this remind me of my computer growing up."
				<< " Yet this function was added in 2015 to Windows 10!" << "\nSource: ";
			ConsoleColor(11); // Set the font color to blue for the link
			cout << "https://learn.microsoft.com/en-us/windows/console/ecosystem-roadmap" << endl;
			ConsoleColor(14); // Set font color back to tan
			cout << "It's the small joys that keep programming interesting, even when you're new like me." << endl;
			MenuColorDefault();
			break;
		}
		cout << endl; // Cosmetic line break
	}
	cout << "Good bye." << endl;
	return;
}


/**
 * Opens a file, reads its data, and parses each line and adds it to the Hash Table
 *
 * Time: O(n)
 * Space: O(n)
 * @param string filename to be opened
 */
void Menu::CreateFromFile(string &file, int &currLoaded) {
	DataStructureReleaser(currLoaded); // Release the current data structure
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
		LoadIntoStructure(currLoaded, course); // FIXME: Send to function that discerns the data structure, not just hash table
		//cout << "create hash step 3.42, course inserted" << endl;
		prerequisites.clear(); // Clear the prerequisites vector for the next course
	}
	infile.close(); // Close the file
}

/**
 * Load the course into the appropriate data structure
 *
 * Time: O(1)
 * Space: O(1)
 * @param int currLoaded: The currently loaded data structure, 1 for vector, 2 for hash table, 3 for binary search tree
 * @param Course course: The course to be loaded
 */
void Menu::LoadIntoStructure(int &currLoaded, Course &course) {
	if (currLoaded == 1) {
		if (courseVector != nullptr) { // Exists
					courseVector->Insert(course);
		}
		else { // Does not exist, create it first
					CreateVector();
					courseVector->Insert(course);
		}
	}
	if (currLoaded == 2) {
		if (courseTable != nullptr) { // Exists
			courseTable->InsertItem(course);
		}
		else { // Does not exist, create it first
			CreateCourseTable();
			courseTable->InsertItem(course);
		}
		
	}
	if (currLoaded == 3) {
		if (courseBST != nullptr) { // Exists
			courseBST->Insert(course);
		}
		else { // Does not exist, create it first
			CreateBST();
			courseBST->Insert(course);
		}
	}

}

/**
 * Print the data structure in all relevant ways
 *
 * Time: O(1)
 * Space: O(1)
 * @param int currLoaded: The currently loaded data structure, 1 for vector, 2 for hash table, 3 for binary search tree
 */
void Menu::DataStructureChoicePrinter(int& curLoaded) {
	if (curLoaded == 1) {
		PrintVector();
	}
	if (curLoaded == 2) {
		PrintCourseTable();
	}
	if (curLoaded == 3) {
		PrintBST();
	}
	return;
}

/**
 * Demonstrate the search function of the data structure
 * 
 * Time: O(1)
 * Space: O(1)
 * @param int currLoaded: The currently loaded data structure, 1 for vector, 2 for hash table, 3 for binary search tree
 * @param string searchTerm: The course number to be searched for
 */
void Menu::DataStructureChoiceFinder(int &curLoaded, string &searchTerm) {
	if (curLoaded == 1) {
		SearchVector(searchTerm);
	}
	if (curLoaded == 2) {
		SearchCourseTable(searchTerm);
	}
	if (curLoaded == 3) {
		SearchBST(searchTerm);
		return;
	}
}


/**
 * Demonstrate removal of a course from the data structure
 *
 * Time: O(1)
 * Space: O(1)
 * @param int currLoaded: The currently loaded data structure, 1 for vector, 2 for hash table, 3 for binary search tree
 * @param string searchTerm: The course number to be removed
 */
void Menu::DataStructureChoiceRemover(int &curLoaded, string &removeTerm) {
	if (curLoaded == 1) {
		RemoveVector(removeTerm);
	}
	if (curLoaded == 2) {
		RemoveCourseTable(removeTerm);
	}
	if (curLoaded == 3) {
		RemoveBST(removeTerm);
		return;
	}
}

/**
 * Free the memory of the data structure
 *
 * Time: O(1)
 * Space: O(1)
 * @param int currLoaded: The currently loaded data structure, 1 for vector, 2 for hash table, 3 for binary search tree
 */
void Menu::DataStructureReleaser(int& curLoaded) {
	if (curLoaded == 1) {
		DeleteVector();
	}
	if (curLoaded == 2) {
		DeleteCourseTable();
	}
	if (curLoaded == 3) {
		DeleteBST();
	}
	return;
}

clock_t Menu::PrintBST() {
	cout << "\nBinary Search Tree:\n"
		<< "Course Number: Course Title | Course Prerequisites, " << endl;
	clock_t ticks = clock();
	//courseBST->InOrderNonRecursive();
	courseBST->InOrder();
	ticks = clock() - ticks;
	courseBST->PreOrder();
	courseBST->PostOrder();
	return ticks;
}

/**
 * Print the hash table
 *
 * Time: O(n)
 * Space: O(n)
 */
clock_t Menu::PrintCourseTable() {
	clock_t ticks; // Vector to hold the clock ticks for each operation
	
	cout << "\nHash Table:\n"
		<< "Course Number: Course Title | Course Prerequisites, " << endl;
	courseTable->PrintHash();
	cout << "\nHash Table in order:\n"
		<< "Course Number: Course Title | Course Prerequisites, " << endl;
	ticks = clock();
	courseTable->PrintHashInOrder();
	ticks = clock() - ticks;
	return ticks;
}

/**
 * Demonstrate the relative speeds of the data structures.
 * Mnemoic: Sampo
 *
 * Time: O(n^2)
 * Space: O(n)
 */
void Menu::RelativeSpeeds(string &filename) {
	int currLoaded = 1;
	string useTerm = "CSCI300";
	vector<clock_t> VectorClock; // Vector to hold the clock ticks for each operation by Vector
	vector<clock_t> HashTableClock; // Vector to hold the clock ticks for each operation by Hash Table
	vector<clock_t> BSTClock; // Vector to hold the clock ticks for each operation by Binary Search Tree

	// Create
	// Search
	// Remove
	// Print

	// Vector:
	clock_t ticks = clock();
	CreateFromFile(filename, currLoaded); // Create
	ticks = clock() - ticks;
	VectorClock.push_back(ticks);
	ticks = clock();
	DataStructureChoiceFinder(currLoaded, useTerm); // Search
	ticks = clock() - ticks;
	VectorClock.push_back(ticks);
	ticks = clock();
	DataStructureChoiceRemover(currLoaded, useTerm); // Remove
	ticks = clock() - ticks;
	VectorClock.push_back(ticks);
	ticks = clock();
	DataStructureChoicePrinter(currLoaded); // Print
	ticks = clock() - ticks;
	VectorClock.push_back(ticks);
	

	// Hash Table:
	currLoaded = 2;
	ticks = clock();
	CreateFromFile(filename, currLoaded); // Create
	ticks = clock() - ticks;
	HashTableClock.push_back(ticks);
	ticks = clock();
	DataStructureChoiceFinder(currLoaded, useTerm); // Search
	ticks = clock() - ticks;
	HashTableClock.push_back(ticks);
	ticks = clock();
	DataStructureChoiceRemover(currLoaded, useTerm); // Remove
	ticks = clock() - ticks;
	HashTableClock.push_back(ticks);
	ticks = clock();
	int returnValue = PrintCourseTable(); // Print
	HashTableClock.push_back(returnValue);

	// Binary Search Tree:
	currLoaded = 3;
	ticks = clock();
	CreateFromFile(filename, currLoaded); // Create
	ticks = clock() - ticks;
	BSTClock.push_back(ticks);
	ticks = clock();
	DataStructureChoiceFinder(currLoaded, useTerm); // Search
	ticks = clock() - ticks;
	BSTClock.push_back(ticks);
	ticks = clock();
	DataStructureChoiceRemover(currLoaded, useTerm); // Remove
	ticks = clock() - ticks;
	BSTClock.push_back(ticks);
	ticks = clock();
	DataStructureChoicePrinter(currLoaded); // Print
	ticks = clock() - ticks;
	BSTClock.push_back(ticks);
	// Done with the operations
	
	// Print the results
	cout << "\n\nRelative speeds of data structures:" << endl;
	cout << "Vector: Create: " << VectorClock[0] << ", Search: "
		<< VectorClock[1] << ", Remove: " << VectorClock[2] << ", Print: " << VectorClock[3] << endl;
	cout << "Hash Table: Create: " << HashTableClock[0] << ", Search: "
		<< HashTableClock[1] << ", Remove: " << HashTableClock[2] << ", Print: " << HashTableClock[3] << endl;
	cout << "Binary Search Tree: Create: " << BSTClock[0] << ", Search: "
		<< BSTClock[1] << ", Remove: " << BSTClock[2] << ", Print: " << BSTClock[3] << endl;
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
	catch (const exception& error) { // Something went wrong
		infile.close();
		cerr << "Error in file verification. Please check the console log." << endl;
		return;
	}
}

/**
 * Halt the program until the user presses any key
 * (Obsolete)
 * 
 * Time: O(1)
 * Space: O(1)
 * Code based on ony of my own public repositories, Hold_to_Craft
 * https://github.com/Kubia-Beta/Hold_to_Craft
 */
void pressAnyKeyToContinue() { //just a simple halt for the user that doesn't enter anything into our buffer
	cout << "\nPress any key to continue...";
	(void)_getch(); //assigns _getch() to wait for a key press
	cout << endl;
}


//============================================================================
// Main
//============================================================================

int main(int argc, char* argv[]) {
	string filename = "ABCU_Advising_Program_Input.txt";
	Menu menu;
	menu.InputMenu(filename);
	return 0;
}
