/*
 * ptrtree: implementaion of tree.hh using pointers
 * Written by Alex Grant
 */

#include "ptrtree.hh"


namespace tree {

///////////////////////////////////////////////////////////////////////////////
PtrTree::PtrTree(value_t value)
    : Tree(), value_(value), left_(nullptr), right_(nullptr)
    // since no children have been specified, set them to leaves
{ }

///////////////////////////////////////////////////////////////////////////////
PtrTree::PtrTree(value_t value, const PtrTree& left, const PtrTree& right)
    : Tree(), value_(value), left_((PtrTree*)&left), right_((PtrTree*)&right) 
{ }

///////////////////////////////////////////////////////////////////////////////
// Sets left_ to the given tree
void
PtrTree::setLeft(const PtrTree& left) {
    left_ = std::make_shared<PtrTree>(left);
}

///////////////////////////////////////////////////////////////////////////////
// Sets right_ to the given tree
void
PtrTree::setRight(const PtrTree& right) {
    right_ = std::make_shared<PtrTree>(right);
}

///////////////////////////////////////////////////////////////////////////////
unsigned
PtrTree::size() const
{
    unsigned retval = 1; // value to return
    if (left_ != nullptr){
        retval += left_->size();
    }
    if (right_ != nullptr){
        retval += right_->size();
    }
    // size will be 1 if this is leaf 
    return retval;
}

///////////////////////////////////////////////////////////////////////////////
// recursively check each child for the value
std::string
PtrTree::pathTo(value_t value) const
{
    std::string path_err_msg = "Value not found!";
    // base case
    if (value == value_) {
        return "";
    }
    const std::shared_ptr<PtrTree> branches[2] = {left_, right_};
    const char directions[2] = {'L', 'R'};
    std::string path = "";
    // loop through my two branches
    for (int i = 0; i < 2; ++i){
        if (branches[i] != nullptr) {
            try {
                path = branches[i]->pathTo(value);
                path.insert(path.begin(), directions[i]);
                break; // if left has the value, don't check right
            } catch (const std::runtime_error& e){
                //rethrow error if it isn't ours
                if(e.what() != path_err_msg){
                    throw e;
                } 
            }
        } 
    }
    // if neither branch had the value, throw the error
    if (path == ""){
        throw std::runtime_error(path_err_msg);
    }

    return path;
}


///////////////////////////////////////////////////////////////////////////////
// Follow path from root to end of string, returning value when done
PtrTree::value_t
PtrTree::getByPath(const std::string& path) const
{
    // base case
    if (path == ""){
        return value_;
    }

    // find next direction
    std::shared_ptr<PtrTree> dir;
    if (path[0] == 'L'){
        dir = left_;
    } else if (path[0] == 'R'){
        dir = right_;
    } else {
        throw std::runtime_error("Invalid character in path: " + path);
    }

    // recurse in that direction
    if (dir == nullptr){
        // if we can't continue on this path, return this node
        return value_;
    } else {
        return dir->getByPath(path.substr(1,path.length()));
    }
}
} // end namespace
