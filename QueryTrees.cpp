/*
 Title: QueryTrees.cpp
 Author: Emma Kimlin
 Created on: February 28th, 2016
 Description: This is a test program that makes use of a parser to input data from a file and into a BST or AVL tree. 
        The user can then query the tree using recognition sequences.
 Usage: QueryTrees <database filename> <flag>, where <flag> is either "BST" or "AVL"
 Build with: make QueryTrees
 Modifications:
    3.1.2016, Emma Kimlin: Debugged FillTree() and added functionality to insert into AVL Tree.
    3.6.2016, Emma Kimlin: Modified FillTree() so that it skips over header. 
    3.8.2016 Made class QueryTree so that this is neater. 
 */

#include "BinarySearchTree.h"
#include "AvlTree.h"
#include "SequenceMap.h"
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>

template <typename TreeType>
class QueryTree {
public: 
    /**
      * One Parameter Constructor
     */
    QueryTree(std::string filename) {
        db_filename = filename;
        CheckFile();
        FillTree();
    }
    /**
      * TestQueryTree allows user to query a tree using a recognition sequence that they will be prompted to enter at
      * keyboard. 
    */
    void TestQueryTree();
private:
    std::string db_filename;
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
};

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <databasefilename> <tree-type>" << std::endl;
        return 0;
    }
    std::string db_filename(argv[1]);
    std::string param_tree(argv[2]);
    std::cout << "Input filename is " << db_filename << std::endl;
    
    if (param_tree == "BST") {
        std::cout << "I will run the BST code" << std::endl;
        QueryTree<BinarySearchTree<SequenceMap> > a_tree(db_filename);
        a_tree.TestQueryTree();
    } else if (param_tree == "AVL") {
        std::cout << "I will run the AVL code" << std::endl;
        QueryTree<AvlTree<SequenceMap> > a_tree(db_filename);
        a_tree.TestQueryTree();
    } else
        std::cout << "Unknown tree type " << param_tree << " (User should provide BST, or AVL)" << std::endl;
    
    return 0;
}

template <typename TreeType>
void QueryTree<TreeType>::CheckFile() {
    ifstream inStream(db_filename);
    if (inStream.fail()) {
        std::cerr <<"Filename not found." << std::endl;
        exit(1);
    }
}

template <typename TreeType>
void QueryTree<TreeType>::FillTree () {
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
std::string QueryTree<TreeType>::GetEnzymeAcronym( std::string db_line, size_t first_slash) {
    std::string enzyme_acro_ = db_line.substr(0, first_slash);
    return enzyme_acro_;
}

template <typename TreeType>
bool QueryTree<TreeType>::GetNextRecognitionSequence(std::string db_line, std::string& a_reco_seq, size_t& first_slash) {
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
void QueryTree<TreeType>::TestQueryTree() { 
    char ans;
    std::string rec_seq;
    do {
        std::cout << "Query this tree by entering one enzyme acronym and pressing 'Enter'\n" << std::endl;
        std::cin >> rec_seq;
        a_tree.find_and_print(rec_seq);
        std::cout << "Would you like to query this tree again? Enter 'N' if no, \n"
                  << "and any other letter if yes." << std::endl;
        std::cin >> ans;
        }
    while (ans != 'N'); 
}