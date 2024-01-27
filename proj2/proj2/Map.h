// Map.h

#ifndef MAP_INCLUDED
#define MAP_INCLUDED

#include <string>

using KeyType = std::string;
using ValueType = double;

const int DEFAULT_MAX_ITEMS = 150;

class Map {
public:
    Map(); // Constructor
    Map(const Map& other); // Copy constructor
    Map& operator= (const Map& other); // Equality operator
    ~Map(); // Destructor
    
    bool empty() const; // Inline implementation
    int size() const; // Inline implementation
    
    bool insert(const KeyType& key, const ValueType& value);
    bool update(const KeyType& key, const ValueType& value);
    bool insertOrUpdate(const KeyType& key, const ValueType& value);
    bool erase(const KeyType& key);
    bool contains(const KeyType& key) const;
    bool get(const KeyType& key, ValueType& value) const;
    bool get(int i, KeyType& key, ValueType& value) const;
    void swap(Map& other);
    
    void dump() const;
    
private:
    struct Pair {
        KeyType m_key;
        ValueType m_value;
    };
    
    struct Node {
        Pair m_pair;
        Node* m_next;
        Node* m_prev;
    };
    
    Node* head; // holds the linked list of pairs
    int m_size; // number of pairs in the map
};

bool merge(const Map& m1, const Map& m2, Map& result);
void reassign(const Map& m, Map& result);

inline
int Map::size() const {
    // Return the number of key/value pairs in the map.
    return m_size;
}

inline
bool Map::empty() const {
    // Return true if the map is empty, otherwise false.
    return size() == 0;
}

#endif // MAP_INCLUDED
