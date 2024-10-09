/******************************************************************************
*
*  LeafNode class implementation
*
*  (C) Boson Database, Bolat Basheyev 2022-2024
*
******************************************************************************/

#include "BalancedIndex.h"
#include <ios>

using namespace Boson;


/*
* @brief Leaf Node Constructor (calls Node Constructor)
* @param bi BalancedIndex object
*/
LeafNode::LeafNode(BalancedIndex& bi) : Node(bi, NodeType::LEAF) {

}


/*
* @brief Leaf Node Constructor (used from Node::loadNode)
* @param bi BalancedIndex object
* @param offsetInFile position in the storage file
* @param loadedData NodeData object with loaded data
*/
LeafNode::LeafNode(BalancedIndex& bi, uint64_t offsetInFile, NodeData& loadedData) : Node(bi) {
    position = offsetInFile;
    memcpy(&(this->data), &loadedData, sizeof NodeData);
    isPersisted = true;
}


/*
* @brief Leaf Node Destructor
*/
LeafNode::~LeafNode() {
    if (!isPersisted) persist();
}


/*
* @brief Search index of key in this node (binary search in sorted array)
* @param key required key
* @return index of child node of the specified key
*/
uint32_t LeafNode::search(uint64_t key) {
    int32_t start = 0;                            // we need signed integers for
    int32_t end = data.keysCount - 1;             // correct comparison in while loop
    uint32_t mid;                                 // middle index can not be negative
    uint64_t entry;                               // variable to hold value

    while (start <= end) {                        // while start index <= end index
        mid = start + (end - start) / 2;          // calculate middle index between start & end
        entry = data.keys[mid];                   // get value in keys array at middle index
        if (entry == key) return mid;             // if value equals to key return index
        if (key < entry) end = mid - 1; else      // if key < value bound end index to middle-1
            if (key > entry) start = mid + 1;     // if key > value bound start index to middle+1 
    }

    return NOT_FOUND_KEY;                             // Key is definitely not found
}


/*
*  @brief Return value at specified index in this node
*  @param index of value
*  @return returns value string
*/
std::shared_ptr<std::string> LeafNode::getValueAt(uint32_t index) {

    // Go to required position in storage file
    RecordFileIO& recordsFile = this->index.getRecordsFile();
    uint64_t offsetInFile = data.values[index];
    recordsFile.setPosition(offsetInFile);    

    // load data from storage file record
    uint32_t valueLength = recordsFile.getDataLength() + 1;
    std::unique_ptr<char> cStr = std::make_unique<char>(valueLength);
    uint64_t offset = recordsFile.getRecordData(cStr.get(), valueLength);
    if (offset == NOT_FOUND) throw std::ios_base::failure("Can't read value.");

    // add null terminator to C style string
    cStr.get()[valueLength - 1] = 0; 

    // convert to C++ string and return as shared pointer
    return std::make_shared<std::string>(cStr.get());
}



/*
*  @brief Set value at specified index in this node
*  @param index of value
*  @param value string
*/
void LeafNode::setValueAt(uint32_t index, const std::string& value) {
    
    // Go to required position in storage file
    RecordFileIO& recordsFile = this->index.getRecordsFile();
    uint64_t offsetInFile = data.values[index];
    recordsFile.setPosition(offsetInFile);

    // Write value to the storage file
    uint32_t valueLength = value.length() + 1;
    const char* cStr = value.c_str();
    uint64_t offset = recordsFile.setRecordData(cStr, valueLength);
    if (offset == NOT_FOUND) throw std::ios_base::failure("Can't write value.");

    // update offset if its changed
    data.values[index] = offset;
    isPersisted = false;
}


/*
*  @brief Insert key/value pair to this node in sorted order
*  @param key
*  @param value
*/
bool LeafNode::insertKey(uint64_t key, const std::string& value) {
    // find index to insert new key/value pair in sorted order
    size_t insertIndex = searchPlaceFor(key);
    if (insertIndex == NOT_FOUND) return false;
    // insert key/value
    insertAt(insertIndex, key, value);
    return true;
}


/*
*  @brief Insert key/value pair at specified index in this node
*  @param index
*  @param key
*  @param value
*/
void LeafNode::insertAt(uint32_t index, uint64_t key, const std::string& value) {

}


bool LeafNode::deleteKey(uint64_t key) {
    return false;
}


void LeafNode::deleteAt(uint32_t index) {

}


uint64_t LeafNode::split() {
    return NOT_FOUND;
}


void LeafNode::merge(uint64_t key, uint64_t siblingRight) {

}


uint64_t LeafNode::pushUpKey(uint64_t key, uint64_t leftChild, uint64_t rightChild) {
    return NOT_FOUND;
}


void LeafNode::borrowChildren(uint64_t borrower, uint64_t lender, uint32_t borrowIndex) {

}


uint64_t LeafNode::mergeChildren(uint64_t leftChild, uint64_t rightChild) {
    return NOT_FOUND;
}


void LeafNode::mergeWithSibling(uint64_t key, uint64_t rightSibling) {

}


uint64_t LeafNode::borrowFromSibling(uint64_t key, uint64_t sibling, uint32_t borrowIndex) {
    return NOT_FOUND;
}


NodeType LeafNode::getNodeType() {
    return NodeType::LEAF;
}


uint32_t LeafNode::searchPlaceFor(uint64_t key) {

    return NOT_FOUND_KEY;
}

