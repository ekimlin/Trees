/*
 Title: TestRangeQuery.cpp
 Author: Emma Kimlin
 Created on: March 10th, 2016
 Description: Test the AVL function print_Between()
 Usage: TestRangeQuery <database filename> <key1> <key2> where key1 and key2 are recognition sequences
 Build with: make TestRangeQuery
 Modifications:
 */

#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include "SequenceMap.h"
#include "BinarySearchTree.h"
#include "AvlTree.h"

/**
  * Checks to make sure a file opens and issues an error message if not.
 */
void CheckFile(std::string db_filename);

/** 
  * FillTree() takes the input from db_filename, parses it and stores it in either an AVL Tree or BST made of
  *     SequenceMap objects.
  * Pre-Condition: a_tree is an empty BST or AVL tree. 
  * Pre-Condition: Each line in db_filename in this format: EnzymeAcronym/Sequence1/Sequence2/.../Sequence n//
  * Pre-Condition: The file provided has a 10 line header that will be skipped over. 
 */
template <typename TreeType>
void FillTree(std::string db_filename, TreeType &a_tree);

/**
  * GetEnzymeAcronym() takes a line from a file and returns the Enzyme Acronym.
  * Pre-Condition: The line is in a particular format where the Enzyme Acroynm appears first, followed by a '/'. 
 */
std::string GetEnzymeAcronym(const std::string& db_line, size_t first_slash);

/** 
  * Parses the db_line string to pull out a sequence. It is intended to be used as a
  *     condition to a while loop, as it will continually pull the next sequence from the db_line until no more sequences remain.
  * Pre-Condition: Each sequence in db_line is separated by a '/' and db_line ends in "//". Recognition Sequences begin after the
  *     first '/'
 */
bool GetNextRecognitionSequence(std::string db_line, std::string& a_reco_seq, size_t& first_letter);

int main(int argc, char **argv) {
    if (argc != 4) {
        std::cout << "Usage: " << argv[0] << " <databasefilename> <queryfilename> <tree-type>" << std::endl;
        return 0;
    }
    std::string db_filename(argv[1]);
    std::string str1(argv[2]);
    std::string str2(argv[3]);
    std::cout << "Input file is " << db_filename << " ";
    std::cout << "String 1 is " << str1 << "   and string 2 is " << str2 << std::endl;
    CheckFile(db_filename);
    
    AvlTree<SequenceMap> a_tree;
    FillTree(db_filename, a_tree);
    a_tree.print_Between(str1, str2);
    return 0;
}

void CheckFile(std::string db_filename) {
    ifstream inStream(db_filename);
    if (inStream.fail()) {
        std::cerr <<"Filename not found." << std::endl;
        exit(1);
    }
}
template <typename TreeType>
void FillTree (std::string db_filename, TreeType &a_tree) {
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
std::string GetEnzymeAcronym(const std::string& db_line, size_t first_slash) {
    std::string enzyme_acro_ = db_line.substr(0, first_slash);
    return enzyme_acro_;
}

bool GetNextRecognitionSequence(std::string db_line, std::string& a_reco_seq, size_t& first_slash) {
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





