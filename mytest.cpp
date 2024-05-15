// Title: mytest.cpp
// Author: Andrew Tang
// Date: 9/15/2023
// Description: This is a tester file for satnet.cpp

#include "satnet.h"
#include <math.h>
#include <algorithm>
#include <random>
#include <vector>
using namespace std; 

enum RANDOM {UNIFORMINT, UNIFORMREAL, NORMAL, SHUFFLE};
class Random {
public:
    Random(int min, int max, RANDOM type=UNIFORMINT, int mean=50, int stdev=20) : m_min(min), m_max(max), m_type(type)
    {
        if (type == NORMAL){
            //the case of NORMAL to generate integer numbers with normal distribution
            m_generator = std::mt19937(m_device());
            //the data set will have the mean of 50 (default) and standard deviation of 20 (default)
            //the mean and standard deviation can change by passing new values to constructor 
            m_normdist = std::normal_distribution<>(mean,stdev);
        }
        else if (type == UNIFORMINT) {
            //the case of UNIFORMINT to generate integer numbers
            // Using a fixed seed value generates always the same sequence
            // of pseudorandom numbers, e.g. reproducing scientific experiments
            // here it helps us with testing since the same sequence repeats
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_unidist = std::uniform_int_distribution<>(min,max);
        }
        else if (type == UNIFORMREAL) { //the case of UNIFORMREAL to generate real numbers
            m_generator = std::mt19937(10);// 10 is the fixed seed value
            m_uniReal = std::uniform_real_distribution<double>((double)min,(double)max);
        }
        else { //the case of SHUFFLE to generate every number only once
            m_generator = std::mt19937(m_device());
        }
    }
    void setSeed(int seedNum){
        // we have set a default value for seed in constructor
        // we can change the seed by calling this function after constructor call
        // this gives us more randomness
        m_generator = std::mt19937(seedNum);
    }

    void getShuffle(vector<int> & array){
        // the user program creates the vector param and passes here
        // here we populate the vector using m_min and m_max
        for (int i = m_min; i<=m_max; i++){
            array.push_back(i);
        }
        shuffle(array.begin(),array.end(),m_generator);
    }

    void getShuffle(int array[]){
        // the param array must be of the size (m_max-m_min+1)
        // the user program creates the array and pass it here
        vector<int> temp;
        for (int i = m_min; i<=m_max; i++){
            temp.push_back(i);
        }
        std::shuffle(temp.begin(), temp.end(), m_generator);
        vector<int>::iterator it;
        int i = 0;
        for (it=temp.begin(); it != temp.end(); it++){
            array[i] = *it;
            i++;
        }
    }

    int getRandNum(){
        // this function returns integer numbers
        // the object must have been initialized to generate integers
        int result = 0;
        if(m_type == NORMAL){
            //returns a random number in a set with normal distribution
            //we limit random numbers by the min and max values
            result = m_min - 1;
            while(result < m_min || result > m_max)
                result = m_normdist(m_generator);
        }
        else if (m_type == UNIFORMINT){
            //this will generate a random number between min and max values
            result = m_unidist(m_generator);
        }
        return result;
    }

    double getRealRandNum(){
        // this function returns real numbers
        // the object must have been initialized to generate real numbers
        double result = m_uniReal(m_generator);
        // a trick to return numbers only with two deciaml points
        // for example if result is 15.0378, function returns 15.03
        // to round up we can use ceil function instead of floor
        result = std::floor(result*100.0)/100.0;
        return result;
    }
    
    private:
    int m_min;
    int m_max;
    RANDOM m_type;
    std::random_device m_device;
    std::mt19937 m_generator;
    std::normal_distribution<> m_normdist;//normal distribution
    std::uniform_int_distribution<> m_unidist;//integer uniform distribution
    std::uniform_real_distribution<double> m_uniReal;//real uniform distribution
};

class Tester{
    public:

    // ==================================================| Test Cases for the SATNET Class |============================================================
    //Function: insert(const Sat& satellite)
    //Case: Normal Case
    //Expected result: resulting tree is as expected
    bool insertNormal() {
        cout << "TEST 1 RESULTS:" << endl; 

        SatNet network;
        // insert in nodes
        for (int i = 0; i < 5; i++){
            Sat satellite(10000 + i);
            network.insert(satellite);
        }

        // make sure its still an avl tree
        if (!bstChecker(network.m_root) || !balanceChecker(network.m_root)){
            return false; 
        }

        // make sure that output is as expected, has the correct nodes
        if (out(network.m_root) != "((10000:0)10001:2((10002:0)10003:1(10004:0)))"){
            return false;  
        }
        return true; 
    }

    //Function: insert(const Sat& satellite)
    //Case: Edge case where duplicates are inserted
    //Expected result: resulting tree is as expected, duplicates are not inserted
    bool insertEdge() {
        cout << "TEST 2 RESULTS:" << endl; 

        SatNet network;
        // insert in nodes
        for (int i = 0; i < 5; i++){
            Sat satellite(10000 + i);
            network.insert(satellite);
        }

        // inserts the same node
        for (int i = 0; i < 5; i++){
            Sat satellite(10000 + i);
            network.insert(satellite);
        }

        // make sure its still an avl tree
        if (!bstChecker(network.m_root) || !balanceChecker(network.m_root)){
            return false; 
        }

        // make sure that duplicates are handled by being not added, has the correct nodes
        if (out(network.m_root) != "((10000:0)10001:2((10002:0)10003:1(10004:0)))"){
            return false; 
        }
        return true; 
    }

    //Function: insert(const Sat& satellite)
    //Case: Error Case where nodes have invalid IDs are inserted
    //Expected result: resulting tree is as expected, nothing is inserted
    bool insertError() {
        cout << "TEST 3 RESULTS:" << endl; 

        SatNet network;
        // insert nodes with values less than the min
        for (int i = 0; i < 5; i++){
            Sat satellite(1000 + i);
            network.insert(satellite);
        }

        // insert nodes with values greater than the max
        for (int i = 0; i < 5; i++){
            Sat satellite(100000 + i);
            network.insert(satellite);
        }

        // make sure its still an avl tree
        if (!bstChecker(network.m_root) || !balanceChecker(network.m_root)){
            return false; 
        }

        // nothing should be added
        if (out(network.m_root) != ""){
            return false; 
        }
        return true; 
    }

    //Function: insert(const Sat& satellite)
    //Case: Normal Case
    //Expected result: resulting tree is balanced as expected
    bool balanceCheckerInsert() {
        cout << "TEST 4 RESULTS:" << endl; 

        Random idGen(MINID,MAXID);
        Random inclinGen(0,3);
        Random altGen(0,3); 

        // insert 300 random nodes
        SatNet network;
        for (int i = 0; i < 1500; i++){
            Sat satellite(idGen.getRandNum(), static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
            network.insert(satellite);
        }

        // check that balance was maintained
        return balanceChecker(network.m_root);
    }

    //Function: insert(const Sat& satellite)
    //Case: Normal Case
    //Expected result: resulting tree is has BST property
    bool bstCheckerInsert() {
        cout << "TEST 5 RESULTS:" << endl; 

        Random idGen(MINID,MAXID);
        Random inclinGen(0,3);
        Random altGen(0,3); 

        // insert 300 random nodes
        SatNet network;
        for (int i = 0; i < 300; i++){
            Sat satellite(idGen.getRandNum(), static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
            network.insert(satellite);
        }
        
        // check that the bst property is maintained
        return bstChecker(network.m_root);
    }

    //Function: remove(int id)
    //Case: Normal Case
    //Expected result: resulting tree is as expected
    bool removeNormal() {
        cout << "TEST 6 RESULTS:" << endl; 

        SatNet network;
        // insert some nodes
        for (int i = 0; i < 5; i++){
            Sat satellite(10000 + i);
            network.insert(satellite);
        }
        // remove some 
        network.remove(10000);
        network.remove(10001);
        network.remove(10003);

        // make sure its still an avl tree
        if (!bstChecker(network.m_root) || !balanceChecker(network.m_root)){
            return false; 
        }

        // make sure output was as expected, has the correct nodes
        if (out(network.m_root) != "((10002:0)10004:1)"){
            return false;  
        }
        return true; 
    }

    //Function: remove(int id)
    //Case: Edge Case where nodes are removed that are not in the tree
    //Expected result: resulting tree is as expected
    bool removeEdge() {
        cout << "TEST 7 RESULTS:" << endl; 

        SatNet network;
        // insert some nodes
        for (int i = 0; i < 5; i++){
            Sat satellite(10000 + i);
            network.insert(satellite);
        }
        // remove nodes that are not in the tree
        network.remove(9999);
        network.remove(10010);
        network.remove(200000);

        // make sure its still an avl tree, has the correct nodes
        if (!bstChecker(network.m_root) || !balanceChecker(network.m_root)){
            return false; 
        }

        if (out(network.m_root) != "((10000:0)10001:2((10002:0)10003:1(10004:0)))"){
            return false;  
        }
        return true; 
    }

    //Function: remove(int id)
    //Case: Normal Case
    //Expected result: resulting tree is balanced as expected
    bool balanceCheckerRemove() {
        cout << "TEST 8 RESULTS:" << endl; 

        Random idGen(MINID,MAXID);
        Random inclinGen(0,3);
        Random altGen(0,3); 
    
        SatNet network;
        
        // insert 300 random nodes
        int insertedIDs[300];
        for (int i = 0; i < 1300; i++){
            insertedIDs[i] = idGen.getRandNum();
            Sat satellite(insertedIDs[i], static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
            network.insert(satellite);
        }

        // remove every other one
        for (int i = 0; i < 300; i += 2) {
            network.remove(insertedIDs[i]);
        }
        // make sure balance was maintained
        return balanceChecker(network.m_root);
    }

    //Function: remove(int id)
    //Case: Normal Case
    //Expected result: resulting tree maintains BST property as expected
    bool bstCheckerRemove() {
        cout << "TEST 9 RESULTS:" << endl; 

        Random idGen(MINID,MAXID);
        Random inclinGen(0,3);
        Random altGen(0,3); 
    
        SatNet network;
        
        // insert 300 random nodes
        int insertedIDs[300];
        for (int i = 0; i < 300; i++){
            insertedIDs[i] = idGen.getRandNum();
            Sat satellite(insertedIDs[i], static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
            network.insert(satellite);
        }

        // remove every other one
        for (int i = 0; i < 300; i += 2) {
            network.remove(insertedIDs[i]);
        }
        
        // make sure BST property was maintained
        return bstChecker(network.m_root);
    }

    //Function: removeDeorbited()
    //Case: Normal Case
    //Expected result: resulting tree removes deorbited nodes
    bool removeDeorbitedNormal() {
        cout << "TEST 10 RESULTS:" << endl; 

        SatNet network;
        Random idGen(MINID, MAXID);
        Random inclinGen(0, 3);
        Random altGen(0, 3);

        // insert some random nodes
        for (int i = 0; i < 10; ++i) {
            Sat satellite(idGen.getRandNum(), static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
            if (i % 2 == 0) {
                // set some to be deorbited
                satellite.setState(STATE::DEORBITED);
            }
            network.insert(satellite);
        }

        // remove 
        network.removeDeorbited();

        // make sure its still an avl tree
        if (!bstChecker(network.m_root) || !balanceChecker(network.m_root)){
            return false; 
        }
        
        // make sure that there are no more deorbited nodes
        return removeDeorbitedChecker(network.m_root); 
    }

    //Function: removeDeorbited()
    //Case: Edge case where there are no deorbited nodes
    //Expected result: resulting tree does not change
    bool removeDeorbitedEdge() {
        cout << "TEST 11 RESULTS:" << endl; 

        SatNet network;
        Random idGen(MINID, MAXID);
        Random inclinGen(0, 3);
        Random altGen(0, 3);

        // insert some random nodes where none are defined as deorbited
        for (int i = 0; i < 10; ++i) {
            Sat satellite(idGen.getRandNum(), static_cast<ALT>(altGen.getRandNum()), static_cast<INCLIN>(inclinGen.getRandNum()));
            network.insert(satellite);
        }

        string before = out(network.m_root);
        network.removeDeorbited();
        string after = out(network.m_root);

        // make sure its still an avl tree
        if (!bstChecker(network.m_root) || !balanceChecker(network.m_root)){
            return false; 
        }

        // make sure that the tree has not been altered
        if(before != after){
            return false; 
        }

        return true; 
    }

    //Function: countSatellites(INCLIN degree)
    //Case: Normal Case
    //Expected result: counts correctly
    bool countSatellitesNormal() {
        cout << "TEST 12 RESULTS:" << endl; 

        SatNet network;

        // insert some nodes with incline of I48
        for (int i = 0; i < 5; i++){
            Sat satellite(10000 + i, ALT::MI208, INCLIN::I48);
            network.insert(satellite);
            
        }

        // insert some nodes with incline of I53
        for (int i = 0; i < 5; i++){
            Sat satellite(10010 + i, ALT::MI208, INCLIN::I53);
            network.insert(satellite);
            
        }
        
        // make sure the count is correct of I48
        if (network.countSatellites(INCLIN::I48) != 5){
            return false; 
        }

        // make sure the count is correct of I53
        if (network.countSatellites(INCLIN::I53) != 5){
            return false; 
        }

        return true; 
    }
    
    //Function: countSatellites(INCLIN degree)
    //Case: Edge Case where there are no nodes of that degree
    //Expected result: counts correctly; i.e 0 
    bool countSatellitesEdge() {
        cout << "TEST 13 RESULTS:" << endl; 

        SatNet network;

        // insert some nodes with incline of I48
        for (int i = 0; i < 5; i++){
            Sat satellite(10000 + i, ALT::MI208, INCLIN::I48);
            network.insert(satellite);
            
        }

        // insert some nodes with incline of I53
        for (int i = 0; i < 5; i++){
            Sat satellite(10010 + i, ALT::MI208, INCLIN::I53);
            network.insert(satellite);
            
        }

        // there should be no nodes of height 70
        if (network.countSatellites(INCLIN::I70) != 0){
            return false; 
        }

        return true; 
    }

    //Function: findSatellite(int id)
    //Case: Normal Case
    //Expected result: returns true
    bool findSatelliteNormal() {
        cout << "TEST 14 RESULTS:" << endl; 

        SatNet network;
        // insert some nodes
        for (int i = 0; i < 5; i++){
            Sat satellite(10000 + i, ALT::MI208, INCLIN::I48);
            network.insert(satellite);
            
        }

        // insert target node
        Sat satellite(99999, ALT::MI208, INCLIN::I48);

        network.insert(satellite);

        // make sure that target node was found
        return network.findSatellite(99999);
    }

    //Function: findSatellite(int id)
    //Case: Error case where no node of that id is present
    //Expected result: returns false
    bool findSatelliteError() {
        cout << "TEST 15 RESULTS:" << endl; 

        SatNet network;
        // insert some nodes
        for (int i = 0; i < 5; i++){
            Sat satellite(10000 + i, ALT::MI208, INCLIN::I48);
            network.insert(satellite);
            
        }
    
        Sat satellite(99999, ALT::MI208, INCLIN::I48);

        network.insert(satellite);

        // make sure it says that node with id 99998 was not found 
        return !(network.findSatellite(99998));
    }

    //Function: operator=(const SatNet & rhs)
    //Case: Normal Case
    //Expected result: rhs = lhs
    bool assignmentNormal() {
        cout << "TEST 16 RESULTS:" << endl; 

        SatNet network1;
        // insert some nodes
        for (int i = 0; i < 5; i++){
            Sat satellite(10000 + i);
            network1.insert(satellite);
        }
        
        SatNet network2;
        // insert some nodes
        for (int i = 0; i < 5; i++){
            Sat satellite(10010 + i);
            network2.insert(satellite);
        }
        
        // assign network 1 to network 2
        network2 = network1;

        // makes sure that they are equal 
        if (!(isEqual(network1.m_root, network2.m_root))) {
            return false;
        }

        // makes sure that this is a deep copy
        if (&network1 == &network2 || &network1.m_root == &network2.m_root) {
            return false;
        }
         
        return true;
    }

    //Function: operator=(const SatNet & rhs)
    //Case: Error case where rhs and lhs are both empty
    //Expected result: rhs = lhs, and their m_root is nullptr
    bool assignmentError() {
        cout << "TEST 17 RESULTS:" << endl; 

        SatNet network1;
        SatNet network2;

        network2 = network1;

        // makes sure that the roots are both nullptr
        if (network1.m_root != nullptr || network2.m_root != nullptr) {
            return false;
        }

        // makes sure that they are still equal 
        if (!(isEqual(network1.m_root, network2.m_root))) {
            return false;
        }

        return true;
    }

    //Function: remove(int id)
    //Case: Normal Case
    //Expected result: remove performs in O(logn)
    bool removePerformanceChecker(){
        cout << "TEST 18 RESULTS:" << endl; 
        
        Random idGen(MINID, MAXID);
        Random inclinGen(0, 3);
        Random altGen(0, 3);
        
        // set number of nodes
        const int n1 = 1000;
        const int n2 = 2000;

        SatNet network1;
        SatNet network2;

        int insertedID1[n1];
        int insertedID2[n2];

        // insert n1 random nodes
        for (int i = 0; i < n1; ++i) {
            insertedID1[i] = idGen.getRandNum();
            Sat satellite(insertedID1[i],
                        static_cast<ALT>(altGen.getRandNum()),
                        static_cast<INCLIN>(inclinGen.getRandNum()));
            network1.insert(satellite);
        }

        // insert n2 random nodes
        for (int i = 0; i < n2; ++i) {
            insertedID2[i] = idGen.getRandNum();
            Sat satellite(insertedID2[i],
                        static_cast<ALT>(altGen.getRandNum()),
                        static_cast<INCLIN>(inclinGen.getRandNum()));
            network2.insert(satellite);
        }

        // measure the time it takes to remove n1 nodes
        clock_t start = clock();
        for (int i = 0; i < n1; ++i) {
            network1.remove(insertedID1[i]);
        }
        clock_t end = clock();      
        double time1 = static_cast<double>(end - start) / CLOCKS_PER_SEC;

        // measure the time it takes to remove n2 nodes
        start = clock();
        for (int i = 0; i < n2; ++i) {
            network2.remove(insertedID2[i]);
        }
        end = clock();
        double time2 = static_cast<double>(end - start) / CLOCKS_PER_SEC;

        // what is the expected ratio
        double expectedRatio = (n2*log(n2)) / (2*n1*log(n1));

        // our measured ratio
        double observedRatio = time2 / (2*time1);

        cout << "Expected Ratio: " << expectedRatio << endl;
        cout << "Observed Ratio: " << observedRatio << endl;

        double errorMargin = 0.4;
        double lowerBound = expectedRatio - errorMargin;
        double upperBound = expectedRatio + errorMargin;

        // make sure that the result in within the error margins
        if (!(observedRatio >= lowerBound && observedRatio <= upperBound)) {
            return false;
        } 
        return true;
    }

    //Function: insert(const Sat& satellite)
    //Case: Normal Case
    //Expected result: insert performs in O(logn)
    bool insertPerformanceChecker(){
        cout << "TEST 19 RESULTS:" << endl; 

        Random idGen(MINID, MAXID);
        Random inclinGen(0, 3);
        Random altGen(0, 3);

        // set number of nodes
        const int n1 = 1000;
        const int n2 = 2000;

        SatNet network1;
        SatNet network2;

        // measure the amount of time it takes to insert n1 nodes
        clock_t start = clock();
        for (int i = 0; i < n1; ++i) {
            int ID = idGen.getRandNum();
            Sat satellite(ID,
                        static_cast<ALT>(altGen.getRandNum()),
                        static_cast<INCLIN>(inclinGen.getRandNum()));
            network1.insert(satellite);
        }
        
        clock_t end = clock();      
        double time1 = static_cast<double>(end - start) / CLOCKS_PER_SEC;

        // measure the amount of time it takes to insert n2 nodes
        start = clock();
        for (int i = 0; i < n2; ++i) {
            int ID = idGen.getRandNum();
            Sat satellite(ID,
                        static_cast<ALT>(altGen.getRandNum()),
                        static_cast<INCLIN>(inclinGen.getRandNum()));
            network2.insert(satellite);
        }

        end = clock();
        double time2 = static_cast<double>(end - start) / CLOCKS_PER_SEC;

        // what is the expected ratio
        double expectedRatio = (n2*log(n2)) / (2*n1*log(n1));

        // our measured ratio
        double observedRatio = time2 / (2*time1);

        cout << "Expected Ratio: " << expectedRatio << endl;
        cout << "Observed Ratio: " << observedRatio << endl;

        double errorMargin = 0.4;
        double lowerBound = expectedRatio - errorMargin;
        double upperBound = expectedRatio + errorMargin;
        
        // make sure that the result in within the error margins
        if (!(observedRatio >= lowerBound && observedRatio <= upperBound)) {
            return false;
        } 
        return true;
    }

    //Function: setState(int id, STATE state)
    //Case: Normal Case
    //Expected result: returns true
    bool setStateNormal(){
        cout << "TEST 20 RESULTS:" << endl; 

        SatNet network;

        // insert some nodes
        for (int i = 0; i < 5; i++){
            Sat satellite(10000 + i, ALT::MI208, INCLIN::I48);
            network.insert(satellite);
            
        }

        // make a special satellite with default state active
        Sat satellite(99999);
        network.insert(satellite);

        // turn it into deorbited
        if (!network.setState(99999, STATE::DEORBITED)){
            return false; 
        }

        // should be gone after this
        network.removeDeorbited();

        // should not be able to find the node
        if(network.findSatellite(99999)){
            return false; 
        }

        return true; 
    }

    //Function: setState(int id, STATE state)
    //Case: Edge Case
    //Expected result: returns false
    bool setStateEdge(){
        cout << "TEST 21 RESULTS:" << endl; 

        SatNet network;

        // insert some nodes
        for (int i = 0; i < 5; i++){
            Sat satellite(10000 + i, ALT::MI208, INCLIN::I48);
            network.insert(satellite);
            
        }

        // call setState on a node thats not in the tree
        if (network.setState(99999, STATE::DEORBITED)){
            return false; 
        }

        return true; 
    }

    //Function: clear()
    //Case: Normal case
    //Expected result: makes object empty
    bool clearNormal(){
        cout << "TEST 22 RESULTS:" << endl; 

        SatNet network;

        // insert some nodes
        for (int i = 0; i < 5; i++){
            Sat satellite(10000 + i, ALT::MI208, INCLIN::I48);
            network.insert(satellite);
            
        }

        network.clear();

        if (network.m_root != nullptr){
            return false; 
        }

        return true; 
    }

    //Function: clear()
    //Case: Edge case
    //Expected result: keeps empty object empty
    bool clearEdge(){
        cout << "TEST 23 RESULTS:" << endl; 

        SatNet network;

        network.clear();

        if (network.m_root != nullptr){
            return false; 
        }

        return true; 
    }

    private:
    
    /**********************************************
    * If we need helper functions to be reused in *
    *   test functions they can be declared here!
    **********************************************/

    // this helper outputs all the nodes in the tree
    string out(Sat* satellite) const{
        string result = "";
        if (satellite != nullptr){
            result += "(";
            result += out(satellite->m_left);  // first visit the left child
            result += to_string(satellite->m_id) + ":" + to_string(satellite->m_height);  // second visit the node itself
            result += out(satellite->m_right);  // third visit the right child
            result += ")";
        }
        return result; 
    }
    
    // this helper makes sure that the tree is balanced
    bool balanceChecker(Sat* node) {
        // base case
        if (node == nullptr) {
            return true;
        }
        
        // initilaize varaibles
        int leftHeight = -1;
        int rightHeight = -1;
        
        if (node->m_left != nullptr) {
            leftHeight = node->m_left->m_height;
        }

        if (node->m_right != nullptr) {
            rightHeight = node->m_right->m_height;
        }
        
        // make sure that the difference is less than 1
        if (abs(leftHeight - rightHeight) <= 1) {
            // recursive call 
            if (balanceChecker(node->m_left) && balanceChecker(node->m_right)) {
                return true;
            }
        }
    
        return false;
    }

    // this helper makes sure that the bst property is maintained
    bool bstChecker(Sat* node, Sat* min = nullptr, Sat* max = nullptr) {
        // an empty network is a binary seach network by definition
        if (node == nullptr) {
            return true;
        }

        // check if the value is bounded by the max and min
        if (min != nullptr && node->getID() <= min->getID()) {
            return false;
        }
        if (max != nullptr && node->getID() >= max->getID()) {
            return false;
        }

        // recursive call 
        if (bstChecker(node->m_left, min, node) && bstChecker(node->m_right, node, max)){
            return true; 
        }

        return false;
    }
    
    // this makes sure that none of the nodes have the deorbited state
    bool removeDeorbitedChecker(Sat* node) {
        // base case
        if (node == nullptr) {
            return true;
        }
        
        // if deorbited state is present set as false
        if (node->getState() == DEORBITED) {
            return false;
        }
        
        // recurse on rhs and lhs
        if (!removeDeorbitedChecker(node->m_left) || !removeDeorbitedChecker(node->m_right)) {
            return false;
        }

        return true;
    }

    // makes sure that two given trees are equal 
    bool isEqual(const Sat* node1, const Sat* node2) const {
        // base case
        if (node1 == nullptr && node2 == nullptr) {
            return true;
        }

        // if one of them is nullptr and the other is not
        if (node1 == nullptr || node2 == nullptr) {
            return false;
        }
        
        // see if all the member vairables are the same
        if (node1->getID() != node2->getID() ||
            node1->getAlt() != node2->getAlt() ||
            node1->getInclin() != node2->getInclin() ||
            node1->getState() != node2->getState()) {
            return false;
        }
        
        // recurse on rhs and lhs
        return isEqual(node1->m_left, node2->m_left) && isEqual(node1->m_right, node2->m_right);
    }

};

int main(){
    Tester tester;

    cout << endl << "\033[1;31m=============================|Test Cases for the SATNET Class|==============================\033[0m" << endl;

    cout << "________________________________________________________" << endl; 
    
    cout << "Test the insertion function for a normal case." << endl; 

    if (tester.insertNormal()) {
        cout << "\033[1;32mSUCCESS\033[0m insert function passed for a normal test" << endl;
    } 
    else {
        cout << "FAILURE: insert function failed for a normal test" << endl;
    }

    cout << "________________________________________________________" << endl; 
    
    cout << "Test the insertion function for a edge case where duplicates are inserted." << endl; 

    if (tester.insertEdge()) {
        cout << "\033[1;32mSUCCESS\033[0m insert function passed for a edge test" << endl;
    } 
    else {
        cout << "FAILURE: insert function failed for a edge test" << endl;
    }

    cout << "________________________________________________________" << endl; 
    
    cout << "Test the insertion function for a error case where nodes with invalid IDs are inserted." << endl; 

    if (tester.insertError()) {
        cout << "\033[1;32mSUCCESS\033[0m insert function passed for a error test" << endl;
    } 
    else {
        cout << "FAILURE: insert function failed for a error test" << endl;
    }

    cout << "________________________________________________________" << endl; 
    
    cout << "Test whether the network is balanced after a decent number of insertions, e.g. 300 insertions." << endl; 

    if (tester.balanceCheckerInsert()) {
        cout << "\033[1;32mSUCCESS\033[0m balance check passed for a normal test"<< endl;
    } 
    else {
        cout << "FAILURE: balance check failed for a normal test" << endl;
    }

    cout << "________________________________________________________" << endl; 
    
    cout << "Test whether the BST property is preserved after all insertions. (Note: this requires visiting all nodes and comparing key values)" << endl; 

    if (tester.bstCheckerInsert()) {
        cout << "\033[1;32mSUCCESS\033[0m BST check passed for a normal test"<< endl;
    } 
    else {
        cout << "FAILURE: BST check failed for a normal test" << endl;
    }

    cout << "________________________________________________________" << endl; 

    cout << "Test the remove function for a normal case." << endl; 

    if (tester.removeNormal()) {
        cout << "\033[1;32mSUCCESS\033[0m remove function passed for a normal test" << endl;
    } 
    else {
        cout << "FAILURE: remove function failed for a normal test" << endl;
    }

    cout << "________________________________________________________" << endl; 

    cout << "Test the remove function for a edge case where you are removing values that don't exist." << endl; 

    if (tester.removeEdge()) {
        cout << "\033[1;32mSUCCESS\033[0m remove function passed for a edge test" << endl;
    } 
    else {
        cout << "FAILURE: remove function failed for a edge test" << endl;
    }

    cout << "________________________________________________________" << endl; 

    cout << "Test whether the network is balanced after multiple removals. For example, insert 300 nodes, then remove 150, and check the property." << endl; 

    if (tester.balanceCheckerRemove()) {
        cout << "\033[1;32mSUCCESS\033[0m balance check passed for a normal test"<< endl;
    } 
    else {
        cout << "FAILURE: balance check failed for a normal test" << endl;
    }

    cout << "________________________________________________________" << endl; 

    cout << "Test whether the BST property is preserved after multiple removals." << endl; 

    if (tester.bstCheckerRemove()) {
        cout << "\033[1;32mSUCCESS\033[0m BST check passed for a normal test"<< endl;
    } 
    else {
        cout << "FAILURE: BST check failed for a normal test" << endl;
    }

    cout << "________________________________________________________" << endl; 

    cout << "Test the removeDeorbited() functionality for a normal test" << endl; 

    if (tester.removeDeorbitedNormal()) {
        cout << "\033[1;32mSUCCESS\033[0m RemoveDeorbited passed for a normal test"<< endl;
    } 
    else {
        cout << "FAILURE: RemoveDeorbited failed for a normal test" << endl;
    }

    cout << "________________________________________________________" << endl; 

    cout << "Test the removeDeorbited() functionality for a edge test where there are no deorbited nodes" << endl; 

    if (tester.removeDeorbitedEdge()) {
        cout << "\033[1;32mSUCCESS\033[0m RemoveDeorbited passed for a edge test"<< endl;
    } 
    else {
        cout << "FAILURE: RemoveDeorbited failed for a edge test" << endl;
    }

    cout << "________________________________________________________" << endl; 

    cout << "Test the countSatellites() functionality for a normal case" << endl; 

    if (tester.countSatellitesNormal()) {
        cout << "\033[1;32mSUCCESS\033[0m countSatellites passed for a normal test"<< endl;
    } 
    else {
        cout << "FAILURE: countSatellites failed for a normal test" << endl;
    }

    cout << "________________________________________________________" << endl; 

    cout << "Test the countSatellites() functionality for a edge case where no satellites of that incline are there" << endl; 

    if (tester.countSatellitesEdge()) {
        cout << "\033[1;32mSUCCESS\033[0m countSatellites passed for a edge test"<< endl;
    } 
    else {
        cout << "FAILURE: countSatellites failed for a edge test" << endl;
    }

    cout << "________________________________________________________" << endl; 

    cout << "Test the findSatellite(...) functionality for a normal case." << endl; 

    if (tester.findSatelliteNormal()) {
        cout << "\033[1;32mSUCCESS\033[0m findSatellite passed for a normal test"<< endl;
    } 
    else {
        cout << "FAILURE: findSatellite failed for a normal test" << endl;
    }

    cout << "________________________________________________________" << endl; 

    cout << "Test the findSatellite(...) functionality for a error case where no such node exists" << endl; 

    if (tester.findSatelliteError()) {
        cout << "\033[1;32mSUCCESS\033[0m findSatellite passed for a error test"<< endl;
    } 
    else {
        cout << "FAILURE: findSatellite failed for a error test" << endl;
    }

    cout << "________________________________________________________" << endl; 

    cout << "Test the assignment operator for a normal case." << endl; 

    if (tester.assignmentNormal()) {
        cout << "\033[1;32mSUCCESS\033[0m assignment operator passed for a normal test"<< endl;
    } 
    else {
        cout << "FAILURE: assignment operator failed for a normal test" << endl;
    }

    cout << "________________________________________________________" << endl; 

    cout << "Test the assignment operator for an error case, e.g. assigning an empty object to an empty object." << endl; 

    if (tester.assignmentError()) {
        cout << "\033[1;32mSUCCESS\033[0m assignment operator passed for a error test"<< endl;
    } 
    else {
        cout << "FAILURE: assignment operator failed for a error test" << endl;
    }

    cout << "________________________________________________________" << endl; 

    cout << "Prove that the removal performs in O(log n). " << endl; 

    if (tester.removePerformanceChecker()) {
        cout << "\033[1;32mSUCCESS\033[0m The removal passed for a time complexity test"<< endl;
    } 
    else {
        cout << "FAILURE: The removal failed for a time complexity test" << endl;
    }

    cout << "________________________________________________________" << endl; 

    cout << "Prove that the insertion performs in O(log n). " << endl; 

    if (tester.insertPerformanceChecker()) {
        cout << "\033[1;32mSUCCESS\033[0m The insertion passed for a time complexity test"<< endl;
    } 
    else {
        cout << "FAILURE: The insertion failed for a time complexity test" << endl;
    }

    cout << "________________________________________________________" << endl; 

    cout << "Test the setState functionality for a normal case" << endl; 

    if (tester.setStateNormal()) {
        cout << "\033[1;32mSUCCESS\033[0m setState passed for a normal test"<< endl;
    } 
    else {
        cout << "FAILURE: setState failed for a normal test" << endl;
    }

    cout << "________________________________________________________" << endl; 

    cout << "Test the setState functionality for a edge case where it was called on a node that was not in the tree" << endl; 

    if (tester.setStateEdge()) {
        cout << "\033[1;32mSUCCESS\033[0m setState passed for a edge test"<< endl;
    } 
    else {
        cout << "FAILURE: setState failed for a edge test" << endl;
    }

    cout << "________________________________________________________" << endl; 

    cout << "Test the clear functionality for a normal case" << endl; 

    if (tester.clearNormal()) {
        cout << "\033[1;32mSUCCESS\033[0m clear passed for a normal test"<< endl;
    } 
    else {
        cout << "FAILURE: clear failed for a normal test" << endl;
    }

    cout << "________________________________________________________" << endl; 

    cout << "Test the clear functionality for a edge case where the network was already empty" << endl; 

    if (tester.clearEdge()) {
        cout << "\033[1;32mSUCCESS\033[0m clear passed for a edge test"<< endl;
    } 
    else {
        cout << "FAILURE: clear failed for a edge test" << endl;
    }
    
    return 0;
}
