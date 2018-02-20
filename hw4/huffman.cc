/*
 * huffman.cc: implementaion of the api in huffman.hh
 * Written by Alex Grant
 */

#include "huffman.hh"
#include "ptrtree.hh"

#include <algorithm>
#include <unordered_map>
#include <queue>
#include <iostream>

// max number of characters in the encoding we're using
const unsigned NUM_CHARS = 256;
const unsigned EOF_NUM = 256;

namespace huffman {

///////////////////////////////////////////////////////////////////////////////
// This will hold all of the data for our implementation
struct Huffman::Impl {
    tree::PtrTree* hTree; // our huffman tree
    // the values in this map will be the frequencies and keys are characters
    std::unordered_map<unsigned, int> freqMap;
    Impl() noexcept;
    ~Impl() noexcept;
    class treeComp;
    void incFreqHelper(unsigned sym);
    Huffman::encoding_t encodeHelper(unsigned sym) const;
};

Huffman::Impl::Impl() noexcept {
    // initialize each element to 0
    for (unsigned i = 0; i < NUM_CHARS; i++){
        freqMap.insert({static_cast<symbol_t>(i), 0}); 
    }
    hTree = new tree::PtrTree(std::make_pair(EOF_NUM, 0));
}

Huffman::Impl::~Impl() noexcept {
    delete hTree;
}

// will be used by priority queue to compare the frequencies of the characters
// of two trees
class Huffman::Impl::treeComp
{
public:
    bool operator() 
    (const tree::PtrTree* lhs, const tree::PtrTree* rhs) const{
        auto lhsval = (*lhs).getByPath("");
        auto rhsval = (*rhs).getByPath("");
         
        // also prioritizes nodes that represent actual characters
        // because this will help keep our tree shorter if values without
        // parents get parents first
        if(lhsval.second == rhsval.second) {
            // we want to return true if rhs is a real character and lhs
            // is not. We want to return false if lhs is a real character
            // and rhs is not. We don't care if they are both of the same
            // kind
            return rhsval.first < NUM_CHARS;
        }

        return lhsval.second > rhsval.second;
    }
};

// The huffman incFreq acts as a wrapper to this class that only takes
// symbols. This one takes unsigned so i can incremement the eof character
// it uses a priority queue to build up the tree with the least frequent elements on the bottom
void
Huffman::Impl::incFreqHelper(unsigned sym) {
    delete hTree;
    freqMap[sym]++; 
    //build priority queue
    std::priority_queue<tree::PtrTree*, 
                        std::vector<tree::PtrTree*>, 
                        Impl::treeComp> treeQ;
    for (std::pair<unsigned, int> el: freqMap) {
        treeQ.push(new tree::PtrTree(el));
    }
    // now we build the tree
    // we combine the two least frequent elements in the queue under one parent,
    // then we put that parent back on the queue
    while (treeQ.size() > 1) {
        auto tree1 = treeQ.top();
        treeQ.pop();
        auto tree2 = treeQ.top();
        treeQ.pop();
        // freq(tree3) = freq(tree1) + freq(tree2)
        auto tree3 = new tree::PtrTree
            (std::make_pair(
                NUM_CHARS + 1, 
                (*tree1).getByPath("").second + (*tree2).getByPath("").second
             ),
            *tree2, *tree1);
        treeQ.push(tree3);
    }
    hTree = treeQ.top(); 
}


///////////////////////////////////////////////////////////////////////////////
// another helper
// we just use ptrtree's pathTo method to get the path down the tree to
// the symbol we want
Huffman::encoding_t 
Huffman::Impl::encodeHelper(unsigned sym) const {
    auto freq = freqMap.at(sym);
    std::string pathLR = hTree->pathTo(std::make_pair(sym, freq));
    // convert lrs to bits
    encoding_t myEnc;
    myEnc.resize(pathLR.size());
    std::transform(pathLR.begin(), pathLR.end(), myEnc.begin(),
        [](char d){
            if(d == 'L'){
                return ZERO;
            } else if (d=='R'){
                return ONE;
            }else{
                throw std::runtime_error("Invalid path character");
            }
        });
    return myEnc;
}

///////////////////////////////////////////////////////////////////////////////
Huffman::Huffman() noexcept 
: pImpl_(new Impl) 
{
    pImpl_->incFreqHelper(EOF_NUM);
}

///////////////////////////////////////////////////////////////////////////////
Huffman::~Huffman() noexcept {}

///////////////////////////////////////////////////////////////////////////////
void
Huffman::incFreq(symbol_t sym){
    pImpl_->incFreqHelper(static_cast<unsigned>(sym));
}

///////////////////////////////////////////////////////////////////////////////
Huffman::encoding_t 
Huffman::encode(symbol_t sym) const {
    return pImpl_->encodeHelper(static_cast<unsigned>(sym));
}

///////////////////////////////////////////////////////////////////////////////
// similar to encode, we use ptrtree's getByPath to find the symbol at the 
// path we were given. We then move begin up however long the encoding was.
Huffman::symbol_t 
Huffman::decode(enc_iter_t& begin, const enc_iter_t& end) const noexcept(false) {
    // Convert 0s and 1s to Ls and Rs
    std::string pathLR;
    pathLR.resize(end-begin);
    std::transform(begin, end, pathLR.begin(),
        [](bit_t d){
            if(d == ZERO){
                return 'L';
            } else if (d == ONE){
                return 'R';
            }else{
                throw std::runtime_error("Invalid encoding bit");
            }
        });
    // want to stop decoding if we see an EOF
    auto mySymbol = pImpl_->hTree->getByPath(pathLR).first;
    if (mySymbol == EOF_NUM) {
        begin = end;
        return 0;
    } 
    // move begin up past symbol
    auto enc = pImpl_->encodeHelper(mySymbol);
    begin += enc.size();
    return static_cast<symbol_t>(mySymbol);
}

Huffman::encoding_t 
Huffman::eofCode() const{
    return pImpl_->encodeHelper(EOF_NUM);
}
}// end namespace
