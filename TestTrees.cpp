/*
 Title: TestTrees.cpp
 Author: Emma Kimlin
 Created on: March 1st, 2016
 Description: This is a test program that makes use of a parser to input data from a file and into a BST or AVL tree. 
        Data about the tree is printed to the screen, such as the number of nodes and the average depth. 
 Usage: TestTrees <database filename> <queries filename> <flag>,  where <flag> is either "BST" or "AVL"
 Build with: make TestTrees
 Modifications:
    3.6.2016, Emma Kimlin: Added search_Sequences() and remove_Sequences(). 
    3.8.2016 Made class TestTree so that this is neater. 
 */

#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include "SequenceMap.h"
#include "BinarySearchTree.h"
#include "AvlTree.h"

template <typename TreeType>
class TestTree {
public: 
    /**
      * One Parameter Constructor
     */
    TestTree(std::string database, std::string query) : db_filename(database), query_filename(query) {
        CheckFile();
        FillTree();
    }
    /**
      * Calculates the number of nodes, average depth, and ratio of average depth to log2n of this tree.
      * Pre-Condition: a_tree contains at least one element.
      * Post-Condition: The result of these calculations is outputted to screen. 
     */
    void TestQueryTree();
private:
    std::string db_filename;
    std::string query_filename;
    TreeType a_tree;
    /**
      * Takes the input from db_filename, parses it and stores it in either an AVL Tree or BST made of
      *     SequenceMap objects.
      * Pre-Condition: a_tree is an empty BST or AVL tree. 
      *     Each line in db_filename in this format: EnzymeAcronym/Sequence1/Sequence2/.../Sequence n//
      *     The file provided has a 10 line header that will be skipped over. 
     */
    void FillTree();

    /** 
     * Checks to make sure a file opens and issues an error message if not.
     */
    void CheckFile();

    /**
      * Takes a line from a file and returns the Enzyme Acronym.
      * Pre-Condition: The line is in a particular format where the Enzyme Acroynm appears first, followed by a '/'. 
    */
    std::string GetEnzymeAcronym(std::string db_line, size_t first_slash);

    /**
      * Parses the db_line string to pull out a sequence. It is intended to be used as a condition to a while loop, as it 
      *     will continually pull the next sequence from the db_line until no more sequences remain.
      * Pre-Condition: Each sequence in db_line is separated by a '/' and db_line ends in "//". Recognition Sequences begin after the
      *     first '/'
     */
    bool GetNextRecognitionSequence(std::string db_line, std::string& a_reco_seq, size_t& first_letter);
    /**
      * Searches a_tree for each sequence in file and returns total number of successful queries.  
     */
    void search_Sequences();

    /** 
      * Removes every other sequence in file from the tree. Prints the total number of successfull removes.
      * Prints the average number of recursion calls.
     */
    void remove_Sequences();
};

int main(int argc, char **argv) {
    if (argc != 4) {
        cout << "Usage: " << argv[0] << " <databasefilename> <queryfilename> <tree-type>" << endl;
        return 0;
    }
    string db_filename(argv[1]);
    string query_filename(argv[2]);
    string param_tree(argv[3]);
    cout << "Input file is " << db_filename << ", and query file is " << query_filename << endl;
    
    if (param_tree == "BST") {
        std::cout << "I will run the BST code" << std::endl;
        TestTree<BinarySearchTree<SequenceMap> > a_tree(db_filename, query_filename);
        a_tree.TestQueryTree();
    } else if (param_tree == "AVL") {
        std::cout << "I will run the AVL code" << std::endl;
        TestTree<AvlTree<SequenceMap> > a_tree(db_filename, query_filename);
        a_tree.TestQueryTree();
    } else
        std::cout << "Unknown tree type " << param_tree << " (User should provide BST, or AVL)" << std::endl;
    return 0;
}

template <typename TreeType>
void TestTree<TreeType>::CheckFile() {
    ifstream inStream(db_filename);
    if (inStream.fail()) {
        std::cerr <<"Filename not found." << std::endl;
        exit(1);
    }
}
template <typename TreeType>
void TestTree<TreeType>::FillTree () {
    ifstream inStream(db_filename);
    std::string db_line, an_enz_acro, a_reco_seq, garbage_line;
    for (int i = 0; i < 10; i ++)                               //Skip over the header and begin reading on line 11. 
        getline(inStream, garbage_line);
    while (std::getline (inStream, db_line)) {                  //inStream has reached line 11 and will begin to parse data.
        if (db_line.empty()) continue;
        size_t first_slash = db_line.find("/");
        an_enz_acro = GetEnzymeAcronym(db_line, first_slash);
        while (GetNextRecognitionSequence(db_line, a_reco_seq, first_slash)) {
            SequenceMap new_sequence_map(a_reco_seq, an_enz_acro);
            a_tree.insert(new_sequence_map);
        }
    }
}
template <typename TreeType>
std::string TestTree<TreeType>::GetEnzymeAcronym(std::string db_line, size_t first_slash) {
    std::string enzyme_acro_ = db_line.substr(0, first_slash);
    return enzyme_acro_;
}
template <typename TreeType>
bool TestTree<TreeType>::GetNextRecognitionSequence(std::string db_line, std::string& a_reco_seq, size_t& first_slash) {
    int length;
    size_t next_slash = db_line.find("/", first_slash+1);
    if (next_slash == first_slash + 1) //see if there are two '/' in a row
        return false; //there are no more recognition sequences (all db_lines end in "//")
    else {
        length = next_slash-first_slash-1;
        a_reco_seq = db_line.substr(first_slash+1, length);
        first_slash = next_slash;
        return true;
    }
}

template <typename TreeType>
void TestTree<TreeType>::TestQueryTree() 
{
    std::cout << "This tree has " << a_tree.count_Nodes() << " nodes.\n";
    std::cout << "The Average Depth of this tree is " << a_tree.calculate_Avg_Depth() << ".\n";
    std::cout << "The ratio of the average depth to log2n is " << a_tree.calculate_ratio() <<".\n";
    search_Sequences();
    remove_Sequences();
    std::cout << "After remove_Sequences(): \n";
    std::cout << "This tree has " << a_tree.count_Nodes() << " nodes.\n"; 
    std::cout << "The Average Depth of this tree is " << a_tree.calculate_Avg_Depth() << ".\n";
    std::cout << "The ratio of the average depth to log2n is " << a_tree.calculate_ratio() <<".\n";
}

template <typename TreeType>
void TestTree<TreeType>::search_Sequences()
{
    ifstream inStream(query_filename);
    std::string sequence;
    int success_query_count = 0, total_queries = 0, recursion_calls = 0, sum_recursion_calls = 0, avg_recursion_calls;
    while(std::getline(inStream, sequence)) {
        if (sequence.empty()) continue;
        total_queries++;
        if (a_tree.find(sequence, recursion_calls)) 
            success_query_count++;
        sum_recursion_calls += recursion_calls; 
    }
    avg_recursion_calls = sum_recursion_calls / total_queries;
    std::cout << "The number of sucessful queries was " << success_query_count << "." << std::endl;
    std::cout << "The total number of queries was " << total_queries << "." << std::endl;
    std::cout << "The average number of recursion calls for search_Sequences() was " << avg_recursion_calls << "." << std::endl;
}

template <typename TreeType>
void TestTree<TreeType>::remove_Sequences()
{
    ifstream inStream(query_filename);
    std::string sequence;
    int every_other = 0, success_delete_count = 0, recursion_calls = 0, sum_recursion_calls = 0, total_calls = 0;   
    while (std::getline(inStream, sequence)) {
        if (sequence.empty()) continue;
        if (every_other % 2 == 0) {           //If every_other is odd, don't call remove(). If it is even, call remove().  
            total_calls++;                              
            if (a_tree.remove(sequence, recursion_calls))             //Try to remove sequence from a_tree
                success_delete_count++; 
            sum_recursion_calls += recursion_calls;     //Even if delete wasn't successfull, still count this toward recursion call sum
        } 
        every_other++;                                      
    }
    double avg_recursion_calls = sum_recursion_calls / total_calls;
    std::cout << "The number of sucessful removes was " << success_delete_count << "." << std::endl;
    std::cout << "The average number of recursion calls for remove() was " << avg_recursion_calls << "." << std::endl; 
}



















