#include "RedBlackTree.h"
#include "TestRedBlackTree.h"
#include <iostream>
#include <string>
#include <vector>
//Tests for the important test cases.
bool testRedBlackTree() {
    RedBlackTree tree;

    std::cout << "=== Red-Black Tree Comprehensive Test ===" << std::endl;

    // Test cases to check:
    // 1. Root removal
    // 2. Black leaf node removal
    // 3. Red leaf node removal
    // 4. Node with one child removal
    // 5. Node with two children removal
    // 6. Multiple rotations and recoloring

    struct TestCase {
        std::string description;
        int valueToRemove;
        std::vector<int> valuesToAdd;
    };

    std::vector<TestCase> testCases = {
        // Case 1: Root removal - single node
        {
            "Root removal (single node)",
            10,
            {10}
        },

        // Case 2: Root removal with children
        {
            "Root removal with children",
            20,
            {20, 10, 30}
        },

        // Case 3: Red leaf node removal
        {
            "Red leaf node removal",
            40,
            {20, 10, 30, 40} // 40 should be a red leaf
        },

        // Case 4: Black leaf node removal
        {
            "Black leaf node removal",
            5,
            {20, 10, 30, 5, 15} // 5 should be a black leaf
        },

        // Case 5: Node with one red child
        {
            "Node with one red child removal",
            30,
            {20, 10, 30, 40} // 30 has one red child
        },

        // Case 6: Node with two children (complex)
        {
            "Node with two children removal",
            20,
            {20, 10, 30, 5, 15, 25, 40} // 20 has two children
        },

        // Case 7: Remove causing double-black and rebalancing
        {
            "Removal causing double-black",
            10,
            {20, 10, 30, 5, 25, 40} // 10 removal should cause rebalancing
        },

        // Case 8: Complex tree with multiple rotations needed
        {
            "Complex tree with multiple rotations",
            25,
            {50, 25, 75, 10, 30, 60, 90, 5, 15, 27, 40, 55, 70, 80, 100}
        }
    };

    bool allTestsPassed = true;

    for (const auto& test : testCases) {
        std::cout << "\n--- Testing: " << test.description << " ---" << std::endl;

        // Create a fresh tree for each test
        RedBlackTree* rbt = new RedBlackTree();

        // Insert all values
        std::cout << "Inserting values: ";
        for (int val : test.valuesToAdd) {
            std::cout << val << " ";
            rbt->insert(rbt->root, nullptr, val);
        }
        std::cout << std::endl;

        // Print the tree before removal
        std::cout << "Tree before removal:" << std::endl;
        rbt->print(rbt->root);

        // Validate the tree before removal
        std::cout << "Validating tree before removal:" << std::endl;
        rbt->checkTree();

        // Find and remove the target value
        std::cout << "Removing value: " << test.valueToRemove << std::endl;
        Node* nodeToRemove = RedBlackTree::getNode(rbt->root, test.valueToRemove);

        if (nodeToRemove == nullptr) {
            std::cout << "ERROR: Value " << test.valueToRemove << " not found in tree!" << std::endl;
            allTestsPassed = false;
        } else {
            rbt->remove(nodeToRemove);

            // Print the tree after removal
            std::cout << "Tree after removal:" << std::endl;
            rbt->print(rbt->root);

            // Validate the tree after removal
            std::cout << "Validating tree after removal:" << std::endl;
            rbt->checkTree();

            // Verify the node is actually removed
            if (RedBlackTree::getNode(rbt->root, test.valueToRemove) != nullptr) {
                std::cout << "ERROR: Value " << test.valueToRemove << " still exists in the tree after removal!" << std::endl;
                allTestsPassed = false;
            } else {
                std::cout << "Value " << test.valueToRemove << " successfully removed." << std::endl;
            }
        }

        delete rbt;
    }

    // Test random insertions and removals
    std::cout << "\n--- Testing random insertions and removals ---" << std::endl;
    RedBlackTree* randomTree = new RedBlackTree();
    std::vector<int> values = {44, 17, 88, 8, 32, 65, 97, 28, 54, 82, 93, 21, 29, 76, 80};

    for (int val : values) {
        randomTree->insert(randomTree->root, nullptr, val);
    }

    std::cout << "Initial random tree:" << std::endl;
    randomTree->print(randomTree->root);
    randomTree->checkTree();

    // Remove values in a different order
    std::vector<int> removeOrder = {44, 32, 88, 28, 97, 21, 8, 65, 93, 17, 82, 54, 76, 29, 80};

    for (int val : removeOrder) {
        std::cout << "\nRemoving " << val << ":" << std::endl;
        Node* nodeToRemove = RedBlackTree::getNode(randomTree->root, val);

        if (nodeToRemove != nullptr) {
            randomTree->remove(nodeToRemove);
            randomTree->print(randomTree->root);
            randomTree->checkTree();
        } else {
            std::cout << "ERROR: Value " << val << " not found in tree!" << std::endl;
            allTestsPassed = false;
        }
    }

    delete randomTree;

    if (allTestsPassed) {
        std::cout << "\n=== All Red-Black Tree tests PASSED! ===" << std::endl;
    } else {
        std::cout << "\n=== Some Red-Black Tree tests FAILED! ===" << std::endl;
    }

    return allTestsPassed;
}
