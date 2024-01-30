#include "Map.h"
#include <cassert>
#include <iostream>

using namespace std;

int main () {
    // MARK: Smallbergs's Test Cases
    Map msd; // KeyType is std::string, ValueType is double
    assert(msd.insert("ccc", 80));
    assert(msd.insert("aaa", 40));
    assert(msd.insert("ddd", 60));
    assert(msd.insert("bbb", 20));
    KeyType k = "xxx";
    ValueType v;
    assert(!msd.get(4, k, v)  &&  k == "xxx");  // x is unchanged
    assert(msd.get(1, k, v)  &&  k == "bbb");   // "bbb" is greater than exactly 1 item
    
    /** merge()*/
    Map names;
    assert(names.insert("Fred", 123));
    assert(names.insert("Ethel", 456));
    assert(names.insert("Lucy",789));
    Map names1;
    assert(names1.insert("Lucy", 789));
    assert(names1.insert("Ricky", 321));
    Map result;
    assert(merge(names, names1, result));
    assert(result.size() == 4 && result.get(0, k, v) && k == "Ethel" && v == 456 && result.get(1, k, v) && k == "Fred" && v == 123 && result.get(2, k, v) && k == "Lucy" && v == 789 && result.get(3, k, v) && k == "Ricky" && v == 321);
    names1.insertOrUpdate("Lucy",654);
    assert(!merge(names, names1, result));
    assert(result.size() == 3 && result.get(0, k, v) && k == "Ethel" && v == 456 && result.get(1, k, v) && k == "Fred" && v == 123 && result.get(2, k, v) && k == "Ricky" && v == 321);
    
    /** reassign() */
    Map reassignResult;
    assert(names.insert("Ricky", 321));
    reassign(names, reassignResult);
    KeyType k1, k2, k3;
    ValueType v1, v2, v3;
    assert(reassignResult.size() == 4 && reassignResult.get(0, k, v) && reassignResult.get(1, k1, v1) && reassignResult.get(2, k2, v2) && reassignResult.get(3, k3, v3) && v != v1 != v2 != v3 && v != 456 && v1 != 123 && v2 != 789 && v3 != 321);
    
    assert(names.erase("Ricky"));
    assert(names.insertOrUpdate("Lucy", 456));
    reassign(names, reassignResult);
    assert(reassignResult.size() == 3 && reassignResult.get(0, k, v) && reassignResult.get(1, k1, v1) && reassignResult.get(2, k2, v2) && ((v == 123 && v1 == 456 && v2 == 456) || (v == 456 && v1 == 456 && v2 == 123)));
    
    // MARK: Rachel's Test Cases
    
    KeyType key = "";
    ValueType value = -1;
    
    /** insert() */
    Map test;
    assert(test.insert("a", 23));
    assert(test.insert("b", 46));
    assert(test.insert("c", 93));
    assert(!test.insert("c", 1)); // should not insert if the key already exists
    
    /**Map(const Map& other)**/
    Map cpy = Map(test);
    
    /** update() */
    assert(test.update("c", 7));
    assert(test.get("c", value));
    assert(value == 7);
    
    /** erase() */
    msd.erase("b"); // erase
    assert(!msd.get("b", value) && value == 7);
    
    /** swap() */
    Map test1; // swap test and test1
    assert(test1.insert("d", 109));
    assert(test1.insert("e", 58));
    test.swap(test1);
    assert(test.get(0, key, value) && key == "d" && value == 109);
    assert(test.get(1, key, value) && key == "e" && value == 58);
    cerr << "Passed all test cases." << endl;
}

