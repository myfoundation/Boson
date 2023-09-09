


#include "BalancedIndex.h"
#include <ios>

using namespace Boson;

/*
* @brief Creates new index node in file
*/
Node::Node(BalancedIndex& bi, NodeType type) : index(bi) {   
    // initialize values
    memset(&data, 0, sizeof NodeData);
    this->data.nodeType = type;
    this->data.keysCount = 0;    
    // allocate space in file
    RecordFileIO& recordFile = index.getRecordsFile();
    uint64_t offset = recordFile.createRecord(&data, sizeof NodeData);
    if (offset == NOT_FOUND) {
        throw std::ios_base::failure("Can't write node data.");
    }
    this->position = offset;
    this->isPersisted = true;
}


/*
* @brief Loads node data from specified position in file
*/
Node::Node(BalancedIndex& bi, uint64_t offsetInFile) 
    : index(bi), position(offsetInFile)
{    
    // load node data from specified offset in file
    RecordFileIO& recordsFile = index.getRecordsFile();
    recordsFile.setPosition(position);
    uint64_t offset = recordsFile.getRecordData(&data, sizeof NodeData);
    // Throw exception if file not open, can't read or checksum check failed
    if (offset == NOT_FOUND) {
        // TODO: maybe we can provide more useful information
        throw std::ios_base::failure("Can't read node data.");
    } 
    // Set flag that data is already persisted
    isPersisted = true;
}


/*
* @brief Checks if node data persisted
*/
Node::~Node() {
    if (!isPersisted) persist();
}


/*
*
* @brief Persists node data to the storage
*
* @return returns current offset of record or NOT_FOUND if fails
*
*/
void Node::persist() {
    // write node data to specified position
    RecordFileIO& recordsFile = index.getRecordsFile();
    recordsFile.setPosition(position);    
    // Throw exception if file not open or can't write
    uint64_t offset = recordsFile.setRecordData(&data, sizeof NodeData);
    if (offset == NOT_FOUND) {
        throw std::ios_base::failure("Can't persist node data.");
    }
    // Offset of record in the file could have been changed, so we update it
    position = offset;
    // Set flag that data is already persisted
    isPersisted = true;
}


NodeType Node::getNodeType() {
    return data.nodeType;
}

/*
*  @brief Returns keys count inside Node
*  @return total keys count inside node
*/
uint32_t Node::getKeyCount() {
    return data.keysCount;
}


/*
*  @brief Returns whether node keys count > M-1
*  @return true if keys count more than MAX_DEGREE
*/
bool Node::isOverflow() {
    return data.keysCount > MAX_DEGREE;
}


/*
*  @brief Returns whether node keys count < M / 2
*  @return true if keys count less than MIN_DEGREE
*/
bool Node::isUnderflow() {
    return data.keysCount < MIN_DEGREE;
}


/*
*  @brief Returns whether node keys count > M / 2
*  @return true if keys count more than MIN_DEGREE
*/
bool Node::canLendAKey() {
    return data.keysCount > MIN_DEGREE;
}


/*
*  @brief Returns key at specified index
*/
uint64_t Node::getKeyAt(uint32_t index) {
    if (index >= data.keysCount) return NOT_FOUND;
    return data.keys[index];
}


/*
*  @brief Sets key at specified index
*/
void Node::setKeyAt(uint32_t index, uint64_t key) {
    if (index >= data.keysCount) return;
    data.keys[index] = key;
    this->isPersisted = false;
}


/*
*  @brief Returns Parent node position in file
*/
uint64_t Node::getParent() {
    return data.parent;
}


/*
*  @brief Sets Parent node position in file
*/
void Node::setParent(uint64_t parentPosition) {
    // TODO what if record position changed (not real?)
    data.parent = parentPosition;
    this->isPersisted = false;
}


uint64_t Node::getLeftSibling() {
    return data.leftSibling;
}


void Node::setLeftSibling(uint64_t siblingPosition) {
    data.leftSibling = siblingPosition;
    this->isPersisted = false;
}


uint64_t Node::getRightSibling() {
    return data.rightSibling;
}


void Node::setRightSibling(uint64_t siblingPosition) {
    data.rightSibling = siblingPosition;
    this->isPersisted = false;
}


uint64_t Node::dealOverflow() {
    return NOT_FOUND;
}


uint64_t Node::dealUnderflow() {
    return NOT_FOUND;
}
