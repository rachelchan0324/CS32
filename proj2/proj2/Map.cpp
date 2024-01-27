// Map.cpp

#include "Map.h"
#include <iostream>

using namespace std;

Map::Map()
: m_size(0), head(new Node){
    head->m_next = head;
    head->m_prev = head;
}

Map::Map(const Map& other)
:m_size(0), head(new Node) {
    head->m_next = head;
    head->m_prev = head;
    for(Node *otherP = other.head->m_next; otherP != other.head; otherP = otherP->m_next){
        insert(otherP->m_pair.m_key, otherP->m_pair.m_value);
    }
}

Map& Map::operator=(const Map& other){
    if(this != &other){
        Map temp(other);
        swap(temp);
    }
    return *this;
}
    
Map::~Map() {
    for(Node* p = head->m_next; p != head; ){
        Node* temp = p->m_next;
        delete p;
        p = temp;
    }
    delete head;
}

bool Map::insert(const KeyType& key, const ValueType& value){
    // If key is not equal to any key currently in the map and if the
    // key/value pair can be added to the map, then do so and return true.
    // Otherwise, make no change to the map and return false (indicating
    // that either the key is already in the map, or the map has a fixed
    // capacity and is full).
    if(contains(key))
        return false;
    Node *p = new Node;
    p->m_pair.m_key = key;
    p->m_pair.m_value = value;
    
    Node *prev = head->m_prev; // temp variable to hold previous node
    p->m_prev = prev;
    p->m_next = head;
    
    prev->m_next = p; // updating previous node's next pointer
    head->m_prev = p; // updating the next node's previous pointer
    m_size++;
    return true;
}

bool Map::update(const KeyType& key, const ValueType& value){
    // If key is equal to a key currently in the map, then make that key no
    // longer map to the value that it currently maps to, but instead map to
    // the value of the second parameter; in this case, return true.
    // Otherwise, make no change to the map and return false.
    for(Node* p = head->m_next; p != head; p = p->m_next){
        if(key == p->m_pair.m_key){
            p->m_pair.m_value = value;
            return true;
        }
    }
    return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value){
    // If key is equal to a key currently in the map, then make that key no
    // longer map to the value that it currently maps to, but instead map to
    // the value of the second parameter; in this case, return true.
    // If key is not equal to any key currently in the map, and if the
    // key/value pair can be added to the map, then do so and return true.
    // Otherwise, make no change to the map and return false (indicating
    // that the key is not already in the map and the map has a fixed
    // capacity and is full).
    update(key, value);
    insert(key, value);
    return true;
}

bool Map::erase(const KeyType& key) {
    // If key is equal to a key currently in the map, remove the key/value
    // pair with that key from the map and return true.  Otherwise, make
    // no change to the map and return false.
    
    /* int pos = findFirstAtLeast(key);
    if (pos == m_size  ||  m_data[pos].m_key != key)  // not found
        return false;
    
    // Shift pairs left to replace the one erased
    
    for ( ; pos+1 < m_size; pos++)
        m_data[pos] = m_data[pos+1];
    m_size--;
    return true; */
    if(!contains(key))
        return false;
    for(Node *p = head->m_next; p != head; p = p->m_next){
        if(key == p->m_pair.m_key){
            p->m_prev->m_next = p->m_next;
            p->m_next->m_prev = p->m_prev;
            delete p;
            m_size--;
            break;
        }
    }
    return true;
}

bool Map::contains(const KeyType& key) const {
    // Return true if key is equal to a key currently in the map, otherwise
    // false.
    for(Node* p = head->m_next; p != head; p = p->m_next){
        if(key == p->m_pair.m_key)
            return true;
    }
    return false;
}

bool Map::get(const KeyType& key, ValueType& value) const {
    // If key is equal to a key currently in the map, set value to the
    // value in the map which the key maps to, and return true.  Otherwise,
    // make no change to the value parameter of this function and return
    // false.
    for(Node* p = head->m_next; p != head; p = p->m_next){
        if(key == p->m_pair.m_key){
            value = p->m_pair.m_value;
            return true;
        }
    }
    return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const {
    // If 0 <= i < size(), copy into the key and value parameters the
    // key and value of the key/value pair in the map whose key is strictly
    // greater than exactly i keys in the map and return true.  Otherwise,
    // leave the key and value parameters unchanged and return false.
    if(i < 0 || i >= size())
        return false;
    for(Node* p = head->m_next; p != head; p = p->m_next){
        int greaterThan = 0;
        for(Node* q = head->m_next; q != head; q = q->m_next){
            if(p->m_pair.m_key > q->m_pair.m_key)
                greaterThan++;
        }
        if(i == greaterThan){
            key = p->m_pair.m_key;
            value = p->m_pair.m_value;
            return true;
        }
    }
    return false;
}

void Map::swap(Map& other) {
    // swap the size
    int tempSize = m_size;
    m_size = other.m_size;
    other.m_size = tempSize;
    
    // swap the pointers to the list
    Node* temp = head;
    head = other.head;
    other.head = temp;
}

void Map::dump() const{
    int count = 0;
    for(Node* p = head->m_next; p != head; p = p->m_next, count++){
        cerr << "Pair " << count << ": " << p->m_pair.m_key << ", " << p->m_pair.m_value << "; ";
        cerr << "Prev: " << p->m_prev->m_pair.m_key << "; Next: " << p->m_next->m_pair.m_key << endl;
    }
}

bool merge(const Map& m1, const Map& m2, Map& result){
    // If a key appears in exactly one of m1 and m2, then result must contain a pair consisting of that key and its
    // corresponding value. If a key appears in both m1 and m2, with the same corresponding value in both, then
    // result must contain exactly one pair with that key and value. If there exists a key that appears in both m1 and m2,
    // but with different corresponding values, then this function returns false; if there is no key like this, the
    // function returns true. Even if the function returns false, result must be constituted as defined by the above
    // rules.
    bool allCorrespondingValues = true;
    result = m1;
    
    for(int i = 0; i < m2.size(); i++){
        KeyType m2Key;
        ValueType m2Value;
        m2.get(i, m2Key, m2Value);
        
        ValueType m1Value;
        
        if(m1.get(m2Key, m1Value)){ // if m1 contains m2 key
            if(m1Value != m2Value){
                result.erase(m2Key);
                allCorrespondingValues = false;
            }
        }
        else{ // if m1 doesn't contain m2 key, add m2
            result.insert(m2Key, m2Value);
        }
    }
    return allCorrespondingValues;
}

void reassign(const Map& m, Map& result){
    Map empty;
    result = empty;
    if(m.size() == 1){
        result = m;
        return;
    }
    KeyType key, kTrash;
    ValueType value, vTrash;
    ValueType firstValue;
    m.get(0, kTrash, firstValue);
    for(int i = 0; i < m.size(); i++){
        m.get(i, key, vTrash);
        if(i != m.size() - 1){
            m.get(i + 1, kTrash, value);
            result.insert(key, value);
        }
    }
    result.insert(key, firstValue);
}
