
/*
 * huffman.cc: implementaion of the api in huffman.hh
 * Written by Alex Grant
 */

#include "huffman.hh"
#include "ptrtree.hh"

// max number of characters in the encoding we're using
const int NUM_CHARS = 256;

namespace huffman {

///////////////////////////////////////////////////////////////////////////////
// This will hold all of the data for our implementation
struct Huffman::Impl {
    std::shared_ptr<tree::PtrTree> hTree; // our huffman tree
};

///////////////////////////////////////////////////////////////////////////////
Huffman::Huffman() noexcept {
    //Impl myImpl; 
    //myImpl.hTree = std::make_shared<tree::PtrTree>(0);
    // eof automatically has frequency of 1
    // tree::PtrTree eofTree = 
    // this loop will build up each "row" of the tree
    // the first row will be the inidivdual characters, next row will be
    // groups of two characters, then four and so on
    std::shared_ptr<std::vector<*tree::PtrTree>> lastRow;
}

///////////////////////////////////////////////////////////////////////////////
Huffman::~Huffman() noexcept {
}

///////////////////////////////////////////////////////////////////////////////
encoding_t encode(symbol_t symbol) const {

}

///////////////////////////////////////////////////////////////////////////////
symbol_t decode(enc_iter_t& begin, const enc_iter_t& end) const noexcept(false) {

}

encoding_t eofCode() const{
    return {0} 
}
}// end namespace
