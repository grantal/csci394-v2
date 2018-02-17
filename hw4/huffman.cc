/*
 * huffman.cc: implementaion of the api in huffman.hh
 * Written by Alex Grant
 */

#include "huffman.hh"
#include "ptrtree.hh"

#include <algorithm>
#include <unordered_map>
#include <queue>

// max number of characters in the encoding we're using
const unsigned NUM_CHARS = 256;
const unsigned EOF_NUM = 0;


namespace huffman {

///////////////////////////////////////////////////////////////////////////////
// This will hold all of the data for our implementation
struct Huffman::Impl {
    std::shared_ptr<tree::PtrTree> hTree; // our huffman tree
    // the values in this map will be the frequencies and keys are characters
    std::unordered_map<symbol_t, int> freqMap;
    Impl() noexcept;
    class treeComp;
};

Huffman::Impl::Impl() noexcept {
    // initialize each element to 0
    for (unsigned i = 0; i < NUM_CHARS; i++){
        freqMap.insert({static_cast<symbol_t>(i), 0}); 
    }
    // this loop will build up each "row" of the tree
    // the first row will be the inidivdual characters, next row will be
    // groups of two characters, then four and so on
    /*
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
    hTree = (*lastRow)[0];
    */
}

// will be used by priority queue to compare the frequencies of the characters
// of two trees
class Huffman::Impl::treeComp
{
public:
    bool operator() 
    (const std::shared_ptr<tree::PtrTree> lhs, const std::shared_ptr<tree::PtrTree> rhs) const{
        return std::get<1>((*lhs).getByPath("")) < std::get<1>((*rhs).getByPath(""));
    }
};


///////////////////////////////////////////////////////////////////////////////
Huffman::Huffman() noexcept 
: pImpl_(new Impl) 
{
    incFreq(EOF_NUM);
}

///////////////////////////////////////////////////////////////////////////////
Huffman::~Huffman() noexcept {}

///////////////////////////////////////////////////////////////////////////////
void
Huffman::incFreq(symbol_t sym){
    pImpl_->freqMap[sym]++; 
    //build priority queue
    std::priority_queue<std::shared_ptr<tree::PtrTree>, 
                        std::vector<std::shared_ptr<tree::PtrTree>>, 
                        Impl::treeComp> treeQ;
    for (std::pair<symbol_t, int> el: pImpl_->freqMap) {
        treeQ.push(std::make_shared<tree::PtrTree>
            (std::make_pair(static_cast<unsigned>(el.first), el.second))
        );
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
        auto tree3 = std::make_shared<tree::PtrTree>
            (std::make_pair(
                NUM_CHARS + 1, 
                (*tree1).getByPath("").second + (*tree2).getByPath("").second
             ),
            *tree1, *tree2);
        treeQ.push(tree3);
    }
    pImpl_->hTree = treeQ.top(); 
}

///////////////////////////////////////////////////////////////////////////////
Huffman::encoding_t 
Huffman::encode(symbol_t symbol) const {
    auto freq = pImpl_->freqMap[symbol];
    std::string pathLR = pImpl_->hTree->pathTo(std::make_pair(symbol, freq));
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
Huffman::symbol_t 
Huffman::decode(enc_iter_t& begin, const enc_iter_t& end) const noexcept(false) {
    end - begin;
    return 0;
}

Huffman::encoding_t 
Huffman::eofCode() const{
    return encode(EOF_NUM);
}
}// end namespace
