#include "RedBlackTree.h"
#include <iostream>

// Corrected constructor name
RedBlackTree::RedBlackTree() = default;


//https://en.wikipedia.org/wiki/Tree_rotation visualization
//node passed in is the root of the subtree (perhaps rename later
//direction is the direction to rotate in
Node* RedBlackTree::rotateSubTree(Node* subRoot, const direction dir) {
    //Will rotate left or right.
    Node* parent = subRoot->parent; //save original root parent

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

//Gets the direction of the node passed in relatvie to it's parent
direction nodeDirection(Node* node) {
  if (node == node->parent->right) {
    return right;
  } else {
    return left
  }
}

//position initially is the root, prev is initially nullptr

//insert function broken (misses 3 when addding?)
void RedBlackTree::insert(Node* & pos, Node* prev, const int data, const direction dir) {
    if (pos == nullptr) {
        pos = new Node(data); //this will be the base case of the recursion (will insert once the path has ended)
        pos->parent = prev; //set parent node
        insert(pos, dir); //do the actual insert
        return;
    }


    //if number is already in the tree
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
void RedBlackTree::insert(Node* node, direction dir) {
  Node* parent = node->parent;
  
  if (parent == nullptr) {
    node->color = BLACK;
    root = node; //if there aren't any other nodes in the tree then this node is root
    return;
  }


  Node* grandparent = parent->parent;
  Node* uncle = nullptr;

    parent->setChild(dir, node); //update the parent's child to passed in node

    //Now do the balancing

    /*
     * Requirements:
    *  1 Every node is either red or black
    *  2 All null nodes  are considered black.
    *  3 A red node does not have a red child.
    *  4 Every path from a given node to any of its leaf nodes goes through the same number of black nodes.
    *  (Conclusion) If a node N has exactly one child, the child must be red, because if it were black, its leaves would sit at a different black depth than N's child, violating requirement 4.
     */


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

        dir = (parent == grandparent->right) ? right : left; //direction of parent
	//dir = nodeDirection(parent);
	uncle = grandparent->child(1 - dir); //opposite parent direction

        //Case 5: If parent is red but uncle is black, then parent would have a red child (but we can't just swap colors like in case 2)
        if ((uncle == nullptr || uncle->color == BLACK) && parent->color == RED) {
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

    root->color = BLACK; //make sure root is black (wasn't always set befoer)
}

//remove traversal
void RedBlackTree::remove(Node* & pos, Node* prev, const int toRemove, const direction dir) {
    if (pos->data == toRemove) {
        remove(pos, dir); //do the actual remove
        return;
    }


    //if number is already in the tree
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


void RedBlackTree::remove(Node* rem) {
  // Implementation for removing a node
  Node* sibling = nullptr;
  Node* inner_nephew = nullptr;
  Node* outer_newphew = nullptr;
}

void RedBlackTree::print(const Node* pos , const int depth, bool isRight) {
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
    if (depth > 0 ) {
        std::cout << (isRight ? "Γ" : "L") << " ";
    }
    std::cout << pos->data << " (" << (pos->color == RED ? "R" : "B") << ")" << std::endl; //print out ada and it's color

    print(pos->left, depth + 1, false); //print out left of tree
}

Node* RedBlackTree::getNode(Node* pos, int data) {
    // Implementation for getting a node
    return nullptr; // Placeholder
}

// Corrected destructor
RedBlackTree::~RedBlackTree() = default;
