// Title: satnet.cpp
// Author: Andrew Tang
// Date: 10/22/2023
// Description: This is the implementation file for satnet.h

#include "satnet.h"

// Name - SatNet()
// Desc - The constructor performs the required initializations. It creates an empty object.
SatNet::SatNet(){
    // set member variable to a null value
    m_root = nullptr;
}

// Name - ~SatNet()
// Desc - The destructor performs the required cleanup including memory deallocations and re-initializing.
SatNet::~SatNet(){
    // call clear since the destructor can't be recursively called
    clear();
    m_root = nullptr;
}

// Name - insert(const Sat& satellite)
// Desc - This function inserts a Sat object into the tree in the proper position. 
// The Sat::m_id should be used as the key to traverse the SatNet tree and abide by BST traversal rules. 
// The comparison operators (>, <, ==, !=) work with the int type in C++. A Sat id is a unique number 
// in the range MINID - MAXID. We do not allow a duplicate id or an object with invalid id in the tree.
void SatNet::insert(const Sat& satellite){
    // call overloaded function if the id is valid
    if (satellite.getID() >= MINID && satellite.getID() <= MAXID){
        insert(satellite, m_root);
    }
}

// Name - insert(const Sat& satellite, Sat*& node)
// Desc - overloaded function to allow recursion
void SatNet::insert(const Sat& satellite, Sat*& node) {
    // base case given that you have reached the end or the "bottom" of the tree 
    if (node == nullptr) {
        node = new Sat(satellite.getID(), satellite.getAlt(), satellite.getInclin(), satellite.getState());
        node->m_height = 0;
        return;
    }
    
    // based on if greater or less than the current node value, recurse onto the left or right child nodes
    if (satellite.getID() < node->getID()) {
        insert(satellite, node->m_left);
    } 
    else if (satellite.getID() > node->getID()) {
        insert(satellite, node->m_right);
    } else {
        return;
    }

    // update the heights and fix the balance
    updateHeight(node);
    rebalance(node);
}

// Name - clear()
// Desc - The clear function deallocates all memory in the tree and makes it an empty tree.
void SatNet::clear(){
    // call overloaded function
    clear(m_root);
    m_root = nullptr;
}

// Name - clear(Sat*& node)
// Desc - overloaded function to allow recursion
void SatNet::clear(Sat*& node){
    // check if the node is populated; if it is, call clear recursively on the left and right
    if (node) {
        clear(node->m_left);
        clear(node->m_right);
        delete node;
        node = nullptr;
    }
}

// Name - remove(int id)
// Desc - The remove function traverses the tree to find a node with the id and removes it from the tree.
void SatNet::remove(int id){
    // call overloaded
    remove(id, m_root);
}

// Name - remove(int id, Sat*& node)
// Desc - overloaded function to allow recursion
void SatNet::remove(int id, Sat*& node) {
    // base case where no node found 
    if (node == nullptr) {
        return;
    }
    
    // based on if greater or less than the current node value, recurse onto the left or right child nodes
    if (id < node->getID()) {
        remove(id, node->m_left);
    } 
    else if (id > node->getID()) {
        remove(id, node->m_right);
    } 
    // case where the node was found
    else if (id == node->getID()){
        // case where there are 0 child nodes
        if (node->m_left == nullptr && node->m_right == nullptr) {
            delete node;
            node = nullptr;
        }
        // case where there is 1 child node
        else if (node->m_left == nullptr || node->m_right == nullptr) {
            Sat* temp = nullptr;
            // get either the left or right node, depending on which one is populated
            if (node->m_left != nullptr) {
                temp = node->m_left;
            } 
            if (node->m_right != nullptr) {
                temp = node->m_right;
            }
            // just set the node as its child
            delete node; 
            node = temp; 
        } 
        else {
            Sat* successor = node;
            // go to right subtree
            successor = successor->m_right; 
            // find the min value on the right subtree
            while (successor->m_left != nullptr) {
                successor = successor->m_left;
            }
            node->setID(successor->getID());
            node->setAlt(successor->getAlt());
            node->setInclin(successor->getInclin());
            node->setState(successor->getState());

            // call remove on the successor to handle the case where it has children 
            remove(successor->getID(), node->m_right);
        }
    }
    // update height and rebalance on tail recursion
    updateHeight(node);
    rebalance(node);
}

// Name - dumpTree()
// Desc - outputs the contents of the tree
void SatNet::dumpTree() const {
    dump(m_root);
}

// Name - dump(Sat* satellite)
// Desc - overloaded function to allow recursion
void SatNet::dump(Sat* satellite) const{
    if (satellite != nullptr){
        cout << "(";
        dump(satellite->m_left);//first visit the left child
        cout << satellite->m_id << ":" << satellite->m_height;//second visit the node itself
        dump(satellite->m_right);//third visit the right child
        cout << ")";
    }
}

// Name - listSatellites()
// Desc - This function prints a list of all satellites in the tree to the standard output in the ascending order of IDs. 
// The information for every Sat object will be printed in a new line. 
// For the format of output please refer to the sample output file, i.e. driver.txt.
void SatNet::listSatellites() const {
    // call overloaded
    listSatellites(m_root);
}

// Name - listSatellites(Sat* node)
// Desc - overloaded function to allow recursion
void SatNet::listSatellites(Sat* node) const {
    if (node == nullptr) {
        return;
    }
    
    // perform an inorder search
    listSatellites(node->m_left);

    cout << "\n" << node->getID() << ": " << node->getStateStr() << ": " << node->getInclinStr() << ": " << node->getAltStr();

    listSatellites(node->m_right);
}

// Name - setState(int id, STATE state)
// Desc - This function finds the node with id in the tree and sets its Sat::m_state member variable to state. 
// If the operation is successful, the function returns true otherwise it returns false. For example, when the
// satellite with id does not exist in the tree the function returns false.
bool SatNet::setState(int id, STATE state) {
    return setState(id, state, m_root);
}

// Name - setState(int id, STATE state, Sat*& node)
// Desc - overloaded function to allow recursion
bool SatNet::setState(int id, STATE state, Sat*& node){
    // basecase where node was not found 
    if (node == nullptr) {
        return false;
    }

    // find the correct node then set the state
    if (id < node->getID()) {
        return setState(id, state, node->m_left);
    } 
    else if (id > node->getID()) {
        return setState(id, state, node->m_right);
    } 
    else {
        node->setState(state);
        return true;
    }
}

// Name - removeDeorbited()
// Desc - This function traverses the tree, finds all satellites with 
// DEORBITED state and removes them from the tree. The final tree must be a balanced AVL tree.
void SatNet::removeDeorbited() {
    removeDeorbited(m_root);
}

// Name - removeDeorbited(Sat*& node)
// Desc - overloaded function to allow recursion
void SatNet::removeDeorbited(Sat*& node) {
    if (node == nullptr) {
        return;
    }

    // recursively go through every possible node then call remove on every "DEORBITED" node
    removeDeorbited(node->m_left);
    removeDeorbited(node->m_right);

    if (node->getState() == DEORBITED) {
        remove(node->getID(), node);
    }
}

// Name - findSatellite(int id)
// Desc - This function returns true if it finds the node with id in the tree, otherwise it returns false.
bool SatNet::findSatellite(int id) const {
    return findSatellite(m_root, id);
}

// Name - findSatellite(const Sat* node, int id) 
// Desc - overloaded function to allow recursion
bool SatNet::findSatellite(const Sat* node, int id) const {
    // base case where not found
    if (node == nullptr) {
        return false;
    }
    // base case where the satellite was found
    if (node->getID() == id) {
        return true;
    }

    // look for the node
    if (id < node->getID()) {
        return findSatellite(node->m_left, id);
    }
    else {
        return findSatellite(node->m_right, id);
    }
}

// Name - operator=(const SatNet & rhs)
// Desc - This function overloads the assignment operator for the class SatNet. 
// It creates an exact deep copy of the rhs.
const SatNet & SatNet::operator=(const SatNet & rhs){
    // check if lhs is the rhs
    if (this == &rhs) {
        return *this;
    }
    // clear out the tree
    clear(m_root);
    
    // call the copy operation
    m_root = copy(rhs.m_root);
    return *this;
}

// Name - copy(const Sat* node)
// Desc - overloaded function to allow recursion
Sat* SatNet::copy(const Sat* node) const {
    // base case
    if (node == nullptr) {
        return nullptr;
    }

    // copy the node's data
    Sat* newNode = new Sat(node->getID(), node->getAlt(), node->getInclin(), node->getState());
    // set the height variables
    newNode->m_height = node->m_height;
    // set the children of that node by recursively calling the copy function
    newNode->m_left = copy(node->m_left);
    newNode->m_right = copy(node->m_right);

    return newNode;
}

// Name - countSatellites(INCLIN degree)
// Desc - This function traverses the tree, finds all satellites with the inclination specified by 
// degree variable and returns the total number of satellites with that inclination in the network.
int SatNet::countSatellites(INCLIN degree) const {
    return countSatellites(m_root, degree);
}

// Name - countSatellites(Sat* node, INCLIN degree)
// Desc - overloaded function to allow recursion
int SatNet::countSatellites(Sat* node, INCLIN degree) const {
    // base case
    if (node == nullptr) {
        return 0;
    }

    int count = 0;
    // if the incline is the same, then increment the counter
    if (node->getInclin() == degree) {
        count++;
    }

    // recursive call on left and right children 
    count += countSatellites(node->m_left, degree);
    count += countSatellites(node->m_right, degree);

    return count;
}

// Name - rightRotate(Sat*& node)
// Desc - perform a right rotate on the node
void SatNet::rightRotate(Sat*& node) {
    // perform the right rotate by switching pointers
    Sat* newRoot = node->m_left;
    node->m_left = newRoot->m_right;
    newRoot->m_right = node;
    node = newRoot;

    // update heights 
    updateHeight(node->m_right);
    updateHeight(node);
}

// Name - leftRotate(Sat*& node)
// Desc - perform a left rotate on the node
void SatNet::leftRotate(Sat*& node) {
    // perform a left rotate by swtcihing pointers
    Sat* newRoot = node->m_right;
    node->m_right = newRoot->m_left;
    newRoot->m_left = node;
    node = newRoot;

    // update heights
    updateHeight(node->m_left);
    updateHeight(node);
}

// Name - updateHeight(Sat*& node)
// Desc - defines the height based on the child nodes
void SatNet::updateHeight(Sat*& node) {
    if (node == nullptr){
        return; 
    }
    // get the height of the left node
    int leftHeight = -1;
    if (node->m_left != nullptr) {
        leftHeight = node->m_left->m_height;
    }

    // get the height of the right node
    int rightHeight = -1;
    if (node->m_right != nullptr) {
        rightHeight = node->m_right->m_height;
    }

    // set the new heights based on whichever child height was greater
    node->m_height = 1;
    if (leftHeight > rightHeight) {
        node->m_height += leftHeight;
    } 
    else {
        node->m_height += rightHeight;
    }   
}

// Name - getBalance(Sat* node) 
// Desc - finds the balance factor of a given node
int SatNet::getBalance(Sat* node) {
    if (node == nullptr){
        return 0; 
    }

    // get the height of the left child
    int leftHeight = -1;
    if (node->m_left != nullptr) {
        leftHeight = node->m_left->m_height;
    }
    
    // get the height of the right child 
    int rightHeight = -1;
    if (node->m_right != nullptr) {
        rightHeight = node->m_right->m_height;
    }
    // return the difference
    return leftHeight - rightHeight; 
}

// Name - rebalance(Sat*& node)
// Desc - rebalances the tree depending on the balance factor
void SatNet::rebalance(Sat*& node) {
    int balance = getBalance(node);

    // based on what balance is, perform these operations for all cases
    if (balance > 1) {
        if (getBalance(node->m_left) >= 0) {
            rightRotate(node);
        } 
        else {
            leftRotate(node->m_left);
            rightRotate(node);
        }
    } else if (balance < -1) {
        if (getBalance(node->m_right) <= 0) {
            leftRotate(node);
        } 
        else {
            rightRotate(node->m_right);
            leftRotate(node);
        }
    }
}
