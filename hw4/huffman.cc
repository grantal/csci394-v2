
/*
 * huffman.cc: implementaion of the api in huffman.hh
 * Written by Alex Grant
 */

#include "huffman.hh"
#include "ptrtree.hh"

// max number of characters in the encoding we're using
const unsigned NUM_CHARS = 256;

namespace huffman {

///////////////////////////////////////////////////////////////////////////////
// This will hold all of the data for our implementation
struct Huffman::Impl {
    std::shared_ptr<tree::PtrTree> hTree; // our huffman tree
};

///////////////////////////////////////////////////////////////////////////////
Huffman::Huffman() noexcept {
    // eof automatically has frequency of 1
    // tree::PtrTree eofTree = 
    // this loop will build up each "row" of the tree
    // the first row will be the inidivdual characters, next row will be
    // groups of two characters, then four and so on
    std::shared_ptr<std::vector<std::shared_ptr<tree::PtrTree>>> lastRow;
    for (int rowLength = NUM_CHARS; rowLength > 0; rowLength /= 2){
        auto currRow = std::make_shared<std::vector<std::shared_ptr<tree::PtrTree>>>(rowLength);
        for (int i = 0; i < rowLength; i++){
            // if we're on the first loop, we don't attach any trees
            if (lastRow){
                // the children of this row are gonna be every pair of the
                // last row
                auto left = (*lastRow)[i*2];
                auto right = (*lastRow)[i*2 + 1];
                // all nodes except bottom row will have the same character,
                // which will not be a valid character
                auto myTree = std::make_shared<tree::PtrTree>
                    (std::make_tuple(0, NUM_CHARS + 1), *left, *right);
                (*currRow).push_back(myTree); 
            } else {
                // want to set this node to the ith character in our 'alphabet'
                auto myId = static_cast<Huffman::symbol_t>(i);
                auto myTree = std::make_shared<tree::PtrTree>
                    (std::make_tuple(0, myId));
                (*currRow).push_back(myTree); 
            }
        }
        lastRow = currRow;
    }
    // at the end of our loop, lastrow should just contain one item:
    // the root of our tree
    Impl myImpl; 
    myImpl.hTree = (*lastRow)[0];
}

///////////////////////////////////////////////////////////////////////////////
Huffman::~Huffman() noexcept {
}

///////////////////////////////////////////////////////////////////////////////
Huffman::encoding_t 
Huffman::encode(symbol_t symbol) const {

}

///////////////////////////////////////////////////////////////////////////////
Huffman::symbol_t 
Huffman::decode(enc_iter_t& begin, const enc_iter_t& end) const noexcept(false) {

}

Huffman::encoding_t 
Huffman::eofCode() const{

}
}// end namespace
