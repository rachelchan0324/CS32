#include <string>
#include <list>
#include <vector>
#include <iostream>
template <class T>

class HashMap{
public:
    HashMap(double max_load = 0.75);
    ~HashMap();
    int size() const;
    
    void insert(const std::string& key, const T& value);
    T& operator[](const std::string& key);
    const T* find(const std::string& key) const;
    T* find(const std::string& key) {
        const auto& hm = *this;
        return const_cast<T*>(hm.find(key));
    }
private:
    struct Item {
        std::string key;
        T value;
        
        Item(const std::string& key, const T& value)
        : key(key), value(value) {}
    };
    
    std::vector<std::list<Item>> hashTable;
    double m_maxload;
    int m_buckets;
    int m_items;
    
    unsigned int hash(const std::string& str) const {return std::hash<std::string>()(str) % m_buckets;}
    void rehash();
};

template <class T>
HashMap<T>::HashMap(double max_load){
    if(max_load < 0)
        max_load = 0.75;
    hashTable.resize(10);
    m_maxload = max_load;
    m_buckets = 10;
    m_items = 0;
}

template <class T>
HashMap<T>::~HashMap(){ }

template <class T>
int HashMap<T>::size() const{
    return m_items;
}

template <class T>
void HashMap<T>::insert(const std::string& key, const T& value){
    // check for duplicate
    if(find(key) != nullptr){
        *find(key) = value;
        return;
    }
    rehash(); // check if adding new item will exceed max load
    Item newItem(key, value);
    hashTable[hash(key)].push_back(newItem);
    m_items++;
}

template <class T>
void HashMap<T>::rehash(){
    if((1.0 * (m_items + 1)) / m_buckets <= m_maxload)
        return;
    int oldBuckets = m_buckets;
    m_buckets *= 2;
    std::vector<std::list<Item>> newHashTable(m_buckets);
    
    for(int i = 0; i < oldBuckets; i++){
        while(hashTable[i].size() > 0){
            typename std::list<Item>::iterator it = hashTable[i].begin();
            newHashTable[hash(it->key)].splice(newHashTable[hash(it->key)].begin(), hashTable[i], it);
        }
    }
    hashTable = newHashTable;
}

template <class T>
T& HashMap<T>::operator[](const std::string& key){
    if(find(key) == nullptr)
        insert(key, T());
    return *find(key);
}

template <class T>
const T* HashMap<T>::find(const std::string& key) const{
    unsigned int bucket = hash(key);
    for(typename std::list<Item>::const_iterator it = hashTable[bucket].begin(); it != hashTable[bucket].end(); it++){
        if(it->key == key)
            return &it->value;
    }
    return nullptr;
}
