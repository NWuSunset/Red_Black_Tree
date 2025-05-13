#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <limits>
#include "RedBlackTree.h"



using namespace std;
/*!
  @brief Handles user input
  @param rbt       the red black tree
  @returns         true if the user inputs the "quit" command
 */
bool userSelection(RedBlackTree * rbt);

/*!
  @brief Reads in numbers (separated by spaces) from a file and inserts them into binary tree
  @param rbt       the binary search tree
  @note can take in a file path or the name of a file in the local directory e.g. "C:\Users\476275\Downloads\numbers.txt", numbers.txt (if file is in the local program directory).
 */
void fromFile(RedBlackTree * rbt);

/*!
  @brief Reads in numbers (separated by spaces) from the console and inserts them into binary tree
  @param rbt       the red black tree
 */

void fromConsole(RedBlackTree * rbt);

int main() {
  RedBlackTree * rbt = new RedBlackTree();

  //rbt->insert(rbt->root, nullptr, 4);
  //rbt->insert(rbt->root, nullptr, 5);
  //rbt->insert(rbt->root, nullptr, 6);
  //rbt->insert(rbt->root, nullptr, 7);

  
  
  while (!userSelection(rbt)) {}
  
  return 0;
}

bool userSelection(RedBlackTree* rbt) {
  char userInput[12];
  cout << "Type CONSOLE to enter a series of numbers in the console. Or type FILE to enter a file name. Type PRINT to print out the tree. Type 'remove' to remove a number from the tree. Type SEARCH to search for a number in the tree." << endl;
  cin.getline(userInput, 12);
  
  //in case more than 12 characters are entered (so it won't break the program)
  if (cin.fail()) {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Input Error. Input is too long" << endl;
  }
  
  if (strcasecmp(userInput, "FILE") == 0) {
    fromFile(rbt);
  } else if (strcasecmp(userInput, "CONSOLE") == 0) {
    fromConsole(rbt);
  } else if (strcasecmp(userInput, "PRINT") == 0) {
    if (rbt->root == nullptr) cout << "Nothing in the tree" << endl;
    rbt->print(rbt->root);
  } else if (strcasecmp(userInput, "Remove") == 0) {
    int num;
    cout << "What number do you want to remove from the tree?" << endl;
    cin >> num;

    //get the node that matches user input
    cout << "testing if in tree" << endl;
    Node* n = rbt->getNode(rbt->root, num);
    if (n != nullptr) {
      rbt->remove(n);
    } else {
    cout << "Invalid number" << endl;
    }
    cin.ignore();
  } else if (strcasecmp(userInput, "SEARCH") == 0) {
    int num;
    cout << "What number do you want to search for in the tree?" << endl;
    cin >> num;
    if (rbt->getNode(rbt->root, num) != nullptr) {
      cout << "It is in the tree" << endl;
    } else {
      cout << "This number isn't in the tree" << endl;
    }
    cin.ignore();
  } else if (strcasecmp(userInput, "QUIT") == 0) {
    return true;
  } else {
    cout << "Invalid input." << endl;
  }
  return false;
}

//taking numbers from a file separated by spaces
void fromFile(RedBlackTree* rbt) {
  string filePath;
  cout << "Enter the path of the file you want to read from. Or type the filename if it's in the local directory" << endl;
  getline(cin, filePath);

  // Check if the filePath starts and ends with a double quote, and remove them if so.
  if (filePath.size() >= 2 && filePath.front() == '"' && filePath.back() == '"') {
    filePath = filePath.substr (1, filePath.size() - 2); //makes a substring that doesn't include the double quote
  }
  
  ifstream inputFile(filePath);

  if (inputFile.is_open()) {
    int num;
    cout << "Inserting numbers into the tree..." << endl;
    cout << "Adding numbers from file" << endl;
    while (inputFile >> num) {
      rbt->insert(rbt->root, nullptr, num);
      //      rbt->print(rbt->root);
      //cout << endl;
    }
    inputFile.close();
  } else {
    cout << "Cannot find file specified" << endl;
  }
}


void fromConsole(RedBlackTree * rbt) {
  string input;
  cout << "Enter numbers separated by spaces: " << endl;
  getline(cin, input);

  istringstream iss(input);
  //object to stream the string into different variables (since we input ints separated by spaces)
  int num;

  cout << "Inserting numbers..." << endl;
  //Stream a number till while space is encountered. Do this until end of string is hit
  while (iss >> num) {
    rbt->insert(rbt->root, nullptr, num); //num == rbt->root->data will give 1 if true (will go right)
  }
}
