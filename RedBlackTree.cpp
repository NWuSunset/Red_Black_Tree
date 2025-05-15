#include "RedBlackTree.h"
#include <iostream>

RedBlackTree::RedBlackTree() = default;


/*
 * Requirements:
 *  1 Every node is either red or black
 *  2 All null nodes  are considered black.
 *  3 A red node does not have a red child.
 *  4 Every path from a given node to any of its leaf nodes goes through the same number of black nodes.
 *  (Conclusion) If a node N has exactly one child, the child must be red, because if it were black, its leaves would sit at a different black depth than N's child, violating requirement 4.
 */


//https://en.wikipedia.org/wiki/Tree_rotation visualization
//node passed in is the root of the subtree (perhaps rename a later
//direction is the direction to rotate in
Node* RedBlackTree::rotateSubTree(Node* subRoot, const direction dir) {
    //Will rotate left or right.
    Node* parent = subRoot->parent; //save the original root parent

    //New root is now the subRoot's opposite child
    //(of the direction passed in, so if we want to rotate left this would be the right child)
    Node* newRoot = subRoot->child(1 - dir);

    //swap the dir child of the subRoot and newRoot (swap rotation direction child over (the 'inner' child))
    Node* newChild = newRoot->child(dir);
    subRoot->setChild(1 - dir, newChild);

    newRoot->setChild(dir, subRoot); //set subRoot as the newRoot's child

    //update parents
    if (newChild != nullptr) {
        //for newChild
        newChild->parent = subRoot;
    }

    newRoot->parent = parent; //for new root
    subRoot->parent = newRoot; //for original subRoot


    //update the children for parent
    if (parent != nullptr) {
        //if subRoot == parent->right, it evaluates to 1 which is the right direction (since right and left are basically booleans)
        parent->setChild(subRoot == parent->right, newRoot);
    } else {
        root = newRoot;
    }
    return newRoot;
}

//Gets the direction of the node passed in relative to it's parent
direction RedBlackTree::nodeDirection(const Node* node) {
    if (node == nullptr || node->parent == nullptr) {
        throw std::invalid_argument("Node direction error, node or parent is null when calling function");
    }

    if (node == node->parent->right) {
        return right;
    }
    return left;
}

//basically because nullptr counts as black, use this function
Color RedBlackTree::getColor(const Node* node) {
    if (node == nullptr) {
        return BLACK;
    }
    return node->color;
}

//u is the node to be swapped, and v is the node to swap with
void RedBlackTree::transplant(Node* u, Node* v) {
    if (u->parent == nullptr) {
        //if u is root
        root = v;
    } else {
        const direction dir = nodeDirection(u); //u's direction relative to parent
        u->parent->setChild(dir, v); //replace the parent's child u, with v
    }

    if (v != nullptr) {
        //update the parents
        v->parent = u->parent;
    }
}


//Finds the in order sucessor (leftmost child of right subtree). (like binary tree)
Node* RedBlackTree::tree_min(Node* node) {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}


//position initially is the root, prev is initially nullptr

void RedBlackTree::insert(Node* & pos, Node* prev, const int data, const direction dir) {
    if (pos == nullptr) {
        pos = new Node(data); //this will be the base case of the recursion (will insert once the path has ended)
        pos->parent = prev; //set parent node
        insertBalance(pos, dir); //do the actual insert
        return;
    }


    //if the number is already in the tree
    if (pos->data == data) {
        std::cout << "Already in the tree" << std::endl;
        return;
    }

    //if data being inserted is greater than node then go right
    if (pos->data < data) {
        insert(pos->right, pos, data, right);
    } else if (pos->data > data) {
        //If data being inserted is less than node go left
        insert(pos->left, pos, data, left);
    }
}

// Corrected method definitions
void RedBlackTree::insertBalance(Node* node, direction dir) {
    Node* parent = node->parent;

    if (parent == nullptr) {
        node->color = BLACK;
        root = node; //if there aren't any other nodes in the tree then this node is root
        return;
    }


    Node* grandparent = nullptr;
    Node* uncle = nullptr;

    parent->setChild(dir, node); //update the parent's child to passed in node

    //Now do the balancing

    //recursion or iteration....
    do {
        grandparent = parent->parent;
        //Case 1 (requirements are good)
        if (parent->color == BLACK) {
            return; //requirements hold
        }

        //case 4: Parent is red and the root, but the passed in node is also red (default color)
        if (parent->color == RED && grandparent == nullptr) {
            parent->color = BLACK; //switch root color
            return;
        }

        //direction now becomes the parent's direction relative to the grandparent (to get uncle and check for inner and outer children)

        //dir = (parent == grandparent->right) ? right : left; //direction of parent (this was working, changed to function for ease of use)
        dir = nodeDirection(parent);
        uncle = grandparent->child(1 - dir); //opposite parent direction

        //Case 5: If parent is red but uncle is black, then parent would have a red child (but we can't just swap colors like in case 2)
        if (getColor(uncle) == BLACK && parent->color == RED) {
            //Check if inner child
            if (node == parent->child(1 - dir)) {
                rotateSubTree(parent, dir);
                //rotate parent to grandparent position (dir is the direction parent is in relative to grandparent).

                //update the node names (since we use pointers, the one at the top should be the parent now)
                node = parent;
                parent = grandparent->child(dir);
            }

            //Case 6 (will come after case 5 since this is what fixes requirement 3, since parent and node could both be red after doing case 5)
            rotateSubTree(grandparent, static_cast<direction>(1 - dir));
            //rotate opposite direction at grandparent (parent is now grandparent)
            parent->color = BLACK;
            grandparent->color = RED;
            return;
        }


        //case 2: If both uncle and parent are red then requirement 4 would be violated when inserting a new node
        if (uncle != nullptr && (parent->color == RED && uncle->color == RED)) {
            parent->color = BLACK;
            uncle->color = BLACK;
            grandparent->color = RED; //this may violate rule 3 if grandparent's parent is red
            node = grandparent; //sets node to grandparent so we can continue checking validity upwards through the tree

            //grandparent = node->parent->parent;
        }
    }
    while ((parent = node->parent)); //loop while parent is not null (and go up the tree)

    //case 3 (if case 2 has happened enough for height to increase) then every requirement is met (we can basically exit function)

    root->color = BLACK; //make sure root is black
}

//remove traversal (add some memory cleanup)
void RedBlackTree::remove(Node* toRemove) {
    if (toRemove == nullptr) return;

    Node* x = nullptr; // Replacement node
    Node* y = nullptr; // In order successor
    Node* xParent = nullptr; // Parent of replacement node
    Color originalColor = toRemove->color;


    // Case 1: node to remove has at one child (swap with it's only child)
    if (toRemove->left == nullptr || toRemove->right == nullptr) {
        if (toRemove->left != nullptr) {
            //if it has left child
            x = toRemove->left;
        } else {
            //if it has right child (or no children)
            x = toRemove->right;
        }
        xParent = toRemove->parent;

        // Replace toRemove with its child
        transplant(toRemove, x);

        if (x != nullptr) {
             x->color = BLACK; //switch color to black (switching color to red should never happen).
        }
    }
    // Case 2: has two children (replace with in order successor)
    else {
        // Find the in order successor (minimum of right subtree)
        y = tree_min(toRemove->right);
        originalColor = y->color;
        x = y->right;

        //if the successor is a direct child, track it as the parent of x (since y will become the node to be removed).
        if (y->parent == toRemove) {
            //if the successor is a direct child
            xParent = y;
            if (x != nullptr)
                x->parent = y;
        } else {

            //if the successor is not the direct child, remove it from its position and put it as the right subtree of the node being removed.
            xParent = y->parent; //update xParent, since y will be the node to be replaced by x now.
            //original_dir = right; //update the direction of the original parent (sucessor child is always on the right side)
            transplant(y, y->right); //replace y with its right child
            y->right = toRemove->right; //y's right child is now the toRemove node's right.
            if (y->right != nullptr) {
                //update the right of y subtree (the nodes following y)
                y->right->parent = y;
            }
        }

        // Finally, Replace toRemove with y
        transplant(toRemove, y);
        y->left = toRemove->left;
        if (y->left != nullptr)
            y->left->parent = y; //update the left of y subtree (the nodes before y)
        y->color = toRemove->color;

        if (x != nullptr && originalColor == BLACK) { //special case. where the in order successor has a non null right child
            removeBalance(x);
        }
    }

    // Fix Red-Black properties if removed a black node
    if (originalColor == BLACK) {
        // Handle case of no children and being black with a temporary node
        if (x == nullptr && xParent != nullptr) {
            //the replacement node doesn't exist, meaning toRemove was a leaf node with no children (and it wasn't the root).
            Node tempNode(0); //make a temp node that represents the removed node.
            tempNode.color = BLACK;
            tempNode.parent = xParent;

            // check which child position is null and place the temp node there (check if this could break)
            if (xParent->left == nullptr) {
                xParent->setChild(left, &tempNode);
            } else {
                xParent->setChild(right, &tempNode);
            }
            removeBalance(&tempNode); //balance the case.
        } /*else if (x != nullptr) {
            //in the case that the successor (which was black) is moved.
            removeBalance(x); //balance the case.
        } */
    }
    delete toRemove; //free up memory
}

//add some memory cleanup
void RedBlackTree::removeBalance(const Node* node) {
    Node* parent = node->parent;

    //Initialize then needed nodes
    Node* sibling = nullptr;
    Node* close_nephew = nullptr;
    Node* far_nephew = nullptr;
    direction dir = right; //default direction to right

    //Check if the node we passed in was the temporary node.
    bool isTemp = (node->data == 0 && node->left == nullptr && node->right == nullptr);

    if (parent != nullptr) {
        dir = nodeDirection(node);
        parent->setChild(dir, nullptr);
    }

    //now do the balancing
    /* Loop Follows:
       -At the start of each iteration the height of "node" in the tree represents the iteration number (minus 1) (so the first iteration is zero)
       -The number of black nodes on tree paths through "node" is one less than before deletion, but this is unchanged for other paths. This means the parent of "node" has a violation of requirement 4 if other paths exist.
    */

    do {
        //case 1:
        if (parent == nullptr) {
            return; //exit if this is the root (tree should be balanced)
        }

        //update parameters after every iteration (parent is updated in the while loop):
        sibling = parent->child(1 - dir);

        //Check if the sibling is null.
        if (sibling != nullptr) {
            close_nephew = sibling->child(dir); //close = same direction
            far_nephew = sibling->child(1 - dir); //far = opposite direction as node is to it's parent
        } else {
            close_nephew = nullptr;
            far_nephew = nullptr;
        }

        //Handle case where sibling is nullptr (move up the tree by 1 if this is the case)
        if (sibling == nullptr) {
            //Then this is an unbalance state, go move up the tree
            const Node* current = parent;
            parent = parent->parent;

            if (parent == nullptr) {
                return; //if parent is null we have reached the root.
            }

            //now update a direction for the next iteration.
            dir = nodeDirection(current);
            continue; //skip to the next iteration of the loop (ignore the following cases)
        }

        //Case 3: (sibling is red, so nephews and parent must be black). This rotates the tree so that it can be fixed in cases 4,5, or 6
        if (getColor(sibling) == RED) {
            //(if node is left of parent rotate left, if it's right, do the opposite)
            rotateSubTree(parent, dir); //rotate so that the sibling becomes the new parent (or node's grandparent now)
            parent->color = RED;
            sibling->color = BLACK;
            sibling = close_nephew;
            //update the sibling pointer (since original sibling isn't actually the sibling anymore)

            //update close and far nephews.
            close_nephew = sibling->child(dir);
            far_nephew = sibling->child(1 - dir);
        }

        //Case 4: Sibling and it's children are black, but parent is red (check for null sibling, as that has no children)
        if (getColor(sibling) == BLACK && getColor(sibling->right) == BLACK && getColor(sibling->left) == BLACK &&
            parent->
            color == RED) {
            parent->color = BLACK;
            //switching the colors will add one to the # of black nodes through 'node's' path (since it's parent will now be black), which makes up for the deleted black nodes without altering the # of black nodes through sibling's path.
            sibling->color = RED;

            return; //after case 4 the tree should be balanced
        }

        //Case 5: Sibling is black, the close child is red, the far child is black.
        if (getColor(sibling) == BLACK && getColor(close_nephew) == RED && getColor(far_nephew) == BLACK) {
            //Rotate sibling opposite of node's direction (relative to parent). Now the close child takes sibling's place.
            rotateSubTree(sibling, static_cast<direction>(1 - dir));

            //Swap the close child and sibling's colors and update pointer positions
            sibling->color = RED;
            close_nephew->color = BLACK;
            far_nephew = sibling; //sibling would now be in the position of node's far nephew
            sibling = close_nephew; //close_nephew is now the actual sibling
        }
        //Now case 5 would move to case 6 to fix the tree

        //Case 6: sibling is black, and it's far child (node's far nephew) is red
        if (getColor(sibling) == BLACK && getColor(far_nephew) == RED) {
            rotateSubTree(parent, dir); //rotate parent so that the sibling is now in its original position

            //swap the parent and sibling colors (only happens if parent would be red)
            sibling->color = parent->color;
            parent->color = BLACK;

            far_nephew->color = BLACK; //set the far_nephew color to black

            //Now the subtree still has a root of the same color so requirement 3 is held.
            //The tree paths not passing through "node" also pass through the same number of black nodes as before, but "node now has an extra black ancestor (since parent was shifted to black, or sibling is added as a black grandparent). This means that paths passing through "node" have an additional black node, which fixes requirement 4 from being violated.

            return; //exit loop as the tree should now be balanced after case 6
        }

        //case 2: (if parent, sibling, and sibling's children are black) (req 4 violated)
        if (parent->color == BLACK &&
            getColor(sibling) == BLACK &&
            getColor(sibling->left) == BLACK &&
            getColor(sibling->right) == BLACK) {
            sibling->color = RED; //set sibling color to red
            const Node* current = parent;
            parent = parent->parent; //update parent for nexxt iteration
            node = current; //move to parent


            //update the direction for the next iteration
            if (parent != nullptr) {
                dir = nodeDirection(current);
            } else {
                return; //if parent is null we have reached the root.
            }
        }
    }
    while ((parent = node->parent));
    //will stop once parent is null. (also updates parent to move up the tree every iteration)
}

void RedBlackTree::print(const Node* pos, const int depth, const bool isRight) {
    //Take top then right

    if (pos == nullptr) {
        return; //base case
    }

    //Take the right first
    print(pos->right, depth + 1, true);

    //put spaces for the depth (more depth = more spaces)
    for (int i = 0; i < depth; i++) {
        std::cout << "    ";
    }


    //Print a Γ for a right node and L for a left node (helps show the connections of the tree better)
    if (depth > 0) {
        std::cout << (isRight ? "Γ" : "L") << " ";
    }
    std::cout << pos->data << " (" << (pos->color == RED ? "R" : "B") << ")" << std::endl;
    //print out ada and it's color

    print(pos->left, depth + 1, false); //print out left of tree
}

Node* RedBlackTree::getNode(Node* pos, const int data) {
    // Implementation for getting a node
    if (pos == nullptr) {
        return nullptr;
    }

    if (pos->data == data) {
        return pos;
    }

    //if data is greater than node then go right
    if (pos->data < data) {
        return getNode(pos->right, data);
    } else if (pos->data > data) {
        //If data is less than the node go left
        return getNode(pos->left, data);
    }
    return nullptr;
}


//Checks tree properties (last minute addition for testing), (i don't think this always works also....)
//sourced from: https://stackoverflow.com/questions/70293161/function-that-verifies-the-validity-of-the-red-black-tree
unsigned int RedBlackTree::checkTreeProperties(Node* parent, Node* node) {
    unsigned int left_black_height = 0;
    unsigned int right_black_height = 0;

    if (node == nullptr) {
        return 1; // Null nodes are considered black
    }

    // Check parent-child relationship
    if (node->parent != parent) {
        std::cout << "Node " << node->data << " has wrong parent" << std::endl;
    }

    // Add black node to height count
    if (node->color == BLACK) {
        left_black_height++;
        right_black_height++;
    } else if (node->color != RED) {
        std::cout << "Node " << node->data << " has invalid color" << std::endl;
    }

    // Check red-red violation
    if (node->color == RED && node->parent != nullptr && node->parent->color == RED) {
        std::cout << "RED-RED violation: Node " << node->data
            << " and parent " << node->parent->data << std::endl;
    }

    // Recursively check subtrees
    left_black_height += checkTreeProperties(node, node->left);
    right_black_height += checkTreeProperties(node, node->right);

    // Check black-height balance
    if (left_black_height != right_black_height) {
        std::cout << "Node " << node->data << " unbalanced ("
            << left_black_height << ", " << right_black_height << ")" << std::endl;
    }

    return std::max(left_black_height, right_black_height);
}

//Check Tree function, uses checkTreeProperties
void RedBlackTree::checkTree() {
    if (root == nullptr) {
        std::cout << "Tree is empty" << std::endl;
        return;
    }

    // Check if root is black
    if (root->color != BLACK) {
        std::cout << "Root " << root->data << " is not BLACK" << std::endl;
    }

    // Check both subtrees
    unsigned int leftHeight = checkTreeProperties(root, root->left);
    unsigned int rightHeight = checkTreeProperties(root, root->right);

    if (leftHeight != rightHeight) {
        std::cout << "Root subtrees have different black heights" << std::endl;
    } else {
        std::cout << "Tree passes RBT validation with black height: " << leftHeight << std::endl;
    }
}


// Helper method to recursively delete nodes
void RedBlackTree::deleteSubtree(Node* node) {
    if (node == nullptr) {
        return;
    }

    // First delete both subtrees
    deleteSubtree(node->left);
    deleteSubtree(node->right);

    // Then delete the node itself
    delete node;
}

// Corrected destructor
RedBlackTree::~RedBlackTree() {
    deleteSubtree(root);
}
