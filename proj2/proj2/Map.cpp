#include "Map.h"
#include <iostream>

using namespace std;

Map::Map()
// initializer list for private member variables
: m_size(0), head(new Node){
    // initialize an empty circular linked list with only the dummy node (m_next and m_prev should both point to head)
    head->m_next = head;
    head->m_prev = head;
}

Map::Map(const Map& other)
// initializer list for private member variables
:m_size(0), head(new Node) {
    // initialize an empty circular linked list
    head->m_next = head;
    head->m_prev = head;
    // use insert() to add the nodes from the other Map to this one
    for(Node *otherP = other.head->m_next; otherP != other.head; otherP = otherP->m_next){
        insert(otherP->m_pair.m_key, otherP->m_pair.m_value);
    }
}

Map& Map::operator=(const Map& other){
    // check for aliasing
    if(this != &other){
        // construct a copy of the other Map and swap it with this one
        Map temp(other);
        swap(temp);
    }
    // return a reference to this map for linked assignments
    return *this;
}

Map::~Map() {
    // iterate through nodes of a linked list using pointers until p reaches the dummy node
    for(Node* p = head->m_next; p != head; ){
        // once we delete the current node, we can't access m_next anymore so we need to have temp Node pointer to store it
        Node* temp = p->m_next;
        delete p;
        p = temp;
    }
    // delete head
    delete head;
}

bool Map::insert(const KeyType& key, const ValueType& value){
    // key is already in the map; don't do anything
    if(contains(key))
        return false;
    // create a new node
    Node *p = new Node;
    p->m_pair.m_key = key;
    p->m_pair.m_value = value;
    
    // set new node's m_next and m_prev
    p->m_prev = head->m_prev;
    p->m_next = head;
    
    // change the previous node's m_next
    head->m_prev->m_next = p;
    // change head's m_prev
    head->m_prev = p;
    
    m_size++;
    return true;
}

bool Map::update(const KeyType& key, const ValueType& value){
    // iterate through nodes
    for(Node* p = head->m_next; p != head; p = p->m_next){
        // if key is equal to key currently in the map
        if(key == p->m_pair.m_key){
            // change the value to second parameter
            p->m_pair.m_value = value;
            return true;
        }
    }
    return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value){
    // try to update (if it can't update, it will insert)
    update(key, value);
    // try to insert (if it's already updated, it will not insert)
    insert(key, value);
    // should always return true due to unlimited capacity
    return true;
}

bool Map::erase(const KeyType& key) {
    // if there is no current key in map, do nothing
    if(!contains(key))
        return false;
    // iterate through nodes
    for(Node *p = head->m_next; p != head; p = p->m_next){
        // once key is found
        if(key == p->m_pair.m_key){
            // change the previous node's m_next (skip over p)
            p->m_prev->m_next = p->m_next;
            // change the next node's m_prev (go back one)
            p->m_next->m_prev = p->m_prev;
            
            delete p;
            m_size--;
            break;
        }
    }
    return true;
}

bool Map::contains(const KeyType& key) const {
    // iterate through nodes
    for(Node* p = head->m_next; p != head; p = p->m_next){
        // if key is equal to a key currently in the map, return true
        if(key == p->m_pair.m_key)
            return true;
    }
    return false;
}

bool Map::get(const KeyType& key, ValueType& value) const {
    // iterate through nodes
    for(Node* p = head->m_next; p != head; p = p->m_next){
        // if key is equal to a key currently in the map, set 2nd parameter to value in map
        if(key == p->m_pair.m_key){
            value = p->m_pair.m_value;
            return true;
        }
    }
    return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const {
    // i is not in bounds, do nothing
    if(i < 0 || i >= size())
        return false;
    // iterate through nodes
    for(Node* p = head->m_next; p != head; p = p->m_next){
        // calculates how many keys in the map the current key is greater than
        int greaterThan = 0;
        for(Node* q = head->m_next; q != head; q = q->m_next){
            // compare all other keys to current key (nested loop)
            if(p->m_pair.m_key > q->m_pair.m_key)
                greaterThan++;
        }
        // if strictly greater than exactly i keys in the map, change the parameters
        if(i == greaterThan){
            key = p->m_pair.m_key;
            value = p->m_pair.m_value;
            return true;
        }
    }
    return false;
}

void Map::swap(Map& other) {
    // swap m_size
    int tempSize = m_size;
    m_size = other.m_size;
    other.m_size = tempSize;
    
    // swap the pointers to the linked list (head)
    Node* temp = head;
    head = other.head;
    other.head = temp;
}

void Map::dump() const{
    // for testing/debugging: iterates through linked list, printing the keys and values of each node and the keys of m_prev and m_next to make sure pointers are correct
    int count = 0;
    for(Node* p = head->m_next; p != head; p = p->m_next, count++){
        cerr << "Pair " << count << ": " << p->m_pair.m_key << ", " << p->m_pair.m_value << "; ";
        cerr << "Prev: " << p->m_prev->m_pair.m_key << "; Next: " << p->m_next->m_pair.m_key << endl;
    }
}

bool merge(const Map& m1, const Map& m2, Map& result){
    // false if there exists a key that appears in both m1 and m2 but with different corresponding values; this value is returned at the end of the function
    bool allCorrespondingValues = true;
    
    // invokes assignment operator that copies all of m1's nodes into result
    result = m1;
    
    // iterate through nodes of m2
    for(int i = 0; i < m2.size(); i++){
        // store the keys and values of each node
        KeyType m2Key;
        ValueType m2Value;
        m2.get(i, m2Key, m2Value);
        
        // see if m1 contains the current m2 key
        ValueType m1Value;
        if(m1.get(m2Key, m1Value)){
            // if the corresponding values of the key do not match, then do not include it in the results Map; erase it
            if(m1Value != m2Value){
                result.erase(m2Key);
                allCorrespondingValues = false;
            }
            // if they do match, do nothing; it is already inserted
        }
        // if m1 doesn't contain the m2 key, add m2
        else{
            result.insert(m2Key, m2Value);
        }
    }
    return allCorrespondingValues;
}

void reassign(const Map& m, Map& result){
    // set result to an Empty map with just a head node
    Map empty;
    result = empty;
    
    // if m only has one pair, result only contains a copy of that pair
    if(m.size() == 1){
        result = m;
        return;
    }
    KeyType key, kTrash;
    ValueType value, vTrash;
    
    // store the value of the first key
    ValueType firstValue;
    m.get(0, kTrash, firstValue);
    
    // shift the values of the nodes to the right by 1
    for(int i = 0; i < m.size(); i++){
        m.get(i, key, vTrash);
        if(i != m.size() - 1){
            m.get(i + 1, kTrash, value);
            result.insert(key, value);
        }
    }
    result.insert(key, firstValue);
}

