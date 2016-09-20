/*
 Source.cpp
 Created on: 20 Feb 2014
 Author: ab7012
 Purpose: This code reads a CommandFile.txt and, depending on the instruction written in the txt file (see switch cases),
 calls different functions that operate on the data stored in a linked list (extracted from data input txt file).
 */

//Name: Alexandre Benoit
//Username: ab7012
//Email: alexandre.benoit12@imperial.ac.uk

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Declaration of our LinkedList structure
struct Node {
	int data;
	Node* next;
};

typedef Node* NodePtr;

// Function Declaration
void read_file(string file_nb, NodePtr &hdList);					// Reads the data file of specified number
void sort_list(NodePtr hdList);										// Applies the Bubble Sort algorithm to sort the elements in ascending order
void write_file(string file_nb, NodePtr hdList);					// Writes all the data to an output file of specified number
void insert_element(NodePtr &hdList, NodePtr insert);				// Inserts an elements in the structure
bool check_structured(NodePtr hdList);								// Checks if the list is structured or not
void delete_element(NodePtr &hdList, int tmp);						// Deletes an elements form the linked list
void greater_threshold(NodePtr hdList, int threshold, string file_nb); // Calculates the number of entries above a certain threshold
void maximum_value(NodePtr hdList, string file_nb);					// Return the maximum value of the data in the linked list
void interpolate_list(NodePtr hdList);								// Interpolates the linked list by adding an extra node between every adjacent nodes
void clean_up(NodePtr hdList);										// Used at the end of program to clear the linked list


int main(int argc, char* argv[]){	// argc and argv used to process command line arguments (nb or arguments & contents of arguments)

	char command;					// Command variable stores the next command to process
	string file_nb;
	NodePtr hdList = NULL;			// Initialise the linked list we will be using
	NodePtr insert;
	int tmp;

	cout << endl << "Starting Program..." << endl << endl;

	if (argc != 2){					// If there is no 2nd argument, warn user that there is missing the command file
		cout << "Missing command file!";
		return 0;
	}

	ifstream infile(argv[1]);		// Opens the command file containing the instructions
	while (infile >> command){		// Stores the next instruction to execute in command variable every while loop
		switch (command) {

			case 'r':	// Reads the data file of specified number
				cout << "Reading Input File...      ";
				infile >> file_nb;	// Gets file number
				read_file(file_nb, hdList);
				cout << "Success!" << endl;
				break;

			case 's':	// Applies the Bubble Sort algorithm to sort the elements in ascending order
				cout << "Sorting List...            ";
				sort_list(hdList);
				cout << "Success!" << endl;
				break;

			case 'w':	// Writes all the data to an output file of specified number
				cout << "Writing to Output File...  ";
				write_file(file_nb, hdList);
				cout << "Success!" << endl;
				break;

			case 'i':	// Inserts an elements in the structure
				cout << "Inserting new Element...   ";
				insert = new Node;	// Additional Node used to insert
				infile >> insert->data;
				insert_element(hdList, insert);
				cout << "Success!" << endl;
				break;

			case 'd':	// Deletes an elements form the linked list
				cout << "Deleting Element...        ";
				infile >> tmp;	// Gets the number to delete
				delete_element(hdList, tmp);
				cout << "Success!" << endl;
				break;

			case 'e':	// Calculates the number of entries above a certain threshold
				cout << "Finding nb of entries...   ";
				infile >> tmp;	// Gets threshold value
				greater_threshold(hdList, tmp, file_nb);
				cout << "Success!" << endl;
				break;

			case 'm':	// Return the maximum value of the data in the linked list
				cout << "Finding maximum value...   ";
				maximum_value(hdList, file_nb);
				cout << "Success!" << endl;
				break;

			case 'a':	// Interpolates the linked list by adding an extra node between every adjacent nodes
				cout << "Interpolates list...       ";
				interpolate_list(hdList);
				cout << "Success!" << endl;
				break;

			default:	// Returns 'Invalid Command' and terminates the program if invalid command
				cout << "Invalid Command!" << endl;
				return 0;
		}
	}
	clean_up(hdList);	// Deletes the list when the program terminates to free memory
	cout << endl << "Completed Successfully!"<< endl;
	return 0;
}

void read_file(string file_nb, NodePtr &hdList){

	ifstream infile(("data_" + file_nb + ".txt").c_str());	// Opens file of correct number
	NodePtr new_Node;

	while (!infile.fail()){
		new_Node = new Node;
		infile >> new_Node->data;	// Data input into node every while loop

		if (infile.fail()) {
			delete new_Node;		// If we reached the end of list delete the last one we won't use
		}
		else {
			new_Node->next = hdList;// If not, update the pointers in the Linked List of that specific node
			hdList = new_Node;
		}
	}
	infile.close();					// Close Input File
}

void sort_list(NodePtr hdList){

	if (hdList == NULL) {			// If list if empty, no action (can't sort 0 values)
		return;
	};

	NodePtr previous_node;
	NodePtr next_node;
	Node tmp;
	int modifs = 1;

	while (modifs != 0){			// Executes until no changes are made for a complete cycle of the list
		modifs = 0;
		previous_node = hdList;
		next_node = hdList->next;
		while (next_node != NULL){	// Until we reach the end of the list
			if (next_node->data < previous_node->data) {// If the previous node if larger than the next node then swap them
				tmp = *next_node;
				next_node->data = previous_node->data;	// Note: this can be done using POINTERS but longer
				previous_node->data = tmp.data;
				modifs++;
			}
			previous_node = next_node;	// Pointers are 'incremented' to shift through the list
			next_node = next_node->next;
		}
	}
}

void write_file(string file_nb, NodePtr hdList){

	ofstream outfile(("output_" + file_nb + ".txt").c_str(), ios::app); // Opens the output file
	while (hdList != NULL){			// While we haven't reached the end of the file...
		outfile << hdList->data << endl; // ...output data one by one each loop...
		hdList = hdList->next; 		// ...and update the pointer to go to next node
	}
	outfile.close();
}

void insert_element(NodePtr &hdList, NodePtr insert){

	insert->next = hdList; 			// We use another 'insert' node that we place into the list
	hdList = insert;

	// If the list was sorted beforehand, sort it again, if not, do not do anything
	if (check_structured(insert->next) == true){
		sort_list(hdList);
	}
}

bool check_structured(NodePtr hdList){

	if (hdList == NULL) { 			// If the list is empty, by default we consider it as empty
		return false;
	};

	while (hdList->next != NULL) {	// We go through the list to check if a 'next' value is smaller than the one before to prove it is not sorted
		if (hdList->next->data < hdList->data){
			return false;			// If we find one, directly return false (one is enough to say it is not sorted)
		}
		hdList = hdList->next;		// Update pointer to go through the list
	}
	return true;					// If we can't find an anomaly before the end of the list, it is sorted
}

void delete_element(NodePtr &hdList, int tmp){

	bool found = false;
	NodePtr previous_node = hdList;
	NodePtr current_node = hdList->next;

	if (hdList == NULL){ // Special Case: If list is empty
		return;
	}

	else if (hdList->data == tmp) { // Special Case: Deleting the first entry
		hdList = hdList->next;
		delete previous_node;
	}
	else {

		while ((current_node != NULL) && (!found)) { // Look for the element by comparing to tmp and delete when found
			if (current_node->data == tmp) {
				found = true;		// Toggle found to indicate we found it (we only need to delete the first apperance)
				previous_node->next = current_node->next;
				delete current_node;
			}
			else {					// Update pointers to go through the list
				previous_node = current_node;
				current_node = current_node->next;
			}
		}
	}
}

void greater_threshold(NodePtr hdList, int threshold, string file_nb){

	int count = 0;
	ofstream outfile(("output_" + file_nb + ".txt").c_str(), ios::app); // Open the output file

	while (hdList != NULL) { 			// Go through all the list
		if (threshold < hdList->data) { // If we find one greater than threshold...
			count++;					// ...increment counter
		}
		hdList = hdList->next;
	}
	outfile << "Number of elements with value greater than " << threshold << ": " << count << endl;
	outfile.close(); // Outputs the final counter value and closes the file
}

void maximum_value(NodePtr hdList, string file_nb){

	int max = hdList->data;
	ofstream outfile(("output_" + file_nb + ".txt").c_str(), ios::app); // Opens the output file

	while (hdList != NULL) {		// Until we go through all the list...
		if (max < hdList->data) max = hdList->data; // ...if we find a new max, update the value of max
		hdList = hdList->next;
	}
	outfile << "Maximum value: " << max << endl; // Output the max value
	outfile.close();				// Close file
}

void interpolate_list(NodePtr hdList) {

	NodePtr previous_node = hdList;
	NodePtr current_node = hdList->next;

	if ((hdList != NULL) && (hdList->next != NULL)) {
		while (current_node != NULL){ 		// Go through the whole list
			NodePtr new_node = new Node;
			new_node->data = (previous_node->data + current_node->data)/2; // Average of previous and current node
			new_node->next = current_node; 	// Insert the result between the two
			previous_node->next = new_node;
			previous_node = current_node; 	// Move to the next pair
			current_node = current_node->next;
		}
	}
}

void clean_up(NodePtr hdList) {

	NodePtr nowPtr;
	while (hdList != NULL) { 	// Go through the whole list
		nowPtr = hdList;		// One by one, delete the nodes
		hdList = hdList->next;
		delete nowPtr;
	}
}
