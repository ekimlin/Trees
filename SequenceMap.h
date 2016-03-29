/*
 Title: SequenceMap.h
 Author: Emma Kimlin
 Created on: February 28th, 2016
 Description: A Sequence Map object holds the name of a restriction enzyme, and the possible DNA sites this enzyme may cut.
 Modifications:
 */
#include <iostream>
#include <string>
#include <vector>

#ifndef SequenceMap_h
#define SequenceMap_h

class SequenceMap
{
public:
    // The Big Five:
    ~SequenceMap() = default;
    SequenceMap(const SequenceMap & rhs) = default;
    SequenceMap(SequenceMap && rhs) = default;
    SequenceMap& operator= (const SequenceMap & rhs) = default;
    SequenceMap& operator= (SequenceMap && rhs) = default;
    
    /** 
      * Two Parameter Constructor
      * Parameters: The constructor for a SequenceMap object takes two strings as arguments.
      * Pre-Condition:
     */
    SequenceMap(const std::string &a_rec_seq_, const std::string &enz_acro_) : recognition_sequence_(a_rec_seq_), enzyme_acronym_{enz_acro_} {}
    
    /**
      * Purpose: The < operator compares two SequenceMap objects based on regular string comparison between
      * the recognition sequence strings.
      * Post-Condition: Returns true if lhs < rhs. Returns false if lhs >= rhs
     */
    bool operator<(const SequenceMap &rhs) const {return recognition_sequence_ < rhs.recognition_sequence_;}
    
    /**
      * Purpose: The overloaded output operator allows user to print the contents of a Sequence_Map object
      * to the screen.
      * Post-Condition: The recognition_sequence_ string is printed followed by each value 
      * within enzyme_acroynm_ seperated by a space. Ends with a new line. 
     */
    friend std::ostream& operator<<(std::ostream& os, const SequenceMap& s_map)
    {
        os << s_map.recognition_sequence_ << " ";
        for (int i = 0; i < s_map.enzyme_acronym_.size() ; i++)
            os << s_map.enzyme_acronym_[i] << " ";
        os << std::endl;
        return os;
    }
    
    /**
      * Purpose: Merge merges the other_sequence.recognition_sequence_ with the object's recognition_sequence_.
      * Pre-Condition: Merge assumes the object's enzyme_acronym and other_sequence.enzyme_acroynme are equal.
      * Post-Condition: The calling object has been modified so that it includes other_sequence.recognition_sequence_. other_sequence has not been affected.
     */
    void Merge(const SequenceMap &other_sequence)
    {
        for (int i = 0; i < other_sequence.enzyme_acronym_.size(); i++)
            enzyme_acronym_.push_back(other_sequence.enzyme_acronym_[i]); //add the values in the argument's enzyme_acronym_ to the vector of the calling object's enzyme_acronym_
    }

    //Getter
    std::string getRecSequence() const {return recognition_sequence_;}
    
    /**
      * Prints enzyme_acronym_ in order with each element seperated by space. Ends with a new line. 
     */
    void Print_Vector() {
        for (int i = 0; i < enzyme_acronym_.size(); i++)
            std::cout << enzyme_acronym_[i] << " ";
        std::cout << std::endl;
    }
    
private:
    std::string recognition_sequence_;
    std::vector<std::string> enzyme_acronym_;
};

#endif /* SequenceMap_h */
