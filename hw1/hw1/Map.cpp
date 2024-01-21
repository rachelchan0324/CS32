#include "Map.h"
#include <iostream>

using namespace std;

Map::Map() {
    m_size = 0;
}

bool Map::empty() const{
    // Return true if the map is empty, otherwise false.
    return m_size == 0;
}

int Map::size() const{
    // Return the number of key/value pairs in the map.
    return m_size;
}

bool Map::insert(const KeyType& key, const ValueType& value) {
    // If key is not equal to any key currently in the map and if the
    // key/value pair can be added to the map, then do so and return true.
    // Otherwise, make no change to the map and return false (indicating
    // that either the key is already in the map, or the map has a fixed
    // capacity and is full).
    if(contains(key) || m_size >= DEFAULT_MAX_ITEMS)
        return false;
    m_pairs[m_size].key = key;
    m_pairs[m_size].value = value;
    m_size++;
    return true;
}

bool Map::update(const KeyType& key, const ValueType& value) {
    // If key is equal to a key currently in the map, then make that key no
    // longer map to the value that it currently maps to, but instead map to
    // the value of the second parameter; in this case, return true.
    // Otherwise, make no change to the map and return false.
    for(int i = 0; i < m_size; i++) {
        if(key == m_pairs[i].key){
            m_pairs[i].value = value;
            return true;
        }
    }
    return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value) {
    // If key is equal to a key currently in the map, then make that key no
    // longer map to the value that it currently maps to, but instead map to
    // the value of the second parameter; in this case, return true.
    // If key is not equal to any key currently in the map, and if the
    // key/value pair can be added to the map, then do so and return true.
    // Otherwise, make no change to the map and return false (indicating
    // that the key is not already in the map and the map has a fixed
    // capacity and is full).
    if(update(key, value))
        return true;
    return insert(key, value);
}

bool Map::erase(const KeyType& key) {
    // If key is equal to a key currently in the map, remove the key/value
    // pair with that key from the map and return true.  Otherwise, make
    // no change to the map and return false.
    for(int i = 0; i < m_size; i++){
        if(key == m_pairs[i].key){
            m_pairs[i] = m_pairs[m_size - 1];
            m_size--;
            return true;
        }
    }
    return false;
}

bool Map::contains(const KeyType& key) const {
    // Return true if key is equal to a key currently in the map, otherwise
    // false.
    for(int i = 0; i < m_size; i++) {
        if(key == m_pairs[i].key)
            return true;
    }
    return false;
}

bool Map::get(const KeyType& key, ValueType& value) const {
    // If key is equal to a key currently in the map, set value to the
    // value in the map which the key maps to, and return true.  Otherwise,
    // make no change to the value parameter of this function and return
    // false.
    for(int i = 0; i < m_size; i++) {
        if(key == m_pairs[i].key){
            value = m_pairs[i].value;
            return true;
        }
    }
    return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const {
    // If 0 <= i < size(), copy into the key and value parameters the
    // key and value of the key/value pair in the map whose key is strictly
    // greater than exactly i keys in the map and return true. Otherwise,
    // leave the key and value parameters unchanged and return false
    for(int j = 0; j < m_size; j++) {
        int greaterThan = 0;
        for(int k = 0; k < m_size; k++){
            if(m_pairs[j].key > m_pairs[k].key)
                greaterThan++;
        }
        if(greaterThan == i){
            key = m_pairs[j].key;
            value = m_pairs[j].value;
            return true;
        }
    }
    return false;
}

void Map::swap(Map& other) {
    // Exchange the contents of this map with the other one.
    // MARK: is this do-able?
    Map temp = *this;
    *this = other;
    other = temp;
}

void Map::dump() const {
    // prints all itmes in a map
    if(empty())
        cerr << "Nothing." << endl;
    for(int i = 0; i < m_size; i++){
        cerr << i << ": " << m_pairs[i].key << ", " << m_pairs[i].value << endl;
    }
}

